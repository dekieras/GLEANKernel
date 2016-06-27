#include <iostream>
#include <iomanip>
#include <typeinfo>
//#include <cassert>

#include "Workload_Recorders.h"
#include "Recorder.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "WM_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;

Workload_Accumulator::Workload_Accumulator() : total_time(0.0), frequency (0),
	WM_tags(false)
{
}




void Workload_Method_Profiler::initialize()
{
	accumulate = false;
	overall_start_time = 0;
	overall_stop_time = 0;
	first_AG_time = -1000;	// make any problem obvious
	last_RGA_time = 0;
	profile.clear();
	start_time.clear();
	// empty the stack if necessary
	//while (!goal_stack.empty())
	//	goal_stack.pop();
	goal_stack.clear();
	current_goal = GOMS_Goal("xx", "yy");
	// Normal_out << "Workload_Method_Profiler initialized" << endl;
}

// save the current time as the overall start time
void Workload_Method_Profiler::accept_event(const Start_event * )
{
	overall_start_time = get_time();
//	accumulate = false;	// I don't know why this way here - note possible confusion with arrival of first goal ...
	// Normal_out << processor_info() << " Got a Start_event" << endl;
// embedded recorders to not exist yet - they never get an overall start and stop event
}

// save the current time as the overall stop time
void Workload_Method_Profiler::accept_event(const Stop_event * )
{
	// embedded recorders do not get stop event
	overall_stop_time = get_time();
//	if(Debug_out) Debug_out << processor_info() << " Got a Stop_event" << endl;
}

/*
// when a report event is recieved, output a report, and reset
void Workload_Method_Profiler::accept_event(const Report_event * event_ptr)
{
	Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
	report();
	initialize();
}
*/

// Since this type of recorder assumes a goal hierarchy, it isn't set up to deal with multiple execution threads.
// The fix for now is that it only responds to events from the "Main" thread - the thread with this name,
// and ignores all others. - dk 9/15/04

void Workload_Method_Profiler::accept_event(const Cognitive_event * event_ptr)
{
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;

	if (dynamic_cast<const Cognitive_Step_event *> (event_ptr)) {
		// save some time by trapping this high frequency event first
		return;
		}

	if (const Cognitive_Accomplish_Goal_event * p = dynamic_cast<const Cognitive_Accomplish_Goal_event *> (event_ptr)) {
		if(p->thread_ptr->get_thread_name() != Main_c)	// ignore non-main thread activity
			return;
		current_goal = p->goal;
		goal_stack.push_front(current_goal);
		accumulate = true;
		
		// turn on the WM profiler
		profile[current_goal].WM_tags.start_accumulating();

		// do we ever come here when the current goal has not been set to something?
		if (goal_stack.empty() || current_goal == GOMS_Goal() || current_goal == GOMS_Goal("zz", "zz")) {
			Normal_out << "profiling a non-goal: " << current_goal.action << ' ' << current_goal.object << endl;
			Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
			}

		start_time[current_goal] = get_time();
//		if(Debug_out) Debug_out << processor_info() << " Start time saved: " << start_time[p->goal] << endl;
		// if this is the first AG event, first_AG_time will be -1000, so set it to the current time
		if (first_AG_time == -1000)
			first_AG_time = get_time();
		}
		
	else if (const Cognitive_Return_Goal_Accomplished_event * p = dynamic_cast<const Cognitive_Return_Goal_Accomplished_event *> (event_ptr)) {
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
//		if(Debug_out) Debug_out << processor_info() << profile[p->goal].total << endl;
//		if(Debug_out) Debug_out << processor_info() << " Start time from before: " << start_time[p->goal] << endl;

//		Assert(p->goal == current_goal);
		
		if(p->thread_ptr->get_thread_name() != Main_c)	// ignore non-main thread activity
			return;

		if(p->goal != current_goal) {
			Normal_out << processor_info() << " RGA of " << (p->goal).action << ' ' << (p->goal).object
				<< " does not match current goal " << current_goal.action << ' ' << current_goal.object << endl;
			return;
			}

		// do we ever come here when the current goal has not been set to something?
		if (goal_stack.empty() || current_goal == GOMS_Goal() || current_goal == GOMS_Goal("zz", "zz")) {
			Normal_out << "profiling a non-goal: " << current_goal.action << ' ' << current_goal.object << endl;
			Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
			}

		last_RGA_time = get_time();	// save last RGA time
		double duration = double(get_time() - start_time[current_goal]);
		profile[current_goal].total_time = profile[current_goal].total_time + duration;
		profile[current_goal].frequency++;
//		if(Debug_out) Debug_out << processor_info() << profile[current_goal].total << endl;
		start_time.erase(current_goal);
		// turn off the WM profiler for this goal
		profile[current_goal].WM_tags.stop_accumulating();

		// pop our local stack - but check - it shouldn't be empty!
		Assert(!goal_stack.empty());
		goal_stack.pop_front();
		// this might be the last goal - set to empty goal if so, and stop accumulating
		if(goal_stack.empty()) {
			current_goal = GOMS_Goal("zz", "zz");
			accumulate = false;
			}
		else {
			current_goal = goal_stack.front();
			}
		}

	else {
		if (!accumulate)	// if not accumulating, do not profile the current goal
			return;

		if (const Cognitive_Operation_Complete_event * p = dynamic_cast<const Cognitive_Operation_Complete_event *> (event_ptr)) 
			if(p->thread_ptr->get_thread_name() != Main_c)	// ignore non-main thread activity
			return;

		// do we ever come here when the current goal has not been set to something?
		if (goal_stack.empty() || current_goal == GOMS_Goal() || current_goal == GOMS_Goal("zz", "zz")) {
			Normal_out << "profiling a non-goal: " << current_goal.action << ' ' << current_goal.object << endl;
			Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
			}
		// broadcast the event to the embedded recorders for all the goals currently on the stack 
		// it may be an action complete event for one of them
		// WM_Tag_Counter is not interested in cognitive events - so it is not copied in
		// Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		// Normal_out << processor_info() << " copying event to recorders for goal: " << current_goal.action << ' ' << current_goal.object << endl;
//		Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++) {
//			Normal_out << "profiling goal: " << (*it).action << ' ' << (*it).object << endl;
			profile[*it].visual_actions.accept_event(event_ptr);
			profile[*it].auditory_actions.accept_event(event_ptr);
			profile[*it].manual_actions.accept_event(event_ptr);
			profile[*it].vocal_actions.accept_event(event_ptr);
			// Normal_out << processor_info() << " done copying event" << endl;
			}
		}
}

// these event acceptors do not filter on the thread, and so they can mistakenly accumulate with activity from other threads

// forward each incoming modality-specific event to the appropriate embedded recorder
// for the current goal, but only if accumulating
void Workload_Method_Profiler::accept_event(const Visual_event * event_ptr)
{
	if(!accumulate)
		return;
	for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++)
			profile[*it].visual_actions.accept_event(event_ptr);
}

void Workload_Method_Profiler::accept_event(const Auditory_event * event_ptr)
{
	if(!accumulate)
		return;
	for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++)
		profile[*it].auditory_actions.accept_event(event_ptr);
}

void Workload_Method_Profiler::accept_event(const Manual_event * event_ptr)
{
	if(!accumulate)
		return;
//	Normal_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
	for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++) {
//		Normal_out << "profiling goal: " << (*it).action << ' ' << (*it).object << endl;
		profile[*it].manual_actions.accept_event(event_ptr);
		}
}

void Workload_Method_Profiler::accept_event(const Vocal_event * event_ptr)
{
	if(!accumulate)
		return;
	for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++)
		profile[*it].vocal_actions.accept_event(event_ptr);
}

void Workload_Method_Profiler::accept_event(const WM_event * event_ptr)
{
	if(!accumulate)
		return;
	for (list<GOMS_Goal>::const_iterator it = goal_stack.begin(); it != goal_stack.end(); it++)
		profile[*it].WM_tags.accept_event(event_ptr);
}


void Workload_Method_Profiler::report() const
{
/*
	Output_tee_format_saver saver(Normal_out);
	Normal_out << "Workload Method Execution Profile" << endl;
	Normal_out << "Total Run Duration: " 
		<< fixed << setprecision(3) 
		<< setw(10) << (overall_stop_time - overall_start_time) / 1000. 
		<< " seconds" << endl;
	double total_execution_time = (last_RGA_time - first_AG_time) / 1000.;		// for seconds
	Normal_out << "Total Method Execution Time: " 
		<< fixed << setprecision(3) << setw(10) << total_execution_time << " seconds" << endl;
	Normal_out << "  Freq.    Subtotal  Avg. Time  % of Total   Method for goal/Activity Type" << endl;
	for (map <GOMS_Goal, Workload_Accumulator, less<GOMS_Goal> >::const_iterator it = profile.begin();
		 it != profile.end();
		 it++) {
		Normal_out << (*it).first.action << ' ' << (*it).first.object << ':'<< endl;
		int f = (*it).second.frequency;
		double total = (*it).second.total_time / 1000.;	// for seconds
		double avg = total/f;
		double percent = 100. * (total / total_execution_time);
		
		Normal_out << setw(6) << f 
			<< fixed << setprecision(3) << setw(12) << total << setw(12) << avg
			<< setprecision(2) << setw(12) << percent
//			<< "  " << (*it).first.action << ' ' << (*it).first.object 
			<< "  Method Execution"
			<< setprecision(0)
			<< endl;
		// each embedded recorder reports
		(*it).second.visual_actions.short_report(total_execution_time);
		(*it).second.auditory_actions.short_report(total_execution_time);
		(*it).second.manual_actions.short_report(total_execution_time);
		(*it).second.vocal_actions.short_report(total_execution_time);
		(*it).second.WM_tags.short_report(total_execution_time);
		Normal_out << endl;
		}
*/
	Normal_out << get_report();
}


string Workload_Method_Profiler::get_report() const
{
	ostringstream oss;
	oss << "Workload Method Execution Profile" << endl;
	oss << "Total Run Duration: " 
		<< fixed << setprecision(3) 
		<< setw(10) << (overall_stop_time - overall_start_time) / 1000. 
		<< " seconds" << endl;
	double total_execution_time = (last_RGA_time - first_AG_time) / 1000.;		// for seconds
	oss << "Total Method Execution Time: " 
		<< fixed << setprecision(3) << setw(10) << total_execution_time << " seconds" << endl;
	oss << "  Freq.    Subtotal  Avg. Time  % of Total   Method for goal/Activity Type" << endl;
	for (map <GOMS_Goal, Workload_Accumulator, less<GOMS_Goal> >::const_iterator it = profile.begin();
		 it != profile.end();
		 it++) {
		oss << (*it).first.action << ' ' << (*it).first.object << ':'<< endl;
		int f = (*it).second.frequency;
		double total = (*it).second.total_time / 1000.;	// for seconds
		double avg = total/f;
		double percent = 100. * (total / total_execution_time);
		
		oss << setw(6) << f 
			<< fixed << setprecision(3) << setw(12) << total << setw(12) << avg
			<< setprecision(2) << setw(12) << percent
//			<< "  " << (*it).first.action << ' ' << (*it).first.object 
			<< "  Method Execution"
			<< setprecision(0)
			<< endl;
		// each embedded recorder reports
		oss << (*it).second.visual_actions.get_short_report(total_execution_time);
		oss << (*it).second.auditory_actions.get_short_report(total_execution_time);
		oss << (*it).second.manual_actions.get_short_report(total_execution_time);
		oss << (*it).second.vocal_actions.get_short_report(total_execution_time);
//		oss << (*it).second.WM_tags.get_short_report(total_execution_time); // superseded - dk 7/5/06
		oss << (*it).second.WM_tags.get_short_report();
		oss << endl;
		}
	return oss.str();
}

