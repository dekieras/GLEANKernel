#include "Human_processor.h"
#include "Coordinator.h"
#include "Cognitive_processor.h"
#include "WM_processor.h"
#include "Visual_processor.h"
#include "Auditory_processor.h"
#include "Manual_processor.h"
#include "Vocal_processor.h"
#include "Task_processor.h"
#include "LTM_processor.h"
#include "High_Level_processor.h"
#include "Numeric_utilities.h"
#include "Symbol_Geometry_utilities.h"
#include "Output_tee_globals.h"

#include <iostream>
//#include <typeinfo>
//#include <cassert>
//#include <DebugNew.h>
using namespace std;

//#define NDEBUG	// turn off asserts


// A human processor has a specific id, a coordinator,no container, 
// and serves as its own broadcaster. It assembles some subprocessors into the
// appropriate configuration, serving as the container and broadcaster for them.
Human_processor::Human_processor(const std::string& id) : 
	Processor(id)
{
	cognitive_ptr = new Cognitive_processor(this);
	wm_ptr = new WM_processor(this);
	visual_ptr = new Visual_processor(this);
	auditory_ptr = new Auditory_processor(this);
	manual_ptr = new Manual_processor(this);
	vocal_ptr = new Vocal_processor(this);
	ltm_ptr = new LTM_processor(this);
	task_ptr = new Task_processor(this);
	high_level_ptr = new High_Level_processor(this);
}

Human_processor::~Human_processor()
{
	delete cognitive_ptr;
	delete wm_ptr;
	delete visual_ptr;
	delete auditory_ptr;
	delete manual_ptr;
	delete vocal_ptr;
	delete ltm_ptr;
	delete task_ptr;
	delete high_level_ptr;
}

void Human_processor::initialize()
{
	Assert(cognitive_ptr);
	Assert(wm_ptr);
	Assert(visual_ptr);
	Assert(auditory_ptr);
	Assert(manual_ptr);
	Assert(vocal_ptr);
	Assert(ltm_ptr);
	Assert(task_ptr);
	Assert(high_level_ptr);
	initialize_recorders();
}

// Event interface to rest of the system
void Human_processor::accept_event(const Start_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
}

void Human_processor::accept_event(const Stop_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
}

void Human_processor::accept_event(const Report_event *)
{
	int hours, minutes, seconds, milliseconds;
	time_convert(get_time(), hours, minutes, seconds, milliseconds);
	Normal_out << processor_info() << " ****** Workload Report "
		<< hours << ':' << minutes << ':' << seconds << '.' << milliseconds << " ******" << endl;
}

void Human_processor::make_visual_object_appear(const Symbol& obj_name)
{
	Event * output_event_ptr = 
		new Visual_Input_Object_Appearance_event(get_time(), visual_ptr, obj_name);	
	schedule_event(output_event_ptr);
}

void Human_processor::make_visual_object_appear(const Symbol& obj_name, GU::Point location, GU::Size size)
{
	Event * output_event_ptr = 
		new Visual_Input_Object_Appearance_event(get_time(), visual_ptr,  obj_name);	
	schedule_event(output_event_ptr);
	set_visual_object_location(obj_name, location);
	set_visual_object_size(obj_name, size);
}

void Human_processor::make_visual_object_disappear(const Symbol& obj_name)
{
	Event * output_event_ptr = 
		new Visual_Input_Object_Disappearance_event(get_time(), visual_ptr, obj_name);	
	schedule_event(output_event_ptr);
}

void Human_processor::set_visual_object_location(const Symbol& obj_name, GU::Point location)
{
	set_visual_object_property(obj_name, Location_c, to_Symbol(location));
}

void Human_processor::set_visual_object_size(const Symbol& obj_name, GU::Size size)
{
	set_visual_object_property(obj_name, Size_c, to_Symbol(size));
}

void Human_processor::set_visual_object_property(const Symbol& obj_name, const Symbol& propname, const Symbol& propvalue)
{
	Event * output_event_ptr = 
		new Visual_Input_Object_Change_Symbol_event(get_time(), visual_ptr,  
			obj_name, propname, propvalue);	
	schedule_event(output_event_ptr);
}

void Human_processor::make_auditory_event(const Symbol& message)
{
	Event * output_event_ptr = new Auditory_Input_Symbol_event(get_time(), auditory_ptr, message);
	schedule_event(output_event_ptr);
}

void Human_processor::make_high_level_input_appear(const Symbol& obj_name, const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag)
{
	Event * output_event_ptr =
		new High_Level_Input_event(get_time(),  high_level_ptr,
			obj_name, props, values, tag);
	schedule_event(output_event_ptr);
}

void Human_processor::make_high_level_input_disappear( const Symbol& obj_name)
{
	Event * output_event_ptr =
		new High_Level_Erase_event(get_time(), high_level_ptr, obj_name);
	schedule_event(output_event_ptr);
}



// These functions return a pointer of the base class type Processor *, 
// for convenience in event dispatching. 
// They are not defined inline, in order to help decouple the header files 
// throughout the system because all processor headers then do not need to be included here.
// (e.g. to convert Cognitive_processor * to Processor *, inheritance must be known)

Processor * Human_processor::get_cognitive_ptr() const
{
	return cognitive_ptr;
}

Processor * Human_processor::get_visual_ptr() const
{
	return visual_ptr;
}

Processor * Human_processor::get_auditory_ptr() const
{
	return auditory_ptr;
}

Processor * Human_processor::get_manual_ptr() const
{
	return manual_ptr;
}

Processor * Human_processor::get_vocal_ptr() const
{
	return vocal_ptr;
}

Processor * Human_processor::get_task_ptr() const
{
	return task_ptr;
}

Processor * Human_processor::get_wm_ptr() const
{
	return wm_ptr;
}

Processor * Human_processor::get_ltm_ptr() const
{
	return ltm_ptr;
}

Processor * Human_processor::get_high_level_ptr() const
{
	return high_level_ptr;
}

Processor * Human_processor::get_device_ptr() const
{
	return device_ptr;
}


bool Human_processor::compile(const string& filename)
{
	Assert(cognitive_ptr);
	return cognitive_ptr->compile(filename);
}



void Human_processor::add_to_recorder_list(Processor * proc_ptr)
{
	recorder_list.push_back(proc_ptr);
}

void Human_processor::remove_from_recorder_list(Processor * proc_ptr)
{
	recorder_list.remove(proc_ptr);
}

void Human_processor::initialize_recorders()
{
	list<Processor *>::iterator iter;
	for_each(recorder_list.begin(), recorder_list.end(), mem_fun(&Processor::initialize));
}

void Human_processor::broadcast_to_recorders(const Event * event_ptr)
{
	list<Processor *>::iterator iter;
	
	for (iter = recorder_list.begin(); iter != recorder_list.end(); ++iter)
		event_ptr->send_self(*iter);
}


// break accessors
bool Human_processor::get_breaks_enabled() const
{
	return cognitive_ptr->get_breaks_enabled();
}

void Human_processor::set_breaks_enabled(bool state)
{
	cognitive_ptr->set_breaks_enabled(state);
}

// this checks the next step in all threads
// only check if breaks are enabled
bool Human_processor:: is_next_step_break_set() const
{
	return (cognitive_ptr->get_breaks_enabled()) ? 
		cognitive_ptr->is_next_step_break_set() : false;
}

// these apply to the main thread only
bool Human_processor::get_main_next_step_break_state() const
{
	return cognitive_ptr->get_main_next_step_break_state();
}

void Human_processor::set_main_next_step_break_state(bool state)
{
	cognitive_ptr->set_main_next_step_break_state(state);
}

// these functions work with steps identified in terms of goal and step label
bool Human_processor::is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label)
{
	return cognitive_ptr->is_step_name_valid(goal_action, goal_object, step_label);
}

// an exception is thrown if the step name is invalid
bool Human_processor::get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{
	return cognitive_ptr->get_break_state(goal_action, goal_object, step_label);
}

void Human_processor::set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state)
{
	cognitive_ptr->set_break_state(goal_action, goal_object, step_label, state);
}

// these functions work with steps names in a human-readable form defined internally
// return a list of step names in a standard human readable form
const std::list<std::string>& Human_processor::get_step_names() const
{
	return cognitive_ptr->get_step_names();
}

bool Human_processor::is_step_name_valid(const std::string& step_name)
{
	return cognitive_ptr->is_step_name_valid(step_name);
}

// these functions take the step name in the same form as provided
// an exception is thrown if the step name is invalid
bool Human_processor::get_break_state(const std::string& break_name) const
{
	return cognitive_ptr->get_break_state(break_name);
}

void Human_processor::set_break_state(const std::string& break_name, bool state)
{
	cognitive_ptr->set_break_state(break_name, state);
}

bool Human_processor::get_break_state(const string& file_name, int line_number) const
{
	return cognitive_ptr->get_break_state(file_name, line_number);
}
void Human_processor::set_break_state(const string& file_name, int line_number, bool state)
{
	cognitive_ptr->set_break_state(file_name, line_number, state);
}

/*
bool Human_processor::was_break_step_executed() const
{
	return cognitive_ptr->was_break_step_executed();
}
*/

// trace accessors
bool Human_processor::get_trace_visual() const
{
	return visual_ptr->get_trace();
}

void Human_processor::set_trace_visual(bool trace_)
{
	visual_ptr->set_trace(trace_);
}

void Human_processor::set_trace_auditory (bool trace_)
{
	auditory_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_auditory() const
{
	return auditory_ptr->get_trace();
}

void Human_processor::set_trace_cognitive (bool trace_)
{
	cognitive_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_cognitive() const
{
	return cognitive_ptr->get_trace();
}

void Human_processor::set_trace_WM (bool trace_)
{
	wm_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_WM() const
{
	return wm_ptr->get_trace();
}

void Human_processor::set_trace_task (bool trace_)
{
	task_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_task() const
{
	return task_ptr->get_trace();
}

void Human_processor::set_trace_manual (bool trace_)
{
	manual_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_manual() const
{
	return manual_ptr->get_trace();
}

void Human_processor::set_trace_vocal (bool trace_)
{
	vocal_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_vocal() const
{
	return vocal_ptr->get_trace();
}

void Human_processor::set_trace_highlevel (bool trace_)
{
	high_level_ptr->set_trace(trace_);
}

bool Human_processor::get_trace_highlevel() const
{
	return high_level_ptr->get_trace();
}

// output controls
bool Human_processor::was_operator_executed() const	// returns true if something of interest executed on this step
{
	return cognitive_ptr->was_operator_executed();
}

void Human_processor::reset_operator_executed()
{
	cognitive_ptr->reset_operator_executed();
}

bool Human_processor::get_output_step_labels_only() const	// whether to show basic step activity
{
	return cognitive_ptr->get_output_step_labels_only();
}

void Human_processor::set_output_step_labels_only(bool output_step_labels_only_)
{
	cognitive_ptr->set_output_step_labels_only(output_step_labels_only_);
}

bool Human_processor::get_output_step_listing() const		// whether to output listing of step contents
{
	return cognitive_ptr->get_output_step_listing();
}

void Human_processor::set_output_step_listing(bool output_step_listing_)
{
	cognitive_ptr->set_output_step_listing(output_step_listing_);
}

bool Human_processor::get_output_step_WM() const		// whether to output contents of WM  on each step
{
	return cognitive_ptr->get_output_step_WM();
}

void Human_processor::set_output_step_WM(bool output_step_WM_)
{
	cognitive_ptr->set_output_step_WM(output_step_WM_);
}
		
bool Human_processor::get_output_learning_calculation() const		// whether to output contents of WM  on each step
{
	return cognitive_ptr->get_output_learning_calculation();
}

void Human_processor::set_output_learning_calculation(bool output_learning_calculation_)
{
	cognitive_ptr->set_output_learning_calculation(output_learning_calculation_);
}

bool Human_processor::get_output_model_contents() const		// whether to output contents of WM  on each step
{
	return cognitive_ptr->get_output_model_contents();
}

void Human_processor::set_output_model_contents(bool output_model_contents_)
{
	cognitive_ptr->set_output_model_contents(output_model_contents_);
}


void Human_processor::output_steps() const
{
	cognitive_ptr->display_steps();
}

void Human_processor::output_working_memory_contents() const
{
	wm_ptr->display_contents(Normal_out);
}

void Human_processor::output_tag_store_contents() const
{
	wm_ptr->display_tag_store_contents(Normal_out);
}

void Human_processor::output_object_store_contents() const
{
	wm_ptr->display_object_store_contents(Normal_out);
}

void Human_processor::output_visual_memory_contents() const
{
	visual_ptr->display_object_info(Normal_out);
}

void Human_processor::output_auditory_memory_contents() const
{
	auditory_ptr->display_object_info(Normal_out);
}

void Human_processor::output_task_memory_contents() const
{
	task_ptr->display_object_info(Normal_out);
}

void Human_processor::output_highlevel_memory_contents() const
{
	high_level_ptr->display_object_info(Normal_out);
}

void Human_processor::output_ltm_memory_contents() const
{
	ltm_ptr->display_object_info(Normal_out);
}

// accessors for GOMSL source file information
const string& Human_processor::get_main_current_step_file_name() const
{
	return cognitive_ptr->get_main_current_step_file_name();
}

int Human_processor::get_main_current_step_line_number() const
{
	return cognitive_ptr->get_main_current_step_line_number();
}

std::string Human_processor::get_learning_report() const
{
   return cognitive_ptr->get_learning_report();
}

