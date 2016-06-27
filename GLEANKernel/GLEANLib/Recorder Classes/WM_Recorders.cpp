#include "WM_Recorders.h"
#include "Human_processor.h"
#include "Symbol.h"
#include "Recorder.h"
#include "WM_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

#include <iostream>
#include <iomanip>
#include <set>
//#include <typeinfo>
using namespace std;

/*
To record WM activity during a certain period of time, in particularly, while a goal is active on the stack:
call start_accumulating() when the period of time starts, and stop_accumulating() at the end of
that period of time.

If the accumulate flag is off, the input change events are ignored.

start_accumulating() will set the accumulate flag, save the current time as the last change time 
and the accumulate start time.

Each change event will then calculate itemsXtime value based on the saved last change time.
last_number_of_items is not altered by start/stop_accumulating - so if this method left an item
in memory after its RGA, this is where the average starts when it begins accumulating again.

stop_accumulating calculates the average usage since the last change, and resets the flag; the 
current time - the accumulate start time is added to the total time.

A start calls start_accumulating, and a stop event calls stop_accumulating. 

The total time accumulated should equal the method duration.

*/



void WM_Tag_Counter::initialize()
{
	overall_start_time = 0;
	overall_stop_time = 0;
	accumulate_start_time = 0;
	accumulate_stop_time = 0;

	last_change_time = 0;
	last_number_of_items = 0;
	total_items_x_time = 0.0;
	total_time = 0.0;
	tags.clear(); 
	number_stores = 0;
	number_replacements = 0;
	number_deletes = 0;
	peak_items = 0;
}

void WM_Tag_Counter::start_accumulating()
{
	accumulate = true;
	accumulate_start_time = get_time();
	last_change_time = get_time();
}

void WM_Tag_Counter::stop_accumulating()
{
	accumulate = false;
	accumulate_stop_time = get_time();
	total_time += double(accumulate_stop_time - accumulate_start_time);
	// final change in this episode
	update_items_x_time();
}


// save the current time as the overall start time
void WM_Tag_Counter::accept_event(const Start_event * )
{
	overall_start_time = get_time();
	start_accumulating();
//	if(Debug_out) Debug_out << processor_info() << " Got a Start_event" << endl;
}

// call whenever there is a change to WM contents, to update the total of items X time
void WM_Tag_Counter::update_items_x_time()
{
	long duration = get_time() - last_change_time;
	total_items_x_time += double(duration * last_number_of_items);
	last_number_of_items = static_cast<long>(tags.size());
	last_change_time = get_time();
}

void WM_Tag_Counter::accept_event(const WM_event * event_ptr)
{
	if(!accumulate)
		return;

//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
// we have to count how many tags there are ourselves, since a value can be replaced
// without changing the number of tags present.
	if (const WM_Store_Tag_event * p = dynamic_cast<const WM_Store_Tag_event *> (event_ptr) ) {
		pair<set<Symbol>::iterator, bool> result = 
			tags.insert(p -> tag_name);
		if (result.second) {	// if was inserted (not already present)
			number_stores++;
			update_items_x_time();	// change in number of items
			if (int(tags.size()) > peak_items)
				peak_items = static_cast<int>(tags.size());
			}
		else {	// if was not inserted (already present), no change in number
			number_replacements++;
			}
		}
	else if (const WM_Erase_Tag_event * p = dynamic_cast<const WM_Erase_Tag_event *> (event_ptr) ) {
		tags.erase(p -> tag_name);
		update_items_x_time();	// change in number of items
		number_deletes++;
		}
}

// save the current time as the overall stop time
void WM_Tag_Counter::accept_event(const Stop_event * )
{
	overall_stop_time = get_time();
	if(accumulate)
		stop_accumulating();
//	if(Debug_out) Debug_out << processor_info() << " Got a Stop_event" << endl;
}

// when a report event is received, output a report, and reset, resume recording
void WM_Tag_Counter::accept_event(const Report_event * )
{
	if(accumulate)
		stop_accumulating();
//	short_report(double(event_ptr->duration)/1000.);  // spparently superseded -dk 7/5/06 
	short_report();
	initialize();
	start_accumulating();
}



void WM_Tag_Counter::report() const
{
/*	Output_tee_format_saver saver(Normal_out);

	Normal_out << "WM Tag Statistics:" << endl;
	double total_execution_time = (overall_stop_time - overall_start_time)/1000.;		// for seconds
	Normal_out << fixed << setprecision(3) 
		<< "Total Run Duration: " << total_execution_time 
		<< " seconds, WM usage time: " << total_time/1000. << " seconds" << endl;
	Normal_out 
		<< "Stores: " << number_stores  
		<< ", Replacements: " << number_replacements 
		<< ", Deletions: " << number_deletes
		<< ", Remaining: "<< tags.size()  << endl;
	double avg = total_items_x_time/total_time;
	Normal_out 
		<< "Number of Tags: Peak: " << peak_items
		<< ", Average: " << avg 
		<< setprecision(0) << endl;
*/
	Normal_out << get_report();
}

// output nothing if peak_items is zero	
void WM_Tag_Counter::short_report() const
{
/*	if (peak_items == 0)
		return;
	Output_tee_format_saver saver(Normal_out);
	
	double avg = total_items_x_time/total_time; 
	Normal_out << setw(6) << number_stores + number_replacements + number_deletes
		<< fixed << setprecision(3) << setw(12) << total_time/1000.
		<< setprecision(2) << setw(12) << avg
		<< setprecision(0) << setw(12) << peak_items
		<< "  WM Changes, duration, avg, peak" << endl;
*/
	Normal_out << get_short_report();
}

string WM_Tag_Counter::get_report() const
{
	ostringstream oss;
	oss << "WM Tag Statistics:" << endl;
	double total_execution_time = (overall_stop_time - overall_start_time)/1000.;		// for seconds
	oss << fixed << setprecision(3) 
		<< "Total Run Duration: " << total_execution_time 
		<< " seconds, WM usage time: " << total_time/1000. << " seconds" << endl;
	oss << "Stores: " << number_stores  
		<< ", Replacements: " << number_replacements 
		<< ", Deletions: " << number_deletes
		<< ", Remaining: "<< tags.size()  << endl;
	double avg = total_items_x_time/total_time;
	oss << "Number of Tags: Peak: " << peak_items
		<< ", Average: " << avg 
		<< setprecision(0) << endl;
	return oss.str();
}

// output nothing if peak_items is zero	
string WM_Tag_Counter::get_short_report() const
{
	ostringstream oss;
	if (peak_items != 0) {
		double avg = total_items_x_time/total_time; 
		oss << setw(6) << number_stores + number_replacements + number_deletes
			<< fixed << setprecision(3) << setw(12) << total_time/1000.
			<< setprecision(2) << setw(12) << avg
			<< setprecision(0) << setw(12) << peak_items
			<< "  WM Changes, duration, avg, peak" << endl;
		}
	return oss.str();
}
