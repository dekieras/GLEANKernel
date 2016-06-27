#include <iostream>
#include <iomanip>
//#include <typeinfo>

#include "Vocal_Recorders.h"
#include "Vocal_event_types.h"
#include "Cognitive_event_types.h"
//#include "Human_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;

// Assuming that any event to the Vocal Processor is an action command,
// and this will be followed by a Cognitive_Vocal_Operation_Complete event.
// If there is no overlapping, the time between these two is the effective duration
// of the vocal operation. If there is overlapping, a different completion event
// will be needed, such as the matched device event.
void Vocal_Actions_Counter::accept_event(const Vocal_event * )
{
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
	update_action_start();
}

void Vocal_Actions_Counter::accept_event(const Cognitive_event * event_ptr)
{
	if (dynamic_cast<const Cognitive_Vocal_Operation_Complete_event *> (event_ptr)) {
//	if(Debug_out) Debug_out << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		update_action_end();
		}
}

std::string Vocal_Actions_Counter::get_report() const 
{
	return generate_long_report("Vocal Actions");
}

std::string Vocal_Actions_Counter::get_short_report(double duration) const 
{
	return generate_short_report("Vocal Actions", duration);
}
