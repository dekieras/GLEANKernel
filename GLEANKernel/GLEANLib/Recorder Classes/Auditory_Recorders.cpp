#include <iostream>
#include <iomanip>
//#include <typeinfo>

#include "Auditory_Recorders.h"
#include "Auditory_event_types.h"
#include "Cognitive_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;


// Pick out selected events to the Auditory Processor that are action commands,
// and these will be followed by a Cognitive_Auditory_Operation_Complete event.
// If there is no overlapping, the time between these two is the effective duration
// of the operation. If there is overlapping, a different completion event
// will be needed, such as the matched device event.
void Auditory_Actions_Counter::accept_event(const Auditory_event * event_ptr)
{
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;

	if (dynamic_cast<const Auditory_Get_Object_event *> (event_ptr) ||
		dynamic_cast<const Auditory_Wait_Object_event *> (event_ptr)
		) {
			update_action_start();
			}
}

// The total time and averages produced by this Recorder have to be interpreted carefully.
// A Wait_Object operation takes a duration equal to however long the wait was for, which can be very long.
// This long time is totalled in and used in the average. 
//  -dk 3/1/00

void Auditory_Actions_Counter::accept_event(const Cognitive_event * event_ptr)
{
	if (dynamic_cast<const Cognitive_Auditory_Operation_Complete_event *> (event_ptr)) {
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		update_action_end();
		}
}

std::string Auditory_Actions_Counter::get_report() const 
{
	return generate_long_report("Auditory Actions");
}

std::string Auditory_Actions_Counter::get_short_report(double duration) const 
{
	return generate_short_report("Auditory Actions", duration);
}
