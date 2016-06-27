#include "Action_Recorders.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

#include <iostream>
#include <iomanip>
//#include <cassert>
#include <typeinfo>
using namespace std;

// Modifications to start/stop are necessary because abort_and_restart can cause methods
// to be terminated without an RGA ...

void Action_Recorder::initialize()
{
	action_start_time = 0;
	overall_start_time = 0;
	overall_stop_time = 0;
	number_of_actions_started = 0;
	number_of_actions_finished = 0;
	total_time = 0.0;
	recording = false;
}

// save the current time as the overall start time
void Action_Recorder::accept_event(const Start_event * event_ptr)
{
	overall_start_time = event_ptr->get_time();
//	if(Debug_out) Debug_out << processor_info() << " Got a Start_event" << endl;
}

// save the current time as the overall stop time
void Action_Recorder::accept_event(const Stop_event * event_ptr)
{
	overall_stop_time = event_ptr->get_time();
//	if(Debug_out) Debug_out << processor_info() << " Got a Stop_event" << endl;
}

// when a report event is received, output a report, and reset, resume recording
void Action_Recorder::accept_event(const Report_event * event_ptr)
{
//	Normal_out << processor_info() << ' ';
	short_report(double(event_ptr->duration)/1000.);
	initialize();
}

// if we are already recording, throw away the previous start time 
// and don't count another
void Action_Recorder::update_action_start()
{
	// check on state - should be not recording - this event supplants previous one,
	// don't count another event if already recording
	if (!recording) {
		recording = true;
		number_of_actions_started++;
		}
//	else {
//		Normal_out << "update_action_start while recording in " <<  typeid(*this).name() << endl;
//		}
	action_start_time = get_time();
}
	
void Action_Recorder::update_action_end()
{
	// if not recording, ignore this stop action
	if(!recording) {
//		Normal_out << "update_action_end while not recording in " <<  typeid(*this).name() << endl;
		return;
		}
	recording = false;
	long duration = get_time() - action_start_time;
	total_time += double(duration);
	number_of_actions_finished++;
	// check in case things have gotten out of synch
//	if(number_of_actions_started != number_of_actions_finished) {
//		Normal_out << "number_of_actions_started != number_of_actions_finished in " <<  typeid(*this).name() << endl;
//		char c;
//		Normal_out << "enter a non-whitespace to continue" << endl;
//		cin >> c;
//		}
	Assert(number_of_actions_started == number_of_actions_finished);
}

void Action_Recorder::report() const
{
	Normal_out << get_report();
}

void Action_Recorder::short_report(double duration) const
{
	Normal_out << get_short_report(duration);
}

string Action_Recorder::generate_long_report(const char * title) const
{
	ostringstream oss;	
	oss << title << ':' << endl;

	oss << "Total Run Duration: " 
		<< fixed << setprecision(3) << setw(10) << get_duration() << " seconds" << endl;
	oss << "  Freq.    Subtotal  Avg. Time  % of Total Run Duration" << endl;
	oss << setw(8) << get_frequency()
			<< fixed << setprecision(3) << setw(12) << get_total_time() << setw(12) << get_avg()
			<< setprecision(2) << setw(12) << get_percent()
			<< setprecision(0)
			<< endl;
	return oss.str();
}

// call this to embed the output in other output, duration supplied
// output nothing if frequency is zero
string Action_Recorder::generate_short_report(const char * title, double duration) const
{
	ostringstream oss;	
	if (get_frequency() != 0) {
		oss << setw(6) << get_frequency()
			<< fixed << setprecision(3) << setw(12) << get_total_time() << setw(12) << get_avg()
			<< setprecision(2) << setw(12) << get_percent(duration)
			<< "  " << title
			<< setprecision(0)
			<< endl;
		}
	return oss.str();
}

