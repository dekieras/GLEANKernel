#ifndef GOMS_CONTROL_OPERATORS_H
#define GOMS_CONTROL_OPERATORS_H

#include "GOMS_Model.h"

// forward reference
class Cognitive_processor;

// flow of control operator classes

class GOMS_Accomplish_Goal_Operator : public GOMS_Operator {
	friend class Cognitive_processor;
	friend class GOMS_Execution_Thread;
public:
	GOMS_Accomplish_Goal_Operator () 
		{}
	GOMS_Accomplish_Goal_Operator (GOMS_Goal& g, GOMS_Argument_Sptr_List_t& args) :
		goal(g), arg_list(args)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;

private:
	GOMS_Goal goal;
	GOMS_Argument_Sptr_List_t arg_list;
};

class GOMS_Also_Accomplish_Goal_Operator : public GOMS_Operator {
	friend class Cognitive_processor;
	friend class GOMS_Execution_Thread;
public:
	GOMS_Also_Accomplish_Goal_Operator () 
		{}
	GOMS_Also_Accomplish_Goal_Operator (GOMS_Goal& g, const Symbol& thread_name_, 
			GOMS_Argument_Sptr_List_t& args) :
		goal(g), thread_name(thread_name_), arg_list(args)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;

private:
	GOMS_Goal goal;
	Symbol thread_name;
	GOMS_Argument_Sptr_List_t arg_list;
};


class GOMS_Return_Goal_Accomplished_Operator : public GOMS_Operator {
public:
	GOMS_Return_Goal_Accomplished_Operator () 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
};

class GOMS_Abort_and_Restart_Operator : public GOMS_Operator {
public:
	GOMS_Abort_and_Restart_Operator () 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
};

class GOMS_Resume_Operator : public GOMS_Operator {
public:
	GOMS_Resume_Operator (GOMS_Argument_Sptr_t thread_name_, 
	GOMS_Argument_Sptr_t goal_action_, GOMS_Argument_Sptr_t goal_object_, GOMS_Argument_Sptr_t step_name_) :
		thread_name(thread_name_), goal_action(goal_action_), goal_object(goal_object_), step_name(step_name_)
		{}
	GOMS_Resume_Operator () 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t thread_name;
	GOMS_Argument_Sptr_t goal_action;
	GOMS_Argument_Sptr_t goal_object;
	GOMS_Argument_Sptr_t step_name;
};

class GOMS_Raise_Operator : public GOMS_Operator {
public:
	GOMS_Raise_Operator () 
		{}
	GOMS_Raise_Operator (GOMS_Argument_Sptr_t exception_name_) : exception_name(exception_name_)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t exception_name;
};

class GOMS_Stop_Operator : public GOMS_Operator {
public:
	GOMS_Stop_Operator () 
		{}
	GOMS_Stop_Operator (GOMS_Argument_Sptr_t message_) : message(message_)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t message;
};


class GOMS_Goto_Operator : public GOMS_Operator {
public:
	GOMS_Goto_Operator () 
		{}
	GOMS_Goto_Operator (Symbol const & lbl) :
		label(lbl)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	Symbol label;
};


class GOMS_Decide_Operator : public GOMS_Operator {
public:
	GOMS_Decide_Operator () 
		{}
	GOMS_Decide_Operator (GOMS_Conditional_Sptr_list_t& cl) :
		conditionals(cl)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual int get_number_of_steps() const;
	virtual void request_processors(Processor_Request&) const;

protected:
	GOMS_Conditional_Sptr_list_t conditionals;
};

class GOMS_Selection_Operator : public GOMS_Decide_Operator {
public:
	GOMS_Selection_Operator ()
		{}
	GOMS_Selection_Operator (GOMS_Conditional_Sptr_list_t& cl) :
		GOMS_Decide_Operator(cl)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
};


class GOMS_Interrupt_Rule_Set_Operator : public GOMS_Operator {
public:
	GOMS_Interrupt_Rule_Set_Operator () 
		{}
	GOMS_Interrupt_Rule_Set_Operator (GOMS_Conditional_Sptr_list_t& cl) :
		conditionals(cl)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual int get_number_of_steps() const;
private:
	GOMS_Conditional_Sptr_list_t conditionals;
};

#endif
