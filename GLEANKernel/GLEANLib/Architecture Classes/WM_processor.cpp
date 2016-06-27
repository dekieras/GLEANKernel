#include <iostream>

#include "WM_processor.h"
#include "WM_event_types.h"
#include "LTM_event_types.h"
#include "Coordinator.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "Glean_standard_symbols.h"
#include "Glean_exceptions.h"

using namespace std;

/* notes 4/22/01
WM_processor checks for being in focus in order to allow retrieval of 
any property other than Status - change this to include Event_type and Type also.
*/ 

/* 5/5/00 - modifed WM_Retrieve_Value_Object_event
Output an error message if the object is not present in the object store.
If it is present, access the status.  
If the status is being requested, unconditionally return it.
If the status is in-focus, return the accessed value.
Else return absent.
*/

//notes 7/23/98
// commented out the bizarre increment of current time in handle_event WM_Retrieve_Value_Tag_event

void WM_processor::initialize()
{
	initialize_tag_store();
	object_store.clear();	// initialize the object store
}


void WM_processor::initialize_tag_store()
{
	// do the initialization by copying from the initialization source
	tag_store = get_human_ptr()->get_Cognitive_ptr()->get_GOMS_Model_Sptr()->get_initial_WM_contents();
}

// modified to show less - dk 8/29/03
void WM_processor::display_contents(Output_tee& ot)
{
	ot << "Contents of WM tag store:" << endl;
//	ot << "Initial contents:" << endl;
//	initial_WM_contents.display_contents(ot);
//	ot << "Current contents:" << endl;
	tag_store.display_contents(ot);

/*	
	ot << "Current contents of WM object store:" << endl;
	WM_Object_set_t::iterator it;
	for (it = object_store.begin(); 
		 it != object_store.end();
		 it++) {
		 ot << (*it).object_name << endl;
		}
*/

}

// split to allow more selective display - dk 05/13/06
void WM_processor::display_tag_store_contents(Output_tee& ot)
{
	ot << "Contents of WM tag store:" << endl;
	tag_store.display_contents(ot);
}

// split to allow more selective display - dk 05/13/06
void WM_processor::display_object_store_contents(Output_tee& ot)
{
	ot << "Current contents of WM object store:" << endl;
	WM_Object_set_t::iterator it;
	for (it = object_store.begin(); 
		 it != object_store.end();
		 it++) {
		 ot << (*it).object_name << endl;
		}
}

Symbol_list_t WM_processor::get_object_name_list() const
{
	Symbol_list_t result;
	WM_Object_set_t::const_iterator it;
	for (it = object_store.begin(); 
		 it != object_store.end();
		 it++) {
		result.push_back(it->object_name);
		}
	return result;
}

void WM_processor::accept_event(const WM_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

void WM_processor::handle_event(const WM_Store_Tag_event * event_ptr)
{
	// store the input property_value under input property_name
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Store WM tag: " << event_ptr->tag_name << ' ' << event_ptr->tag_value << endl;
	tag_store.store(event_ptr->tag_name, event_ptr->tag_value);
	notify(&View_base::notify_Wm_tag_set, event_ptr->tag_name, event_ptr->tag_value);

//	display_contents(if(Debug_out) Debug_out);
}

void WM_processor::handle_event(const WM_Erase_Tag_event * event_ptr)
{
	// erase the property_value under input property_name
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Erase WM tag: " << event_ptr->tag_name << endl;
	tag_store.erase(event_ptr->tag_name);
	notify(&View_base::notify_Wm_tag_erase, event_ptr->tag_name);
	//	display_contents(if(Debug_out) Debug_out);
}

// event is non-const because we will modify it.
void WM_processor::handle_event(const WM_Retrieve_Value_Tag_event * event_ptr)
{
	// retrieve the property_value under input property_name and put it in the request event to return it
	WM_Retrieve_Value_Tag_event * p = const_cast<WM_Retrieve_Value_Tag_event *>(event_ptr);
	p->tag_value = tag_store.retrieve(event_ptr->tag_name);
}

// immediate access functions
void WM_processor::store_tag(const Symbol& tag_name, const Symbol& tag_value)
{
	tag_store.store(tag_name, tag_value);
	notify(&View_base::notify_Wm_tag_set, tag_name, tag_value);
}

Symbol WM_processor::retrieve_tag_value(const Symbol& tag_name)
{
	return tag_store.retrieve(tag_name);
}

void WM_processor::erase_tag(const Symbol& tag_name)
{
	tag_store.erase(tag_name);
	notify(&View_base::notify_Wm_tag_erase, tag_name);
	//	display_contents(if(Debug_out) Debug_out);
}

void WM_processor::handle_event(const WM_Store_Object_event * event_ptr)
{
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Store WM_Object " << event_ptr->object_name << endl;
	WM_Store_Object_event * p = const_cast<WM_Store_Object_event *>(event_ptr);
	WM_Object new_WM_obj(p->object_name, p->responsible_processor);	
	// try to insert the property_value
	pair<WM_Object_set_t::iterator, bool> result = object_store.insert(new_WM_obj);
	if (result.second) {
		notify(&View_base::notify_Wm_object_set, event_ptr->object_name, Symbol(event_ptr->responsible_processor->get_name()));
		return;
		}
	// if insertion failed, try to erase the item
	else {
		object_store.erase(result.first);
		// then insert again;
		pair<WM_Object_set_t::iterator, bool> result =
			object_store.insert(new_WM_obj);
		// check to make sure that insert was ok
		Assert(result.second);
		notify(&View_base::notify_Wm_object_set, event_ptr->object_name, Symbol(event_ptr->responsible_processor->get_name()));
/*
		if (result.second)
			return;
		else
			Normal_out << "could not store WM_Object" << endl;
*/
		}
}

void WM_processor::handle_event(const WM_Erase_Object_event * event_ptr)
{
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Erase WM_Object " << event_ptr->object_name << endl;
	// erase the object under the object name, using a temporary probe object
	object_store.erase(WM_Object(event_ptr->object_name, 0));
	notify(&View_base::notify_Wm_object_erase, event_ptr->object_name);
}

// event is non-const because we will modify it.
// Get the responsible processor for the object, and forward the request to it.
void WM_processor::handle_event(const WM_Retrieve_Value_Object_event * event_ptr)
{
	Symbol result;
	Object_Memory_Processor * obj_mem_proc_ptr;

	WM_Object probe(event_ptr->object_name, 0);	

	WM_Object_set_t::iterator it = object_store.find(probe);
	// did we find a match for the probe? if not, object is not in focus
	if (it == object_store.end() ) {
		// this switch is a kludge installed to support the interrupt rule checking - dk 5/4/00
		if (focus_messages_flag) {	// flag to turn not-in-focus messages on & off
			get_human_ptr()->get_Cognitive_ptr()->output_current_execution_state();
			Normal_out << processor_info() << " Attempt to query object \"" << event_ptr->object_name << "\" not in object store." << endl;
			// display the contents here
			display_contents(Normal_out);
			throw Glean_exception(this, " Attempt to query object \"" + event_ptr->object_name.str() + "\" not in object store.");
			// wait for some input
			// get_human_ptr()->get_Cognitive_ptr()->pause_and_then_single_step();
			}
		result = Absent_c;
		}
	else {
		obj_mem_proc_ptr = (*it).responsible_processor;
		// get the property value of the object
		Symbol p_name = event_ptr->property_name;
		Symbol p_value = obj_mem_proc_ptr->retrieve(event_ptr->object_name, p_name);
		// if the property name is one of the special property, return the value unconditionally
		if (p_name == Status_c || p_name == Event_type_c || p_name == Type_c ) {
			// return the result in the event
			result = p_value;
			}
		// for all other p_names, return either the value or Absent if the object is or is not in focus
		else {
			Symbol status_result = obj_mem_proc_ptr->retrieve(event_ptr->object_name, Status_c);
			if (status_result == In_focus_c) 
				// ask that processor for the value. 
				result = obj_mem_proc_ptr->retrieve(event_ptr->object_name, event_ptr->property_name);
			else {
				result = Absent_c;
				// this switch is a kludge installed to support the interrupt rule checking - dk 5/4/00
				if (focus_messages_flag) {	// flag to turn not-in-focus messages on & off
					get_human_ptr()->get_Cognitive_ptr()->output_current_execution_state();
					/*
					Normal_out  << processor_info() << " Attempt to query property \"" << event_ptr->property_name 
								<< "\" of object \"" << event_ptr->object_name << "\" that is not in focus." << endl;
					*/
					// display the contents here
					display_contents(Normal_out);
					get_human_ptr()->output_object_store_contents();
					get_human_ptr()->output_visual_memory_contents();
					get_human_ptr()->output_auditory_memory_contents();
					get_human_ptr()->output_task_memory_contents();
					get_human_ptr()->output_ltm_memory_contents();
					throw Glean_exception(this, " Attempt to query property \"" + event_ptr->property_name.str() 
								+ "\" of object \"" + event_ptr->object_name.str() + "\" that is not in focus.");
					/*
					// wait for some input
					get_human_ptr()->get_Cognitive_ptr()->pause_and_then_single_step();
					*/
					}
				}
			}
		
		}
	// return the result in the message
	WM_Retrieve_Value_Object_event * p = const_cast<WM_Retrieve_Value_Object_event *>(event_ptr);
	p->property_value = result;
}

void WM_processor::handle_event(const WM_Display_Request_event * )
{
	display_contents(Normal_out);
}

