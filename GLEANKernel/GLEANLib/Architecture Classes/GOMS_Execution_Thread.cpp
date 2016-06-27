#include "GOMS_Execution_Thread.h"
#include "Coordinator.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "Glean_exceptions.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include <iostream>
#include <sstream>
//#include <cassert>

using namespace std;


GOMS_Execution_Thread::GOMS_Execution_Thread(const Symbol& name_, int id_, Cognitive_processor * cog_ptr_,
	GOMS_Model_Sptr_t& current_model_sptr_, const GOMS_Goal& starting_goal_, Thread_type_e thread_type_) :
	name(name_), id(id_), cog_ptr(cog_ptr_), current_model_sptr(current_model_sptr_), 
	starting_goal(starting_goal_), thread_type(thread_type_),
	operations_pending(0), runnable(false), advance_next_step(true), 
	operator_executed(false) //, break_on_step(false)
{
	ostringstream oss;
	oss << name << '(' << id << ')';
	thread_info = oss.str();
}


void GOMS_Execution_Thread::start_execution()
{
	operations_pending = 0;
	start_at_goal(starting_goal);
}

void GOMS_Execution_Thread::start_at_goal(const GOMS_Goal& starting_goal_)
{
	// set the starting goal to the supplied value in case they are different
	starting_goal = starting_goal_;
	current_goal = starting_goal;

	// this logic will duplicate output if both Normal_out and Trace_out include the same stream
	if(cog_ptr->get_output_step_labels_only() || cog_ptr->get_output_step_listing() ) {
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Starting to accomplish " << current_goal << endl;
		}
	if(Trace_out && cog_ptr->get_trace()) {
		Trace_out << cog_ptr->cogproc_info() << thread_info 
		<< ": Starting to accomplish " << current_goal << endl;
		}
	// send an immediate event to the recorders 
	Cognitive_Accomplish_Goal_event msg
		(cog_ptr->get_time(), 0, this, current_goal);
	cog_ptr->broadcast_to_recorders(&msg);

	current_procedure_sptr = current_model_sptr->get_procedure(current_goal);
	if(current_procedure_sptr) {
		current_error_goal = current_procedure_sptr->error_goal;
		current_step_itr = current_procedure_sptr->get_first_step();
		runnable = true;		// can now execute steps in the thread
		return;
		}
	else 
		throw Glean_exception(cog_ptr, string("Error: Procedure not found for ") + current_goal.get_string());
}


bool GOMS_Execution_Thread::is_next_step_break_set() const
{
	if (current_procedure_sptr == 0)
		return false;
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	return (step_sptr->break_flag);
}


void GOMS_Execution_Thread::set_next_step_break_state(bool state)
{
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	step_sptr->break_flag = state;
}

bool GOMS_Execution_Thread::get_next_step_break_state() const
{
	if (current_procedure_sptr == 0)
		return false;
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	return step_sptr->break_flag;
}

// accessors for GOMSL source file information
const string& GOMS_Execution_Thread::get_main_current_step_file_name() const
{
	Assert(current_procedure_sptr);
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	return step_sptr->get_file_name();
}

int GOMS_Execution_Thread::get_main_current_step_line_number() const
{
	if (current_procedure_sptr == 0)
		return -1;
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	return step_sptr->get_line_number();
}



// return true if operations are still pending
bool GOMS_Execution_Thread::check_operations_pending() const
{
	return bool(operations_pending);
}

void GOMS_Execution_Thread::pending_operation_complete()
{
	if(operations_pending)	// don't allow to be negative
		operations_pending--;
}

void GOMS_Execution_Thread::new_operation_pending()
{
	operations_pending++;
}
	
// return true if the thread is still runnable, false if terminated
// state variables set to show whether a significant operator was executed, or if this step should be broken
bool GOMS_Execution_Thread::execute_next_step()
{	
	operator_executed = false;
//	break_on_step = false;

	// if not runnable, or there is no current procedure, return false - execution is complete.
	if (!runnable || current_procedure_sptr == 0)
		return runnable;
	
	// if we are waiting for something, return immediately - check on the next step message
	// we are still running, but nothing was executed
	if(operations_pending) {
		if(Trace_out && cog_ptr->get_trace()) {
			Trace_out << "Thread " << thread_info << " waiting for last operation to complete" << endl;
			}
		return runnable;
		}

	// if we are at the end of the procedure, something's wrong - we were supposed to be runnable
	Assert(current_step_itr != current_procedure_sptr->get_last_step());
//	if (current_step_itr == current_procedure_sptr->get_last_step())
//		throw Glean_internal_error(cog_ptr, 
//			string("Running off end of procedure for ") + current_goal.get_string());
	
	// get the step
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;		
	
	// can we execute the step? return if not, leave everything alone
	// but we will try again on the next step event
	// collect the processor requests for the step
	Processor_Request pr;
	step_sptr->request_processors(pr);
	if(!pr.evaluate(cog_ptr)) {
		if(Trace_out && cog_ptr->get_trace()) {
			Trace_out << "Thread " << thread_info << " next step has to wait for processor(s) to be free" << endl;
			}
		return runnable;
		}

	// this switch is a kludge installed to suppress not in focus events from wm  - dk 5/4/00
	if(thread_type == INTERRUPT)
		cog_ptr->get_human_ptr()->get_Wm_ptr()->set_focus_message_desired(false);

	// execute each operator in the step
	advance_next_step = true;	// advance unless cancelled
	GOMS_Operator_Sptr_list_t::iterator op_itr;
	for (op_itr = step_sptr->operator_sptr_list.begin();
		 op_itr != step_sptr->operator_sptr_list.end();
		 op_itr++) 
		(*op_itr)->execute(cog_ptr, this);
	
	// reset the kludged flag
	if(thread_type == INTERRUPT)
		cog_ptr->get_human_ptr()->get_Wm_ptr()->set_focus_message_desired(true);
		
	// do the output *after* the step is executed but before advancing the step iterator
	// set a flag that this step output might be of interest to the user 
	// if this is either a non-interrupt thread or a thread (even interrupt) deeper than than the top level
	bool step_output_interesting = thread_type != INTERRUPT || !execution_stack.empty();
	
	// if step output, and a non-interrupt thread or interrupt thread deeper than the top level, show the step output
	if((cog_ptr->get_output_step_labels_only() || cog_ptr->get_output_step_listing()) && step_output_interesting) {
		// show contents of WM
		if(cog_ptr->get_output_step_WM()) {
			cog_ptr->output_step_WM_info();
			}
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info;
		
		if(cog_ptr->get_output_step_labels_only())
			Normal_out << ":Step " << step_sptr->label << " executed" << endl;
		else if(cog_ptr->get_output_step_listing()) {
			Normal_out << ": executed: ";
			step_sptr->display(Normal_out);
			}
		}
		
	if(Trace_out && cog_ptr->get_trace() && step_output_interesting) {
		// show contents of WM
		cog_ptr->get_human_ptr()->get_Wm_ptr()->display_contents(Trace_out);
		Trace_out << cog_ptr->cogproc_info() << ' ' << thread_info << ":Step " << step_sptr->label
			<< " executed" << endl;
		}

	// update execution state variables
	if (current_procedure_sptr == 0)	// if terminating due to rga
		runnable = false;
	else if (advance_next_step)			// if still runnable, advance to the next step unless cancelled.
		current_step_itr++;
	
	if(step_output_interesting)
		operator_executed = true;	// something of interest was executed
		
//	if(step_sptr->break_flag)
//		break_on_step = true;

	return runnable;
}

// return true if non-empty stack an accomplish_goal operator has been executed
bool GOMS_Execution_Thread::submethods_in_progress() const
{
	return !(execution_stack.empty());
}

const Symbol& GOMS_Execution_Thread::get_current_step_name() const
{
	return (*current_step_itr)->label;
}

void GOMS_Execution_Thread::display_step() const
{
	Normal_out << current_goal << ':';
	GOMS_Step_Sptr_t step_sptr = *current_step_itr;
	step_sptr->display(Normal_out);
}

bool GOMS_Execution_Thread::push_accomplish_goal(const GOMS_Accomplish_Goal_Operator& ag_op)
{
	// push current state onto the stack, and make the new goal the current one
	GOMS_Execution_Stack_Frame frame(current_goal, current_error_goal, current_procedure_sptr, current_step_itr, advance_next_step);
	execution_stack.push_back(frame);
	current_goal = ag_op.goal;
	
	// send an immediate event to the recorders 
	Cognitive_Accomplish_Goal_event msg
		(cog_ptr->get_time(), 0, this, current_goal);
	cog_ptr->broadcast_to_recorders(&msg); 

	if(cog_ptr->get_output_step_labels_only()) {
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Accomplish " << current_goal << endl;
		}
	if(Trace_out && cog_ptr->get_trace()) {
		Trace_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Accomplish " << current_goal << endl;
		}

	current_procedure_sptr = current_model_sptr->get_procedure(current_goal);
	if(current_procedure_sptr) {
		current_step_itr = current_procedure_sptr->get_first_step();
		// copy the values of the using parameters
		copy_psdo_params(ag_op.arg_list, current_procedure_sptr->param_list);
		// if there is an error_goal specified, use it instead of the current one
		if(current_procedure_sptr->error_goal != GOMS_Goal()) {
			current_error_goal = current_procedure_sptr->error_goal;
			}
		advance_next_step = false;	// don't advance to the next step in order
		return true;	// do another operator or step.
		}
	else 
		throw Glean_exception(cog_ptr, 
			string("Error: Procedure not found for ") + current_goal.get_string());
}


void GOMS_Execution_Thread::copy_psdo_params(
	GOMS_Argument_Sptr_List_t arg_list, GOMS_Argument_Sptr_List_t param_list)
{
	// check size of lists first
	if (arg_list.size() != param_list.size())
		throw Glean_exception(cog_ptr, 
			string("Error: pseudo-argument and -parameter lists are different lengths for ") + current_goal.get_string()
			+ " Step "+ (*current_step_itr)->label.str());

	Assert(cog_ptr->get_human_ptr()->get_Wm_ptr()); // wm_processor should be defined!
	
	Symbol wm_value, tag_name;
	GOMS_Argument_Sptr_List_t::iterator arg_it;
	GOMS_Argument_Sptr_List_t::iterator param_it;
	for(arg_it = arg_list.begin(), param_it = param_list.begin();
		arg_it != arg_list.end(); arg_it++, param_it++) {
			wm_value = (*arg_it)->resolve(cog_ptr);
			tag_name = (*param_it)->get_symbol();
			// check if tag_name is already in WM
			if (Symbol() != cog_ptr->get_human_ptr()->get_Wm_ptr()->retrieve_tag_value(tag_name)) {
				string msg = string(" pseudo-parameter tag already in WM!  Will be overwritten! For ")
					+ current_goal.get_string() + " Step "+ (*current_step_itr)->label.str();
				string error_msg = string("Warning:") + msg;
				Normal_out << error_msg << endl;
				// throw Glean_exception(cog_ptr, error_msg);
				}
			cog_ptr->get_human_ptr()->get_Wm_ptr()->store_tag(tag_name, wm_value);
			}
//	wm_ptr->display_contents(if(Debug_out) Debug_out);
}

bool GOMS_Execution_Thread::pop_goal_accomplished()
{
	// delete the pseudo-parameters
	delete_psdo_params(current_procedure_sptr->param_list);
	
	// send an immediate event to the recorders 
	Cognitive_Return_Goal_Accomplished_event msg
		(cog_ptr->get_time(), 0, this, current_goal);
	cog_ptr->broadcast_to_recorders(&msg); 

	if(cog_ptr->get_output_step_labels_only()) {
		Normal_out << cog_ptr->cogproc_info()<< ' '  << thread_info << ": Return with " << current_goal << " accomplished" << endl;
//		if(processing_interrupt)
//			Normal_out << "Interrupt Goal ";
		}
	if(Trace_out && cog_ptr->get_trace()) {
		Trace_out << cog_ptr->cogproc_info()<< ' ' << thread_info << ": Return with " << current_goal << " accomplished" << endl;
		}
	
	bool result = pop_execution_stack();
	if(!result && thread_type == ERROR)
		cog_ptr->cleanup_raised_exception();
	return result;
/*
	// if the stack is empty, execution is finished, set current procedure to 0
	// and do nothing further
	if (execution_stack.empty()) {
		current_procedure_sptr = 0;
		current_goal = GOMS_Goal(); // an empty goal
		current_error_goal = GOMS_Goal();
		advance_next_step = false;
		return false;
		}

	// restore state from the stack frame at the top
	{	// a local scope for the reference to the top frame
		GOMS_Execution_Stack_Frame& frame = execution_stack.top();
		current_goal = frame.goal;
		current_error_goal = frame.error_goal;
		current_procedure_sptr = frame.procedure_sptr;
		current_step_itr = frame.step_itr;
		advance_next_step = frame.advance_next_step;
		}
	execution_stack.pop();
	return true;	// do another operator or step.
*/
}

void GOMS_Execution_Thread::delete_psdo_params(GOMS_Argument_Sptr_List_t param_list)
{
	for(GOMS_Argument_Sptr_List_t::iterator param_it = param_list.begin();
		param_it != param_list.end(); param_it++) {
			Symbol tag_name = (*param_it)->get_symbol();
			cog_ptr->get_human_ptr()->get_Wm_ptr()->erase_tag(tag_name);
			}
//	wm_ptr->display_contents(if(Debug_out) Debug_out);
}

bool GOMS_Execution_Thread::pop_execution_stack() 
{
	// if the stack is empty, execution is finished, set current procedure to 0
	// and do nothing further
	if (execution_stack.empty()) {
		current_procedure_sptr = 0;
		current_goal = GOMS_Goal(); // an empty goal
		current_error_goal = GOMS_Goal();
		advance_next_step = false;
		return false;
		}

	// restore state from the stack frame at the top
	{	// a local scope for the reference to the top frame
		GOMS_Execution_Stack_Frame& frame = execution_stack.back();
		current_goal = frame.goal;
		current_error_goal = frame.error_goal;
		current_procedure_sptr = frame.procedure_sptr;
		current_step_itr = frame.step_itr;
		advance_next_step = frame.advance_next_step;
		}
	execution_stack.pop_back();
	return true;	// do another operator or step.
}

// unwind the stack to the named goal, and then execute
// starting at whatever the current step is
void GOMS_Execution_Thread::resume_at_goal(const GOMS_Goal& resume_goal)
{
	Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": *** resume_at_goal" << endl;

	// send an immediate event to the recorders 
//	Cognitive_Abort_and_Restart_event msg
//		(cog_ptr->get_time(), 0, cog_ptr->get_broadcaster(), current_goal);
//	cog_ptr->call_with_event(msg); 

/*	if(cog_ptr->get_stop_on_abort_and_restart()) {
		Normal_out << " WM contents:" << endl;
		// display the contents here
		cog_ptr->get_human_ptr()->get_Wm_ptr()->display_contents(Normal_out);
		cog_ptr->pause_and_then_single_step();
		}
*/
	
	// empty the execution stack to leave the saved goal and step as the current ones
	unwind_to_goal(resume_goal);

	if(cog_ptr->get_output_step_labels_only()) {
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Resuming goal "
		<< resume_goal << endl;
		}
	// thread should continue executing		
}

// unwind the stack to the named goal, and then execute
// starting at whatever the current step is
void GOMS_Execution_Thread::resume_at_goal_and_step(const GOMS_Goal& resume_goal, const Symbol& resume_step)
{
	Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": *** resume_at_goal_and_step" << endl;

	// send an immediate event to the recorders 
//	Cognitive_Abort_and_Restart_event msg
//		(cog_ptr->get_time(), 0, cog_ptr->get_broadcaster(), current_goal);
//	cog_ptr->call_with_event(msg); 

/*	if(cog_ptr->get_stop_on_abort_and_restart()) {
		Normal_out << " WM contents:" << endl;
		// display the contents here
		cog_ptr->get_human_ptr()->get_Wm_ptr()->display_contents(Normal_out);
		cog_ptr->pause_and_then_single_step();
		}
*/
	
	// empty the execution stack to leave the saved goal and step as the current ones
	unwind_to_goal(resume_goal);
	// set the step to the specified one:
	goto_step_label(resume_step);	// need to set advance step?

	if(cog_ptr->get_output_step_labels_only()) {
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Resuming goal "
		<< resume_goal << endl;
		}
	// thread should continue executing		
}

// unwind the stack to the named goal, and then execute 
// starting at the first step
void GOMS_Execution_Thread::restart_at_goal(const GOMS_Goal& restart_goal)
{

//	print_current_execution_state();
	Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": *** restart_at_goal" << endl;

	// send an immediate event to the recorders 
//	Cognitive_Abort_and_Restart_event msg
//		(cog_ptr->get_time(), 0, cog_ptr->get_broadcaster(), current_goal);
//	cog_ptr->call_with_event(msg); 

/*	if(cog_ptr->get_stop_on_abort_and_restart()) {
		Normal_out << " WM contents:" << endl;
		// display the contents here
		cog_ptr->get_human_ptr()->get_Wm_ptr()->display_contents(Normal_out);
		cog_ptr->pause_and_then_single_step();
		}
*/
	
	// empty the execution stack, leaving the goal as the current one
	unwind_to_goal(restart_goal);
	// set up the first step
	current_step_itr = current_procedure_sptr->get_first_step();

	if(cog_ptr->get_output_step_labels_only()) {
		Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Restarting at goal "
		<< restart_goal << endl;
		}
	// thread should continue executing		
}

// unwind pops each goal off of the stack, deleting the pseudoparameters as it goes
void GOMS_Execution_Thread::unwind_to_top()
{
	while(true) {
		if(cog_ptr->get_output_step_labels_only()) {
			Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
		if(Trace_out && cog_ptr->get_trace()) {
			Trace_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
			
		// send an immediate event to the recorders 
		Cognitive_Return_Goal_Accomplished_event msg
			(cog_ptr->get_time(), 0, this, current_goal);
		cog_ptr->broadcast_to_recorders(&msg); 
	
		// delete the current pseudo-parameters
		delete_psdo_params(current_procedure_sptr->param_list);
		
		if(!pop_execution_stack())
			break;
	/*
		// if the stack is empty, unwinding is finished, set current procedure to 0
		if (execution_stack.empty()) {
			current_procedure_sptr = 0;
			current_goal = GOMS_Goal();	// an empty goal
			break;
			}
		// restore state from the stack frame at the top
		{	// a local scope for the reference to the top frame
			GOMS_Execution_Stack_Frame& frame = execution_stack.top();
			current_goal = frame.goal;
			current_procedure_sptr = frame.procedure_sptr;
			current_step_itr = frame.step_itr;
			advance_next_step = frame.advance_next_step;
			}
		execution_stack.pop(); 
		*/
		}

}

// unwind_to_goal pops each goal off of the stack, until it gets to the specified goal,
// deleting the pseudoparameters as it goes
void GOMS_Execution_Thread::unwind_to_goal(const GOMS_Goal& stop_at_goal)
{
	while(true) {
		// stop the unwind if the current goal is the supplied stopping goal
		if(stop_at_goal == current_goal)
			break;
		if(cog_ptr->get_output_step_labels_only()) {
			Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
		if(Trace_out && cog_ptr->get_trace()) {
			Trace_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
			
		// send an immediate event to the recorders 
		Cognitive_Return_Goal_Accomplished_event msg
			(cog_ptr->get_time(), 0, this, current_goal);
		cog_ptr->broadcast_to_recorders(&msg); 
	
		// delete the current pseudo-parameters
		delete_psdo_params(current_procedure_sptr->param_list);
	
		// if the stack is empty, we didn't find the supplied goal; a problem
		if (!pop_execution_stack()) {
			throw Glean_exception(cog_ptr,
				string("Error: Attempt to unwind to goal ") + stop_at_goal.action.str()
				+ " " + stop_at_goal.object.str() + " that was not found.");
			}
					/*
		// restore state from the stack frame at the top
		{	// a local scope for the reference to the top frame
			GOMS_Execution_Stack_Frame& frame = execution_stack.top();
			current_goal = frame.goal;
			current_procedure_sptr = frame.procedure_sptr;
			current_step_itr = frame.step_itr;
			advance_next_step = frame.advance_next_step;
			}
		execution_stack.pop(); 
		*/
		
		}
}

// this unwinds the stack at least one level, stopping at the first method that
// has an error goal specified
void GOMS_Execution_Thread::unwind_to_next_error_method()
{
	while(true) {
		if(cog_ptr->get_output_step_labels_only()) {
			Normal_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
		if(Trace_out && cog_ptr->get_trace()) {
			Trace_out << cog_ptr->cogproc_info() << ' ' << thread_info << ": Unwinding goal " << current_goal << endl;
			}
			
		// send an immediate event to the recorders 
		Cognitive_Return_Goal_Accomplished_event msg
			(cog_ptr->get_time(), 0, this, current_goal);
		cog_ptr->broadcast_to_recorders(&msg); 
	
		// delete the current pseudo-parameters
		delete_psdo_params(current_procedure_sptr->param_list);
	
		// if the stack is empty, we didn't find the appropriate goal; a problem
		if (!pop_execution_stack()) {
			throw Glean_exception(cog_ptr,
				string("Error: Attempt to unwind to next error method, but none found"));
			}
		// stop the unwind if the new method has an error goal
		if(current_error_goal != GOMS_Goal())
			break;		
		}
}

void GOMS_Execution_Thread::stay_at_step()
{
	advance_next_step = false;
}

bool GOMS_Execution_Thread::goto_step_label(const Symbol& goto_label)
{
	// scan through the procedure steps and set the current_step_itr to the first
	// step with a matching label.
	
	GOMS_Step_Sptr_list_t::iterator step_itr;
	for (step_itr = current_procedure_sptr->get_first_step();
		 step_itr != current_procedure_sptr->get_last_step();
		 step_itr++)
		if ((*step_itr)->label == goto_label)  {
			current_step_itr = step_itr;
			advance_next_step = false;	// don't advance to the next step in order
			return true;
			}
	throw Glean_exception(cog_ptr, 
			string("Error: Couldn't find step label ") + goto_label.str() + string(" for goto in Step ") 
				+ (*current_step_itr)->label.str() + string(" in procedure for ") + current_goal.get_string());
}

// return true if the request can be satisfied. E.g. if manual request, at manual is busy,
// or similar everything else, then return false - only if unrequested
bool Processor_Request::evaluate(Cognitive_processor * cog_ptr) const
{
	if(
		(visual && cog_ptr->is_visual_busy()) ||
		(auditory && cog_ptr->is_auditory_busy()) ||
		(manual && cog_ptr->is_manual_busy()) ||
		(vocal && cog_ptr->is_vocal_busy()) ||
		(ltm && cog_ptr->is_ltm_busy()) ||
		(task && cog_ptr->is_task_busy()) ||
		(cognitive && cog_ptr->is_cognitive_busy())
		)
		return false;
	else
		return true;
}
