//#define NDEBUG	// turn off asserts

#include "Cognitive_processor.h"
#include "Coordinator.h"
#include "Human_processor.h"
#include "Visual_processor.h"
#include "Auditory_processor.h"
#include "Manual_processor.h"
#include "Vocal_processor.h"
#include "WM_event_types.h"
#include "WM_processor.h"
#include "Task_processor.h"
#include "LTM_processor.h"
#include "Cognitive_event_types.h"
#include "GOMS_Execution_Thread.h"
#include "GOMS_Model.h"
#include "GOMS_Memory_Operators.h"
#include "GOMS_Control_Operators.h"
#include "GOMS_Manual_Operators.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_exceptions.h"
#include "Utility_templates.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <utility>	// for std::pair

//#include <DebugNew.h>
using namespace std;

class Glean_abort_and_restart {
	// an empty class
};

// prototype for top-level function for PCCTS-generated parser
GOMS_Model_Sptr_t parse_GOMSL_file(const char * filename);

/* note 5/7/06
Check for memory leaks of threads - not obvious that they get deleted when new
ones created.
*/

/*
All scheduled events leave at step_end_time, which is set at the beginning of
the step execution.
We schedule a step event every 50 ms.  When one is received, the next is always scheduled.
But if there are operations pending, the step event is ignored;
if no operations are pending, then we advance to the next step.
*/

//notes 7/23/98

// values for processor constants - reviewed 7/23/98
const int Cognitive_processor::cognitive_processor_step_time = 50;
const int Cognitive_processor::cognitive_processor_idle_step_limit = 20000;	// 1000 sec

Cognitive_processor::Cognitive_processor(Human_processor * human_ptr) : 
		Human_subprocessor("Cognitive_processor", human_ptr),
		step_end_time(0), last_step_time(0), idle_step_counter(0), operator_executed(false),
		main_thread_ptr(0), error_thread_ptr(0), interrupt_thread_ptr(0), original_exception_thread_ptr(0),
		thread_id_counter(0),
		visual_busy(false), visual_busy_ptr(0), auditory_busy(false), auditory_busy_ptr(0), 
		manual_busy(false), manual_busy_ptr(0), vocal_busy(false), vocal_busy_ptr(0), 
		ltm_busy(false), ltm_busy_ptr(0), task_busy(false), task_busy_ptr(0), 
		high_level_busy(false), high_level_busy_ptr(0),
		cognitive_busy(false), cognitive_busy_ptr(0),
		output_step_labels_only(true), output_step_listing(true), output_step_WM(true), 
		output_model_contents(false), output_learning_calculation(false),
		stop_at_time(false), time_to_stop(0), stop_after_steps(false), steps_to_stop(0),
		stop_on_abort_and_restart(false)
{
}

Cognitive_processor::~Cognitive_processor()
{
	if(error_thread_ptr)
		delete error_thread_ptr;
	if(interrupt_thread_ptr)
		delete interrupt_thread_ptr;
	for_each(thread_list.begin(), thread_list.end(), Delete());
}


bool Cognitive_processor::compile(const string& filename)
{
	goms_model_sptr = 0;	// dispose of previous GOMS model
	goms_model_sptr = parse_GOMSL_file(filename.c_str());
	if(output_model_contents)
		display_model_contents();
	if(output_learning_calculation)
		output_learning_analysis();
	return bool(goms_model_sptr);
}

void Cognitive_processor::output_learning_analysis() const
{
	Normal_out << get_learning_report() << endl;
}

std::string Cognitive_processor::get_learning_report() const
{
	if (goms_model_sptr) {
		return goms_model_sptr->get_learning_report();
		}
	else
		return("Model is invalid - learning results can not be computed.");
}

void Cognitive_processor::display_model_contents() const
{
	if (goms_model_sptr) {
		goms_model_sptr->display(Normal_out);
		(goms_model_sptr->get_LTM()).display_contents(Normal_out);
		(goms_model_sptr->get_initial_WM_contents()).display_contents(Normal_out);
		}
	else
		Normal_out << "Model is invalid - contents not shown." << endl;
}


void Cognitive_processor::initialize()
{
	idle_step_counter = 0; // number of idle steps
	last_step_time = 0;		// time last step was executed
	
	salient_objects.clear();
	if(error_thread_ptr)
		delete error_thread_ptr;
	if(interrupt_thread_ptr)
		delete interrupt_thread_ptr;
	for_each(thread_list.begin(), thread_list.end(), Delete());
	thread_list.clear();
	// for safety/consistency
	error_thread_ptr = 0;
	interrupt_thread_ptr = 0;
	main_thread_ptr = 0;
	initialize_processor_busy_states();
	
	create_starting_threads();
	
}

void Cognitive_processor::create_starting_threads()
{
	thread_id_counter = 0;
	// create the interrupt and main threads, in that order
	// create an interrupt thread only if there is an interrupt method
	{
		GOMS_Goal interrupt_goal(Check__c, Interrupts__c);
		GOMS_Procedure_Sptr_t procedure_sptr = goms_model_sptr->get_procedure(interrupt_goal);
		if(procedure_sptr) {
			interrupt_thread_ptr = new GOMS_Execution_Thread(
			Interrupt_c, thread_id_counter++, this, goms_model_sptr, 
			interrupt_goal, GOMS_Execution_Thread::INTERRUPT);
			}
	}
	main_thread_ptr = new GOMS_Execution_Thread(Main_c, thread_id_counter++, this, goms_model_sptr, 
		goms_model_sptr->start_goal, GOMS_Execution_Thread::MAIN);
	thread_list.push_back(main_thread_ptr);
}

// Event interface to rest of the system
void Cognitive_processor::accept_event(const Start_event *)
{
	step_end_time = get_time() + cognitive_processor_step_time;
	start_execution();
	Coordinator::get_instance().schedule_event(new Cognitive_Step_event(
		get_time() + cognitive_processor_step_time, this));
}

void Cognitive_processor::accept_event(const Cognitive_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}


// On each step, we either execute another step or simply wait, and schedule another
// step event. If nothing is pending, and there are no more steps to execute, 
// then we do not schedule another step event.
void Cognitive_processor::handle_event(const Cognitive_Step_event *) 
{	
	step_end_time = get_time() + cognitive_processor_step_time;
	// execute the next steps; if still runnable, schedule a new step
	if(execute_next_step()) {
		Coordinator::get_instance().schedule_event(new Cognitive_Step_event (step_end_time, this)); 	
		// reset the idle step counter if an operator was executed
		if(operator_executed) {
			idle_step_counter = 0;
			last_step_time = get_time();
			}
		}
	// here if there is no more to do - no more step events get scheduled.
	// simulation is not halted - other processors might be running
	else {
		Normal_out << processor_info() << " No more steps to execute." << endl;
		Coordinator::get_instance().schedule_event(new Cognitive_Stop_event (step_end_time, this));
		}
}

// this just produces a delayed operation-complete event back to the cognitive processor
void Cognitive_processor::handle_event(const Cognitive_Delay_event * event_ptr)
{
	long completion_time = get_time() + event_ptr->delay;
	
	Event * output_event_ptr = new Cognitive_Operation_Complete_event (completion_time, this, event_ptr->thread_ptr);
		
	Coordinator::get_instance().schedule_event(output_event_ptr); 
//	debug_out << get_time() << " Cognitive_Delay_event scheduled for " << completion_time << endl;
}

// tell the main and interrupt threads to start executing
void Cognitive_processor::start_execution()
{
	if(interrupt_thread_ptr)
		interrupt_thread_ptr->start_execution();
	Assert(main_thread_ptr);
	main_thread_ptr->start_execution();
}

void Cognitive_processor::stop_simulation(const Symbol& message)
{
	Normal_out << "Methods stopped: " << message << endl;
	Coordinator::get_instance().stop();
}

// return true if model is runnable according to the definition in get_runnable
bool Cognitive_processor::execute_next_step()
{	
	// set initially that no interesting operator executed
	operator_executed = false;

	// check - we shouldn't be here unless the threads are in a runnable state
	Assert(get_runnable());

	try {
	// if the error thread exists, run it; if still runnable, do nothing more
	// but if it has terminated, run the others.
	if(error_thread_ptr) {
		if(error_thread_ptr->execute_next_step()) {
			operator_executed = operator_executed || error_thread_ptr->get_operator_executed();
			return get_runnable();	// error thread underway
			}
		else {				// error thread has just terminated
			delete error_thread_ptr;
			error_thread_ptr = 0;
			}
		}
	// at this point, either no error thread, or it has just terminated
	// continue with normal execution at this point
	
	// let the interrupt thread run if present before the main and other threads
	// if this thread is present, it never terminates, so we don't check for termination
	// if submethods are in progress, interrupt handling is happening, so do nothing futher.
	if(interrupt_thread_ptr) {
		interrupt_thread_ptr->execute_next_step();
		operator_executed = operator_executed || interrupt_thread_ptr->get_operator_executed();
		if(interrupt_thread_ptr->submethods_in_progress())
			return get_runnable();	// interrupt processing underway
		}
	// here if no interrupt method, or no interrupt submethods being executed
	execute_all_regular_thread_steps();

/*		// if interrupt processing (methods called by the interrupt rules) is in progress,
		// do not advance the other threads
		if(!interrupt_thread_ptr || !(interrupt_thread_ptr->submethods_in_progress()))
			execute_all_thread_steps();
*/		
	// no more threads means no more to execute; if main thread removed, also means to stop
	return get_runnable();
	}
	catch (Glean_abort_and_restart&) {
		terminate_all_threads();
		// create and restart interrupt and main threads
		create_starting_threads();
		start_execution();
		return get_runnable();
		}
}

void Cognitive_processor::execute_all_regular_thread_steps()
{	
	// execute the next step in each thread, in order; keep track of whether a thread advanced a step
	GOMS_thread_list_t dead_threads;
	for(GOMS_thread_list_t::iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
		if((*it)->execute_next_step()) {
			// did an interesting operator get executed?
			operator_executed = operator_executed || (*it)->get_operator_executed();
			}
		else {
			// thread is no longer runnable, but delete it only after all threads have executed
			dead_threads.push_back(*it);
			}
/*		bool thread_runnable = (*it)->execute_next_step();
		// if the result is false, there is nothing more to execute from this thread - eliminate it
		if(!thread_runnable)
			dead_threads.push_back(*it);
		else {
			step_executed = true;
			// did an interesting operator get executed?
			operator_executed = operator_executed || (*it)->get_operator_executed();
			}
*/
		}
	// take any dead threads out of the list and delete them
	for(GOMS_thread_list_t::iterator it = dead_threads.begin(); it != dead_threads.end(); ++it) {
		thread_list.remove(*it);
		if(*it == main_thread_ptr)
			main_thread_ptr = 0;
		delete (*it);
		}
	// add any new threads to the list and start them
	for(GOMS_thread_list_t::iterator it = new_thread_list.begin(); it != new_thread_list.end(); ++it) {
		thread_list.push_back(*it);
		(*it)->start_execution();
		}
	new_thread_list.clear();
}

// called by each thread on each step; for an interrupt thread, when below top level
void Cognitive_processor::output_step_WM_info() const
{
	get_human_ptr()->get_Wm_ptr()->display_tag_store_contents(Normal_out);
	get_human_ptr()->output_visual_memory_contents();
	get_human_ptr()->output_auditory_memory_contents();
	get_human_ptr()->output_task_memory_contents();
	get_human_ptr()->output_highlevel_memory_contents();
}


/*
Following functions are called by GOMS_Operators to change flow of control.
do not schedule any events for the recorders, but call directly  - dk 3/3/01
*/

void Cognitive_processor::spawn_accomplish_goal(const GOMS_Goal& starting_goal, const Symbol& thread_name)
{
	GOMS_Execution_Thread * thread_ptr = new GOMS_Execution_Thread(thread_name, thread_id_counter++, this, goms_model_sptr, starting_goal);
	new_thread_list.push_back(thread_ptr);
}


// abort_and_restart throws an exception to get back to the top level of 
// the simulation execution
void Cognitive_processor::abort_and_restart(GOMS_Execution_Thread * thread_ptr)
{
	Normal_out << get_time() << ' ' << get_human_ptr()->get_name() << " *** abort_and_restart" << endl;
	// send an immediate event to the recorders 
	Cognitive_Abort_and_Restart_event msg
		(get_time(), this, thread_ptr->get_current_goal());
	broadcast_to_recorders(&msg);

//	if(stop_on_abort_and_restart) {
//		Normal_out << " WM contents:" << endl;
		// display the contents here
//		get_human_ptr()->get_Wm_ptr()->display_contents(Normal_out);
//		pause_and_then_single_step();
//		}
		
	throw Glean_abort_and_restart();
}

void Cognitive_processor::resume(const Symbol& thread_name, GOMS_Goal goal, const Symbol& step_name)
{
	Normal_out << get_time() << ' ' << get_human_ptr()->get_name() << " *** resume " << endl;
	// send an immediate event to the recorders 
//	Cognitive_Abort_and_Restart_event msg
//		(get_time(), 0, get_broadcaster(), thread_ptr->get_current_goal());
//	call_with_event(msg); 
	// find the thread
	GOMS_Execution_Thread * thread_ptr = find_thread(thread_name);
	// tell it to resume
	thread_ptr->resume_at_goal_and_step(goal, step_name);
}



void Cognitive_processor::raise_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name)
{
	// suspend the other threads
	// load up tag store with the current information
	// start the error thread
	if(thread_ptr->get_thread_name() == Error_c)
		raise_error_exception(thread_ptr, exception_name);
	else
		raise_normal_exception(thread_ptr, exception_name);
}

// raising an exception normally means to start the error thread
void Cognitive_processor::raise_normal_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name)
{

	// since only one error thread can be running at a time, only one thread
	// can be the originator of the exception that started the error thread
	original_exception_thread_ptr = thread_ptr;

	// suspend the other threads
	// load up tag store with the current information
	// start the error thread
	Symbol exception_thread = thread_ptr->get_thread_name();
	GOMS_Goal exception_goal = thread_ptr->get_current_goal();
	Symbol exception_step = thread_ptr->get_current_step_name();
	
	store_in_tagstore(Exception_name_tag_c, exception_name);
	store_in_tagstore(Exception_current_thread_tag_c, exception_thread);
	store_in_tagstore(Exception_current_goal_action_tag_c, exception_goal.action);
	store_in_tagstore(Exception_current_goal_object_tag_c, exception_goal.object);
	store_in_tagstore(Exception_current_step_tag_c, exception_step);
	
	GOMS_Goal error_handling_goal = thread_ptr->get_current_error_goal();
	if(error_handling_goal == GOMS_Goal()) {
		ostringstream oss;
		oss <<  "No current On_error goal present when GOMSL Exception " 
			<< exception_name << " raised in method for "
			<< exception_goal.action << ' ' <<  exception_goal.object
			<< " Step " << exception_step;
		throw Glean_user_error(oss.str());
		}

	error_thread_ptr = new GOMS_Execution_Thread(
		Error_c, thread_id_counter++, this, goms_model_sptr, error_handling_goal, GOMS_Execution_Thread::ERROR);
	error_thread_ptr->start_execution();
}

// raising an exception in an error thread is handled differently
void Cognitive_processor::raise_error_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name)
{
	// unwind the original thread to the point of another error handling goal
	Assert(original_exception_thread_ptr);
	original_exception_thread_ptr->unwind_to_next_error_method();

	// update the WM information to reflect that it is now the higher-level
	// method that has been interrupted - thread name is the same
	GOMS_Goal exception_goal = original_exception_thread_ptr->get_current_goal();
	Symbol exception_step = original_exception_thread_ptr->get_current_step_name();
	
	store_in_tagstore(Exception_name_tag_c, exception_name);
	store_in_tagstore(Exception_current_goal_action_tag_c, exception_goal.action);
	store_in_tagstore(Exception_current_goal_object_tag_c, exception_goal.object);
	store_in_tagstore(Exception_current_step_tag_c, exception_step);
	
	// flush the current error thread stack
	thread_ptr->unwind_to_top();
	// restart the error thread at the new error handler
	GOMS_Goal error_handling_goal = original_exception_thread_ptr->get_current_error_goal();
	if(error_handling_goal == GOMS_Goal()) {
		ostringstream oss;
		oss <<  "No current On_error goal present when GOMSL Exception " 
			<< exception_name << " raised in error method for "
			<< exception_goal.action << ' ' <<  exception_goal.object
			<< " Step " << exception_step;
		throw Glean_user_error(oss.str());
		}

	thread_ptr->start_at_goal(error_handling_goal);
}



void Cognitive_processor::store_in_tagstore(const Symbol& tag_name, const Symbol& tag_value)
{
	WM_Store_Tag_event msg (
		get_time(), get_human_ptr()->get_wm_ptr(), tag_name, tag_value);
	call_with_event(msg);
}

void Cognitive_processor::cleanup_raised_exception()
{
	remove_from_tagstore(Exception_name_tag_c);
	remove_from_tagstore(Exception_current_thread_tag_c);
	remove_from_tagstore(Exception_current_goal_action_tag_c);
	remove_from_tagstore(Exception_current_goal_object_tag_c);
	remove_from_tagstore(Exception_current_step_tag_c);
	
	// reset the original thread that raised the exception
	original_exception_thread_ptr = 0;
}

void Cognitive_processor::remove_from_tagstore(const Symbol& tag_name)
{
	WM_Erase_Tag_event msg (
		get_time(), get_human_ptr()->get_wm_ptr(), tag_name);
	call_with_event(msg);
}


void Cognitive_processor::terminate_all_threads() 
{
	// unwind and delete all the threads
	if(error_thread_ptr) {
		error_thread_ptr->unwind_to_top();
		clear_processor_busy(error_thread_ptr);
		delete error_thread_ptr;
		error_thread_ptr = 0;
		}
	if(interrupt_thread_ptr) {
		interrupt_thread_ptr->unwind_to_top();
		clear_processor_busy(interrupt_thread_ptr);
		delete interrupt_thread_ptr;
		interrupt_thread_ptr = 0;
		}
	for(GOMS_thread_list_t::iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
		(*it)->unwind_to_top();
		clear_processor_busy(*it);
		delete *it;
		}
	thread_list.clear();
}

// find the thread with the supplied name; first one is return if there is more than one
GOMS_Execution_Thread * Cognitive_processor::find_thread(const Symbol& thread_name) const
{
	for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
		if((*it)->get_thread_name() == thread_name)
			return *it;
		}
	throw Glean_exception(this,
				string("Error: Thread named ") + thread_name.str() + " does not exist.");
}


// reset the pointer to the thread for the processor, but leave the busy state alone
void Cognitive_processor::clear_processor_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(thread_ptr);
// leave open the possibility of more than one processor/thread
	if(thread_ptr == visual_busy_ptr)
		visual_busy_ptr = 0;
	if(thread_ptr == auditory_busy_ptr)
		auditory_busy_ptr = 0;
	if(thread_ptr == manual_busy_ptr)
		manual_busy_ptr = 0;
	if(thread_ptr == vocal_busy_ptr)
		vocal_busy_ptr = 0;
	if(thread_ptr == ltm_busy_ptr)
		ltm_busy_ptr = 0;
	if(thread_ptr == task_busy_ptr)
		task_busy_ptr = 0;
	if(thread_ptr == high_level_busy_ptr)
		high_level_busy_ptr = 0;
	if(thread_ptr == cognitive_busy_ptr)
		cognitive_busy_ptr = 0;
}

/* The busy flags store a pointer to the thread using the processor;
It cannot be set unless the processor is free.
When the complete event arrives, we tell the thread it is finished with an operation
and reset the flag to show that the processor is free.
*/

void Cognitive_processor::set_visual_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!visual_busy_ptr);
	visual_busy_ptr = thread_ptr;
	visual_busy = true;
}

void Cognitive_processor::set_auditory_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!auditory_busy_ptr);
	auditory_busy_ptr = thread_ptr;
	auditory_busy = true;
}

void Cognitive_processor::set_manual_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!manual_busy_ptr);
	manual_busy_ptr = thread_ptr;
	manual_busy = true;
}

void Cognitive_processor::set_vocal_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!vocal_busy_ptr);
	vocal_busy_ptr = thread_ptr;
	vocal_busy = true;
}

void Cognitive_processor::set_ltm_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!ltm_busy_ptr);
	ltm_busy_ptr = thread_ptr;
	ltm_busy = true;
}

void Cognitive_processor::set_task_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!task_busy_ptr);
	task_busy_ptr = thread_ptr;
	task_busy = true;
}

void Cognitive_processor::set_high_level_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!high_level_busy_ptr);
	high_level_busy_ptr = thread_ptr;
	high_level_busy = true;
}

void Cognitive_processor::set_cognitive_busy(GOMS_Execution_Thread * thread_ptr)
{
	Assert(!cognitive_busy_ptr);
	cognitive_busy_ptr = thread_ptr;
	cognitive_busy = true;
}

void Cognitive_processor::handle_event(const Cognitive_Visual_Operation_Complete_event *)
{
	if(visual_busy_ptr) {
		visual_busy_ptr->pending_operation_complete();
		}
	visual_busy_ptr = 0;
	visual_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_Auditory_Operation_Complete_event *)
{
	if(auditory_busy_ptr) {
		auditory_busy_ptr->pending_operation_complete();
		}
	auditory_busy_ptr = 0;
	auditory_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_Manual_Operation_Complete_event *)
{
	if(manual_busy_ptr) {
		manual_busy_ptr->pending_operation_complete();
		}
	manual_busy_ptr = 0;
	manual_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_Vocal_Operation_Complete_event *)
{
	if(vocal_busy_ptr) {
		vocal_busy_ptr->pending_operation_complete();
		}
	vocal_busy_ptr = 0;
	vocal_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_LTM_Operation_Complete_event *)
{
	if(ltm_busy_ptr) {
		ltm_busy_ptr->pending_operation_complete();
		}
	ltm_busy_ptr = 0;
	ltm_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_Task_Operation_Complete_event *)
{
	if(task_busy_ptr) {
		task_busy_ptr->pending_operation_complete();
		}
	task_busy_ptr = 0;
	task_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_High_Level_Operation_Complete_event *)
{
	if(high_level_busy_ptr) {
		high_level_busy_ptr->pending_operation_complete();
		}
	high_level_busy_ptr = 0;
	high_level_busy = false;
}

void Cognitive_processor::handle_event(const Cognitive_Operation_Complete_event * ptr)
{
	if(cognitive_busy_ptr) {
		Assert(ptr->thread_ptr == cognitive_busy_ptr);
		cognitive_busy_ptr->pending_operation_complete();
		}
	cognitive_busy_ptr = 0;
	cognitive_busy = false;
}

// reset all the processor busy states
void Cognitive_processor::initialize_processor_busy_states()
{
	visual_busy_ptr = 0;
	visual_busy = false;
	auditory_busy_ptr = 0;
	auditory_busy = false;
	manual_busy_ptr = 0;
	manual_busy = false;
	vocal_busy_ptr = 0;
	vocal_busy = false;
	ltm_busy_ptr = 0;
	ltm_busy = false;
	task_busy_ptr = 0;
	task_busy = false;
	high_level_busy_ptr = 0;
	high_level_busy = false;
	cognitive_busy_ptr = 0;
	cognitive_busy = false;
}

// Following are services for the rest of the system

// update the containers of the salient objects tested by the Exists predicate
void Cognitive_processor::add_salient_object(const Symbol& add_object)
{
	salient_objects.insert(add_object);
}
void Cognitive_processor::remove_salient_object(const Symbol& remove_object)
{
	salient_objects.erase(remove_object);
}

void Cognitive_processor::schedule_event(Event * p) 
{
	Coordinator::get_instance().schedule_event(p);
}

void Cognitive_processor::call_with_event(Event & p) 
{
	Coordinator::get_instance().call_with_event(p);
}


// Services to be used within this class

void Cognitive_processor::send_WM_store(const Symbol& property_name, const Symbol& property_value)
{		
	WM_Store_Tag_event msg (get_time(), get_human_ptr()->get_wm_ptr(), property_name, property_value);
	Coordinator::get_instance().call_with_event(msg);

//	if(Debug_out) Debug_out << "wm store completed" << endl;
}

void Cognitive_processor::send_WM_display()
{
	WM_Display_Request_event msg (get_time(), get_human_ptr()->get_wm_ptr());	
	Coordinator::get_instance().call_with_event(msg);
}


void Cognitive_processor::output_current_execution_state() const
{
//	main_thread_ptr->print_current_execution_state();
}


void Cognitive_processor::output_log(const Symbol_list_t& items) const
{
	Normal_out << get_time() << ' ' << get_human_ptr()->get_name() << " LOG " ;
	for (Symbol_list_t::const_iterator itr = items.begin();
			itr != items.end(); itr++) {
		Normal_out << (*itr) << ' ';
		}
	Normal_out << endl;
}

string Cognitive_processor::cogproc_info() const
{
	ostringstream oss;
	oss << get_time() << ' ' << get_human_ptr()->get_name();
	return oss.str();
}


// return true if there is a non-interrupt thread that is not waiting for an operation
// to be complete - if there is one, then we want to stop if in single-step mode
// or print the output if step output is desired
bool Cognitive_processor::single_step_should_be_done() const
{
 for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
	if(!((*it)->check_operations_pending()))
		return true;
	}
	return false;
}

/*
void Cognitive_processor::pause_and_then_single_step()
{
   if(disable_pause_and_single_step)
   {
      return;
   }
		// wait for some input
		Normal_out << processor_info() << ": Hit a key to continue in single-step mode" << endl;
		cin.get();
//		single_step = true;
}
*/

// break accessors

// ask each thread whether it has a break step next, return true if there is one
bool Cognitive_processor::is_next_step_break_set() const
{
 for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
	if((*it)->get_next_step_break_state())
		return true;
	}
	return false;
}

// these apply to the main thread only
bool Cognitive_processor::get_main_next_step_break_state() const
{
	if(!main_thread_ptr)
		throw Glean_user_error("Attempt to access break state in non-existent main thread");
	return main_thread_ptr->get_next_step_break_state();
}

void Cognitive_processor::set_main_next_step_break_state(bool set_break)
{
	if(!main_thread_ptr)
		throw Glean_user_error("Attempt to set break state in non-existent main thread");
	main_thread_ptr->set_next_step_break_state(set_break);
}

// these apply to the main thread only
// accessors for GOMSL source file information
const string& Cognitive_processor::get_main_current_step_file_name() const
{
	if(!main_thread_ptr)
		throw Glean_user_error("Attempt to access step file name in non-existent main thread");
	return main_thread_ptr->get_main_current_step_file_name();
}

int Cognitive_processor::get_main_current_step_line_number() const
{
	if(!main_thread_ptr)
		throw Glean_user_error("Attempt to access step line number in non-existent main thread");
	return main_thread_ptr->get_main_current_step_line_number();
}





// these functions work with steps identified in terms of goal and step label
bool Cognitive_processor::is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label)
{
	return goms_model_sptr->is_step_name_valid(goal_action, goal_object, step_label);
}

// an exception is thrown if the step name is invalid
bool Cognitive_processor::get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{
	return goms_model_sptr->get_break_state(goal_action, goal_object, step_label);
}

void Cognitive_processor::set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state)
{
	goms_model_sptr->set_break_state(goal_action, goal_object, step_label, state);
}

// these functions work with steps names in a human-readable form defined internally
// return a list of step names in a standard human readable form
const std::list<std::string>& Cognitive_processor::get_step_names() const
{
	return goms_model_sptr->get_step_names();
}

bool Cognitive_processor::is_step_name_valid(const std::string& step_name)
{
	return goms_model_sptr->is_step_name_valid(step_name);
}

// these functions take the step name in the same form as provided
// an exception is thrown if the step name is invalid
bool Cognitive_processor::get_break_state(const std::string& break_name) const
{
	return goms_model_sptr->get_break_state(break_name);
}

// these functions take the step name in the form of a file name and line number
// an exception is thrown if the step name is invalid
void Cognitive_processor::set_break_state(const std::string& break_name, bool state)
{
	goms_model_sptr->set_break_state(break_name, state);
}

bool Cognitive_processor::get_break_state(const string& file_name, int line_number) const
{
	return goms_model_sptr->get_break_state(file_name, line_number);
}
void Cognitive_processor::set_break_state(const string& file_name, int line_number, bool state)
{
	goms_model_sptr->set_break_state(file_name, line_number, state);
}

void Cognitive_processor::print_step_names() const
{
	bool print_comma = false;
	if(error_thread_ptr) {
		Normal_out << error_thread_ptr->get_thread_info() 
			<< ":Step " << error_thread_ptr->get_current_step_name();
		print_comma = true;
		}
	if(interrupt_thread_ptr) {
		if(print_comma)
			Normal_out << ", ";
		Normal_out << interrupt_thread_ptr->get_thread_info() 
			<< ":Step " << interrupt_thread_ptr->get_current_step_name();
		print_comma = true;
		}
	for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
		if(print_comma)
			Normal_out << ", ";
		Normal_out << (*it)->get_thread_info()
				<< ":Step " << (*it)->get_current_step_name();
		if(it == thread_list.begin()) 
			print_comma = true;
		}
}

void Cognitive_processor::print_current_goal_and_step_names() const
{
	for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
			Normal_out << cogproc_info() << ' ' << (*it)->get_thread_info()
				<< ":Goal " << (*it)->get_current_goal() << ":Step " 
				<< (*it)->get_current_step_name() << endl;
		}
}

void Cognitive_processor::display_steps() const
{
	if(error_thread_ptr) {
		Normal_out << error_thread_ptr->get_thread_info() << ": ";
		error_thread_ptr->display_step();
		}
	if(interrupt_thread_ptr && interrupt_thread_ptr->submethods_in_progress()) {
		Normal_out << interrupt_thread_ptr->get_thread_info() << ": ";
		interrupt_thread_ptr->display_step();
		}
	for(GOMS_thread_list_t::const_iterator it = thread_list.begin(); it != thread_list.end(); ++it) {
			Normal_out << (*it)->get_thread_info() << ": ";
			(*it)->display_step();
		}
}


void Cognitive_processor::show_all_processor_status() const
{
	Normal_out << get_human_ptr()->get_visual_ptr()->get_name() << ": ";
	show_processor_status(visual_busy_ptr);

	Normal_out << get_human_ptr()->get_auditory_ptr()->get_name() << ": ";
	show_processor_status(auditory_busy_ptr);

	Normal_out << get_human_ptr()->get_manual_ptr()->get_name() << ": ";
	show_processor_status(manual_busy_ptr);

	Normal_out << get_human_ptr()->get_vocal_ptr()->get_name() << ": ";
	show_processor_status(vocal_busy_ptr);

	Normal_out << get_human_ptr()->get_ltm_ptr()->get_name() << ": ";
	show_processor_status(ltm_busy_ptr);

	Normal_out << get_human_ptr()->get_task_ptr()->get_name() << ": ";
	show_processor_status(task_busy_ptr);

	Normal_out << get_human_ptr()->get_task_ptr()->get_name() << ": ";
	show_processor_status(high_level_busy_ptr);

	Normal_out << get_name() << ": ";
	show_processor_status(cognitive_busy_ptr);
}


void Cognitive_processor::show_processor_status(GOMS_Execution_Thread * busy_ptr) const
{
	if(busy_ptr)
		Normal_out << "Busy: " << busy_ptr->get_thread_info();
	else
		Normal_out << "Free";
	Normal_out << endl;
}
