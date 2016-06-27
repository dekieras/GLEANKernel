#include "Visual_processor.h"
#include "Coordinator.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "View_base.h"
#include "WM_event_types.h"
#include "Cognitive_event_types.h"
#include "Device_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Numeric_utilities.h"
#include "Symbol_utilities.h"
#include "Standard_Symbols.h"
#include "Geometry.h"


//#include <DebugNew.h>
#include <iostream>
#include <map>
//#include <cassert>
//#include <typeinfo>

using namespace std;
namespace GU = Geometry_Utilities;



/* notes 4/22/01
Change so that a new or saliently-changed object has its identity 
copied to the object store so that expressions can access it.
This is tricky and could easily lead to wrong stuff.
But it looks like WM_processor checks for being in focus in order to allow retrieval of 
any property other than Status - change this to incldue Event_type and Type also.
*/ 


/* notes 8/24/00
Change so that each object has a Event_type property with values:
Event_type is New
Event_type is Color-changed
These are removed after a retention interval or if focussed on

Status is In_focus
Status is Out_of_focus
Status is Disappearing

A new object is set to out-of-focus.
An object is added to the WM object store whenever it becomes in-focus.
If an object goes Out_of_focus, its Status property remains.

*/
// notes 12/17/01 among the many flaws of this version, the fact that visual objects do not have
// unique identities means that some objects might get deleted (from old disappearances) after
// they have newly appeared ... the erase time is thus critical ...

//notes 7/23/98
// only one in-focus item at a time
// take out the code redundancies in the look, wait_for, and input processing

// values for processor constants - reviewed 5/6/99
// time less one to align with next cognitive cycle.
const int Visual_processor::visual_find_time = 1200 - 1;
const int Visual_processor::visual_waitfor_time = 300 - 1; // approximate eye latency + movement time
const int Visual_processor::visual_refind_time = 200 - 1; // approximate eye movement time
const int Visual_processor::visual_check_time = 100 - 1; // approximate eye movement time
//const int Visual_processor::visual_erasure_time = 75 ;	// 5/4/00
const int Visual_processor::visual_erasure_time = 500;	// has to be short enough to avoid confusions with new objects
//const int Visual_processor::visual_event_type_retention_time = 10000;	// long time for debugging purposes, 8/24/00
const int Visual_processor::visual_event_type_retention_time = 500;	// long enough for Tom's WM buffer method? 7/19/01
const int Visual_processor::visual_fixation_duration = 300;

const double colar_availability_radius_c = 7.0;

void Visual_processor::initialize()
{
	set_initial_contents(get_human_ptr()->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_initial_visual_state());
	Object_Memory_Processor::initialize();
	psychobj_counter = 0;
	in_focus_object_name = Nil_c;
	waiting_for_object = false;
	waiting_for_property_names.clear();
	waiting_for_property_values.clear();
	waiting_for_tagname = Nil_c;
	name_map.clear();
	
	Symbol_list_t initial_objects = get_all_object_names();
	// add them to the name map as both psychological and physical names
	for(Symbol_list_t::iterator it = initial_objects.begin(); it != initial_objects.end(); ++it)
		name_map.add_names(*it, *it);
	
}

void Visual_processor::accept_event(const Visual_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

// send an operation complete event
void Visual_processor::send_operation_complete(long completion_time)
{
	Event * event_ptr = new Cognitive_Visual_Operation_Complete_event(completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

// create the new object in the store
// no ability to wait for a new object without specific properties.
// this is not useful in its current form because priority_queue does not guarantee its arrival before
// change events executed afterwards with the same time-stamp
void Visual_processor::handle_event(const Visual_Input_Object_Appearance_event * event_ptr)
{
	// create a psychological name for the object of the form physicalname + "_" + counter
	// and add the physical/psychological name pair to the name map
	// if a duplicate, an exception will be thrown ...
	// use the psychological name thereafter
	// create the object's psychological name, special case if physical name is one of the reserved names
	Symbol phys_name = event_ptr->object_name;
	Symbol psyc_name;
	if(phys_name == Cursor_name_c) {
		// this becomes the psychological name as well
		psyc_name = phys_name;
		}
	else {
		psyc_name = concatenate_to_Symbol(phys_name, "_V", ++psychobj_counter);
		}
	// add it to the name maps - exception thrown if duplicated
//	cerr << processor_info() << " New visual object: " 
//		<< phys_name << '/' << psyc_name << endl;
	name_map.add_names(phys_name, psyc_name);

	GU::Point defaultPoint;
	GU::Size defaultSize;
	notify(&View_base::notify_object_appear, psyc_name, defaultPoint, defaultSize);
	
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " New visual object: " << phys_name << '/' << psyc_name << endl;
		
	create_object(psyc_name);
	store(psyc_name, Event_type_c, New_c);
	store(psyc_name, Status_c, Out_of_focus_c);
	// put the new object in the object store almost now
//	store_WM_object(object_name, get_time() + 10);
	// put it in immediately - 9/23/01
	store_WM_object(psyc_name, get_time());
	get_human_ptr()->get_Cognitive_ptr()->add_salient_object(psyc_name);
	// schedule events to cause the new object object event type to be removed
	Event * output_event_ptr = new Visual_Event_Type_Erasure_event(
		get_time() + visual_event_type_retention_time, this, psyc_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);
}

bool Visual_processor::check_for_salient_change(const Symbol& object_name, const Symbol& property_name)
{
	// this only applies if the object already exists and is not new
	if (!is_present(object_name) || retrieve(object_name, Event_type_c) == New_c)
		return false;
	// check for relevant property
	if (property_name != Color_c &&
		property_name != BgColor_c &&
		property_name != FgColor_c )
		return false;
		
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Salient change: " << object_name << ' ' << property_name << endl;

	if ( property_name == Color_c ) {
		store(object_name, Event_type_c, Color_changed_c);
	} else if ( property_name == BgColor_c ) {
		store(object_name, Event_type_c, BgColor_changed_c);
	} else if ( property_name == FgColor_c ) {
		store(object_name, Event_type_c, FgColor_changed_c);
	}
	get_human_ptr()->get_Cognitive_ptr()->add_salient_object(object_name);
	// schedule events to cause the new object object event type to be removed
	Event * output_event_ptr = new Visual_Event_Type_Erasure_event(
		get_time() + visual_event_type_retention_time, this, object_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	return true;
}

// remove the event type of the object unconditionally
void Visual_processor::handle_event(const Visual_Event_Type_Erasure_event * event_ptr)
{
	// const Symbol& current_status = retrieve(event_ptr->object_name, Event_type_c);
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Losing event type for " << event_ptr->object_name << endl;
	erase(event_ptr->object_name, Event_type_c);
	get_human_ptr()->get_Cognitive_ptr()->remove_salient_object(event_ptr->object_name);
}


// store the supplied value under the property of the object
void Visual_processor::handle_event(const Visual_Input_Object_Change_Symbol_event * event_ptr)
{
	// check for existence of physical name, and get the psychological name from the name map
	Symbol psyc_name = name_map.get_psychological_name(event_ptr->object_name);
	Symbol property_name = event_ptr->property_name;
	Symbol property_value = event_ptr->property_value;
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << "set object " 
		<< event_ptr->object_name << '/' << psyc_name << " property " << property_name 
		<< " to " << property_value << endl;
		
	if (property_name == Location_c) {
		GU::Point location(property_value.get_x(), property_value.get_y());
		notify(&View_base::notify_visual_location_changed, psyc_name, location);
		}
	else if (property_name == Size_c) {
		GU::Size size(property_value.get_x(), property_value.get_y());	
		notify(&View_base::notify_visual_size_changed, psyc_name, size);
		}
	else {
		// if the property is an inter-object relation, change the value to the psychological name
//		cerr << property_name << ' ' << property_value << endl;
		property_value = change_related_object_name(property_name, property_value);
//		cerr << property_name << ' ' << property_value << endl;
		notify(&View_base::notify_visual_property_changed, psyc_name, property_name, property_value);
		}
		
	// update the property of the object
	store(psyc_name, property_name, property_value);
	// see if this sets a special event type
	check_for_salient_change(psyc_name, property_name);
	// check to see if waiting for an object, and see if it is now present
	if (waiting_for_object) {
		Symbol object_name = find_object(waiting_for_property_names, waiting_for_property_values);
		// if object is not present, simply return
		if (object_name == Absent_c) {
			return;
			}
		// otherwise, object is now present, waiting is over
		waiting_for_object = false;

		if(Trace_out && get_trace()) 
			Trace_out << processor_info() << " Visual: Wait_for waited for: " << object_name << endl;

		// if same as in-focus object, no change, no additional time charge
		// but there might be a change in the tag under which it is stored, so store it under the tag also
		// previous tag is left in place - dk 10/6/98
		if (object_name == in_focus_object_name) {
			store_WM_object_under_tag(object_name, waiting_for_tagname, get_time());
			send_operation_complete(get_time());
			return;
			}
		change_focus(object_name);

		// wait_for doesn't involve visual search - ??? must fix this
//		double search_time = estimate_search_time(object_name, waiting_for_property_names, waiting_for_property_values);
		// tell WM and Cog. processor about the newly in-focus object
		long total_time = get_time() + visual_waitfor_time;
//		long total_time = get_time() + search_time;
		store_WM_object(object_name, total_time);
		store_WM_object_under_tag(object_name, waiting_for_tagname, total_time);
		// operation required its total time
		send_operation_complete(total_time);
	}
}

// This should be fixed so that these are not hard coded!
Symbol Visual_processor::change_related_object_name(const Symbol& prop_name, const Symbol& prop_value)
{
	Symbol new_value = prop_value;
	if(prop_value == Nil_c)
		return new_value;
	if(	prop_name == Left_of_c || 
		prop_name == Right_of_c || 	
		prop_name == Above_c || 	
		prop_name == Below_c || 	
		prop_name == Inside_c || 	
		prop_name == In_center_of_c || 	
		prop_name == Outside_c || 	
		prop_name == In_row_c || 	
		prop_name == In_col_c || 	
		prop_name == Placed_on_c	
		) {
		new_value = name_map.get_psychological_name(prop_value);
		}
	return new_value;
}

/*
// this is deprecated - see how it is missing a lot of stuff compared to Change_Symbol
void Visual_processor::handle_event(const Visual_Input_Object_Change_Spatial_event * event_ptr)
{
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Change_Spatial " << event_ptr->object_name << ' ' << event_ptr->property_name 
			<< ' ' << event_ptr->property_value << endl;
	
	check_for_new_object(event_ptr->object_name);
		
	store(event_ptr->object_name, event_ptr->property_name, event_ptr->property_value);

	// Updating the visual view if necessary
	if ( event_ptr->property_name == Location_c ) {
		GU::Point location(event_ptr->property_value.get_x(), event_ptr->property_value.get_y());
		notify(&View_base::notify_visual_location_changed, event_ptr->object_name, location);
		} 
	else if ( event_ptr->property_name == Size_c ) {
		GU::Size size(event_ptr->property_value.get_x(), event_ptr->property_value.get_y());	
		notify(&View_base::notify_visual_size_changed, event_ptr->object_name, size);
		}
	else if ( event_ptr->property_name == Shape_c || 
				event_ptr->property_name == Color_c ||
				event_ptr->property_name == BgColor_c ||
				event_ptr->property_name == FgColor_c ||
				event_ptr->property_name == Text_c ) {
		notify(&View_base::notify_visual_property_changed, 
			event_ptr->object_name, event_ptr->property_name, event_ptr->property_value);
	
		}
}
*/

// mark the object as Disappearing, and then schedule a final erasure event
void Visual_processor::handle_event(const Visual_Input_Object_Disappearance_event * event_ptr)
{
	Symbol psyc_name = name_map.get_psychological_name(event_ptr->object_name);
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Disappearing " << event_ptr->object_name << '/' << psyc_name << endl;

	notify(&View_base::notify_object_disappear, psyc_name);
	
	// update the property of the object to show that it is Disappearing_c
	store(psyc_name, Status_c, Disappearing_c);
	// schedule events to cause the object to be removed after the next cycle
	Event * output_event_ptr = new Visual_Input_Erasure_event(
		get_time() + visual_erasure_time, this, psyc_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	
	// here we set the physical_name to Disappeared_object_c to show that the physical object is actually not there any more
	name_map.replace_physical_name_for_psychological_name(Disappeared_object_c, psyc_name);
}

// erase the object from visual memory and the WM object store
// if it was in-focus, nothing is now in-focus
void Visual_processor::handle_event(const Visual_Input_Erasure_event * event_ptr)
{
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Erasing Visual: " << event_ptr->object_name << endl;
	
	notify(&View_base::notify_erase_object, event_ptr->object_name);
	erase(event_ptr->object_name);
	// erase the object unconditionally
	erase_WM_object(event_ptr->object_name, get_time()); 
	if ((event_ptr->object_name) == in_focus_object_name) {
		in_focus_object_name = Nil_c;
		}

	// remove the object name from the name map now
	name_map.remove_names_with_psychological_name(event_ptr->object_name);
//	if (Debug_out) display_contents(Debug_out);
	
}

// get the value for the property of the object and store in WM under the tag
void Visual_processor::handle_event(const Visual_Get_Value_event * event_ptr)
{
	// retrieve the value for the object and the property, and store the result
	// at the specified time from now.
	long total_time = get_time() + visual_find_time;
	retrieve_and_store_value(event_ptr->object_name, event_ptr->property_name, 
		event_ptr->WM_property_name, total_time);
	send_operation_complete(total_time);
}

// find the object matching a list of properties and values, and store its name under the tag
// it is now the current in_focus object
// This action is used for a full, time-consuming, visual search.
void Visual_processor::handle_event(const Visual_Find_Object_event * event_ptr)
{
	// get object_names back
	Symbol_list_t object_names = find_all_objects (event_ptr->property_names, event_ptr->property_values);
	// we have to pick one object to process, it will be the target_object_name
	// if there are no object names, then the object_name is Absent
	Symbol object_name;
	if (object_names.size() == 0)
		object_name = Absent_c;
	// if only one, use it
	else if (object_names.size() == 1)
		object_name = object_names.front();
	// otherwise, choose one at random
	else {
		int n = random_int(static_cast<int>(object_names.size()));
		object_name = get_nth_Symbol(object_names, n);
		}
	
	// if same as in-focus object, no change, no time charge
	// but there might be a change in the tag under which it is stored, so store it under the tag also
	// previous tag is left in place - dk 10/6/98
	if (object_name == in_focus_object_name) {
		store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}
		
	// at this point, object had to be looked for, so estimate the search time for it
//	long search_time = estimate_search_time(object_name, event_ptr->property_names, event_ptr->property_values);
	long search_time = visual_find_time;
	long total_time = get_time() + search_time;
	
	// object is now in focus (unless it is absent)
	change_focus(object_name);

	// store the result unconditionally under the tag
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
	
	// if object was found, then store in object store
	//	 - we are now looking at it, not the previous one
	// if not found, we are looking at nothing
	if (object_name != Absent_c) {
		if(Trace_out && get_trace()) {
			Trace_out << processor_info() << " Look_for found: " << object_name << endl;
			}
		store_WM_object(object_name, total_time); // it may not be there
		}
		
//	display_contents();
}

// find the object matching a list of properties and values, and store its name under the tag
// it is now the current in_focus object
// This action is used only to access visual properties of objects that might be currently present,
// such as a newly-appeared object, and not when a search over the screen is required.
// I.e. This is for emulating attention-getting by some class of visual events.
void Visual_processor::handle_event(const Visual_Get_Object_event * event_ptr)
{

	// get object_names back
	Symbol_list_t object_names = find_all_objects (event_ptr->property_names, event_ptr->property_values);
	// we have to pick one object to process
	// if there are no object names, then the object_name is "Absent"
	Symbol object_name;
	if (object_names.size() == 0)
		object_name = Absent_c;
	// if only one, use it
	else if (object_names.size() == 1)
		object_name = object_names.front();
	// otherwise, choose one at random
	else {
		int n = random_int(static_cast<int>(object_names.size()));
		object_name = get_nth_Symbol(object_names, n);
		}
	
	// if same as in-focus object, no change, no time charge
	// but there might be a change in the tag under which it is stored, so store it under the tag also
	// previous tag is left in place - dk 10/6/98
	if (object_name == in_focus_object_name) {
		store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}
		
	long total_time = get_time() + visual_check_time;
	
	// object is now in focus (unless it is absent)
	change_focus(object_name);

	// store the result unconditionally under the tag
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
	
	// if object was found, then store in object store, and update in-focus
	//	 - we are now looking at it, not the previous one
	// if not found, we are looking at nothing
	if (object_name != Absent_c) {
		if(Trace_out && get_trace()) {
			Trace_out << processor_info() << " Check_for found: " << object_name << endl;
			}
		store_WM_object(object_name, total_time); // it may not be there
		}
		
//	display_contents();
}

// find the object matching a list of properties and values. If not present, set up a wait for it, until
// it does get added.  When present, store its name under the tag. It is now the current in_focus object
void Visual_processor::handle_event(const Visual_Wait_Object_event * event_ptr)
{
	// get object_names back
	Symbol_list_t object_names = find_all_objects (event_ptr->property_names, event_ptr->property_values);
	// we have to pick one object to process
	// if there are no object names, then the object_name is Absent
	Symbol object_name;
	if (object_names.size() == 0)
		object_name = Absent_c;
	// if only one, use it
	else if (object_names.size() == 1)
		object_name = object_names.front();
	// otherwise, choose one at random
	else {
		int n = random_int(static_cast<int>(object_names.size()));
		object_name = get_nth_Symbol(object_names, n);
		}
	
	// if object is not present, set up a wait for it and return
	if (object_name == Absent_c) {
		waiting_for_object = true;
		waiting_for_property_names = event_ptr->property_names;
		waiting_for_property_values = event_ptr->property_values;
		waiting_for_tagname = event_ptr->WM_tag_name;
		return;
		}
	
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Wait_for found: " << object_name << endl;

	// if same as in-focus object, no change, no time charge
	// but there might be a change in the tag under which it is stored, so store it under the tag also
	// previous tag is left in place - dk 10/6/98
	if (object_name == in_focus_object_name) {
		store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}

	// object is present, but requires a change in focus.
	change_focus(object_name);

	// tell WM and Cog. processor about the newly in-focus object
	// get the estimated search time
	// get the estimated number of fixations required:
//	double search_time = estimate_search_time(object_name, event_ptr->property_names, event_ptr->property_values);
	long total_time = get_time() + visual_find_time;
//	long total_time = get_time() + search_time;
	// store the name of the newly in-focus object in WM object store and tag store
	store_WM_object(object_name, total_time); // it may not already be there
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);
	
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
		
//	display_contents();
}

// This function "refinds" an object given its name, taking e.g. an eye-movement time
// and makes it the current in_focus object
// no change made to tag store
void Visual_processor::handle_event(const Visual_Look_at_Object_event * event_ptr)
{
	Symbol object_name = event_ptr->object_name;
	// check to see if the object is present - if not, print message and 
	// set object name to Absent
	if (!is_present(object_name)) {
		object_name = Absent_c;
		}
	// check to see if it was disappearing - it's absent if it was
	else {
		Symbol current_status = retrieve(event_ptr->object_name, Status_c);
		if (current_status == Disappearing_c) {
			object_name = Absent_c;
			}
		}

	// if same as in-focus object, no change, no time charge
	if (object_name == in_focus_object_name) {
		send_operation_complete(get_time());
		return;
		}
		
	long total_time = get_time() + visual_refind_time;
	
	// object is now in focus - unless it is Absent
	change_focus(object_name);

	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Look_at: " << object_name << endl;
	// if object was found, then store in object store
	if (object_name != Absent_c) {
		store_WM_object(object_name, total_time); // in case it wasn't there - can happen
		}
		
	// always store the name of the object under the tag - absent if it wasn't there
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
}

void Visual_processor::change_focus(const Symbol& object_name)
{
	// if the object_name is absent, it will not become the new focus-on object (which will be Nil_c)
	// change the status of the current object - note all properties remain!
	// WM_Retrieve_Value_Object_event handler filters properties that are not in focus!
	if (in_focus_object_name != Nil_c) {
		store(in_focus_object_name, Status_c, Out_of_focus_c);
		}
	if (object_name != Absent_c) {
		// make the current object the in-focus object
		store(object_name, Status_c, In_focus_c);
		in_focus_object_name = object_name;
		Symbol new_location = retrieve(in_focus_object_name, Location_c);
		current_eye_location = (new_location.has_Point_numeric_value()) ? new_location.get_Point() : GU::Point();

		notify(&View_base::notify_eye_movement, current_eye_location);

		// now that it is in focus, event type info should be deleted
		if(Trace_out && get_trace()) {
			Trace_out << processor_info() << " Losing event type for " << object_name;
			}
		erase(object_name, Event_type_c);
		}
	else {
		in_focus_object_name = Nil_c;	// we are looking at nothing (not at Absent_c)
		}
	// send a message to the device that the visual focus has changed
	// this generates a lot of events of focusing on Nil - not sure this is correct ... maybe it should
	// only change when the object_name != Absent_c
	Symbol phys_name = (in_focus_object_name == Nil_c) ? Nil_c : name_map.get_physical_name(in_focus_object_name);
	Event * output_event_ptr = new Device_VisualFocusChange_event(
			get_time(), get_human_ptr()->get_device_ptr(), phys_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);

}			
	
	
void Visual_processor::display_object_info(Output_tee& ot) const
{
	Object_Memory_Processor::display_object_info(ot, "Visual Memory");
	ot << "  In focus: " << in_focus_object_name << endl;
}

void Visual_processor::display_contents(Output_tee& ot) const
{
	ot << "Visual Memory contents:" << endl;
	ot << "In focus: " << in_focus_object_name << endl;
	Object_Memory_Processor::display_contents(ot);
}

// given that we are searching for object_name, and it is present, and the search is specified by the property name/value pairs,
// calculate an estimate of how many fixations are required to locate it.
long Visual_processor::estimate_search_time(const Symbol& target_name, const Symbol_list_t& prop_names, const Symbol_list_t prop_values)
{
	int fixations = estimate_fixations_required(target_name, prop_names, prop_values);
	// adjust for the assumed number of repeats
	double nrepprop = (fixations * .01) - .03;
	if(nrepprop < 0.)
		nrepprop = 0.;
	double adjusted_fixations = fixations + nrepprop * fixations;
	// estimate visual_fixation_duration per fixation
	long search_time = visual_fixation_duration * adjusted_fixations;
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Estimated fixations: " << fixations << " adjusted: " << adjusted_fixations << " estimated search time: " << search_time << endl;
	
	return long(search_time);
}

// emulate an EPIC-style visual search
// start with no colors known
// 1. add the colors for any unvisted objects visible from the current eye position
// 2. if one or more of the objects has the sought-for color, choose one at random; 
//		else choose one with unknown color at random.
// 3. increment n_fixations, make the chosen object the current eye position, mark the chosen object as visited
//	if it matches the rest of the properties, stop and return the number of fixations
//		else go to 1.
int Visual_processor::estimate_fixations_required(const Symbol& target_name, const Symbol_list_t& prop_names, const Symbol_list_t prop_values)
{
	// first find and keep the the color property if present
	Symbol_list_t::const_iterator itp = prop_names.begin(), itv = prop_values.begin();
	Symbol target_color = Symbol("---");	// color assumed not specified
	for(;itp != prop_names.end(); ++itp, ++itv)
		if(*itp == Color_c)
			target_color = *itv;

	Search_objects_t search_objects;
	
	// get a a list of all the object names and build a container of search objects
	Symbol_list_t all_object_names = get_all_object_names();
	for(Symbol_list_t::const_iterator it = all_object_names.begin(); it != all_object_names.end(); ++it) {
		Symbol obj_location = retrieve(*it, Location_c);
		GU::Point loc = (obj_location.has_Point_numeric_value()) ? obj_location.get_Point() : GU::Point();
		Symbol obj_size = retrieve(*it, Size_c);
		// object size is average of horizontal and vertical sizes
		double sz = (obj_size.has_pair_numeric_value()) ? (obj_size.get_x() + obj_size.get_y()) / 2. : 0.;
		search_objects.push_back(Search_object(*it, loc, sz));	// color is unspecified at this time
		}


	// current eye location is location of in_focus_object, and is our first "chosen object"
	// if none, we assume start from center of visual space
	Symbol chosen_object_name = in_focus_object_name; 
	
	int n_fixations = 0;
	std::vector<Symbol> same_color_names, no_color_names, unvisited_names;
	while(true) {
		Symbol chosen_object_loc = retrieve(chosen_object_name, Location_c);
		GU::Point eye_location = (chosen_object_loc.has_Point_numeric_value()) ? chosen_object_loc.get_Point() : GU::Point();

		for(Search_objects_t::iterator it = search_objects.begin(); it != search_objects.end(); ++it) {
			double eccentricity = GU::cartesian_distance(eye_location, it->location);
			if(accuity_function(eccentricity, it->size))
				it->color = retrieve(it->name, Color_c);
			}
		
		same_color_names.clear(); no_color_names.clear(); unvisited_names.clear();
		for(Search_objects_t::iterator it = search_objects.begin(); it != search_objects.end(); ++it) {
			if(target_color == Symbol("---"))	// if color not specified, drop everybody into an unvisited list
				unvisited_names.push_back(it->name);
			else if(it->color == target_color)
				same_color_names.push_back(it->name);
			else if(it->color == Nil_c)
				no_color_names.push_back(it->name);
			}
	
		// debugging info 
		if(Trace_out && get_trace()) 
			Trace_out << processor_info() << " Number of: fixations: " << n_fixations << " search objects: " << search_objects.size() 
				<< " target color: " << same_color_names.size() << " unknown color: " << no_color_names.size()  
				<< " unvisited: " << unvisited_names.size() << endl;

		// now pick an object 
		n_fixations++;

		// if we have no color specified, pick only from unvisited objects
		if(unvisited_names.size() > 0) {
			int i = random_int(unvisited_names.size());
			chosen_object_name = unvisited_names[i];
			}
		// pick a same colored object at random
		else if(same_color_names.size() > 0) {
			int i = random_int(same_color_names.size());
			chosen_object_name = same_color_names[i];
			}
		//pick one of the uncolored objects at random
		else if(no_color_names.size() > 0) {
			int i = random_int(no_color_names.size());
			chosen_object_name = no_color_names[i];
			}
		else {
			// object is not there and can't be found - the object we are looking for must be absent
			Assert(target_name == Absent_c);
			return n_fixations;
			}
			
		// if the chosen object is the sought-for object, the search is done
		if(chosen_object_name == target_name)
			return n_fixations;
		// if not, remove this object from the search objects (it has already been visited)
		for(Search_objects_t::iterator it = search_objects.begin(); it != search_objects.end(); ++it)
			if(it->name == chosen_object_name) {
				search_objects.erase(it);
				break;
				}
		// and repeat
		}
}

const double zone = 1.0;
const double coefvar = 0.7;
const double intercept = 0.1 ;
const double x_coeff = 0.1;
const double x2_coeff = 0.035;

bool Visual_processor::accuity_function(double eccentricity, double obj_size)
{
	if(eccentricity < zone)
		return true;
//	else
//		return false;
	double threshold_size = intercept + x_coeff * eccentricity + x2_coeff * eccentricity * eccentricity;
	double fluctuation = normal_random_variable(0., coefvar * obj_size);
	return (obj_size + fluctuation > threshold_size);
}		

