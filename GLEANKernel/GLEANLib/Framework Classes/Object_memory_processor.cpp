#include "Object_Memory_Processor.h"
#include "Human_processor.h"

//#include <typeinfo>
#include "Coordinator.h"
#include "LTM_event_types.h"
#include "WM_event_types.h"
#include "WM_processor.h"
#include "Cognitive_event_types.h"
#include "Cognitive_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

//#include <DebugNew.h>
#include <iostream>
#include <cassert>
using namespace std;

void Object_Memory_Processor::initialize()
{
	// reset object memory
	Object_Store::initialize();
}

// get the value for the property of the object and store in WM under the tag,
// scheduling it for the completion time.
void Object_Memory_Processor::retrieve_and_store_value(const Symbol& object_name, 
	const Symbol& property_name, const Symbol& WM_property_name, long completion_time)	
{
	// retrieve the value for object and property
	Symbol property_value = objects.retrieve(object_name, property_name);

	// store it in WM using a scheduled event.
	WM_Store_Tag_event * wm_event_ptr = 
		new WM_Store_Tag_event (completion_time, get_human_ptr()->get_wm_ptr(), WM_property_name, property_value);
	Coordinator::get_instance().schedule_event(wm_event_ptr);
}

// find the object matching a list of properties and values, and store its name under the tag
// send the store event at the time given in completion time

Symbol Object_Memory_Processor::find_and_store_object(
	const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& WM_tag_name, long completion_time)
{
	
	// retrieve the object name from the lists of property names and values
	Symbol object_name = objects.find(props, values);
	
	// store its name in WM using a scheduled event.
	WM_Store_Tag_event * wm_event_ptr = 
		new WM_Store_Tag_event (completion_time, get_human_ptr()->get_wm_ptr(), WM_tag_name, object_name);
	Coordinator::get_instance().schedule_event(wm_event_ptr);
	// put it in the object store
	WM_Store_Object_event * wm_obj_event_ptr =
		new WM_Store_Object_event(completion_time, get_human_ptr()->get_wm_ptr(), object_name, this);
	Coordinator::get_instance().schedule_event(wm_obj_event_ptr);
			
	// return found object name
	return object_name;
}

// store name of object in WM object store
void Object_Memory_Processor::store_WM_object(const Symbol& object_name, long completion_time)
{
	WM_Store_Object_event * wm_obj_event_ptr =
		new WM_Store_Object_event(completion_time, get_human_ptr()->get_wm_ptr(), object_name, this);
	Coordinator::get_instance().schedule_event(wm_obj_event_ptr);
	
}

// store name of object in WM under the tag
void Object_Memory_Processor::store_WM_object_under_tag(
	const Symbol& object_name, const Symbol& WM_tag_name, long completion_time)
{
	// store its name in WM using a scheduled event.
	WM_Store_Tag_event * wm_event_ptr = 
		new WM_Store_Tag_event (completion_time, get_human_ptr()->get_wm_ptr(), WM_tag_name, object_name);
	Coordinator::get_instance().schedule_event(wm_event_ptr);
}


// Tell wm to remove the object given by the tag
// send the erase event at the completion time
void Object_Memory_Processor::erase_WM_tag(const Symbol& WM_tag_name, long completion_time)
{
	// erase its name in WM using a scheduled event.
	WM_Erase_Tag_event * wm_tag_event_ptr = 
		new WM_Erase_Tag_event (completion_time, get_human_ptr()->get_wm_ptr(), WM_tag_name);
	Coordinator::get_instance().schedule_event(wm_tag_event_ptr);
}

// Tell wm to remove the object if it is not Nil
// send the erase event at the completion time
void Object_Memory_Processor::erase_WM_object(const Symbol& object_name, long completion_time)
{
	if (object_name == Nil_c)
		return;
	WM_Erase_Object_event * wm_obj_event_ptr = 
		new WM_Erase_Object_event (completion_time, get_human_ptr()->get_wm_ptr(), object_name);
	Coordinator::get_instance().schedule_event(wm_obj_event_ptr);
}


void Object_Memory_Processor::display_contents(Output_tee& ot) const
{
	Object_Store::display_contents(ot);
}
