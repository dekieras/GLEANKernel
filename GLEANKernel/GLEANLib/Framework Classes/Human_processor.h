#ifndef HUMAN_PROCESSOR_H
#define HUMAN_PROCESSOR_H

#include "Human_base.h"
#include "Processor.h"
#include "Event_types.h"

// These classes are all forward declared here to reduce compile-time coupling
// forward declarations
class Human_subprocessor;
class Cognitive_processor;
class Visual_processor;
class Auditory_processor;
class Manual_processor;
class Vocal_processor;
class Task_processor;
class WM_processor;
class High_Level_processor;
class Device_processor;

class Output_tee;
class View_base;

// This class defines a Human as a collection of other processors with certain connections,
// and provides the interface between the Human and the outside world. 

class Human_processor : public Human_base, public Processor {
public:
	Human_processor(const std::string& id);
	virtual ~Human_processor();
	virtual void initialize(); 
//	virtual void set_parameters();
//	virtual void describe_parameters(Output_tee& ot) const;

	// Event interface to rest of the system
	virtual void accept_event(const Start_event *);
	virtual void accept_event(const Stop_event *);

	// inputs
	void make_visual_object_appear(const Symbol& obj_name);
	void make_visual_object_appear(const Symbol& obj_name, GU::Point location, GU::Size size);
	void make_visual_object_disappear(const Symbol& obj_name);
	void set_visual_object_location(const Symbol& obj_name, GU::Point location);
	void set_visual_object_size(const Symbol& obj_name, GU::Size size);
	void set_visual_object_property(const Symbol& obj_name, const Symbol& propname, const Symbol& propvalue);

	void make_auditory_event(const Symbol& message);
	// A high-level input event
	virtual void make_high_level_input_appear(const Symbol& object_name, const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag);
	// A high-level input disappears
	virtual void make_high_level_input_disappear(const Symbol& object_name);
 
	// services 
	bool compile(const std::string& filename);

	void initialize_recorders();
	void add_to_recorder_list (Processor *);
	void remove_from_recorder_list (Processor *);
	void broadcast_to_recorders(const Event *);
	
	// connection functions
	void connect(Device_processor * dpp)
		{device_ptr = dpp;}
	
	// These functions (with class name capitalized) return a constituent processor pointer 
	// of the actual processor derived-class type. 
	Cognitive_processor * get_Cognitive_ptr() const {return cognitive_ptr;}
	Visual_processor * get_Visual_ptr() const {return visual_ptr;}
	Auditory_processor * get_Auditory_ptr() const {return auditory_ptr;}
	Manual_processor * get_Manual_ptr() const {return manual_ptr;}
	Vocal_processor * get_Vocal_ptr() const {return vocal_ptr;}
	Task_processor * get_Task_ptr() const {return task_ptr;}
	WM_processor * get_Wm_ptr() const {return wm_ptr;}
	LTM_processor * get_Ltm_ptr() const {return ltm_ptr;}
	High_Level_processor * get_High_level_ptr() const {return high_level_ptr;}
	Device_processor * get_Device_ptr() const {return device_ptr;}

	// These functions return a pointer of the base class type Processor *, 
	// for convenience in event dispatching. 
	// They are not defined inline, in order to help decouple the header files 
	// throughout the system because all processor headers then do not need to be included here.
	// (e.g. to convert Cognitive_processor * to Processor *, inheritance must be known)
	Processor * get_cognitive_ptr() const;
	Processor * get_visual_ptr() const;
	Processor * get_auditory_ptr() const;
	Processor * get_manual_ptr() const;
	Processor * get_vocal_ptr() const;
	Processor * get_task_ptr() const;
	Processor * get_wm_ptr() const;
	Processor * get_ltm_ptr() const;
	Processor * get_high_level_ptr() const;
	Processor * get_device_ptr() const;
	
	virtual void accept_event(const Report_event *);
	
	// UI controls
	bool was_operator_executed() const;	// returns true if something of interest executed on this step
	void reset_operator_executed();

	// break accessors
	bool get_breaks_enabled() const;
	void set_breaks_enabled(bool state);
	// this checks the next step in all threads
	bool is_next_step_break_set() const;
	// these apply to the main thread only
	bool get_main_next_step_break_state() const;
	void set_main_next_step_break_state(bool state);
	// these functions work with steps identified in terms of goal and step label
	bool is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label);
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const;
	void set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state);
	// these functions work with steps names in a human-readable form defined internally
	// return a list of step names in a standard human readable form
	const std::list<std::string>& get_step_names() const;
	bool is_step_name_valid(const std::string& step_name);
	// these functions take the step name in the same form as provided
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& break_name) const;
	void set_break_state(const std::string& break_name, bool state);
	// these functions take the step name in the form of a file name and line number
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& file_name, int line_number) const;
	void set_break_state(const std::string& file_name, int line_number, bool state);
	// return whether a break step was executed
	// bool was_break_step_executed() const;
	
	
	// trace accesssors
	bool get_trace_visual() const;
	void set_trace_visual(bool trace_);
	bool get_trace_auditory() const;
	void set_trace_auditory(bool);
	bool get_trace_cognitive() const;
	void set_trace_cognitive(bool);
	bool get_trace_WM() const;
	void set_trace_WM(bool);
	bool get_trace_task() const;
	void set_trace_task(bool);
	bool get_trace_manual() const;
	void set_trace_manual(bool);
	bool get_trace_vocal() const;
	void set_trace_vocal(bool);
	bool get_trace_highlevel() const;
	void set_trace_highlevel(bool);



	bool get_output_step_labels_only() const;	// whether to show basic step activity
	void set_output_step_labels_only(bool output_step_labels_only_);

	bool get_output_step_listing() const;	// whether to output listing of step contents
	void set_output_step_listing(bool output_step_listing_);

	bool get_output_step_WM() const;		// whether to output contents of WM  on each step
	void set_output_step_WM(bool output_step_WM_);
		
	bool get_output_learning_calculation() const;		// whether to output contents of WM  on each step
	void set_output_learning_calculation(bool output_learning_calculation_);

	bool get_output_model_contents() const;
	void set_output_model_contents(bool output_model_contents_);
	

	
	void output_steps() const;		// output (now) the current steps to Normal_out
	// output the contents of memories to Normal_out
	void output_working_memory_contents() const;
	void output_tag_store_contents() const;
	void output_object_store_contents() const;
	void output_visual_memory_contents() const;
	void output_auditory_memory_contents() const;
	void output_task_memory_contents() const;
	void output_highlevel_memory_contents() const;
	void output_ltm_memory_contents() const;

	// accessors for GOMSL source file information
	const std::string& get_main_current_step_file_name() const;
	int get_main_current_step_line_number() const;

	std::string get_learning_report() const;

private:
	typedef std::list<Processor *> Recorders_t;
	Recorders_t recorder_list;	// list of recorder processors
	
	Cognitive_processor * cognitive_ptr;
	Visual_processor * visual_ptr;
	Auditory_processor * auditory_ptr;
	Manual_processor * manual_ptr;
	Vocal_processor * vocal_ptr;
	Task_processor * task_ptr;
	WM_processor * wm_ptr;
	LTM_processor * ltm_ptr;
	High_Level_processor * high_level_ptr;
	Device_processor * device_ptr;
	
	// disallow copy and assignment
	Human_processor(const Human_processor&);
	Human_processor& operator= (const Human_processor&);
};

#endif
