#ifndef COGNITIVE_PROCESSOR_H
#define COGNITIVE_PROCESSOR_H

#include <set>
#include <list>

#include "Human_subprocessor.h"
#include "GOMS_Model.h"
#include "Cognitive_event_types.h"
#include "GOMS_Control_Operators.h"
#include "WM_processor.h"

class GOMS_Execution_Thread;


/*
The Cognitive_processor manages GOMS_Execution_Threads. It handles all events on behalf
of the threads.

A step event calls for asking each thread to advance a step if it is ready to do so.
An operation-complete event is handed to the originating thread.
*/

class Cognitive_processor : public Human_subprocessor {
public:
	Cognitive_processor(Human_processor * human_ptr);
	virtual ~Cognitive_processor();
	virtual void initialize(); 
	
	// compile and load the GOMSL in the supplied file
	bool compile(const std::string& filename);
	// provide a pointer to the GOMS model for other processors to access
	// the initial state information in order to initialize themselves
	GOMS_Model_Sptr_t get_GOMS_Model_Sptr() const
		{return goms_model_sptr;}
	// return whether the Cognitive Processor is in a runnable state
	// this definition is used internally as well as available externally.
	// the Cognitive_processor is runnable only if there is a main thread
	bool get_runnable() const
		{return bool(main_thread_ptr);}
	// Event interface to rest of the system
	virtual void accept_event(const Start_event *);
	virtual void accept_event(const Cognitive_event *);
	virtual void handle_event(const Cognitive_Step_event *);
	virtual void handle_event(const Cognitive_Stop_event *) {}	// no definition
	virtual void handle_event(const Cognitive_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Visual_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Auditory_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Manual_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Vocal_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_LTM_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Task_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_High_Level_Operation_Complete_event *);
	virtual void handle_event(const Cognitive_Delay_event *);			

	// Following are services for the rest of the system
	long get_step_end_time() {return step_end_time;}
	void schedule_event(Event *);
	void call_with_event(Event&);
	
	// Following are used by GOMS_Operators that require flow-of-control modification
	void spawn_accomplish_goal(const GOMS_Goal& starting_goal, const Symbol& thread_name);
	void abort_and_restart(GOMS_Execution_Thread * thread_ptr);
	void resume(const Symbol& thread_name, GOMS_Goal goal, const Symbol& step_name);
	void raise_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name);
	void cleanup_raised_exception();
	void stop_simulation(const Symbol& message);
	
// why was this public accessor defined?
//	typedef std::list<GOMS_Execution_Thread *> GOMS_thread_list_t;
//	const GOMS_thread_list_t& get_threads() const
//		{return thread_list;}

	// access/set peripheral processor status
	bool is_visual_busy() const
		{return visual_busy;}
	void set_visual_busy(GOMS_Execution_Thread *);
	bool is_auditory_busy() const
		{return auditory_busy;}
	void set_auditory_busy(GOMS_Execution_Thread *);
	bool is_manual_busy() const
		{return manual_busy;}
	void set_manual_busy(GOMS_Execution_Thread *);
	bool is_vocal_busy() const
		{return vocal_busy;}
	void set_vocal_busy(GOMS_Execution_Thread *);
	bool is_ltm_busy() const
		{return ltm_busy;}
	void set_ltm_busy(GOMS_Execution_Thread *);
	bool is_task_busy() const
		{return task_busy;}
	void set_task_busy(GOMS_Execution_Thread *);
	bool is_high_level_busy() const
		{return high_level_busy;}
	void set_high_level_busy(GOMS_Execution_Thread *);
	bool is_cognitive_busy() const	// for mental operators
		{return cognitive_busy;}
	void set_cognitive_busy(GOMS_Execution_Thread *);

	// add and remove object names for the Exists predicate to work on
	void add_salient_object(const Symbol& add_object);
	void remove_salient_object(const Symbol& remove_object);
	const std::set<Symbol> & get_salient_objects() const
		{return salient_objects;}

	// user interface functions
	void display_steps() const;
	void set_stop_on_abort_and_restart(bool state) 
		{stop_on_abort_and_restart = state;}
	bool get_stop_on_abort_and_restart() const 
		{return stop_on_abort_and_restart;}
	// write some information about where the process currently is executing
	virtual void output_current_execution_state() const;
	// write some logging output followed by the list of items
	virtual void output_log(const Symbol_list_t&) const;
	std::string cogproc_info() const;

	// UI controls
	bool get_output_step_labels_only() const	// whether to output step labels only during execution
		{return output_step_labels_only;}
	void set_output_step_labels_only(bool output_step_labels_only_)
		{output_step_labels_only = output_step_labels_only_;}

	bool get_output_step_listing() const // whether to output listing of step contents during execution
		{return output_step_listing;}
	void set_output_step_listing(bool output_step_listing_)
		{output_step_listing = output_step_listing_;}

	bool get_output_step_WM() const		// whether to output contents of WM  on each step
		{return output_step_WM;}
	void set_output_step_WM(bool output_step_WM_)
		{output_step_WM = output_step_WM_;}
	void output_step_WM_info() const;

	bool get_output_learning_calculation() const
		{return output_learning_calculation;}
	void set_output_learning_calculation(bool output_learning_calculation_)
		{output_learning_calculation = output_learning_calculation_;}

	bool get_output_model_contents() const
		{return output_model_contents;}
	void set_output_model_contents(bool output_model_contents_)
		{output_model_contents = output_model_contents_;}
	void output_learning_analysis() const;
	std::string get_learning_report() const;

	// accessors for GOMSL source file information
	const std::string& get_main_current_step_file_name() const;
	int get_main_current_step_line_number() const;
	
	// break accessors
	bool get_breaks_enabled() const
		{return breaks_enabled;}
	void set_breaks_enabled(bool state)
		{breaks_enabled = state;}
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

	bool was_operator_executed() const 
		{return operator_executed;}
	void reset_operator_executed(){operator_executed = false;}

private:
	// protected constant
	static const int cognitive_processor_step_time;
	static const int cognitive_processor_idle_step_limit;

	// execution state
	GOMS_Model_Sptr_t goms_model_sptr;
	long step_end_time;
	long last_step_time;
	int idle_step_counter;	// counts steps with no action
	bool operator_executed;	// an interesting operator got executed; user might want to see
	bool breaks_enabled;		// true if we should set break-on-step
//	bool break_on_step;		// a break step was executed

	// private helpers for thread management
	void create_starting_threads();
	void start_execution();
	bool execute_next_step();
	void execute_all_regular_thread_steps();
	void raise_normal_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name);
	void raise_error_exception(GOMS_Execution_Thread * thread_ptr, const Symbol& exception_name);
	void terminate_all_threads();
	GOMS_Execution_Thread * find_thread(const Symbol& thread_name) const;
	void clear_processor_busy(GOMS_Execution_Thread * thread_ptr);
	void initialize_processor_busy_states();
	void store_in_tagstore(const Symbol& tag_name, const Symbol& tag_value);
	void remove_from_tagstore(const Symbol& tag_name);

	// thread management state
	GOMS_Execution_Thread * main_thread_ptr;
	GOMS_Execution_Thread * error_thread_ptr;
	GOMS_Execution_Thread * interrupt_thread_ptr;
	GOMS_Execution_Thread * original_exception_thread_ptr;	// originator of error exception
	int thread_id_counter;	// used to generate unique thread ids
	typedef std::list<GOMS_Execution_Thread *> GOMS_thread_list_t;
	GOMS_thread_list_t thread_list;
	GOMS_thread_list_t new_thread_list;

	// processor states - which processors in use by what thread
	bool visual_busy;
	GOMS_Execution_Thread * visual_busy_ptr;
	bool auditory_busy;
	GOMS_Execution_Thread * auditory_busy_ptr;
	bool manual_busy;
	GOMS_Execution_Thread * manual_busy_ptr;
	bool vocal_busy;
	GOMS_Execution_Thread * vocal_busy_ptr;
	bool ltm_busy;
	GOMS_Execution_Thread * ltm_busy_ptr;
	bool task_busy;
	GOMS_Execution_Thread * task_busy_ptr;
	bool high_level_busy;
	GOMS_Execution_Thread * high_level_busy_ptr;
	bool cognitive_busy;
	GOMS_Execution_Thread * cognitive_busy_ptr;
	
	// other helpers
	void send_WM_store(const Symbol&, const Symbol&);
//	Symbol send_WM_retrieve (const Symbol& property_name);
	void send_WM_display();

	// the set of salient objects for the Exists predicate
	std::set<Symbol> salient_objects;
			
	// UI controls
	bool output_step_labels_only;		// whether to show step labels only
	bool output_step_listing;			// whether to listing of step
	bool output_step_WM;				// whether to output contents of WM  on each step
	bool output_model_contents;
	bool output_learning_calculation;
	
	bool stop_at_time;		// continue steps until a certain time
	long time_to_stop;		// stop when time equals or exceeds this value
	bool stop_after_steps;	// continue steps until a certain number have been done
	int steps_to_stop;		// count-down for number of steps to stop at
	bool stop_on_abort_and_restart;	// pause on abort_and_restart operation
	
//	bool disable_pause_and_single_step;

	// UI helpers
	bool single_step_should_be_done() const;
	void print_step_names() const;
	void print_current_goal_and_step_names() const;
	void set_unset_breakpoint(bool set_break);
	void show_all_processor_status() const;
	void show_processor_status(GOMS_Execution_Thread *) const;
	void display_learning_calculation() const;
	void display_model_contents() const;


};

#endif
