#include <iostream>
#include <iomanip>
//#include <typeinfo>

#include "Manual_Recorders.h"
#include "Manual_event_types.h"
#include "Cognitive_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;

// Assuming that any event to the Manual Processor is an action command,
// and this will be followed by a Cognitive_Manual_Operation_Complete event.
// If there is no overlapping, the time between these two is the effective duration
// of the manual operation. If there is overlapping, a different completion event
// will be needed, such as the matched device event.
void Manual_Actions_Counter::accept_event(const Manual_event * )
{
//	cout << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
	update_action_start();
}

void Manual_Actions_Counter::accept_event(const Cognitive_event * event_ptr)
{
//	if (Cognitive_Manual_Operation_Complete_event * p = dynamic_cast<Cognitive_Manual_Operation_Complete_event *> (event_ptr)) {
	if (dynamic_cast<const Cognitive_Manual_Operation_Complete_event *> (event_ptr)) {
///	cout << processor_info() << " input is " << get_time() << ' ' << typeid(*event_ptr).name() << endl;
		update_action_end();
		}
}

std::string Manual_Actions_Counter::get_report() const 
{
	return generate_long_report("Manual Actions");
}

std::string Manual_Actions_Counter::get_short_report(double duration) const 
{
	return generate_short_report("Manual Actions", duration);
}
