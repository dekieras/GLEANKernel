#include "GOMS_Model.h"
#include "GOMS_Control_Operators.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Event_types.h"
#include "Cognitive_event_types.h"
#include "WM_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_exceptions.h"

//#include <DebugNew.h>
using namespace std;


// flow of control operator functions

void GOMS_Accomplish_Goal_Operator::execute(Cognitive_processor *, GOMS_Execution_Thread * thread_ptr) 
{
	// do not schedule a event for the recorders - dk 3/3/01
//	Cognitive_event * event_ptr = new Cognitive_Accomplish_Goal_event 
//		(cog_proc_ptr->get_step_end_time(), 0,  goal);
//	cog_proc_ptr->schedule_event(event_ptr); 
	thread_ptr->push_accomplish_goal(*this);
}

void GOMS_Accomplish_Goal_Operator::display(Output_tee& ot) const
{
	ot << "Accomplish_goal: ";
	goal.display(ot);
	if (!arg_list.empty()) {
		ot << " using ";
		for (GOMS_Argument_Sptr_List_t::const_iterator itr = arg_list.begin();
			itr != arg_list.end(); itr++) {
			(*itr) ->display(ot);
			// is this the last argument?
			GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
			itr2++;
			if (itr2 != arg_list.end())
				ot << ", " ;
			}
		}
}
		
bool GOMS_Accomplish_Goal_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Accomplish_Goal_Operator * p = dynamic_cast<const GOMS_Accomplish_Goal_Operator *>(bp)) {
		// similar if goal terms are similar
		if ( 
			!::similar(goal.action, p->goal.action, cm) ||
			!::similar(goal.object, p->goal.object, cm) 
			)
			return false;
		// pseudo arguments must match
		GOMS_Argument_Sptr_List_t::const_iterator itr1, itr2;
		for (itr1 = arg_list.begin(), itr2 = p->arg_list.begin();
			itr1 != arg_list.end() && itr2 != p->arg_list.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}

void GOMS_Also_Accomplish_Goal_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
	// do not schedule a event for the recorders - dk 3/3/01
//	Cognitive_event * event_ptr = new Cognitive_Accomplish_Goal_event 
//		(cog_proc_ptr->get_step_end_time(), 0,  goal);
//	cog_proc_ptr->schedule_event(event_ptr); 
	cog_proc_ptr->spawn_accomplish_goal(goal, thread_name);
}

void GOMS_Also_Accomplish_Goal_Operator::display(Output_tee& ot) const
{
	ot << "Also_accomplish_goal: ";
	goal.display(ot);
	ot << " as " << thread_name;
	if (!arg_list.empty()) {
		ot << " using ";
		for (GOMS_Argument_Sptr_List_t::const_iterator itr = arg_list.begin();
			itr != arg_list.end(); itr++) {
			(*itr) ->display(ot);
			// is this the last argument?
			GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
			itr2++;
			if (itr2 != arg_list.end())
				ot << ", " ;
			}
		}
}
		
bool GOMS_Also_Accomplish_Goal_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Also_Accomplish_Goal_Operator * p = dynamic_cast<const GOMS_Also_Accomplish_Goal_Operator *>(bp)) {
		// similar if goal terms are similar
		if ( 
			!::similar(goal.action, p->goal.action, cm) ||
			!::similar(goal.object, p->goal.object, cm) 
			)
			return false;
		// similar if thread names are similar
		if ( 
			!::similar(thread_name, p->thread_name, cm) 
			)
			return false;
		// pseudo arguments must match
		GOMS_Argument_Sptr_List_t::const_iterator itr1, itr2;
		for (itr1 = arg_list.begin(), itr2 = p->arg_list.begin();
			itr1 != arg_list.end() && itr2 != p->arg_list.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}

void GOMS_Return_Goal_Accomplished_Operator::execute(Cognitive_processor *, GOMS_Execution_Thread * thread_ptr) 
{
	// do not schedule a event for the recorders - dk 3/3/01
//	Cognitive_event * event_ptr = new Cognitive_Return_Goal_Accomplished_event 
//		(cog_proc_ptr->get_step_end_time(), 0,  cog_proc_ptr->current_goal);
//	cog_proc_ptr->schedule_event(event_ptr); 
	thread_ptr->pop_goal_accomplished();
}

void GOMS_Return_Goal_Accomplished_Operator::display(Output_tee& ot) const
{
	ot << "Return_with_goal_accomplished";
}

bool GOMS_Return_Goal_Accomplished_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t&) const
{
//	if (const GOMS_Return_Goal_Accomplished_Operator * p = dynamic_cast<const GOMS_Return_Goal_Accomplished_Operator *>(bp)) 
	if (dynamic_cast<const GOMS_Return_Goal_Accomplished_Operator *>(bp)) 
		// similar if same type
		return true;
	else
		return false;
}

void GOMS_Abort_and_Restart_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// do not schedule a event for the recorders - dk 3/3/01
//	Cognitive_event * event_ptr = new Cognitive_Abort_and_Restart_event 
//		(cog_proc_ptr->get_step_end_time(), 0,  cog_proc_ptr->current_goal);
//	cog_proc_ptr->schedule_event(event_ptr); 
	cog_proc_ptr->abort_and_restart(thread_ptr);
}

void GOMS_Abort_and_Restart_Operator::display(Output_tee& ot) const
{
	ot << "Abort_and_restart";
}

bool GOMS_Abort_and_Restart_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t&) const
{
//	if (const GOMS_Abort_and_Restart_Operator * p = dynamic_cast<const GOMS_Abort_and_Restart_Operator *>(bp)) 
	if (dynamic_cast<const GOMS_Abort_and_Restart_Operator *>(bp)) 
		// similar if same type
		return true;
	else
		return false;
}

void GOMS_Resume_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
	Symbol rthread_name = thread_name->resolve(cog_proc_ptr);
	GOMS_Goal goal(goal_action->resolve(cog_proc_ptr), goal_object->resolve(cog_proc_ptr));
	Symbol rstep_name = step_name->resolve(cog_proc_ptr);
	
	cog_proc_ptr->resume(rthread_name, goal, rstep_name);
}

void GOMS_Resume_Operator::display(Output_tee& ot) const
{
	ot << "Resume " << thread_name->get_symbol() << " accomplishing " 
		<< goal_action->get_symbol() << ' ' << goal_object->get_symbol() << " at " << thread_name->get_symbol();
}

bool GOMS_Resume_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Resume_Operator * p = dynamic_cast<const GOMS_Resume_Operator *>(bp)) 
		if (::similar(thread_name->get_symbol(), p->thread_name->get_symbol(), cm) &&
			::similar(goal_action->get_symbol(), p->goal_action->get_symbol(), cm) &&
			::similar(goal_object->get_symbol(), p->goal_object->get_symbol(), cm) &&
			::similar(step_name->get_symbol(), p->step_name->get_symbol(), cm))
			return true;
	return false;
}


void GOMS_Raise_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Symbol rexception_name = exception_name->resolve(cog_proc_ptr);
	cog_proc_ptr->raise_exception(thread_ptr, rexception_name);
}

void GOMS_Raise_Operator::display(Output_tee& ot) const
{
	ot << "Raise " << exception_name->get_symbol();
}

bool GOMS_Raise_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Raise_Operator * p = dynamic_cast<const GOMS_Raise_Operator *>(bp)) 
		if (::similar(exception_name->get_symbol(), p->exception_name->get_symbol(), cm))
			return true;
	return false;
}

void GOMS_Stop_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
	Symbol rmessage = message->resolve(cog_proc_ptr);
	cog_proc_ptr->stop_simulation(rmessage);
}

void GOMS_Stop_Operator::display(Output_tee& ot) const
{
	ot << "Stop_with_message " << message->get_symbol();
}

bool GOMS_Stop_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Stop_Operator * p = dynamic_cast<const GOMS_Stop_Operator *>(bp)) 
		if (::similar(message->get_symbol(), p->message->get_symbol(), cm))
			return true;
	return false;
}


void GOMS_Goto_Operator::execute(Cognitive_processor *, GOMS_Execution_Thread * thread_ptr) 
{
	thread_ptr->goto_step_label(label);
}

void GOMS_Goto_Operator::display(Output_tee& ot) const
{
	ot << "Goto " << label;
}

bool GOMS_Goto_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Goto_Operator * p = dynamic_cast<const GOMS_Goto_Operator *>(bp)) {
		// similar if step label term is similar
		if (::similar(label, p->label, cm))
			return true;
		}
	return false;
}

void GOMS_Decide_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr)
{
	// start evaluating the conditionals, stop when one evaluates to true.
	// the conditional will have executed its operators
	for (GOMS_Conditional_Sptr_list_t::iterator itr = conditionals.begin();
			itr != conditionals.end(); itr++) {
		if ((*itr) ->evaluate(cog_proc_ptr, thread_ptr))
			break;
	}

}

 void GOMS_Decide_Operator::display(Output_tee& ot) const{
	ot << "Decide: ";
	for (GOMS_Conditional_Sptr_list_t::const_iterator itr = conditionals.begin();
			itr != conditionals.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last conditional?
		GOMS_Conditional_Sptr_list_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != conditionals.end())
			ot << "; " ;
		}

}

bool GOMS_Decide_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Decide_Operator * p = dynamic_cast<const GOMS_Decide_Operator *>(bp)) {
		// conditionals must match
		GOMS_Conditional_Sptr_list_t::const_iterator itr1, itr2;
		for (itr1 = conditionals.begin(), itr2 = p->conditionals.begin();
			itr1 != conditionals.end() && itr2 != p->conditionals.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}

int GOMS_Decide_Operator::get_number_of_steps() const
{
	// return the number of conditionals
	return static_cast<int>(conditionals.size());
}

void GOMS_Decide_Operator::request_processors(Processor_Request& pr) const
{
	GOMS_Conditional_Sptr_list_t::const_iterator it;
	for (it = conditionals.begin(); it != conditionals.end(); it++)
			(*it)->request_processors(pr);

}

void GOMS_Selection_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr)
{
	// start evaluating the conditionals, stop when one evaluates to true.
	// the conditional will have executed its operators
	bool allConditionsFalse = true;
	for (GOMS_Conditional_Sptr_list_t::iterator itr = conditionals.begin();
			itr != conditionals.end(); itr++)
	{
		if ((*itr) ->evaluate(cog_proc_ptr, thread_ptr))
		{
			allConditionsFalse = false;
			break;
		}
	}
	if (allConditionsFalse)
	{
		Normal_out << "*** Model Error: All conditions in a Selection Rule were false." << std::endl;
		display(Normal_out);
		Normal_out << "WM contents: " << endl;
		cog_proc_ptr->get_human_ptr()->get_Wm_ptr()->display_contents(Normal_out);
		throw Glean_exception("*** Model Error: All conditions in a Selection Rule were false.");
		//cog_proc_ptr->pause_and_then_single_step();
	}
}

bool GOMS_Selection_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Selection_Operator * p = dynamic_cast<const GOMS_Selection_Operator *>(bp)) {
		// conditionals must match
		GOMS_Conditional_Sptr_list_t::const_iterator itr1, itr2;
		for (itr1 = conditionals.begin(), itr2 = p->conditionals.begin();
			itr1 != conditionals.end() && itr2 != p->conditionals.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}


void GOMS_Interrupt_Rule_Set_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr)
{
	thread_ptr->stay_at_step();	// do not advance the step iterator - stay looping on this step
	// start evaluating the conditionals, stop when one evaluates to true.
	// the conditional will have executed its operators
	for (GOMS_Conditional_Sptr_list_t::iterator itr = conditionals.begin();
			itr != conditionals.end(); itr++) {
		if ((*itr) ->evaluate(cog_proc_ptr, thread_ptr))
			break;
	}

}

 void GOMS_Interrupt_Rule_Set_Operator::display(Output_tee& ot) const{
	for (GOMS_Conditional_Sptr_list_t::const_iterator itr = conditionals.begin();
			itr != conditionals.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last conditional?
		GOMS_Conditional_Sptr_list_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != conditionals.end())
			ot << "; " ;
		}

}

bool GOMS_Interrupt_Rule_Set_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Interrupt_Rule_Set_Operator * p = dynamic_cast<const GOMS_Interrupt_Rule_Set_Operator *>(bp)) {
		// conditionals must match
		GOMS_Conditional_Sptr_list_t::const_iterator itr1, itr2;
		for (itr1 = conditionals.begin(), itr2 = p->conditionals.begin();
			itr1 != conditionals.end() && itr2 != p->conditionals.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}

int GOMS_Interrupt_Rule_Set_Operator::get_number_of_steps() const
{
	// return the number of conditionals
	return static_cast<int>(conditionals.size());
}

