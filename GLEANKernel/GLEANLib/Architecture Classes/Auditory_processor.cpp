#include "Auditory_processor.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "Coordinator.h"
#include "WM_event_types.h"
#include "Cognitive_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Numeric_utilities.h"
#include "Glean_standard_symbols.h"

#include <iostream>
//#include <cassert>
//#include <typeinfo>
//#include <DebugNew.h>
using namespace std;


// bug fix 4/23/0
// auditory objects stored in working memory now have unique names 
// like "Aobj123" instead of simple unique serial numbers. This avoids conflict
// with other objects that have simple numerical names.

// notes 3/2/00
// the times on the auditory commands need re-evaluating - they are not consistent between
// the get_object and wait_for_object

//notes 7/23/98
// no limit on number of in-focus items, but they automatically disappear after decay
// take out the code redundancies in the listen, wait_for, and input processing
// contains task-specific encodings!

//notes from 5/98
// This version is just a clone of the visual processor - gets us started
// need to think about all of this, including the times

// e.g. keeping it straight which words have been attended to already, etc.
// we probably want a bundle of features to come in simultaneously, instead of one at a time

// values for processor constants - reviewed 7/23/98
// after code reorganization, decay was happening too soon 11/1/98
// time less one to align with next cognitive cycle.
const int Auditory_processor::auditory_find_time = 100 - 1;
// const int Auditory_processor::auditory_decay_time = 200;
// time less one to align with next cognitive cycle.
//const int Auditory_processor::auditory_decay_time = 1000 -1 ;	// 11/1/98
// make a lot longer to allow for use of "check" operator in conjunction with look-for
//const int Auditory_processor::auditory_decay_time = 10000 -1 ;	// 8/24/00
// const int Auditory_processor::auditory_decay_time = 25000 -1 ;	// from TS, as of 11/9/00
// const int Auditory_processor::auditory_decay_time = 10000 -1 ;	// changed back 5/6/01
const int Auditory_processor::auditory_decay_time = 500 -1 ;	// from Tom 8/9/01


void Auditory_processor::initialize()
{
	input_object_number = 0;
	auditory_encodings = get_human_ptr()->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_initial_auditory_state();
	Object_Memory_Processor::initialize();
	// do the initialization by copying from the initialization source
//	if(Debug_out) Debug_out << "Initializing visual memory from:" << endl;
//	initial_visual_state.display_contents(if(Debug_out) Debug_out);
//	symbol_memory = initial_auditory_state;
//	if(Debug_out) Debug_out << "Result is:" << endl;
//	auditory_encodings.display_contents(if(Debug_out) Debug_out);	// show contents of auditory encodings
}

void Auditory_processor::accept_event(const Auditory_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

// send an operation complete event
void Auditory_processor::send_operation_complete(long completion_time)
{
	Event * event_ptr = new Cognitive_Auditory_Operation_Complete_event(completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

// store the supplied value under the property of the object
void Auditory_processor::handle_event(const Auditory_Input_Symbol_event * event_ptr)
{
	
	const Symbol& input_symbol = event_ptr->get_input_symbol();	// will exist through this function
	
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Heard: " << input_symbol << endl;

	// store a default representation of a numeric object name with property "Content" 
	// and value of the input symbol
	
	// change the number to a string of the form "Aobj123" to help ensure it is unique.
	string s("Aobj");
	s += int_to_string(input_object_number++);
	
	Symbol object_name(s.c_str()); // create the unique object name
	store(object_name, Content_c, input_symbol);
	// make its Status In_focus to handle problems stemming from exception rules
	store(object_name, Status_c, In_focus_c);

	do_auditory_encoding(object_name, input_symbol);
/*
	Symbol_list_t prop_names, prop_values;
	// look up the input content in the auditory encodings, and add the corresponding properties and values
	// if not found, look for an word named "Default"; if not found, store a default representation
	if (auditory_encodings.get_pv_lists(input_symbol, prop_names, prop_values))
		store(object_name, prop_names, prop_values);
	else if (auditory_encodings.get_pv_lists("Default", prop_names, prop_values))
		store(object_name, prop_names, prop_values);
	else 
		store(object_name, Type_c, "Information");
*/
		
	// put the new object in the object store almost now  - dk 5/6/01
//	store_WM_object(object_name, get_time() + 10);
	// put it in immediately - 9/23/01
	store_WM_object(object_name, get_time());
	get_human_ptr()->get_Cognitive_ptr()->add_salient_object(object_name);
	
	// schedule a decay event for the entire object
	Event * output_event_ptr = new Auditory_Input_Decay_event(
		get_time() + auditory_decay_time, this, object_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	
//	display_contents(if(Debug_out) Debug_out);
	
	// check to see if waiting for an object, and see if it is now present
	if (waiting_for_object) {
		Symbol object_name = find_object (waiting_for_property_names, waiting_for_property_values);
		// if object is not present, simply return
		if (object_name == Absent_c) {
			return;
			}
		// otherwise, object is now present, waiting is over
		waiting_for_object = false;
		// if same as in-focus object, no change, no additional time charge
//		if (object_name == in_focus_object_name) {
//			send_operation_complete(get_time());
//			return;
//			}
	
		// object is present, but time is required to make it available in WM ???
		long total_time = get_time() + auditory_find_time;
	
		// store the name in WM object store and tag store
// dk 5/6/01		store_WM_object(object_name, total_time);
		store_WM_object_under_tag(object_name, waiting_for_tagname, total_time);

		// operation required its total time plus some more to make sure stores are complete
		send_operation_complete(total_time + 50);
					
	}
//	display_contents(if(Debug_out) Debug_out);
	
}

void Auditory_processor::do_auditory_encoding (const Symbol& object_name, const Symbol& input_symbol)
{

	Symbol_list_t prop_names, prop_values;
	// look up the input content in the auditory encodings, and add the corresponding properties and values
	if (auditory_encodings.get_pv_lists(input_symbol, prop_names, prop_values)) {
		store(object_name, prop_names, prop_values);
		return;
		}

	// look for all of the matches to a substring of the content
	// go through all of the encodings
	// if a substring of the input symbol matches an encoding, add the property-value pairs

	bool encoding_found = false;
	string input_str = input_symbol.str();
	Symbol_list_t encoding_name_list = auditory_encodings.get_all_object_names();
	for(Symbol_list_t::iterator encoding_it = encoding_name_list.begin();
			encoding_it != encoding_name_list.end(); encoding_it++) {
			
		Symbol encoding_symbol = *encoding_it;
		string encoding_str = encoding_symbol.str();
	
		string::iterator it = search(input_str.begin(), input_str.end(), encoding_str.begin(), encoding_str.end());
		if(it != input_str.end()) {
			// we have a substring match - access and store its properties on the object
			if (auditory_encodings.get_pv_lists(encoding_symbol, prop_names, prop_values)) {
//				store(*encoding_it, prop_names, prop_values);
				store(object_name, prop_names, prop_values);
				encoding_found = true;
				break;	// stop at first encoding found
				}
			}
		}
	// if we found a substring match, look no further
	if(encoding_found)
		return;
	// if not found, look for an word named "Default"; if not found, store a default representation
	else if (auditory_encodings.get_pv_lists(Default_c, prop_names, prop_values))
		store(object_name, prop_names, prop_values);
	else 
		store(object_name, Type_c, Information_c);

}


// erase the object from auditory memory
void Auditory_processor::handle_event(const Auditory_Input_Decay_event * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Losing Auditory: " << event_ptr->object_name << endl;
	erase(event_ptr->object_name);
	erase_WM_object(event_ptr->object_name, get_time()); // no longer in focus in WM -immediate
	get_human_ptr()->get_Cognitive_ptr()->remove_salient_object(event_ptr->object_name);
				
//	display_contents(if(Debug_out) Debug_out);
	
}

void Auditory_processor::handle_event(const Auditory_Input_Spatial_event * event_ptr)
{
	store(event_ptr->object_name, event_ptr->property_name, event_ptr->property_value);
}

// get the value for the property of the object and store in WM under the tag
void Auditory_processor::handle_event(const Auditory_Get_Value_event * event_ptr)
{
	// retrieve the value for the object and the property, and store the result
	// at the specified time from now.
	retrieve_and_store_value(event_ptr->object_name, event_ptr->property_name, 
		event_ptr->WM_property_name, get_time() + auditory_find_time);
}

// find the object matching a list of properties and values, and store its name under the tag
// it is now the current in_focus object
void Auditory_processor::handle_event(const Auditory_Get_Object_event * event_ptr)
{
	// get object_name back
	Symbol object_name = find_object (event_ptr->property_names, event_ptr->property_values);
		
	long total_time = get_time() + auditory_find_time;
	
	// if the found object is absent, we store that in the tag, but not in the object store
	if(object_name != Absent_c)
		store_WM_object(object_name, total_time);
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus a little more - WM update must be made first
	send_operation_complete(total_time);
	
		
//	display_contents(if(Debug_out) Debug_out);
}

// find the object matching a list of properties and values. If not present, set up a wait for it, until
// it does get added.  When present, store its name under the tag. It is now the current in_focus object
void Auditory_processor::handle_event(const Auditory_Wait_Object_event * event_ptr)
{
	// get object_name back
	Symbol object_name = find_object (event_ptr->property_names, event_ptr->property_values);
	// if object is not present, set up a wait for it and return
	if (object_name == Absent_c) {
		waiting_for_object = true;
		waiting_for_property_names = event_ptr->property_names;
		waiting_for_property_values = event_ptr->property_values;
		waiting_for_tagname = event_ptr->WM_tag_name;
		if(Trace_out && get_trace())
			Trace_out << processor_info() << " Waiting for object to be " << waiting_for_tagname << endl;
		return;
		}
	
	// object is present, but requires time
	long total_time = get_time() + auditory_find_time;
	
	// store the name in WM object store and tag store
	store_WM_object(object_name, total_time);
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus some extra to make sure tag memory is updated
	send_operation_complete(total_time + 50);
			
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Already heard - no wait for " << object_name << endl;
//	display_contents(if(Debug_out) Debug_out);
}

void Auditory_processor::display_object_info(Output_tee& ot) const
{
	Object_Memory_Processor::display_object_info(ot, "Auditory Memory");
}

void Auditory_processor::display_contents(Output_tee& ot) const
{
	ot << "Auditory Memory contents:" << endl;
	Object_Memory_Processor::display_contents(ot);
}

