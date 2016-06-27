#ifndef GOMS_EXECUTION_THREAD
#define GOMS_EXECUTION_THREAD

#include "Processor.h"
#include "GOMS_Model.h"
#include "GOMS_Control_Operators.h"
#include "WM_processor.h"

#include <vector>
#include <string>

//forward declarations for friend classes
class GOMS_Accomplish_Goal_Operator;
class GOMS_Return_Goal_Accomplished_Operator;
class GOMS_Goto_Operator;
class Cognitive_processor;



class GOMS_Execution_Thread {
public:
	enum Thread_type_e {MAIN, INTERRUPT, ERROR, NORMAL};
	GOMS_Execution_Thread(const Symbol& name_, int id_, Cognitive_processor * cog_ptr_, 
		GOMS_Model_Sptr_t& current_model_sptr_, const GOMS_Goal& starting_goal_, Thread_type_e thread_type_ = NORMAL);

	// provide access to governing cognitive processor
	Cognitive_processor * get_cog_ptr() const 
		{return cog_ptr;}

	// called by Cognitive_processor to control execution
	void start_execution();
	void start_at_goal(const GOMS_Goal& starting_goal_);
	// returned value is whether the thread is still runnable (true) or terminated.
	bool execute_next_step();
	// readers for thread execution state
	bool get_runnable() const {return runnable;}	// runnable (true) or terminated (false)
	bool submethods_in_progress() const;	// return true if non-empty stack
	// return true if operations are still pending
	bool check_operations_pending() const;
	// called by inter-step GOMS operators to show that an operation is starting
	void new_operation_pending();
	// called to show that an operation has completed
	void pending_operation_complete();
	
	// readers for thread informaton
	const Symbol& get_thread_name() const 
		{return name;}
	int get_thread_id() const 
		{return id;}
	const std::string& get_thread_info() const 
		{return thread_info;}
  	Thread_type_e get_thread_type() const 
  		{return thread_type;}
	
	// readers for method execution state
	const GOMS_Goal& get_current_goal() const 
		{return current_goal;}
	const Symbol& get_current_step_name() const;
  	const GOMS_Step& get_current_step() const 
  		{return **current_step_itr;}
	const GOMS_Goal& get_current_error_goal() const 
		{return current_error_goal;}
		
	// readers for state information for use by UI and top-level execution control
	// called by user interface directly or indirectly
	bool get_operator_executed() const 	// whether an operator was executed on last step - show for single step
		{return operator_executed;}
	// bool get_break_on_step() const 		// whether a break was set on this last step
	//	{return break_on_step;}
	bool is_next_step_break_set() const;
	void set_next_step_break_state(bool set_break);
	bool get_next_step_break_state() const;
	void display_step() const;
	void print_current_execution_state() const;
	
	// accessors for GOMSL source file information
	const std::string& get_main_current_step_file_name() const;
	int get_main_current_step_line_number() const;

	// Following are used by GOMS_Operators that require flow-of-control modification
	bool push_accomplish_goal(const GOMS_Accomplish_Goal_Operator& ag_op);
	void copy_psdo_params(
		GOMS_Argument_Sptr_List_t arg_list, GOMS_Argument_Sptr_List_t param_list);
	bool pop_goal_accomplished();
	void delete_psdo_params(GOMS_Argument_Sptr_List_t param_list);
	void resume_at_goal(const GOMS_Goal& resume_goal);
	void resume_at_goal_and_step(const GOMS_Goal& resume_goal, const Symbol& resume_step);
	void restart_at_goal(const GOMS_Goal& restart_goal);
	void unwind_to_top();
	void unwind_to_goal(const GOMS_Goal& stop_at_goal);
	void unwind_to_next_error_method();
	bool goto_step_label(const Symbol& goto_label);
	void stay_at_step();
	
	// this is public in order to allow external client to access the execution state
	class GOMS_Execution_Stack_Frame {
	public:

		GOMS_Execution_Stack_Frame() {}
		GOMS_Execution_Stack_Frame (const GOMS_Goal& g, const GOMS_Goal& eg, const GOMS_Procedure_Sptr_t& mp,GOMS_Step_Sptr_list_t::iterator si, bool ans) :
			goal(g), error_goal(eg), procedure_sptr(mp), step_itr(si), advance_next_step(ans) {}
		~GOMS_Execution_Stack_Frame() {}

		GOMS_Goal goal;
		GOMS_Goal error_goal;
		GOMS_Procedure_Sptr_t procedure_sptr;
		GOMS_Step_Sptr_list_t::iterator step_itr;
		bool advance_next_step;

	};
	
	typedef std::vector<GOMS_Execution_Stack_Frame> GOMS_Execution_Stack_t;
	const GOMS_Execution_Stack_t& get_execution_stack() const { return execution_stack; }

private:
	// initialized state
	Symbol name;
	int id;		// unique identifier
	std::string thread_info;
	Cognitive_processor * cog_ptr;
	GOMS_Model_Sptr_t& current_model_sptr;
	GOMS_Goal starting_goal;
	Thread_type_e thread_type;

	// execution state
	int operations_pending;
	bool runnable;				// true if thread has started and not finished
	bool advance_next_step;
	bool operator_executed;		// set if an operator of interest executed on this step
//	bool break_on_step;			// set if this step had a break, set after execution

	GOMS_Goal current_goal;
	GOMS_Goal current_error_goal;
	GOMS_Procedure_Sptr_t current_procedure_sptr;
	GOMS_Step_Sptr_list_t::iterator current_step_itr;
	GOMS_Execution_Stack_t execution_stack;
	
	// private helpers
	bool pop_execution_stack();
};

// This class is used to collect which processors operators need to use
class Processor_Request {
public:
	Processor_Request() :
		visual(false), auditory(false), 
		manual(false), vocal(false), 
		ltm(false), task(false), high_level(false), cognitive(false)
		{}
	bool evaluate(Cognitive_processor *) const;

	void request_visual() {visual = true;}
	void request_auditory() {auditory = true;}
	void request_manual() {manual = true;}
	void request_vocal() {vocal = true;}
	void request_ltm() {ltm = true;}
	void request_task() {task = true;}
	void request_high_level() {high_level = true;}
	void request_cognitive() {cognitive = true;}

	bool is_visual_requested() const {return visual;}
	bool is_auditory_requested() const {return auditory;}
	bool is_manual_requested() const {return manual;}
	bool is_vocal_requested() const {return vocal;}
	bool is_ltm_requested() const {return ltm;}
	bool is_task_requested() const {return task;}
	bool is_high_level_requested() const {return high_level;}
	bool is_cognitive_requested() const {return cognitive;}


private:
	bool visual;
	bool auditory;
	bool manual;
	bool vocal;
	bool ltm;
	bool task;
	bool high_level;
	bool cognitive;
};

#endif
