#include "Model.h"
#include "View_base.h"

#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_exceptions.h"
#include "Assert_throw.h"
#include "Exception.h" 

#include "Coordinator.h"
#include "Human_processor.h"	

// these subprocessors should be hidden behind Human_processor's interface!
#include "Cognitive_processor.h"
#include "Manual_processor.h"
#include "Vocal_processor.h"
#include "Visual_processor.h"
#include "Auditory_processor.h"
#include "Task_processor.h"
#include "Visual_Recorders.h"
#include "Auditory_Recorders.h"
#include "Manual_Recorders.h"
#include "WM_Recorders.h"
#include "Vocal_Recorders.h"
#include "Cognitive_Recorders.h"
#include "Workload_Recorders.h"
#include "Device_Processor.h"
#include "Device_base.h"


#include <string>
#include <cassert>
#include <iostream>
#include <functional>

using std::string;
using std::endl;
using std::mem_fun;
using std::set;


// The constructor for the Model creates and connects the objects in the simulation
Model::Model(Device_base *device_ptr_):
	device_ptr(device_ptr_), device_processor_ptr(0), human_ptr(0), method_profiler(0), 
	gomsl_filename_set(false), compiled(false), initialized(false), running(false),
	trace_all(false)
//	, trace_device(false)
{	
	create_human_processor();	
}

/*
void Model::connect_device()
{
	Assert(device_ptr);
	Assert(!device_processor_ptr);
	Assert(human_ptr);
	device_processor_ptr = new Device_processor("Device_Interface");
	device_processor_ptr->connect(device_ptr);
	device_ptr->connect(device_processor_ptr);
	device_processor_ptr->connect(human_ptr);
	human_ptr->connect(device_processor_ptr);
}
*/

// the Device_base has already been connected to a Device_processor
void Model::connect_device_to_human(Device_base * dev_base_ptr)
{
	Assert(dev_base_ptr);
	device_ptr = dev_base_ptr;
	Assert(!device_processor_ptr);
	device_processor_ptr = device_ptr->get_device_proc_ptr();
	Assert(device_processor_ptr);	// should already be connected

	Assert(human_ptr);
	device_processor_ptr->connect(human_ptr);
	human_ptr->connect(device_processor_ptr);
}

void Model::create_human_processor()
{
	human_ptr = new Human_processor("Human");
	
	// set debugging mode from single Model::control variable
//	human_ptr->get_cognitive_ptr()->set_single_step(single_step); // ???
	human_ptr->set_output_step_labels_only(true);
	human_ptr->set_output_step_listing(true);
	human_ptr->set_output_step_WM(true);
	
	
	// connect the devices to the humans
//	Assert(device_processor_ptr);
//	human_ptr->connect(device_processor_ptr);
//	device_processor_ptr->connect(human_ptr);
	// Visual/Auditory, Manual/Vocal will be connected to/from the device internally

	set_trace_all(false);	// turn off all tracing
	
	// Attach the method_profiler
//	method_profiler = new Cognitive_Method_Profiler;
	method_profiler = new Workload_Method_Profiler;
	human_ptr->add_to_recorder_list(method_profiler);
}

// destroy the model
// Views are assumed to have been removed before hand, because
// their existence relative to the model is unclear
// the device_ptr was created by caller, so is deleted by caller
Model::~Model()
{
	delete human_ptr;
//	delete device_processor_ptr;	// created elsewhere
	delete method_profiler;
}

const string& Model::get_gomsl_filename() const
{
	return gomsl_filename;
}

void Model::set_gomsl_filename(const std::string& filename) 	
{
	gomsl_filename = filename; 
	gomsl_filename_set = true; 
}

// Compile is supposed to reset the model so that it is ready to run
// but views will only be cleared when it is initialized
// return true if compilation was successful, false if not
bool Model::compile()
{
	Assert(gomsl_filename_set);
	compiled = false;
	initialized = false;
	running = false;
		
	Normal_out << "\nCompiling " << gomsl_filename << endl;
	compiled = human_ptr->compile(gomsl_filename);
	
	if(compiled) {
		Normal_out << gomsl_filename << " compiled and executable" << endl;
		}
	else {
		Normal_out << "Model is invalid due to errors." << endl;
		}
	return compiled;
}

bool Model::initialize()
{
	Assert(device_ptr);
	clear_all_views();
	Coordinator::get_instance().initialize();
	initialized = true;
	running = true;
	Normal_out << "*** Simulation is starting ***" << endl;
   return true;
}

long Model::get_time() const
{
	return Coordinator::get_instance().get_time();
}


// return true if simulation is still in run state, false if not (it terminated)
bool Model::run_time(long duration)
{
	Assert(compiled);
	Assert(method_profiler);
	
	if(!initialized) {
      initialize();
		}
	
	if(running && Coordinator::get_instance().is_finished()) {
		initialized = false;
		running = false;
		Normal_out << "*** Simulation is finished ***" << endl;
		return false;
		}
	Coordinator::get_instance().run_for(duration);

	if(Coordinator::get_instance().is_finished()) {
		// don't run any more past the finished state
		initialized = false;
		running = false;
		}

	else if(Coordinator::get_instance().is_paused()) {
		// leave initialized and running, but return false
		return false;
		}
	// here if Coordinator simply timed out
	return running;
}

// check!!
// run until it stops and returns or halts; should always return false
/* bool Model::run()
{
	return run_time(0);
}
*/
	
// stop the simulation; it is no longer initialized or running,
// but it stays in the compiled state
bool Model::stop()
{
	Coordinator::get_instance().stop(); // unconditionally terminate
	initialized = false;
	running = false;
	return running;
}

void Model::output_workload_reports()
{
	method_profiler->report();
}


/* View maintenance and notification functions */
// forwarded to human processor to attach to correct subprocessors

void Model::add_visual_physical_view(View_base * view_ptr)
{
	human_ptr->get_Visual_ptr()->add_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::remove_visual_physical_view(View_base * view_ptr)
{
	view_ptr->detach_from_all();
	human_ptr->get_Visual_ptr()->remove_view(view_ptr);
	view_ptrs.erase(view_ptr);
}

void Model::add_wm_view(View_base * view_ptr)
{
	human_ptr->get_Wm_ptr()->add_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::remove_wm_view(View_base * view_ptr)
{
	view_ptr->detach_from_all();
	human_ptr->get_Wm_ptr()->remove_view(view_ptr);
	view_ptrs.erase(view_ptr);
}

void Model::add_task_view(View_base * view_ptr)
{
	human_ptr->get_Task_ptr()->add_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::remove_task_view(View_base * view_ptr)
{
	view_ptr->detach_from_all();
	human_ptr->get_Task_ptr()->remove_view(view_ptr);
	view_ptrs.erase(view_ptr);
}




/* View functions not implemented yet */
/*-------------------------------
void Model::add_visual_sensory_view(View_base * view_ptr)
{
	human_ptr->add_visual_sensory_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::add_visual_perceptual_view(View_base * view_ptr)
{
	human_ptr->add_visual_perceptual_view(view_ptr);
	view_ptrs.insert(view_ptr);
}
	
void Model::remove_visual_physical_view(View_base * view_ptr)
{
	human_ptr->remove_visual_physical_view(view_ptr);
	view_ptrs.erase(view_ptr);
}
	
void Model::remove_visual_sensory_view(View_base * view_ptr)
{
	human_ptr->remove_visual_sensory_view(view_ptr);
	view_ptrs.erase(view_ptr);
}

void Model::remove_visual_perceptual_view(View_base * view_ptr)
{
	human_ptr->remove_visual_perceptual_view(view_ptr);
	view_ptrs.erase(view_ptr);
}
	
void Model::add_auditory_physical_view(View_base * view_ptr)
{
	human_ptr->add_auditory_physical_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::add_auditory_sensory_view(View_base * view_ptr)
{
	human_ptr->add_auditory_sensory_view(view_ptr);
	view_ptrs.insert(view_ptr);
}

void Model::add_auditory_perceptual_view(View_base * view_ptr)
{
	human_ptr->add_auditory_perceptual_view(view_ptr);
	view_ptrs.insert(view_ptr);
}
	
void Model::remove_auditory_physical_view(View_base * view_ptr)
{
	human_ptr->remove_auditory_physical_view(view_ptr);
	view_ptrs.erase(view_ptr);
}

void Model::remove_auditory_sensory_view(View_base * view_ptr)
{
	human_ptr->remove_auditory_sensory_view(view_ptr);
	view_ptrs.erase(view_ptr);
}

void Model::remove_auditory_perceptual_view(View_base * view_ptr)
{
	human_ptr->remove_auditory_perceptual_view(view_ptr);
	view_ptrs.erase(view_ptr);
}



// tell all views to detach themselves from all processors,then empty the list
void Model::remove_all_views()
{
	for(set<View_base *>::iterator it = view_ptrs.begin(); it != view_ptrs.end(); it++)
		(*it)->detach_from_all();

	view_ptrs.clear();
}

-------------------------------*/

// use this function to add a view that is not specific to a particular processor
// such as the view used for Normal output
void Model::add_view(View_base * view_ptr)
{
	view_ptrs.insert(view_ptr);
}

 // tell this view to detach itself and then take it out of the list
void Model::remove_view(View_base * view_ptr)
{
	view_ptr->detach_from_all();
	view_ptrs.erase(view_ptr);
}

// tell all views to clear themselves
void Model::clear_all_views()
{
	for(set<View_base *>::iterator it = view_ptrs.begin(); it != view_ptrs.end(); it++)
		(*it)->clear();
}

/* trace functions not implemented yet */
// tracing accessors
void Model::set_trace_all(bool trace_)
{
	trace_all = trace_;
//	human_ptr->set_trace_all(trace_);
//	set_trace_device(trace_);
}


bool Model::get_trace_visual() const
{
	return human_ptr->get_trace_visual();
}

void Model::set_trace_visual (bool trace_)
{
	human_ptr->set_trace_visual(trace_);
}

void Model::set_trace_auditory (bool trace_)
{
	human_ptr->set_trace_auditory(trace_);
}

bool Model::get_trace_auditory() const
{
	return human_ptr->get_trace_auditory();
}

void Model::set_trace_cognitive (bool trace_)
{
	human_ptr->set_trace_cognitive(trace_);
}

bool Model::get_trace_cognitive() const
{
	return human_ptr->get_trace_cognitive();
}


void Model::set_trace_WM (bool trace_)
{
	human_ptr->set_trace_WM(trace_);
}

bool Model::get_trace_WM() const
{
	return human_ptr->get_trace_WM();
}

void Model::set_trace_task (bool trace_)
{
	human_ptr->set_trace_task(trace_);
}

bool Model::get_trace_task() const
{
	return human_ptr->get_trace_task();
}

void Model::set_trace_manual (bool trace_)
{
	human_ptr->set_trace_manual(trace_);
}

bool Model::get_trace_manual() const
{
	return human_ptr->get_trace_manual();
}

void Model::set_trace_vocal (bool trace_)
{
	human_ptr->set_trace_vocal(trace_);
}

bool Model::get_trace_vocal() const
{
	return human_ptr->get_trace_vocal();
}

void Model::set_trace_highlevel (bool trace_)
{
	human_ptr->set_trace_highlevel(trace_);
}

bool Model::get_trace_highlevel() const
{
	return human_ptr->get_trace_highlevel();
}

void Model::set_trace_device (bool trace_)
{
	device_processor_ptr->set_trace(trace_);
}

bool Model::get_trace_device() const
{
	return device_processor_ptr->get_trace();
}





bool Model::get_output_compiler_messages() const
{
	return human_ptr->get_output_learning_calculation();
}

void Model::set_output_compiler_messages(bool flag)
{
	human_ptr->set_output_learning_calculation(flag);
}

bool Model::get_output_compiler_details() const
{
	return human_ptr->get_output_model_contents();
}

void Model::set_output_compiler_details(bool flag)
{
	human_ptr->set_output_model_contents(flag);
}

bool Model:: was_operator_executed() const
{
	return human_ptr->was_operator_executed();
}

void Model:: reset_operator_executed()
{
	human_ptr->reset_operator_executed();
}

bool Model:: get_output_run_messages() const
{
	return human_ptr->get_output_step_labels_only();
}

void Model::set_output_run_messages(bool flag)
{
	human_ptr->set_output_step_labels_only(flag);
}

bool Model::get_output_run_details() const
{
	return human_ptr->get_output_step_listing();
}

void Model::set_output_run_details(bool flag)
{
	human_ptr->set_output_step_listing(flag);
}

bool Model:: get_output_run_memory_contents() const
{
	return human_ptr->get_output_step_WM();
}

void Model::set_output_run_memory_contents(bool flag)
{
	human_ptr->set_output_step_WM(flag);
}

void Model::output_steps() const
{
	human_ptr->output_steps();
}

void Model::output_working_memory_contents() const
{
	human_ptr->output_working_memory_contents();
}

void Model::output_tag_store_contents() const
{
	human_ptr->output_tag_store_contents();
}

void Model::output_object_store_contents() const
{
	human_ptr->output_object_store_contents();
}

void Model::output_visual_memory_contents() const
{
	human_ptr->output_visual_memory_contents();
}

void Model::output_auditory_memory_contents() const
{
	human_ptr->output_auditory_memory_contents();
}

void Model::output_task_memory_contents() const
{
	human_ptr->output_task_memory_contents();
}

void Model::output_ltm_memory_contents() const
{
	human_ptr->output_ltm_memory_contents();
}



// break accessors
bool Model::get_breaks_enabled() const
{
	return human_ptr->get_breaks_enabled();
}

void Model::set_breaks_enabled(bool state)
{
	human_ptr->set_breaks_enabled(state);
}

// this checks the next step in all threads
// only check if breaks are enabled
bool Model::is_next_step_break_set() const
{
	return (human_ptr->get_breaks_enabled()) ? 
		human_ptr->is_next_step_break_set() : false;
}

// these apply to the main thread only
bool Model::get_main_next_step_break_state() const
{
	return human_ptr->get_main_next_step_break_state();
}

void Model::set_main_next_step_break_state(bool state)
{
	human_ptr->set_main_next_step_break_state(state);
}

// these functions work with steps identified in terms of goal and step label
bool Model::is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label)
{
	return human_ptr->is_step_name_valid(goal_action, goal_object, step_label);
}

// an exception is thrown if the step name is invalid
bool Model::get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{
	return human_ptr->get_break_state(goal_action, goal_object, step_label);
}

void Model::set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state)
{
	human_ptr->set_break_state(goal_action, goal_object, step_label, state);
}

// these functions work with steps names in a human-readable form defined internally
// return a list of step names in a standard human readable form
const std::list<std::string>& Model::get_step_names() const
{
	return human_ptr->get_step_names();
}

bool Model::is_step_name_valid(const std::string& step_name)
{
	return human_ptr->is_step_name_valid(step_name);
}

// these functions take the step name in the same form as provided
// an exception is thrown if the step name is invalid
bool Model::get_break_state(const std::string& break_name) const
{
	return human_ptr->get_break_state(break_name);
}

void Model::set_break_state(const std::string& break_name, bool state)
{
	human_ptr->set_break_state(break_name, state);
}

// these functions take the step name in the form of a file name and line number
// an exception is thrown if the step name is invalid
bool Model::get_break_state(const string& file_name, int line_number) const
{
	return human_ptr->get_break_state(file_name, line_number);
}

void Model::set_break_state(const string& file_name, int line_number, bool state)
{
	human_ptr->set_break_state(file_name, line_number, state);
}

// accessors for GOMSL source file information
const string& Model::get_main_current_step_file_name() const
{
	return human_ptr->get_main_current_step_file_name();
}

int Model::get_main_current_step_line_number() const
{
	return human_ptr->get_main_current_step_line_number();
}

string Model::get_learning_report() const
{
    return human_ptr->get_learning_report();
}

string Model::get_recorder_report() const
{
    return method_profiler->get_report();
}



/*
// return whether a break step was executed
bool Model:: was_break_step_executed() const
{
	return human_ptr->was_break_step_executed();
}
*/



	

/*-----------------------

bool Model::get_fault_enabled() const
{
	return human_ptr->get_fault_enabled();
}

void Model::set_fault_enabled(bool state)
{
	return human_ptr->set_fault_enabled(state);
}
-----------------*/

/*
void Model::set_fault_state(const Symbol& fault_name, bool state)
{
	return human_ptr->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->set_step_fault_state(fault_name, state);
}

bool Model::get_fault_state(const Symbol& fault_name) const
{
	return human_ptr->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_step_fault_state(fault_name);
}
*/

/*-----------------------
const Symbol_list_t& Model::get_fault_step_names() const
{
	return human_ptr->get_fault_step_names();
}
-----------------*/

void Model::set_device_parameter_string(const string& str_)
{
	Assert(device_ptr);
	device_ptr->set_parameter_string(str_);
}

string Model::get_device_parameter_string() const
{
	Assert(device_ptr);
	return device_ptr->get_parameter_string();
}

