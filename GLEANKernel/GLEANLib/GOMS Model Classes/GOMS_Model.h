#ifndef GOMS_MODEL_H
#define GOMS_MODEL_H

#include "OPV_Containers.h"
#include "OPV_Containers.h"
#include "Symbol.h"
#include "Smart_Pointer.h"
#include "Glean_standard_symbols.h"

#include <iosfwd>
#include <set>
#include <list>
#include <string>

class Output_tee;

// Execution time context is provided by a pointer to a GOMS_GOMS_Cognitive_Processor

/*
Class structure for representing a GOMS Model.  The objects in a GOMS model
all read-only - once built, they do not change during interpretation.
The only objects with an execute method are the operators; the remainder are
merely containers.


GOMS Model:
	Contains:
		A model_name, a starting goal
		A set container indexed by Goal, containing smart pointers to Procedures 
		Property_Symbol_Object_Containers for LTM, task description,
			initial visual state, and initial WM contents
		
Goal:
	A pair of Symbols, an Action and an Object term.
	
Procedure:
	Contains a goal, and error_goal, a pseudo-parameter list
	a list of smart pointers to Steps, ordered by appearance in source, not by step labels.
	
Method:
	Is a Procedure.

	
Selection_Rule_Set:
	Is a Procedure.
	The selection rules are represented as a procedure with 
	a dummy step with a decide operator containing the Selection Rules, and
	a dummy step for the Return_with_goal_accomplished operator.
	
Selection_Rule:
	Contains a Condition and an Accomplish_Goal operator.
	
Step:
	Has a Symbol Label.
	Contains a list of operators, ordered by appearance in source.
	
Operator
	An abstract base class.
	virtual execute()

Specific Operators
	Contain some number of Arguments.
	execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) sends events, etc., to carry out the operator function
	
Argument
	An abstract base class.
	virtual resolve()
	
Constant_Argument
	Contains an Symbol constant.
	resolve() returns the constant.
	
WM_Argument
	Contains an Symbol WM tag.
	resolve() accesses WM with the tag, returns the retrieved Symbol value.
*/

// When comparing GOMS_Procedures A and B, a correspondence map is loaded with 
// certain terms from B stored under keys of the corresponding terms from A. 
// If terms do not match directly, the corresponding term is looked up and
// checked for a match

typedef PV_Container Correspondence_map_t;

// A global function that compares two Symbols using a correspondence map
bool similar(const Symbol& term_a, const Symbol& term_b, const Correspondence_map_t& cm_a_b);


// forward reference
class Cognitive_processor;
class GOMS_Execution_Thread;
class Processor_Request;

// A GOMS_Goal is not pointed to with Smart_Pointers, and so can be defined locally

class GOMS_Goal {
public:	
	GOMS_Goal() {}
	
	GOMS_Goal(const Symbol& a, const Symbol& o) :
		action(a), object(o)
		{}
	
	bool operator< (const GOMS_Goal& y) const
		{
			if (action < y.action)
				return true;
			else 
				if (action == y.action)
					if (object < y.object)
						return true;
					else
						return false;
				else
					return false;
		}
	
	bool operator== (const GOMS_Goal& rhs) const
		{
			return (action == rhs.action && object == rhs.object);
		}

	bool operator!= (const GOMS_Goal& rhs) const
		{
			return !(*this == rhs);
		}
	
	void display(Output_tee&) const;
	std::string get_string() const
		{return action.str() + " " + object.str();}
		
	Symbol action;
	Symbol object;
};

std::ostream& operator<< (std::ostream& os, const GOMS_Goal& g);


// GOMS Argument classes - used for arguments to operators.
// The resolve method returns the value of the argument in the current execution context.
// The get_symbol method returns the symbol name of the argument.
// The class has a fat interface to accomodate more specialized derived argument types.
// See GOMS_Memory_Operators.cpp for implementations.

class GOMS_Argument : public Reference_Counted_Object {
public:
	GOMS_Argument () 
		{}
	virtual ~GOMS_Argument ()	// this is a base class
		{}
	virtual Symbol resolve(Cognitive_processor *) const = 0;	// abstract base class
	virtual Symbol get_symbol() const = 0;	// return the symbol, not the resolution
// Are these functions needed?
	virtual Symbol resolve_object_name(Cognitive_processor *) const
		{return Symbol ("unresolved");}	// default no-error result
	virtual Symbol resolve_property_name(Cognitive_processor *) const
		{return Symbol ("unresolved");}	// default no-error result
	virtual Symbol resolve_property_value(Cognitive_processor *) const
		{return Symbol ("unresolved");}	// default no-error result
	virtual Symbol get_object_name(Cognitive_processor *) const
		{return Symbol ("undefined");}	// default no-error result
	virtual Symbol get_property_name(Cognitive_processor *) const
		{return Symbol ("undefined");}	// default no-error result
	virtual Symbol get_property_value(Cognitive_processor *) const
		{return Symbol ("undefined");}	// default no-error result
	virtual void display(Output_tee&) const {}
	virtual bool similar(const GOMS_Argument *, const Correspondence_map_t&) const = 0;
};


typedef Smart_Pointer<GOMS_Argument> GOMS_Argument_Sptr_t;
typedef std::list<GOMS_Argument_Sptr_t> GOMS_Argument_Sptr_List_t;

// A constant argument resolves to the same value as its symbol.
class GOMS_Constant_Argument : public GOMS_Argument {
public:
	GOMS_Constant_Argument() : 
		arg_value("default constant argument") 
		{}
	GOMS_Constant_Argument(const Symbol& x) :
		arg_value(x) 
		{}
	virtual Symbol resolve(Cognitive_processor *) const;
	virtual Symbol get_symbol() const {return arg_value;}	
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Argument *, const Correspondence_map_t&) const;
		
private:
	Symbol arg_value;
};

// A WM argument has a tag name as its symbol, and resolves to the value stored in WM under
// that tag.

class GOMS_WM_Argument : public GOMS_Argument {
public:
	GOMS_WM_Argument() : 
		tag_name("default") 
		{}
	GOMS_WM_Argument(const Symbol& x) :
		tag_name(x) 
		{}
	virtual Symbol resolve(Cognitive_processor *) const; // look up in WM, then return
	virtual Symbol get_symbol() const {return tag_name;}	
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Argument *, const Correspondence_map_t&) const;
private:
	Symbol tag_name;
};	

// A PO argument is expressed as <property-name> of <object-name>.  The resolution is the value
// stored in the object of the specified name under the property name. 
// The property name and object name are resolved first, and so may be other types of arguments.

class GOMS_PO_Argument : public GOMS_Argument {
public:
	GOMS_PO_Argument()
		{}
	GOMS_PO_Argument(const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		prop_name_sp(x), obj_name_sp(y)
		{}
	virtual Symbol resolve(Cognitive_processor *) const;
	virtual Symbol get_symbol() const;  // returns printable version of argument contents
	virtual Symbol get_object_name(Cognitive_processor *) const
		{return obj_name_sp->get_symbol();}
	virtual Symbol get_property_name(Cognitive_processor *) const
		{return prop_name_sp->get_symbol();}
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Argument *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t prop_name_sp;
	GOMS_Argument_Sptr_t obj_name_sp;
};	

// A PV argument is expressed as <property-name> is <property-value>.  
// The resolution is different since there are two argument members
// The normal resolve is stubbed as an empty function.
// There are two separate resolves.  Note that code must know that there
// is one of these objects.
class GOMS_PV_Argument : public GOMS_Argument {
public:
	GOMS_PV_Argument()
		{}
	GOMS_PV_Argument(const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		prop_name_sp(x), prop_value_sp(y)
		{}
	virtual Symbol resolve(Cognitive_processor *) const 
		{return Absent_c;}	// default no-error result
	virtual Symbol get_symbol() const
		{return prop_name_sp->get_symbol();}	
	virtual Symbol resolve_property_name(Cognitive_processor * cog_proc_ptr) const
			{return prop_name_sp->resolve(cog_proc_ptr);}
	virtual Symbol resolve_property_value(Cognitive_processor * cog_proc_ptr) const
			{return prop_value_sp->resolve(cog_proc_ptr);}
	virtual Symbol get_property_name(Cognitive_processor *) const
		{return prop_name_sp->get_symbol();}
	virtual Symbol get_property_value(Cognitive_processor *) const
		{return prop_value_sp->get_symbol();}
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Argument *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t prop_name_sp;
	GOMS_Argument_Sptr_t prop_value_sp;
};	


// The GOMS_Operator abstract class represents executable GOMS operators.
// The execute method is used to execute the operator in the current context.
// See GOMS_xxx_Operators.h, .cpp for concrete classes

class GOMS_Operator : public Reference_Counted_Object {
public:
	GOMS_Operator () :
		faulted(false)
		{}
	virtual ~GOMS_Operator ()	// this is a base class
		{}
	
	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *) = 0; // abstract base class
	virtual void display(Output_tee&) const {}
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const = 0;
	virtual int get_number_of_steps() const {return 0;}	// override if there are substeps

	virtual void request_processors(Processor_Request&) const {} // override as needed
	bool get_faulted() const
		{return faulted;}
	void set_faulted(bool state)
		{faulted = state;}
private:
	bool faulted;
};

typedef Smart_Pointer<GOMS_Operator> GOMS_Operator_Sptr_t;
typedef std::list<GOMS_Operator_Sptr_t> GOMS_Operator_Sptr_list_t;

// The GOMS_Predicate abstract class represents binary predicates 
// that can be used in conditionals.
// The evaluate method evaluates the predicate in the current context.
// See GOMS_Predicates.h, .cpp for concrete classes
class GOMS_Predicate : public Reference_Counted_Object {
public:
	GOMS_Predicate () 
		{}
	GOMS_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		first_arg(x), second_arg(y)
		{}
	virtual ~GOMS_Predicate () 
		{}
	
	virtual bool evaluate(Cognitive_processor *) = 0;
	virtual void display(Output_tee&) const {}
	// predicates are all similar if type is the same and arguments are similar
	virtual bool similar(const GOMS_Predicate * p, const Correspondence_map_t&) const;

protected:
	GOMS_Argument_Sptr_t first_arg;
	GOMS_Argument_Sptr_t second_arg;
};

typedef Smart_Pointer<GOMS_Predicate> GOMS_Predicate_Sptr_t;
typedef std::list<GOMS_Predicate_Sptr_t> GOMS_Predicate_Sptr_list_t;


// The GOMS_Conditional class is used for both selection rules and decide operators.
// It contains a list of predicates and operators.
// The evaluate method evaluates all of the predicates; if all are true, then each operator
// is executed.

class GOMS_Conditional : public Reference_Counted_Object {
public:
	GOMS_Conditional () :
		predicate_flag(false)
		{}
	GOMS_Conditional (GOMS_Predicate_Sptr_list_t& p, GOMS_Operator_Sptr_list_t& o) :
		predicate_flag(false), predicates(p), operators(o)
		{}
	GOMS_Conditional (GOMS_Operator_Sptr_list_t& o) :
		predicate_flag(true), operators(o)
		{}
	virtual ~GOMS_Conditional () 
		{}
	
	virtual bool evaluate(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee&) const;
	bool similar(const GOMS_Conditional * p, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;


private:
	bool predicate_flag;	// if true, don't check predicates, it's true
	GOMS_Predicate_Sptr_list_t predicates;
	GOMS_Operator_Sptr_list_t operators;
};

typedef Smart_Pointer<GOMS_Conditional> GOMS_Conditional_Sptr_t;
typedef std::list<GOMS_Conditional_Sptr_t> GOMS_Conditional_Sptr_list_t;


// Large units

//A GOMS_Step is a container for a list of operators.  It should be a read-only class.
// except for the provision for setting a break.
class GOMS_Step : public Reference_Counted_Object {
public:
	GOMS_Step() : break_flag(false), line_number(-1)
		{}
	GOMS_Step(const Symbol& lbl) :
		 label(lbl), break_flag(false), line_number(-1)
		{}
	~GOMS_Step ()
		{}
		
	virtual void request_processors(Processor_Request&) const;
	void display(Output_tee&) const;
	
	Symbol label;		// The step number
	GOMS_Operator_Sptr_list_t operator_sptr_list;	// operator list
	bool similar(const GOMS_Step * p, const Correspondence_map_t&) const;
	int get_number_of_steps();
	bool break_flag;

	bool get_faulted() const
		{return faulted;}
	void set_faulted(bool state);	// set all operators to the faulted state

	const std::string& get_file_name() const 
   		{return file_name;}
	int get_line_number() const
		{return line_number;}
	void set_file_location(const std::string& f, int l) 
		{ 
			file_name = f;
			line_number = l; 
		}

private:
	bool faulted;
	std::string file_name;
	int line_number;

};

typedef Smart_Pointer<GOMS_Step> GOMS_Step_Sptr_t;
typedef std::list<GOMS_Step_Sptr_t> GOMS_Step_Sptr_list_t;

// A GOMS_Procedure is a base class to represent either a method or a selection rule.
// It should be abstract, except we have to be able to construct one for use in creating
// a probe for the containers. This could be fixed by defining the procedure container in the
// GOMS_Model class differently.

class GOMS_Procedure : public Reference_Counted_Object {
public:
	GOMS_Procedure ()
		{}
	GOMS_Procedure(const GOMS_Goal& g) :
		goal(g)
		{}
	GOMS_Procedure(const GOMS_Goal& g, GOMS_Argument_Sptr_List_t& p) :
		goal(g), param_list(p)
		{}
 	virtual ~GOMS_Procedure () // this is a base class
		{}

	GOMS_Step_Sptr_t get_step_sptr(const Symbol& step_name) const;
	GOMS_Step_Sptr_t get_step_sptr(const std::string& file_name, int line_number) const;

	// return iterators for the first and last step in the procedure
	GOMS_Step_Sptr_list_t::iterator get_first_step();
	GOMS_Step_Sptr_list_t::iterator get_last_step();

	virtual void display(Output_tee&) const;
	GOMS_Goal goal;							// goal for the procedure
	GOMS_Goal error_goal;					// goal for error recovery
	GOMS_Argument_Sptr_List_t param_list;	// the pseudo-parameters for the procedure
	GOMS_Step_Sptr_list_t step_sptr_list;	// the steps
	int get_number_of_steps();
	int similarity(const GOMS_Procedure *) const;
	std::list<std::string> get_step_names() const;
};


typedef Smart_Pointer<GOMS_Procedure> GOMS_Procedure_Sptr_t;

struct less_GOMS_Procedure_SP
{
	bool operator() (const GOMS_Procedure_Sptr_t& x, const GOMS_Procedure_Sptr_t& y) const
	{ return  x->goal < y->goal;}
};

typedef std::set<GOMS_Procedure_Sptr_t, less_GOMS_Procedure_SP> 
	GOMS_Procedure_Sptr_set_t;
typedef std::list<GOMS_Procedure_Sptr_t> GOMS_Procedure_Sptr_list_t;



class GOMS_Method : public GOMS_Procedure {
public:	
	GOMS_Method ()
		{}
	GOMS_Method(const GOMS_Goal& g) :
		GOMS_Procedure(g)
		{}
	GOMS_Method(const GOMS_Goal& g, GOMS_Argument_Sptr_List_t& a_l_p) :
		GOMS_Procedure(g, a_l_p)
		{}
	
	virtual void display(Output_tee&) const; // override base class definition
	
};

class GOMS_Selection_Rule_Set : public GOMS_Procedure {
public:	
	GOMS_Selection_Rule_Set ()
		{}
	GOMS_Selection_Rule_Set(const GOMS_Goal& g) :
		GOMS_Procedure(g)
		{}
	GOMS_Selection_Rule_Set(const GOMS_Goal& g, GOMS_Argument_Sptr_List_t& a_l_p) :
		GOMS_Procedure(g, a_l_p)
		{}
	
	virtual void display(Output_tee&) const; // override base class definition	
};

class GOMS_Interrupt_Rule_Set : public GOMS_Procedure {
public:	
	GOMS_Interrupt_Rule_Set() :
		GOMS_Procedure(GOMS_Goal(Check__c, Interrupts__c))
		{}
	GOMS_Interrupt_Rule_Set(GOMS_Argument_Sptr_List_t& a_l_p) :
		GOMS_Procedure(GOMS_Goal(Check_c, Interrupts_c), a_l_p)
		{}
	
	virtual void display(Output_tee&) const; // override base class definition	
};


class GOMS_Model : public Reference_Counted_Object {
public:
	
	// return a smart pointer to the procedure that matches the supplied goal	
	GOMS_Procedure_Sptr_t get_procedure(const GOMS_Goal& goal) const;

	// return reference to the LTM memory container.
	OPV_Container& get_LTM()
		{return theLTM;}

	// return reference to the task_description container.
	OPV_Container& get_task_description()
		{return task_description;}

	// return reference to the initial_visual_state container.
	OPV_Container& get_initial_visual_state() 
		{return initial_visual_state;}

	// return reference to the initial auditory encodings container.
	OPV_Container& get_initial_auditory_state() 
		{return initial_auditory_state;}

	// return reference to the initial WM memory container.
	PV_Container& get_initial_WM_contents()
		{return initial_WM_contents;}

	// calculate the learning time analysis and return as a string
	std::string get_learning_report() const;

	virtual void display(Output_tee&) const;
	
	// break support accessors
	// these functions work with steps identified in terms of goal and step label
	bool is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const;
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const;
	void set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state);
	// these functions work with steps names in a human-readable form defined internally
	// return a list of step names in a standard human readable form
	const std::list<std::string>& get_step_names() const;
	bool is_step_name_valid(const std::string& step_name) const;
	// these functions take the step name in the same form as provided
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& break_name) const;
	void set_break_state(const std::string& break_name, bool state);
	// these functions take the step name in the form of a file name and line number
	// an exception is thrown if the step name is invalid
	bool get_break_state(const std::string& file_name, int line_number) const;
	void set_break_state(const std::string& file_name, int line_number, bool state);

	bool get_fault_enabled() const;
	void set_fault_enabled(bool);
	bool get_step_fault_state(const std::string& rule_name) const;
	void set_step_fault_state(const std::string& rule_name, bool state);
//	const Symbol_list_t& get_fault_step_names() const;

	
	Symbol model_name;
	GOMS_Goal start_goal;
	GOMS_Operator_Sptr_t interrupt_rule_set; // represented as a single operator, no steps
	// procedure_set is keyed by goal; use to find procedure given goal, list alphabetically
	GOMS_Procedure_Sptr_set_t procedure_set;
	// procedure_list is in order of appearance in the input; use for learning calculations	
	GOMS_Procedure_Sptr_list_t procedure_list;
	OPV_Container theLTM;
	OPV_Container task_description;
	OPV_Container initial_visual_state;
	OPV_Container initial_auditory_state;	// contains auditory encodings
	PV_Container initial_WM_contents;

private:	
	mutable std::list<std::string> step_names;	//cache them here
	GOMS_Step_Sptr_t get_step_sptr(const std::string& full_step_name) const;
	GOMS_Step_Sptr_t get_step_sptr(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const;
	GOMS_Step_Sptr_t get_step_sptr(const std::string& file_name, int line_number) const;
};

typedef Smart_Pointer<GOMS_Model> GOMS_Model_Sptr_t;


#endif
