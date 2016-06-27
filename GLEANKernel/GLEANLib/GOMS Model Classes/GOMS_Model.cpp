#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <algorithm>

#include "GOMS_Model.h"
#include "Cognitive_processor.h"
#include "Event_types.h"
#include "WM_event_types.h"
#include "Output_tee_globals.h"
#include "Assert_throw.h"
#include "Glean_exceptions.h"


//#include <DebugNew.h>
using namespace std;

static string encode_step_name(const string& goal_action, const string& goal_object, const string& step_label);
static void decode_step_name(const string& step_name, string& goal_action, string& goal_object, string& step_label);

// GOMS_Argument resolve methods are in GOMS_Memory_Operators.cpp

// A global function that compares two Symbols using a correspondence map
bool similar( const Symbol& term_a, const Symbol& term_b, const Correspondence_map_t& cm_a_b)
{
	if (term_a == term_b)
		return true;
	else if (cm_a_b.retrieve(term_a) == term_b)
		// if not found, retrieve returns Symbol() which is Nil_c.
		return true;
	else
		return false;
}


// execution/evaluation methods

bool GOMS_Conditional::evaluate(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr)
{

	// must have itr1, itr2 here to satisfy MSVC5!

	for (GOMS_Predicate_Sptr_list_t::iterator itr1 = predicates.begin();
			itr1 != predicates.end(); itr1++) 
		// if any predicate is false, return false
		if (! ((*itr1) ->evaluate(cog_proc_ptr)) ) 
			return false;

	// all predicates are true - execute actions, return true		
	for (GOMS_Operator_Sptr_list_t::iterator itr2 = operators.begin();
			itr2 != operators.end(); itr2++) 
		(*itr2) ->execute(cog_proc_ptr, thread_ptr);

	return true;
}

// These functions return iterators for the first and last step in a GOMS_Procedure
GOMS_Step_Sptr_list_t::iterator 
GOMS_Procedure::get_first_step()
{
	GOMS_Step_Sptr_list_t::iterator itr = step_sptr_list.begin();
	return itr;
}

GOMS_Step_Sptr_list_t::iterator 
GOMS_Procedure::get_last_step()
{
	GOMS_Step_Sptr_list_t::iterator itr = step_sptr_list.end();
	return itr;
}

// return a smart pointer to the procedure that matches the supplied goal

GOMS_Procedure_Sptr_t 
GOMS_Model::get_procedure(const GOMS_Goal& goal) const
{
	GOMS_Procedure_Sptr_t goal_probe_sptr = new GOMS_Procedure(goal);
	// return smart pointer to procedure
	GOMS_Procedure_Sptr_set_t::const_iterator it = procedure_set.find(goal_probe_sptr);
	if (it == procedure_set.end()) {
		return 0;
		}
	else
		return (*it);	// return smart pointer to retrieved procedure.
}

// display and similarity functions for GOMS Model components

void GOMS_Goal::display(Output_tee& ot) const
{
	ot << action << ' ' << object;
}

ostream& operator<< (ostream& os, const GOMS_Goal& g)
{
	os << g.action << ' ' << g.object;
	return os;
}

void GOMS_Constant_Argument::display(Output_tee& ot) const
{
	ot << arg_value;
}

bool GOMS_Constant_Argument::similar(const GOMS_Argument * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Constant_Argument * p = dynamic_cast<const GOMS_Constant_Argument *>(bp)) {
		// similar if argument is similar
		return ::similar(arg_value, p->arg_value, cm); 
		}
	return false;
}

void GOMS_WM_Argument::display(Output_tee& ot) const{
	ot << tag_name;
}

bool GOMS_WM_Argument::similar(const GOMS_Argument * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_WM_Argument * p = dynamic_cast<const GOMS_WM_Argument *>(bp)) {
		// similar if tag_name is similar
		return ::similar(tag_name, p->tag_name, cm);
		}
	return false;
}

// assuming that get this symbol is just for display purposes;
// return a Symbol containing the same string as would be used for display

Symbol GOMS_PO_Argument::get_symbol() const 
{
	string s;
	s += prop_name_sp->get_symbol().c_str();
	s += " of ";
	s += obj_name_sp->get_symbol().c_str();
	
	Symbol ss (s.c_str());

	return ss;
}

void GOMS_PO_Argument::display(Output_tee& ot) const
{
	ot << prop_name_sp->get_symbol() << " of " << obj_name_sp->get_symbol();
}

bool GOMS_PO_Argument::similar(const GOMS_Argument * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_PO_Argument * p = dynamic_cast<const GOMS_PO_Argument *>(bp)) {
		// similar if prop_name and obj_name are similar
		// arguments here are pointers to GOMS_Arguments
		return prop_name_sp->similar(p->prop_name_sp, cm) && 
			   obj_name_sp->similar(p->obj_name_sp, cm);
		}
	return false;
}

void GOMS_PV_Argument::display(Output_tee& ot) const
{
	ot << prop_name_sp->get_symbol() << " is " << prop_value_sp->get_symbol();
}

bool GOMS_PV_Argument::similar(const GOMS_Argument * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_PV_Argument * p = dynamic_cast<const GOMS_PV_Argument *>(bp)) {
		// similar if prop_name and prop_value are similar
		return prop_name_sp->similar(p->prop_name_sp, cm) && 
			   prop_value_sp->similar(p->prop_value_sp, cm);
		}
	return false;
}

// predicates are all similar if type is the same and arguments are similar
bool GOMS_Predicate::similar(const GOMS_Predicate * p, const Correspondence_map_t& cm) const
{
	if (
		typeid(*this) == typeid(*p) &&
		this->first_arg->similar(p->first_arg, cm) &&
		this->second_arg->similar(p->second_arg, cm)
		)
		return true;
	else
		return false;
}



void GOMS_Conditional::display(Output_tee& ot) const
{

	// can't declare the same iterator name in the for's because of MSVC5!
	
	// if no predicates then is "else" form
	if (!predicates.empty()) {
		ot << "\nIf ";
		for (GOMS_Predicate_Sptr_list_t::const_iterator itr1 = predicates.begin();
				itr1 != predicates.end(); itr1++) {
			(*itr1) ->display(ot);
			// is this the last predicate?
			GOMS_Predicate_Sptr_list_t::const_iterator itr2 = itr1;
			itr2++;
			if (itr2 != predicates.end())
				ot << ", and " ;
			else
				ot << ' ' ;
			}
		ot << "\nThen ";
		}
	else {
		ot << "\nElse ";
		}
		
	for (GOMS_Operator_Sptr_list_t::const_iterator itr3 = operators.begin();
			itr3 != operators.end(); itr3++) {
		(*itr3) ->display(ot);
		// is this the last operator?
		GOMS_Operator_Sptr_list_t::const_iterator itr4 = itr3;
		itr4++;
		if (itr4 != operators.end())
			ot << "; " ;
		}
}

bool GOMS_Conditional::similar(const GOMS_Conditional * p, const Correspondence_map_t& cm) const
{
	// two conditionals are similar if 
	// all of the predicates and all of the operators are similar
	
	{GOMS_Predicate_Sptr_list_t::const_iterator it1, it2;
	for (
		it1  = predicates.begin(), it2  = p->predicates.begin();
		it1 != predicates.end() && it1 != p->predicates.end();
		it1++, it2++) {
			if (!((*it1)->similar(*it2, cm)))
				return false;
			}
	}
			
	{GOMS_Operator_Sptr_list_t::const_iterator it1, it2;
	for (
		it1  = operators.begin(), it2  = p->operators.begin();
		it1 != operators.end() && it1 != p->operators.end();
		it1++, it2++) {
			if (!((*it1)->similar(*it2, cm)))
				return false;
			}
	}
	return true;
}

void GOMS_Conditional::request_processors(Processor_Request& pr) const
{
	GOMS_Operator_Sptr_list_t::const_iterator it;
	for (it = operators.begin(); it != operators.end(); it++)
			(*it)->request_processors(pr);
}

void GOMS_Step::display(Output_tee& ot) const
{
	ot << "Step " << label << ". ";
	for (GOMS_Operator_Sptr_list_t::const_iterator itr = operator_sptr_list.begin();
			itr != operator_sptr_list.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last operator?
		GOMS_Operator_Sptr_list_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != operator_sptr_list.end())
			ot << "; " ;
		else
			ot << '.' ;
		}
	ot << endl;
}

int GOMS_Step::get_number_of_steps()
{
	int n = 0;
	// ask each operator how many substeps it has, with 0 as default
	for (GOMS_Operator_Sptr_list_t::const_iterator it = operator_sptr_list.begin();
			it != operator_sptr_list.end(); it++) 
		n += (*it)->get_number_of_steps();
	// if some operators return some substeps, return the total
	if (n > 0)
		return n;
	// else count only one step
	return 1;	
}

void GOMS_Step::set_faulted(bool state)	// set all operators to the faulted state
{
	faulted = state;
	// set each operator to the faulted state
	for (GOMS_Operator_Sptr_list_t::iterator it = operator_sptr_list.begin();
			it != operator_sptr_list.end(); it++) 
		(*it)->set_faulted(faulted);
}

void GOMS_Step::request_processors(Processor_Request& pr) const
{
	GOMS_Operator_Sptr_list_t::const_iterator it;
	for (it = operator_sptr_list.begin(); it != operator_sptr_list.end(); it++)
			(*it)->request_processors(pr);
}


bool GOMS_Step::similar(const GOMS_Step * p, const Correspondence_map_t& cm) const
{
	// two steps are similar if all of the operators are similar
	// and appear in the same order
	// step numbers/labels are ignored
	GOMS_Operator_Sptr_list_t::const_iterator it1, it2;
	for (
		it1  = operator_sptr_list.begin(), it2  = p->operator_sptr_list.begin();
		it1 != operator_sptr_list.end() && it1 != p->operator_sptr_list.end();
		it1++, it2++) {
			if (!((*it1)->similar(*it2, cm)))
				return false;
			}
	return true;
}


void GOMS_Procedure::display(Output_tee& ot) const
{
	goal.display(ot);
//	ot << "On error: ";
//	error_goal.display(ot);
	// output using list if it has anything in it
	if (!param_list.empty()) {
		ot << " using ";
		for (GOMS_Argument_Sptr_List_t::const_iterator itr = param_list.begin();
				itr != param_list.end(); itr++) {
			(*itr) ->display(ot);
			// is this the last argument?
			GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
			itr2++;
			if (itr2 != param_list.end())
				ot << ", " ;
			}
		}
	ot << endl;
	for (GOMS_Step_Sptr_list_t::const_iterator it = step_sptr_list.begin();
			it != step_sptr_list.end(); it++) 
		(*it) ->display(ot);
}

// return a list of step names that are in the form: 
// <goal_action> <goal_object>:Step <label>
//Symbol_list_t GOMS_Procedure::get_step_names() const
list<string> GOMS_Procedure::get_step_names() const
{
	string goal_action(goal.action.str());
	string goal_object(goal.object.str());
	list<string> result;
	
	for (GOMS_Step_Sptr_list_t::const_iterator it = step_sptr_list.begin();
			it != step_sptr_list.end(); it++) {
		result.push_back(encode_step_name(goal_action, goal_object, (*it)->label.str()));
		}

	return result;
}

int GOMS_Procedure::get_number_of_steps()
{
	int n = 0;
	// ask each step how many substeps it has, with 1 as default
	for (GOMS_Step_Sptr_list_t::const_iterator it = step_sptr_list.begin();
			it != step_sptr_list.end(); it++) 
		n += (*it)->get_number_of_steps();
	return n + 1;	// plus one for the method or selection rule set statement
}

GOMS_Step_Sptr_t GOMS_Procedure::get_step_sptr(const Symbol& label) const
{
	// scan through the procedure steps and set the current_step_itr to the first
	// step with a matching label.
	GOMS_Step_Sptr_list_t::const_iterator step_itr;
	for (step_itr = step_sptr_list.begin();
		 step_itr != step_sptr_list.end();
		 step_itr++)
		if ((*step_itr)->label == label)  {
			return *step_itr;
			}
	return 0;
}

GOMS_Step_Sptr_t GOMS_Procedure::get_step_sptr(const string& file_name, int line_number) const
{
	// scan through the procedure steps and return the iterator to the one matching file_name and line_number
	GOMS_Step_Sptr_list_t::const_iterator step_itr;
	for (step_itr = step_sptr_list.begin();
		 step_itr != step_sptr_list.end();
		 step_itr++)
		if ((*step_itr)->get_line_number() == line_number && (*step_itr)->get_file_name() == file_name)  {
			return *step_itr;
			}
	return 0;
}

int GOMS_Procedure::similarity(const GOMS_Procedure * p) const
{
	// for two procedures to be similar, the goals and pseudo-parameter lists must be similar
	// are the goals similar? either one or both of the goal terms are the same
	bool actions_same = (goal.action == p->goal.action);
	bool objects_same = (goal.object == p->goal.object);

	if (!actions_same && !objects_same) {
		return 0;
		}
	else if (actions_same && objects_same) {
		Normal_out << "*** Error: Two procedures have identical goals: ***" << endl;
		goal.display(Normal_out);
		p->goal.display(Normal_out);
		return 0;
		}

	Correspondence_map_t correspondence_map;

	// store other goal term in correspondence map
	if (actions_same) {
		correspondence_map.store(goal.object, p->goal.object);
		}
	else if (objects_same) {
		correspondence_map.store(goal.action, p->goal.action);
		}

	// parameter lists need to be the same size
	if (param_list.size() != p->param_list.size())
		return 0;
	// add the symbols in the parameter list to the correspondence map
	// they are all tag names
	{
	GOMS_Argument_Sptr_List_t::const_iterator it1, it2;
	for(it1 = param_list.begin(), it2 = p->param_list.begin();
		it1 != param_list.end() && it2 != p->param_list.end();
		it1++, it2++) 
			correspondence_map.store((*it1)->get_symbol(), (*it2)->get_symbol());
//	if(Debug_out) Debug_out << "correspondence:" << endl;
//	correspondence_map.display_contents(if(Debug_out) Debug_out);
	}
	
	// the similarity of two procedures is the number of steps that are
	// similiar, counting from the beginning, and stopping when dissimilar
	// step numbers/labels are ignored
	int similar_steps = 0;
	
	GOMS_Step_Sptr_list_t::const_iterator it1, it2;
	for (
		it1  = step_sptr_list.begin(), it2  = p->step_sptr_list.begin();
		it1 != step_sptr_list.end() && it1 != p->step_sptr_list.end();
		it1++, it2++) {
			if ((*it1)->similar(*it2, correspondence_map))
				similar_steps++;
			else
				break;
			}
//	if there was some similarity, then count the method or SRS statement also
	if (similar_steps > 0)
		similar_steps++;
	return similar_steps;
}

void GOMS_Method::display(Output_tee& ot) const
{
	ot << "\nMethod_for_goal: ";
	GOMS_Procedure::display(ot);
}
void GOMS_Selection_Rule_Set::display(Output_tee& ot) const
{
	ot << "\nSelection_Rule_Set_for_goal: ";
	GOMS_Procedure::display(ot);
}

void GOMS_Interrupt_Rule_Set::display(Output_tee& ot) const
{
	ot << "\nInterrupt_rules: ";
	GOMS_Procedure::display(ot);
}

void GOMS_Model::display(Output_tee& ot) const
{
	ot << "Model name: " << model_name << endl;
	ot << "Starting goal: ";
	start_goal.display(ot);
	ot << endl;
	ot << "\nLTM Contents: " << endl;
	theLTM.display_contents(ot);
	ot << "\nTask Description Contents: " << endl;
	task_description.display_contents(ot);
	ot << "\nProcedural knowledge:" << endl;
	for (GOMS_Procedure_Sptr_set_t::const_iterator it = procedure_set.begin();
			it != procedure_set.end(); it++) 
		(*it) ->display(ot);
	ot << "\nInterrupt Rules: " << endl;
	if(interrupt_rule_set)
		interrupt_rule_set->display(ot);
	ot << "\nInitial WM Contents: " << endl;
	initial_WM_contents.display_contents(ot);
	ot << "\nInitial Visual State: " << endl;
	initial_visual_state.display_contents(ot);
	ot << "\n" << endl;
}


// Compare each procedure with the preceding procedures
// calculate the number of steps to learn for each procedure
// by getting the minimum of (number of steps+1 - similarity with each previous procedure)
string GOMS_Model::get_learning_report() const
{
	ostringstream oss;
	
	int total_steps_in_procedures = 0;
	int total_steps_transferred = 0;
	int total_steps_to_learn = 0;

	// first procedure has steps_to_learn equal to the number of steps
	// subsequent procedures are compared to all previous procedures
	// find maximum similarity, keeping track of goal of procedure contributing it
	// save maximum similarity

	GOMS_Procedure_Sptr_list_t::const_iterator current_it;	// points to current procedure
	GOMS_Procedure_Sptr_list_t::const_iterator scan_it;		// scans over previous procedures
	
	// fancy formatting - get maximum length of goal action and object strings and number of steps
	int max_action_len = 0;
	int max_object_len = 0;
	int max_step_len = 0;
	for(current_it = procedure_list.begin(); current_it != procedure_list.end(); current_it++) {
		int len = int((*current_it)->goal.action.length());
		if (len > max_action_len)
			max_action_len = len;
		len = int((*current_it)->goal.object.length());
		if (len > max_object_len)
			max_object_len = len;
		len = (*current_it)->get_number_of_steps();
		if (len > max_step_len)
			max_step_len = len;		
		}
	// add 1 for space
	max_action_len++;
	max_object_len++;
	
	// set the maximum field width for the steps
/*	if (max_step_len > 999)
		max_step_len = 6;
	else
		max_step_len = 4;
*/
	max_step_len = 10;

	oss << "LEARNING ANALYSIS" << endl;
	oss << left << setw(2 * max_action_len + 2 * max_object_len + 3 * max_step_len) << setfill('-') << '-' << setfill(' ') << endl;
	oss << left
		 << setw(max_action_len) << "Method"
		 << right << setw(max_object_len) << "Units:" << "  "
		 << setw(max_step_len - 5) << "number"
		 << setw(max_step_len) << "learned"
		 << setw(max_step_len) << "to learn" << "   "
		 << "Source Method" 
		 << endl;
	oss << left << setw(2 * max_action_len + 2 * max_object_len + 3 * max_step_len) << setfill('-') << '-' << setfill(' ') << endl;
	for(current_it = procedure_list.begin(); current_it != procedure_list.end(); current_it++) {
		int steps_in_procedure = 0;
		int steps_to_learn = 0;
		int steps_in_common = 0;
		int max_similarity = 0;
		GOMS_Goal max_goal(Empty_string_c, Empty_string_c);
			
		// scan previous procedures
		for (scan_it = procedure_list.begin(); scan_it != current_it; scan_it++) {
			// get the similarity of this procedure relative to the previous one
			steps_in_common = (*current_it)->similarity(*scan_it);
			// if current-scanned similarity is greater than max_similarity, save the scanned goal
			if (steps_in_common > max_similarity) {
				max_similarity = steps_in_common;
				max_goal = (*scan_it)->goal;
				}
			}
		steps_in_procedure = (*current_it)->get_number_of_steps();
		steps_to_learn = steps_in_procedure - max_similarity;
		oss << left 
			 << setw(max_action_len) << (*current_it)->goal.action << setw(max_object_len) << (*current_it)->goal.object 
			 << right 
			 << setw(max_step_len - 5) << steps_in_procedure 
			 << setw(max_step_len) << max_similarity 
			 << setw(max_step_len) << steps_to_learn << "   "
			 << left 
			 << setw(max_action_len) <<  max_goal.action <<  setw(max_object_len) << max_goal.object  
			 << endl;
		total_steps_in_procedures += steps_in_procedure;
		total_steps_transferred += max_similarity;
		total_steps_to_learn += steps_to_learn;
		}
	oss << left << setw(2 * max_action_len + 2 * max_object_len + 3 * max_step_len) << setfill('-') << '-' << setfill(' ') << endl;
	oss << left 
		 << setw(max_action_len + max_object_len) << "Totals:" 
		 << right 
		 << setw(max_step_len - 5) << total_steps_in_procedures 
		 << setw(max_step_len) << total_steps_transferred 
		 << setw(max_step_len) << total_steps_to_learn 
		 << endl;
	return oss.str();
}


// break support accessors
// these functions work with steps identified in terms of goal and step label
bool GOMS_Model::is_step_name_valid(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{
	return get_step_sptr(goal_action, goal_object, step_label);

}

// an exception is thrown if the step name is invalid
bool GOMS_Model::get_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(goal_action, goal_object, step_label);
	if(!step_ptr)
		throw Glean_user_error("Step name \"" + encode_step_name(goal_action, goal_object, step_label) + "\" is invalid");
	return step_ptr->break_flag;
}

void GOMS_Model::set_break_state(const std::string& goal_action, const std::string& goal_object, const std::string& step_label, bool state)
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(goal_action, goal_object, step_label);
	if(!step_ptr)
		throw Glean_user_error(encode_step_name(goal_action, goal_object, step_label) + " is invalid");
	step_ptr->break_flag = state;
}

// these functions work with steps names in a human-readable form defined internally

// return a list of step names in a standard human readable form
// This list is useful for a GUI list showing the possible steps to break on.
// Thus, the steps will be in the order they appear in the method,
// but the methods will be in the container order
// cache the names in the list to save time.
const list<string>& GOMS_Model::get_step_names() const
{
	if(!step_names.empty())
		return step_names;
		
	for (GOMS_Procedure_Sptr_set_t::const_iterator it = procedure_set.begin();
			it != procedure_set.end(); it++) {
		list<string> proc_step_names = (*it)->get_step_names();
		step_names.splice(step_names.end(), proc_step_names);
		}
	return step_names;
}

bool GOMS_Model::is_step_name_valid(const std::string& step_name) const
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(step_name);
	return step_ptr;
}





// these functions take the step name in the same form as provided
// an exception is thrown if the step name is invalid
bool GOMS_Model::get_break_state(const std::string& step_name) const
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(step_name);
	if(!step_ptr)
		throw Glean_user_error("Step name \"" + step_name + "\" is invalid");
	return step_ptr->break_flag;
}

void GOMS_Model::set_break_state(const std::string& step_name, bool state)
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(step_name);
	if(!step_ptr)
		throw Glean_user_error("Step name \"" + step_name + "\" is invalid");
	step_ptr->break_flag = state;
}

// these functions take the step name as a file name and line number
bool GOMS_Model::get_break_state(const string& file_name, int line_number) const
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(file_name, line_number);
	if(!step_ptr) {
		ostringstream oss;
		oss << "Step specification file \"" << file_name << "\" at line " << line_number << " is invalid";
		throw Glean_user_error(oss.str());
		}
	return step_ptr->break_flag;
}

void GOMS_Model::set_break_state(const string& file_name, int line_number, bool state)
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(file_name, line_number);
	if(!step_ptr) {
		ostringstream oss;
		oss << "Step specification file \"" << file_name << "\" at line " << line_number << " is invalid";
		throw Glean_user_error(oss.str());
		}
	step_ptr->break_flag = state;
}



/*
bool GOMS_Model::get_step_fault_state(const Symbol& fault_point_name) const
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(fault_point_name);
	return step_ptr->get_faulted();
}

void GOMS_Model::set_step_fault_state(const Symbol& fault_point_name, bool state)
{
	GOMS_Step_Sptr_t step_ptr = get_step_sptr(fault_point_name);
	step_ptr->set_faulted(state);
}
*/



// find the pointer to a specified step; return 0 if not found
GOMS_Step_Sptr_t GOMS_Model::get_step_sptr(const std::string& goal_action, const std::string& goal_object, const std::string& step_label) const
{	
	GOMS_Step_Sptr_t step_ptr = 0;
	GOMS_Procedure_Sptr_t proc_ptr = get_procedure(GOMS_Goal(Symbol(goal_action), Symbol(goal_object)));
	if(proc_ptr)
		step_ptr = proc_ptr->get_step_sptr(Symbol(step_label));
	return step_ptr;
}

// find the pointer to a step by parsing the goal out of the step name; return 0 if not found
GOMS_Step_Sptr_t GOMS_Model::get_step_sptr(const string& full_step_name) const
{
	string goal_action, goal_object, step_label;
	decode_step_name(full_step_name, goal_action, goal_object, step_label);
	return get_step_sptr(goal_action, goal_object, step_label);
}

// find the pointer to a step by searching for the matching file name and line number
GOMS_Step_Sptr_t GOMS_Model::get_step_sptr(const string& file_name, int line_number) const
{
	GOMS_Step_Sptr_t step_sptr = 0;
	for (GOMS_Procedure_Sptr_set_t::const_iterator it = procedure_set.begin();
			it != procedure_set.end(); it++) {
		if((step_sptr = (*it)->get_step_sptr(file_name, line_number)))
			return step_sptr;
		}
	return step_sptr;
}

// put the strings for goal action, object, and step label into the standard form
string encode_step_name(const string& goal_action, const string& goal_object, const string& step_label)
{
		string stepname(goal_action);
		stepname += " ";
		stepname += goal_object;
		stepname += ": Step ";
		stepname += step_label;
		return stepname;
}

void decode_step_name(const string& step_name, string& goal_action, string& goal_object, string& step_label)
{
	std::string::size_type term_start, term_end, term_len;
	// assume string is now in correct form
	term_start = 0;
	// find end of action term
	term_end = step_name.find(" ", term_start);
	if(term_end == string::npos)
		Glean_user_error Glean_user_error("Could not interpret step name \"" + step_name + "\"");
	term_len = term_end - term_start;
	goal_action = step_name.substr(term_start, term_len);
	term_start = term_end + 1;
	// find end of object term
	term_end = step_name.find(":", term_start);
	if(term_end == string::npos)
		throw Glean_user_error("Could not interpret step name \"" + step_name + "\"");
	term_len = term_end - term_start;
	goal_object = step_name.substr(term_start, term_len);
	term_start = term_end + 2;
	// skip past the word "Step"
	term_end = step_name.find(" ", term_start);
	if(term_end == string::npos)
		throw Glean_user_error("Could not interpret step name \"" + step_name + "\"");
	// bump past the space
	term_start = term_end + 1;
	// step name is between term_start and extends for term_len
	if(step_name.length() <= term_start)
		throw Glean_user_error("Could not interpret step name \"" + step_name + "\"");	
	term_len = step_name.length() - term_start;
	step_label = step_name.substr(term_start, term_len);
}


