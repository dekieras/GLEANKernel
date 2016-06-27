#include "LTM_processor.h"
#include "Coordinator.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "WM_event_types.h"
#include "Cognitive_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_exceptions.h"

#include <iostream>
//#include <typeinfo>
//#include <DebugNew.h>
using namespace std;


//notes 7/23/98
// only one in-focus item at a time

// values for processor constants - reviewed 7/23/98
// time less one to align with next cognitive cycle.
const int LTM_processor::default_retrieval_time = 1200 - 1;

void LTM_processor::initialize()
{
	set_initial_contents(get_human_ptr()->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_LTM());
	Object_Memory_Processor::initialize();
	in_focus_object_name = Nil_c;	
}

void LTM_processor::accept_event(const LTM_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

// send an operation complete event
void LTM_processor::send_operation_complete(long completion_time)
{
	Event * event_ptr = new Cognitive_LTM_Operation_Complete_event(completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

// get the value for the property of the object and store in WM under the tag
void LTM_processor::handle_event(const LTM_Recall_Value_event * event_ptr)
{
	// retrieve the value for the object and the property, and store the result
	// at the specified time from now.
	long total_time = get_time() + default_retrieval_time;
	if(event_ptr->object_name == Nil_c || event_ptr->object_name == Absent_c)
		throw Glean_exception(this, "Attempt to retrieve property value of Nil or Absent object");
	retrieve_and_store_value(event_ptr->object_name, event_ptr->property_name, 
		event_ptr->WM_property_name, total_time);
	send_operation_complete(total_time);
}

// find the object matching a list of properties and values, and store its name under the tag
// it is now the current in_focus object
void LTM_processor::handle_event(const LTM_Recall_Object_event * event_ptr)
{
	// get object_name back
	Symbol object_name = find_object (event_ptr->property_names, event_ptr->property_values);
	// if same as in-focus object, no change, no time charge
	if (object_name == in_focus_object_name) {
		store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}
		
	long total_time = get_time() + default_retrieval_time;
	
	// whether object was found, we store the name in WM object store and tag store
	// object is now in focus - unless it is absent
	change_focus(object_name, total_time);
//	store_WM_object(object_name, total_time);
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
			
/*	// if object was found, then update in-focus - we are now thinking of it, not the previous one
	if (object_name != Absent_c) {
		erase_WM_object(in_focus_object_name, get_time()); // no longer in focus in WM -immediate
		in_focus_object_name = object_name;
		}
*/		
}


void LTM_processor::change_focus(const Symbol& object_name, long change_time)
{
	// if the object_name is absent, it will not become the new focus-on object (which will be "nil_c")
	// change the Status_c of the current object - note all properties remain!
	// WM_Retrieve_Value_Object_event handler filters properties that are not in focus!
	// tell WM to add/remove object from object store accordingly - only one accessible at a time,
	// and that is the one that is in focus.
	if (in_focus_object_name != Nil_c && is_present(in_focus_object_name)) {
		remove_focus(in_focus_object_name, change_time);
//		store(in_focus_object_name, Status_c, Out_of_focus_c);
		}
	if (object_name != Absent_c) {
		// make the current object the in-focus object
		store_WM_object(object_name, change_time);
		store(object_name, Status_c, In_focus_c);
		in_focus_object_name = object_name;
		}
	else
		in_focus_object_name = Nil_c;	// we are thinking of nothing (not of "Absent_c")
}

// if the named object was in focus, it is no longer, and now nothing is in focus
// this should be called before the object is erased
void LTM_processor::remove_focus(const Symbol& object_name, long change_time)
{
	if(object_name == in_focus_object_name) {
		erase_WM_object(object_name, change_time);
		store(object_name, Status_c, Out_of_focus_c);
		in_focus_object_name = Nil_c;
		}
}


void LTM_processor::display_contents(Output_tee& ot) const
{
	ot << "LTM contents:" << endl;
	Object_Memory_Processor::display_contents(ot);
	ot << "In focus: " << in_focus_object_name << endl;
}

void LTM_processor::display_object_info(Output_tee& ot) const
{
	Object_Memory_Processor::display_object_info(ot, "LTM");
	ot << "  In focus: " << in_focus_object_name << endl;
}

