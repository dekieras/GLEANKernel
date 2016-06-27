#include <iostream>
#include <iomanip>
#include <typeinfo>

#include "Cognitive_Recorders.h"
#include "Recorder.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "WM_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;

void Cognitive_Method_Profiler::initialize()
{
	profile.clear();
	start_time.clear();
	overall_start_time = 0;
	overall_stop_time = 0;
	first_AG_time = 0;
	last_RGA_time = 0;
}

// save the current time as the overall start time
void Cognitive_Method_Profiler::accept_event(const Start_event * event_ptr)
{
	overall_start_time = event_ptr->get_time();
//	if(Trace_out && get_trace()) Trace_out << processor_info() << " Got a Start_event" << endl;
}

// save the current time as the overall stop time
void Cognitive_Method_Profiler::accept_event(const Stop_event * event_ptr)
{
	overall_stop_time = event_ptr->get_time();
//	if(Trace_out && get_trace()) Trace_out << processor_info() << " Got a Stop_event" << endl;
}

void Cognitive_Method_Profiler::accept_event(const Cognitive_event * event_ptr)
{
	if (const Cognitive_Accomplish_Goal_event * p = dynamic_cast<const Cognitive_Accomplish_Goal_event *> (event_ptr)) {
		if(Trace_out && get_trace()) Trace_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		start_time[p->goal] = get_time();
		if(Trace_out && get_trace()) Trace_out << processor_info() << " Start time saved: " << start_time[p->goal] << endl;
		// if this is the first AG event, first_AG_time will be zero, so set it to the current time
		if (first_AG_time == 0)
			first_AG_time = get_time();
		}
	if (const Cognitive_Return_Goal_Accomplished_event * p = dynamic_cast<const Cognitive_Return_Goal_Accomplished_event *> (event_ptr)) {
		if(Trace_out && get_trace()) Trace_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		if(Trace_out && get_trace()) Trace_out << processor_info() << profile[p->goal].total_time << endl;
//		if(Trace_out && get_trace()) Trace_out << processor_info() << " Start time from before: " << start_time[p->goal] << endl;
		last_RGA_time = get_time();	// save last RGA time
		double entry_start_time = double(start_time[p->goal]);
		// if start time is zero,then the RGA is or an item that was not on the stack
		// e.g. as a result of an abort_and_restart on the same cycle as an Accomplish_Goal
//		double duration;
		if (entry_start_time == 0) {
//			duration = 0.0;
			Normal_out << processor_info() << " Start time from before: " << entry_start_time << endl;
			}
		else {
			double duration = double(get_time() - start_time[p->goal]);
			profile[p->goal].total_time = profile[p->goal].total_time + duration;
			profile[p->goal].frequency++;
			if(Trace_out && get_trace()) Trace_out << processor_info() << profile[p->goal].total_time << endl;
			start_time.erase(p->goal);
			}
		}
}

void Cognitive_Method_Profiler::report() const
{
	Output_tee_format_saver saver(Normal_out);

	Normal_out << "Method Execution Profile" << endl;
	Normal_out << "Total Run Duration: " 
		<< fixed << setprecision(3) 
		<< setw(10) << (overall_stop_time - overall_start_time) / 1000. 
		<< " seconds" << endl;
	double total_execution_time = (last_RGA_time - first_AG_time) / 1000.;		// for seconds
	Normal_out << "Total Method Execution Time: " 
		<< fixed << setprecision(3) << setw(10) << total_execution_time << " seconds" << endl;
//	Normal_out << " number of entries: " << profile.size() << endl;
	Normal_out << "  Freq.    Subtotal  Avg. Time  % of Total   Method for goal" << endl;
	for (map <GOMS_Goal, Accumulator, less<GOMS_Goal> >::const_iterator it = profile.begin();
		 it != profile.end();
		 it++) {
	
		 GOMS_Goal temp_goal = (*it).first;
//		 Accumulator temp_accum = (*it).second;
		int f = (*it).second.frequency;
		double total = (*it).second.total_time / 1000.;	// for seconds
		double avg = total/f;
		double percent = 100. * (total / total_execution_time);
		
		
		Normal_out << setw(6) << f 
			<< fixed << setprecision(3) << setw(12) << total << setw(12) << avg
			<<  setprecision(2) << setw(12) << percent
			<< "  " << (*it).first.action << ' ' << (*it).first.object 
			<< setprecision(0)
			<< endl;
		}
}
