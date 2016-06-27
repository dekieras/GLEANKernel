#include "High_Level_processor.h"
#include "Coordinator.h"
#include "Vocal_event_types.h"
#include "Device_event_types.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "Syllable_counter.h"
#include "Numeric_utilities.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_exceptions.h"

#include <iostream>

using namespace std;


const long Get_device_delay_c = 1000;	// time delay for a Get_from_device
const long Put_device_delay_c = 1000;	// time delay for a Put_to_device
const long input_store_time_c = 900;	// time delay to store input
const long input_delay_time_c = 100;	// additional time for operation to be complete



void High_Level_processor::initialize()
{
	waiting_for_object = false;
	Object_Memory_Processor::initialize();
}

void High_Level_processor::display_object_info(Output_tee& ot) const
{
	Object_Memory_Processor::display_object_info(ot, "High_Level Memory");
}


void High_Level_processor::accept_event(const High_Level_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	event_ptr->handle_self(this);
}

void High_Level_processor::handle_event(const High_Level_Put_Command * event_ptr)
{
	long completion_time = get_time() + Put_device_delay_c;
	
//	// time less one to align with next cognitive cycle.
	if ((completion_time % 50) == 0)
		completion_time--;

	if(Trace_out && get_trace()) {
		Trace_out << processor_info() << " High-level Put_to_device at time " << completion_time << ":" << endl;
		Symbol_list_t::const_iterator pit = event_ptr->props.begin();
		Symbol_list_t::const_iterator vit = event_ptr->values.begin();
		for(; pit != event_ptr->props.end() && vit != event_ptr->values.end(); ++pit, ++vit) {
			if(pit != event_ptr->props.begin())
				Trace_out << ", ";
			Trace_out << *pit << " is " << *vit;
			}
		Trace_out << endl;
		}

	// send the event at the completion time to the device
	Event * output_event_ptr = new Device_HLPut_event(completion_time, get_human_ptr()->get_device_ptr(), 
			event_ptr->props, event_ptr->values);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_operation_complete(completion_time);
	
}

// send a request to the device to supply some input, and wait for a response.
// 
// the first High_Level_Get_Input_event is taken to satisfy the request
void High_Level_processor::handle_event(const High_Level_Get_Command * event_ptr)
{
	long completion_time = get_time() + Get_device_delay_c;
	
	// time less one to align with next cognitive cycle.
	if ((completion_time % 50) == 0)
		completion_time--;

	if(Trace_out && get_trace()) {
		Trace_out << processor_info() << " High-level Get_from_device at time " << completion_time << ":" << endl;
		Symbol_list_t::const_iterator pit = event_ptr->props.begin();
		Symbol_list_t::const_iterator vit = event_ptr->values.begin();
		for(; pit != event_ptr->props.end() && vit != event_ptr->values.end(); ++pit, ++vit) {
			if(pit != event_ptr->props.begin())
				Trace_out << ", ";
			Trace_out << *pit << " is " << *vit;
			}
		Trace_out << endl;
		}

	// send the event at the completion time to the device
	Event * output_event_ptr = new Device_HLGet_event(completion_time, get_human_ptr()->get_device_ptr(), 
			event_ptr->props, event_ptr->values, event_ptr->tag);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	waiting_for_object = true;
//	send_acknowledgement(completion_time);
}


// the input that arrives is used to update the object memory and the tag store, and then
// the operation is deemed complete
void High_Level_processor::handle_event(const High_Level_Input_event * event_ptr)
{
	if(Trace_out && get_trace()) {
		Trace_out << processor_info() << " High_Level device information arrived for " << event_ptr->object_name << ":" << endl;
		Symbol_list_t::const_iterator pit = event_ptr->props.begin();
		Symbol_list_t::const_iterator vit = event_ptr->values.begin();
		for(;pit != event_ptr->props.end() && vit != event_ptr->values.end(); ++pit, ++vit)
			Trace_out << *pit << " is " << *vit << ", ";
		Trace_out << "and_store_under " << event_ptr->tag << endl;
		}

	if(!waiting_for_object)
		throw Glean_exception(this, "Unexpected High_Level_Input_event arrived");
		
	// store the info
	store(event_ptr->object_name, event_ptr->props, event_ptr->values);
	// all objects are in focus
	store(event_ptr->object_name, Status_c, In_focus_c);
//	// assume a constant store time per property 	
//	long total_time = get_time() + input_property_store_time_c * long(event_ptr->values.size());
	// ensure that stores arrive before the operation is considered complete
	long total_time = get_time() + input_store_time_c;
		// time less one to align with next cognitive cycle.
	if ((total_time % 50) == 0)
		total_time--;
	store_WM_object(event_ptr->object_name, total_time);
	store_WM_object_under_tag(event_ptr->object_name, event_ptr->tag,total_time);
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + input_delay_time_c);
	
//	display_contents(Normal_out);
	
}

// the input that arrives is used to update the object memory and the tag store, and then
// the operation is deemed complete
void High_Level_processor::handle_event(const High_Level_Erase_event * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " High_Level Erase from device of " << event_ptr->object_name << endl;
	// erase the object
	erase(event_ptr->object_name);
	erase_WM_object(event_ptr->object_name, get_time());
	long total_time = get_time() + input_store_time_c;
	// operation required its total time to make sure WM objects have been updated
	send_operation_complete(total_time + input_delay_time_c);
	
//	display_contents(Normal_out);
	
}

void High_Level_processor::send_operation_complete(int completion_time)
{	
	Event * event_ptr = new Cognitive_High_Level_Operation_Complete_event (completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

