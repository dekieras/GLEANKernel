#include "Task_processor.h"
#include "Coordinator.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "WM_event_types.h"
#include "Cognitive_event_types.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Numeric_utilities.h"
#include "Glean_standard_symbols.h"
#include "Glean_exceptions.h"

#include <iostream>
//#include <typeinfo>
//#include <DebugNew.h>
using namespace std;


//notes 7/23/98
// only one in-focus item at a time
// notes 05/17/06
// modified to try to ensure that WM object store has the last-accessed item with
// write and update operators so that property-of-object constructions reflect current focus

// values for processor constants - modified 07/20/03
// time less one to align with next cognitive cycle.
const int Task_processor::task_find_time = 1200 - 1;
const int Task_processor::task_store_time = 1200 - 1;
const int Task_processor::task_property_store_time = 1200 - 1;
const int Task_processor::task_property_erase_time = 1200 - 1;
const int Task_processor::task_object_erase_time = 1200 - 1;
const int Task_processor::task_list_create_time = 1200 - 1;
const int Task_processor::task_list_modification_time = 1200 - 1;
const int Task_processor::task_list_count_time = 1200 - 1;

void Task_processor::initialize()
{
	set_initial_contents(get_human_ptr()->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_task_description());
	Object_Memory_Processor::initialize();
	in_focus_object_name = Nil_c;	
//	display_contents(Normal_out);
}

void Task_processor::accept_event(const Task_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

// send an operation complete event
void Task_processor::send_operation_complete(long completion_time)
{
	Event * event_ptr = new Cognitive_Task_Operation_Complete_event(completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

void Task_processor::display_object_info(Output_tee& ot) const
{
	Object_Memory_Processor::display_object_info(ot, "Task Memory");
	ot << "  In focus: " << in_focus_object_name << endl;
}

void Task_processor::display_contents(Output_tee& ot) const
{
	ot << "Task Memory contents:" << endl;
	Object_Memory_Processor::display_contents(ot);
	ot << "In focus: " << in_focus_object_name << endl;
}

void Task_processor::store_and_notify(const Symbol& obj_name, const Symbol& prop_name, const Symbol& prop_value)
{
	store(obj_name, prop_name, prop_value);
	notify(&View_base::notify_memory_property_changed, obj_name, prop_name, prop_value);
}

void Task_processor::store_and_notify(const Symbol& object_name, const Symbol_list_t& property_names, const Symbol_list_t& property_values)
{
	store(object_name, property_names, property_values);

	Symbol_list_t::const_iterator p_it = property_names.begin();
	Symbol_list_t::const_iterator v_it = property_values.begin();
		for(; p_it != property_names.end() && v_it != property_values.end(); ++p_it, ++v_it) {
			notify(&View_base::notify_memory_property_changed, object_name, *p_it, *v_it);
			}
}

void Task_processor::erase_and_notify(const Symbol& obj_name)
{
	erase(obj_name);
	notify(&View_base::notify_memory_object_erase, obj_name);
}

void Task_processor::erase_and_notify(const Symbol& obj_name, const Symbol& prop_name)
{
	erase(obj_name, prop_name);
	notify(&View_base::notify_memory_property_erase, obj_name, prop_name);
}


void Task_processor::change_focus(const Symbol& object_name, long change_time)
{
	// if the object_name is absent, it will not become the new focus-on object (which will be "Nil_c")
	// change the Status_c of the current object - note all properties remain!
	// WM_Retrieve_Value_Object_event handler filters properties that are not in focus!
	// tell WM to add/remove object from object store accordingly - only one accessible at a time,
	// and that is the one that is in focus.
	if (in_focus_object_name != Nil_c && is_present(in_focus_object_name)) {
		erase_WM_object(in_focus_object_name, change_time);
		store_and_notify(in_focus_object_name, Status_c, Out_of_focus_c);
		in_focus_object_name = Nil_c;
		}
		
	if (object_name != Absent_c) {
		// make the current object the in-focus object
		store_WM_object(object_name, change_time);
		store_and_notify(object_name, Status_c, In_focus_c);
		in_focus_object_name = object_name;
		}
	else
		in_focus_object_name = Nil_c;	// we are thinking of nothing (not of "Absent_c")
}


// if the named object was in focus, it is no longer, and now nothing is in focus
// this should be called before the object is erased.  
void Task_processor::remove_focus(const Symbol& object_name, long change_time)
{
	if(object_name == in_focus_object_name) {
		erase_WM_object(object_name, change_time);
//		store_and_notify(object_name, Status_c, Out_of_focus_c);
		in_focus_object_name = Nil_c;
		}
}

// get the value for the property of the object and store in WM under the tag
void Task_processor::handle_event(const Task_Get_Value_event * event_ptr)
{
	// retrieve the value for the object and the property, and store the result
	// at the specified time from now.
	long total_time = get_time() + task_find_time;
	retrieve_and_store_value(event_ptr->object_name, event_ptr->property_name, 
		event_ptr->WM_property_name, total_time);
	send_operation_complete(total_time);
}

// find the object matching a list of properties and values, and store its name under the tag
// it is now the current In_focus_c object
void Task_processor::handle_event(const Task_Get_Object_event * event_ptr)
{
	// get object_name back
	Symbol object_name = find_object (event_ptr->property_names, event_ptr->property_values);
	// if same as in-focus object, no change, no time charge
	if (object_name == in_focus_object_name) {
		store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}
		
	long total_time = get_time() + task_find_time;
	
	// whether object was found, we store the name in WM object store and tag store
	// object is now in focus - unless it is absent
	change_focus(object_name, total_time);
//	store_WM_object(object_name, total_time);
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, total_time);

	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);

}

// Write_task_item_whose ... // always creates new object (error if same properties as
//	one that is already there, store anyway)
// Update_for_task_item <item_name> that ... // updates previous object (error if not
//  already there, new one is created ...)
// same handler; if supplied object name is empty, create new object, otherwise 
// try to find it.

// store the object with a list of properties and values
void Task_processor::handle_event(const Task_Write_Object_event * event_ptr)
{
	Symbol object_name = make_new_item_name();
	// store the info
	store_and_notify(object_name, event_ptr->property_names, event_ptr->property_values);

//	Normal_out << processor_info() << " Task store: " << object_name << endl;
	
	// assume a constant store time per property 	
	long total_time = get_time() + task_store_time + task_property_store_time * 
		long(event_ptr->property_values.size());
	// put object that was written in focus - dk 3/31/06
	// object is now in focus - unless it is absent
	change_focus(object_name, total_time);
	store_WM_object_under_tag(object_name, event_ptr->WM_tag_name, get_time());
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
	
}

Symbol Task_processor::make_new_item_name()
{
	// create a new task object name
	string s("Tobj");
	s += int_to_string(input_object_number++);
	Symbol object_name(s); // create the unique object name
	return object_name;
}

void Task_processor::handle_event(const Task_Update_Object_event * event_ptr)
{
	Symbol object_name = event_ptr->object_name;
//	Normal_out << processor_info() << " Task update: " << object_name << endl;
	if(!is_present(object_name)) {
//		Normal_out << processor_info() << " *** Error: " << object_name << " not in Task store" << endl;
		throw Glean_exception(this, object_name.str() + " not in Task store");
		return;
		}
		
	
	// store the object, properties and values, updating if already there
	store_and_notify(object_name, event_ptr->property_names, event_ptr->property_values);

	// assume a constant store time per property 	
	long total_time = get_time() + task_store_time + task_property_store_time * 
		long(event_ptr->property_values.size());
	
	// put object that was updated in focus - dk 3/31/06
	// object is now in focus - unless it is absent
	change_focus(object_name, total_time);
	
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
}

// erase a property of the object
void Task_processor::handle_event(const Task_Erase_Property_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_Erase_Property_event: " << event_ptr->object_name << endl;

	// store the object, properties and values
	erase_and_notify(event_ptr->object_name, event_ptr->property_name);

	// assume a constant erase time per property 	
	long total_time = get_time() + task_store_time + task_property_erase_time;
	
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
}

// erase the object
void Task_processor::handle_event(const Task_Erase_Object_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_Erase_Object_event: " << event_ptr->object_name << endl;

	
	erase_and_notify(event_ptr->object_name);

	// assume a constant erase time
	long total_time = get_time() + task_object_erase_time;
	
	// if the object was in focus, now nothing is in focus
	// and remove it from the object store
	if(event_ptr->object_name == in_focus_object_name)
		remove_focus(event_ptr->object_name, total_time);
	else
		erase_WM_object(event_ptr->object_name, total_time);
	
	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);
}

/*
Task List Functions
This set of functions maintains a set of objects in the Task Object Store that 
basically implements a linked list of objects, each with their own set of properties and values.
The list can be modified and accessed basically by moving down the list in the usual ways.
The list objects have names in the form of Tobjnnn, and have a Label property consisting of the list name,
in the case of the "list header" objects that denote the whole list. The "list node" objects
have whatever additional properties the user desires. 
*/


void Task_processor::handle_event(const Task_List_Create_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_List_Create_event: " << event_ptr->task_list_name << endl;

	Symbol list_header = make_new_item_name();
	store_and_notify(list_header, Label_c, event_ptr->task_list_name);
	store_and_notify(list_header, First_c, Absent_c);
	store_and_notify(list_header, Last_c, Absent_c);
	
	long total_time = get_time() + task_list_create_time;
		
	send_operation_complete(total_time);
}

// This removes all of the list node objects, and leaves the list header object in the "empty" state
void Task_processor::handle_event(const Task_List_Erase_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_List_Erase_event: " << event_ptr->task_list_name << endl;

	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	Symbol node = retrieve(list_header, First_c);
	Symbol next_node;
	for(Symbol node = retrieve(list_header, First_c); node != Absent_c;
			node = next_node) {
		next_node = retrieve(node, Next_c);
		erase_and_notify(node);
		}
		
	store_and_notify(list_header, First_c, Absent_c);
	store_and_notify(list_header, Last_c, Absent_c);

	long total_time = get_time() + task_list_create_time;
		
	send_operation_complete(total_time);
}

void Task_processor::handle_event(const Task_List_Size_event * event_ptr)
{
	// count the number of nodes in the list
//	Normal_out << processor_info() << " Task_List_Size_event: " << event_ptr->task_list_name << endl;

	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	Symbol node = retrieve(list_header, First_c);
	int number = 0;
	for(Symbol node = retrieve(list_header, First_c); node != Absent_c;
			node = retrieve(node, Next_c)) {
		number++;
		}
	Symbol result(number);
	
	long total_time = get_time() + task_list_count_time;

	store_WM_object_under_tag(result, event_ptr->WM_tag_name, total_time);
	send_operation_complete(total_time);

}

// add the specified task object to the end of the list. 
void Task_processor::handle_event(const Task_List_Prepend_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_List_Prepend_event: " 
//		<< event_ptr->task_list_name << ' ' << event_ptr->task_item_name << endl;

	list_prepend(event_ptr->task_list_name, event_ptr->task_item_name);

	long total_time = get_time() + task_list_modification_time;
	send_operation_complete(total_time);
}

// add the specified task object to the end of the list. 
void Task_processor::handle_event(const Task_List_Append_event * event_ptr)
{
//	Normal_out << processor_info() << " Task_List_Append_event: " 
//		<< event_ptr->task_list_name << ' ' << event_ptr->task_item_name << endl;

	list_append(event_ptr->task_list_name, event_ptr->task_item_name);

	long total_time = get_time() + task_list_modification_time;
	send_operation_complete(total_time);
}

void Task_processor::list_prepend(const Symbol& list_name, const Symbol& new_node)
{
	if(new_node == Absent_c || new_node == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid item being added to list: " << new_node << endl;
		throw Glean_exception(this, "Invalid item being added to list: " + new_node.str());
		return;
		}
	
	// set list membership
	store_and_notify(new_node, In_list_c, list_name);
	
	// if there is already a first node, make the new item the first
	Symbol list_header = find_object(Label_c, list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << list_name << endl;
		throw Glean_exception(this, "Invalid list name: " + list_name.str());
		return;
		}
	Symbol first_node = retrieve(list_header, First_c);
	if(first_node != Absent_c) {
		store_and_notify(first_node, Previous_c, new_node);
		store_and_notify(new_node, Previous_c, Absent_c);
		store_and_notify(new_node, Next_c, first_node);
		store_and_notify(list_header, First_c, new_node);
		}
	// else it becomes the first and last node
	else {
		store_and_notify(new_node, Previous_c, Absent_c);
		store_and_notify(new_node, Next_c, Absent_c);
		store_and_notify(list_header, First_c, new_node);
		store_and_notify(list_header, Last_c, new_node);
		}
}

void Task_processor::list_append(const Symbol& list_name, const Symbol& new_node)
{
	if(new_node == Absent_c || new_node == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid item being added to list: " << new_node << endl;
		throw Glean_exception(this, " Invalid item being added to list:  " + new_node.str());
		return;
		}
	// set list membership
	store_and_notify(new_node, In_list_c, list_name);
	// if there is already a last node, make the new item the last
	Symbol list_header = find_object(Label_c, list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + list_name.str());
		return;
		}
	Symbol last_node = retrieve(list_header, Last_c);
	if(last_node != Absent_c) {
		store_and_notify(last_node, Next_c, new_node);
		store_and_notify(new_node, Previous_c, last_node);
		store_and_notify(new_node, Next_c, Absent_c);
		store_and_notify(list_header, Last_c, new_node);
		}
	// else it becomes the first and last node
	else {
		store_and_notify(new_node, Previous_c, Absent_c);
		store_and_notify(new_node, Next_c, Absent_c);
		store_and_notify(list_header, First_c, new_node);
		store_and_notify(list_header, Last_c, new_node);
		}
}


// add the specified task object before the specified one. If the specified one
// is Absent or Nil, the insertion is done at the end of the list
void Task_processor::handle_event(const Task_List_Insert_Before_event * event_ptr)
{
	//Normal_out << processor_info() << " Task_List_Insert_Before_event: " 
	//	<< event_ptr->task_list_name << ' ' << event_ptr->task_list_name << ' ' 
	//	<< event_ptr->loc_item_name << endl;

	Symbol list_name = event_ptr->task_list_name;
	Symbol new_node = event_ptr->task_item_name;
	if(new_node == Absent_c || new_node == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid item being added to list: " << new_node << endl;
		throw Glean_exception(this, " Invalid item being added to list: " + new_node.str());
		return;
		}
	// if the location is at the first, then simply make the new node the first
	Symbol list_header = find_object(Label_c, list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + list_name.str());
		return;
		}
	Symbol loc_node = event_ptr->loc_item_name;
	if(loc_node == retrieve(list_header, First_c))
		list_prepend(list_name, new_node);
	else if(loc_node == Absent_c || loc_node == Nil_c)
		list_append(list_name, new_node);
	else {
		Symbol prev_node = retrieve(loc_node, Previous_c);
		store_and_notify(prev_node, Next_c, new_node);
		store_and_notify(new_node, In_list_c, list_name);
		store_and_notify(new_node, Previous_c, prev_node);
		store_and_notify(new_node, Next_c, loc_node);
		store_and_notify(loc_node, Previous_c, new_node);
		}

	long total_time = get_time() + task_list_modification_time;
	send_operation_complete(total_time);
}


// find the item on the specified list with the specified properties, and store
// the list object (node) name in the specified tag
void Task_processor::handle_event(const Task_List_Find_Item_event * event_ptr)
{
	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << event_ptr->task_list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + event_ptr->task_list_name.str());
		return;
		}
	// we will find the item whose In_list property is the list name, and
	// that has the other specified properties
	Symbol_list_t props(event_ptr->property_names), values(event_ptr->property_values);
	props.push_front(In_list_c);
	values.push_front(event_ptr->task_list_name);
	
	Symbol item_name = find_object (props, values);
	store_find_results(item_name, event_ptr->WM_tag_name);
}

void Task_processor::store_find_results(const Symbol& item_name, const Symbol& tag_name)
{
	// if same as in-focus object, no change, no time charge
	if (item_name == in_focus_object_name) {
		// save the label of the object in the WM tag
		store_WM_object_under_tag(item_name, tag_name, get_time());
		send_operation_complete(get_time());
		return;
		}
		
	long total_time = get_time() + task_find_time;
	
	// whether object was found, we store the name in WM object store and tag store
	// object is now in focus - unless it is absent
	change_focus(item_name, total_time);
//	store_WM_object(item_name, total_time);
	store_WM_object_under_tag(item_name, tag_name, total_time);

	// operation required its total time plus some extra to make sure WM objects have been updated
	send_operation_complete(total_time + 50);

}

void Task_processor::handle_event(const Task_List_Find_Before_event * event_ptr)
{
	Symbol item_name = retrieve(event_ptr->loc_item_name, Previous_c);
	store_find_results(item_name, event_ptr->WM_tag_name);
}

void Task_processor::handle_event(const Task_List_Find_After_event * event_ptr)
{
	Symbol item_name = retrieve(event_ptr->loc_item_name, Next_c);
	store_find_results(item_name, event_ptr->WM_tag_name);
}

void Task_processor::handle_event(const Task_List_Find_First_event * event_ptr)
{
	Symbol list_object = find_object(Label_c, event_ptr->task_list_name);
	Symbol item_name = retrieve(list_object, First_c);
	store_find_results(item_name, event_ptr->WM_tag_name);
}

void Task_processor::handle_event(const Task_List_Find_Last_event * event_ptr)
{
	Symbol list_object = find_object(Label_c, event_ptr->task_list_name);
	Symbol item_name = retrieve(list_object, Last_c);
	store_find_results(item_name, event_ptr->WM_tag_name);
}

void Task_processor::handle_event(const Task_List_Remove_Item_event * event_ptr)
{
	//Normal_out << processor_info() << " Task_List_Remove_Item_event: " << event_ptr->task_list_name 
	//<< ' ' << event_ptr->task_item_name << endl;

	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << event_ptr->task_list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + event_ptr->task_list_name.str());
		return;
		}
	Symbol item_name = event_ptr->task_item_name;
	if(item_name == retrieve(list_header, First_c))
		remove_first(list_header);
	else if(item_name == retrieve(list_header, Last_c))
		remove_last(list_header);
	else {
		Symbol prev_node = retrieve(item_name, Previous_c);
		Symbol next_node = retrieve(item_name, Next_c);
		store_and_notify(prev_node, Next_c, next_node);
		store_and_notify(next_node, Previous_c, prev_node);
		finish_remove_list_item(item_name);
		}
		
}

void Task_processor::finish_remove_list_item(const Symbol& item_name)
{
	erase_and_notify(item_name, In_list_c);
	erase_and_notify(item_name, Next_c);
	erase_and_notify(item_name, Previous_c);
	long total_time = get_time() + task_list_modification_time;
	send_operation_complete(total_time);
}



void Task_processor::handle_event(const Task_List_Remove_First_event * event_ptr)
{
	//Normal_out << processor_info() << " Task_List_Remove_First_event: " << event_ptr->task_list_name << endl;

	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << event_ptr->task_list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + event_ptr->task_list_name.str());
		return;
		}
	remove_first(list_header);
}

void Task_processor::remove_first(const Symbol& list_header)
{
	Symbol first_node = retrieve(list_header, First_c);
	if(first_node == Absent_c)
		return;
	Symbol next_node = retrieve(first_node, Next_c);
	// if no next node, the list is now empty
	if(next_node == Absent_c) {
		store_and_notify(list_header, Last_c, Absent_c);
		store_and_notify(list_header, Next_c, Absent_c);
		}
	else {
		store_and_notify(next_node, Previous_c, Absent_c);
		store_and_notify(list_header, First_c, next_node);
		}
	finish_remove_list_item(first_node);
}

void Task_processor::handle_event(const Task_List_Remove_Last_event * event_ptr)
{
	//Normal_out << processor_info() << " Task_List_Remove_Last_event: " << event_ptr->task_list_name << endl;
	
	Symbol list_header = find_object(Label_c, event_ptr->task_list_name);
	if(list_header == Absent_c || list_header == Nil_c) {
//		Normal_out << processor_info() << " *** Error: Invalid list name: " << event_ptr->task_list_name << endl;
		throw Glean_exception(this, " Invalid list name: " + event_ptr->task_list_name.str());
		return;
		}
	remove_last(list_header);
}

void Task_processor::remove_last(const Symbol& list_header)
{
	Symbol last_node = retrieve(list_header, Last_c);
	if(last_node == Absent_c)
		return;
	Symbol prev_node = retrieve(last_node, Previous_c);
	// if no previous node, the list will be empty
	if(prev_node == Absent_c) {
		store_and_notify(list_header, First_c, Absent_c);
		store_and_notify(list_header, Last_c, Absent_c);
		}
	else {
		store_and_notify(prev_node, Next_c, Absent_c);
		store_and_notify(list_header, Last_c, prev_node);
		}	
	finish_remove_list_item(last_node);
}



