#ifndef GLEAN_MODEL_H
#define GLEAN_MODEL_H

//#include "Symbol.h"
#include <string>
#include <set>
#include <list>

class View_base;
class Device_base;
class Device_processor;
class Human_processor;
class Recorder;
class Cognitive_Method_Profiler;
class Workload_Method_Profiler;

class Model {
public:
	// constructor requires a pointer to a Device_base object
	// that is used to construct a Device_processor object
	Model(Device_base *device_ptr_ = 0);
	~Model();

	Human_processor* get_human_ptr() const
		{return human_ptr;}
	Device_base * get_device_ptr() const
		{return device_ptr;}
	void set_device_ptr(Device_base *device_ptr_) 
		{connect_device_to_human(device_ptr_);}
	Device_processor * get_device_processor_ptr() 
		{return device_processor_ptr;}
	// the supplied Device_base is already connected to a Device_processor
	void connect_device_to_human(Device_base * dev_base_ptr);

	/* functions that control the model from outside 
	they return true or false to show the run state of the model 
	the corresponding state variable is also set and can be accessed */
	bool compile();	// returns true if compile successful, false if not
	bool initialize();
	// these return true if the model is still runnable, false if not
	// run for a time
	bool run_time(long);
	// run until done ...
	// bool run();
	// stop the model, after letting it run to completion ...
	bool stop();

	long get_time() const;

	// accessors for state of the model
	const std::string& get_gomsl_filename() const;
	void set_gomsl_filename(const std::string& filename);

	bool get_gomsl_filename_set() const
		{return gomsl_filename_set;}
	bool get_compiled() const
		{return compiled;}
	bool get_initialized() const
		{return initialized;}
	bool get_running() const
		{return running;}

	/* View maintenance and notification functions */
	// add, remove Views to be notified of changes
	// forwarded to human processor to attach to correct subprocessors
	void add_visual_physical_view(View_base * view_ptr);
	void add_visual_sensory_view(View_base * view_ptr);
	void add_visual_perceptual_view(View_base * view_ptr);

	void remove_visual_physical_view(View_base * view_ptr);
	void remove_visual_sensory_view(View_base * view_ptr);
	void remove_visual_perceptual_view(View_base * view_ptr);

	void add_auditory_physical_view(View_base * view_ptr);
	void add_auditory_sensory_view(View_base * view_ptr);
	void add_auditory_perceptual_view(View_base * view_ptr);

	void remove_auditory_physical_view(View_base * view_ptr);
	void remove_auditory_sensory_view(View_base * view_ptr);
	void remove_auditory_perceptual_view(View_base * view_ptr);
	
	void add_wm_view(View_base * view_ptr);
	void remove_wm_view(View_base * view_ptr);

	void add_task_view(View_base * view_ptr);
	void remove_task_view(View_base * view_ptr);

	// use this function to add a view that is not specific to a particular processor
	// such as the view used for Normal output
	void add_view(View_base * view_ptr);
	void remove_view(View_base * view_ptr);
	void remove_all_views();
	void clear_all_views();

	// tracing accessors - forwarded to human processor
	bool get_trace_all() const
		{return trace_all;}
	void set_trace_all(bool);
	bool get_trace_visual() const;
	void set_trace_visual(bool);
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

	bool get_trace_device() const;
	void set_trace_device(bool);

	// functions to get/set output flags in GOMS model
	bool get_output_compiler_messages() const;
	void set_output_compiler_messages(bool flag);

	bool get_output_compiler_details() const;
	void set_output_compiler_details(bool flag);

	bool was_operator_executed() const;	// returns true if something of interest executed on this step
	void reset_operator_executed();

	bool get_output_run_messages() const;
	void set_output_run_messages(bool flag);

	bool get_output_run_details() const;
	void set_output_run_details(bool flag);

	bool get_output_run_memory_contents() const;
	void set_output_run_memory_contents(bool flag);

	void output_steps() const;		// output (now) the current steps to Normal_out
	// output the contents of memories to Normal_out
	void output_working_memory_contents() const;
	void output_tag_store_contents() const;
	void output_object_store_contents() const;
	void output_visual_memory_contents() const;
	void output_auditory_memory_contents() const;
	void output_task_memory_contents() const;
	void output_ltm_memory_contents() const;
	void output_learning_analysis();
	void output_goms_model();
	void output_workload_reports();
	
	// return a string containing the learning analysis report	
	std::string get_learning_report() const;
	// return a string containing the workload report
	std::string get_recorder_report() const;

	// accessors for GOMSL source file information
	const std::string& get_main_current_step_file_name() const;
	int get_main_current_step_line_number() const;


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

	// fault accessors - these should parallel the break facilities
	bool get_fault_enabled() const;
	void set_fault_enabled(bool);
	void set_fault_state(const std::string& fault_name, bool state);
	bool get_fault_state(const std::string& fault_name) const;

	// device accessors
	void set_device_parameter_string(const std::string&);
	std::string get_device_parameter_string() const;

private:
	std::string gomsl_filename;			// holds the current GOMSL file name	
	Device_base * device_ptr;			// needed to talk to the device ...
	Device_processor * device_processor_ptr;
	Human_processor * human_ptr;
	//	Cognitive_Method_Profiler * method_profiler;
	Workload_Method_Profiler * method_profiler;

	std::set<View_base *> view_ptrs;	// pointers to all attached EPIC_Views - set for uniqueness

	bool gomsl_filename_set;// GOMSL filename has been set
	bool compiled;			// GOMSL has been compiled
	bool initialized;		// simulation has been initialized
	bool running;			// simulation is running

	bool trace_all;
//	bool trace_device;

	void create_human_processor();
	void connect_device();
	
	// forbid copy and assignment
	Model(const Model&);
	Model& operator= (const Model&);

};

#endif
