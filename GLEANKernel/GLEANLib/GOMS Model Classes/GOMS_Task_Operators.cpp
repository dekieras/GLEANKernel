#include "GOMS_Task_Operators.h"
#include "GOMS_Model.h"
#include "GOMS_Memory_Operators.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Event_types.h"
#include "Cognitive_event_types.h"
#include "WM_event_types.h"
#include "Task_event_types.h"
#include "Task_processor.h"
#include "Device_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include <typeinfo>


//#include <DebugNew.h>
using namespace std;

void GOMS_Task_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_task();
}

void GOMS_Task_Memory_Access_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_task();
}

void GOMS_Get_Task_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to Task processor.

	Symbol_list_t arg_props, arg_values; // lists of resolved property names and values
	
	Symbol_list_t::iterator prop_it, value_it;
	
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it; //to iterate through pv_arg_list
	
	Symbol arg_prop, arg_value, tag_name;
//	GOMS_PV_Argument * pv_arg_ptr;
	
	// build lists of resolved property names and property values
	for(pv_arg_it = prop_value_arg_list.begin();
		pv_arg_it != prop_value_arg_list.end();
		pv_arg_it++) {
		
			arg_prop = (*pv_arg_it) -> resolve_property_name(cog_proc_ptr);
			arg_value = (*pv_arg_it) -> resolve_property_value(cog_proc_ptr);
			arg_props.push_back(arg_prop);
			arg_values.push_back(arg_value);
		}
		
	tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
		cog_proc_ptr->schedule_event(
			new Task_Get_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Get_Task_Operator::display(Output_tee& ot) const
{
	ot << "Get_task_item_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}


// write to the task description is a different kind of operator
void GOMS_Write_Task_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to Task processor.

	Symbol_list_t arg_props, arg_values; // lists of resolved property names and values
	
	Symbol_list_t::iterator prop_it, value_it;
	
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it; //to iterate through pv_arg_list
	
	Symbol arg_prop, arg_value, tag_name;
//	GOMS_PV_Argument * pv_arg_ptr;
	
	// build lists of resolved property names and property values
	for(pv_arg_it = prop_value_arg_list.begin();
		pv_arg_it != prop_value_arg_list.end();
		pv_arg_it++) {
		
			arg_prop = (*pv_arg_it) -> resolve_property_name(cog_proc_ptr);
			arg_value = (*pv_arg_it) -> resolve_property_value(cog_proc_ptr);
			arg_props.push_back(arg_prop);
			arg_values.push_back(arg_value);
		}
	
	tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);

	cog_proc_ptr->schedule_event(
			new Task_Write_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(),  arg_props, arg_values, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Write_Task_Operator::display(Output_tee& ot) const
{
	ot << "Write_task_item_whose ";
	for (GOMS_Argument_Sptr_List_t::const_iterator itr = prop_value_arg_list.begin();
			itr != prop_value_arg_list.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last pv arg?
		GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != prop_value_arg_list.end())
			ot << ", and " ;
		}
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Write_Task_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Write_Task_Operator * p = dynamic_cast<const GOMS_Write_Task_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// all property-value pairs in lists must be similar
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it1, pv_arg_it2; //to iterate through pv_arg_list
	for(pv_arg_it1 = prop_value_arg_list.begin(), pv_arg_it2 = p->prop_value_arg_list.begin();
		pv_arg_it1 != prop_value_arg_list.end() && pv_arg_it2 != p->prop_value_arg_list.end();
		pv_arg_it1++, pv_arg_it2++) {
			if (!((*pv_arg_it1)->similar(*pv_arg_it2, cm)))
				return false;
			}
	// finally, tag name must be similar				
	return WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm);
}

// write to the task description is a different kind of operator
void GOMS_Update_Task_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to Task processor.

	Symbol_list_t arg_props, arg_values; // lists of resolved property names and values
	
	Symbol_list_t::iterator prop_it, value_it;
	
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it; //to iterate through pv_arg_list
	
	Symbol arg_prop, arg_value, tag_name;
//	GOMS_PV_Argument * pv_arg_ptr;
	
	// build lists of resolved property names and property values
	for(pv_arg_it = prop_value_arg_list.begin();
		pv_arg_it != prop_value_arg_list.end();
		pv_arg_it++) {
		
			arg_prop = (*pv_arg_it) -> resolve_property_name(cog_proc_ptr);
			arg_value = (*pv_arg_it) -> resolve_property_value(cog_proc_ptr);
			arg_props.push_back(arg_prop);
			arg_values.push_back(arg_value);
		}
			
	cog_proc_ptr->schedule_event(
			new Task_Update_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				object_name_sptr->resolve(cog_proc_ptr), arg_props, arg_values)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Update_Task_Operator::display(Output_tee& ot) const
{
	ot << "Update_for_task_item ";
	object_name_sptr->display(ot);
	ot << " that ";
	for (GOMS_Argument_Sptr_List_t::const_iterator itr = prop_value_arg_list.begin();
			itr != prop_value_arg_list.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last pv arg?
		GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != prop_value_arg_list.end())
			ot << ", and " ;
		}
}

bool GOMS_Update_Task_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Update_Task_Operator * p = dynamic_cast<const GOMS_Update_Task_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// object name must be similar				
	if(!object_name_sptr->similar(p->object_name_sptr, cm))
		return false;
	// all property-value pairs in lists must be similar
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it1, pv_arg_it2; //to iterate through pv_arg_list
	for(pv_arg_it1 = prop_value_arg_list.begin(), pv_arg_it2 = p->prop_value_arg_list.begin();
		pv_arg_it1 != prop_value_arg_list.end() && pv_arg_it2 != p->prop_value_arg_list.end();
		pv_arg_it1++, pv_arg_it2++) {
			if (!((*pv_arg_it1)->similar(*pv_arg_it2, cm)))
				return false;
			}
	return true;
}


void GOMS_Erase_Task_Prop_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol arg_obj = object_name_sptr->resolve(cog_proc_ptr);
	Symbol arg_prop = property_name_sptr->resolve_property_name(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_Erase_Property_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				arg_obj, arg_prop)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Erase_Task_Prop_Operator::display(Output_tee& ot) const
{
	ot << "Erase_task_item_property ";
	object_name_sptr->display(ot);
	ot << " ";
	property_name_sptr->display(ot);
}

bool GOMS_Erase_Task_Prop_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Erase_Task_Prop_Operator * p = dynamic_cast<const GOMS_Erase_Task_Prop_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// object name and property name must be similar				
	if(!object_name_sptr->similar(p->object_name_sptr, cm) || !property_name_sptr->similar(p->property_name_sptr, cm) )
		return false;
	return true;
}

void GOMS_Erase_Task_Obj_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol arg_obj = object_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_Erase_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				arg_obj)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Erase_Task_Obj_Operator::display(Output_tee& ot) const
{
	ot << "Erase_task_item ";
	object_name_sptr->display(ot);
}

bool GOMS_Erase_Task_Obj_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Erase_Task_Obj_Operator * p = dynamic_cast<const GOMS_Erase_Task_Obj_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// object name must be similar				
	if(!object_name_sptr->similar(p->object_name_sptr, cm) )
		return false;
	return true;
}




void GOMS_Create_Task_List_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Symbol task_list_obj = task_list_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Create_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				task_list_obj)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Create_Task_List_Operator::display(Output_tee& ot) const
{
	ot << "Create_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Create_Task_List_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Create_Task_List_Operator * p = dynamic_cast<const GOMS_Create_Task_List_Operator *>(bp);
	if (!p)
		return false;
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) )
		return false;
	return true;
}


void GOMS_Erase_Task_List_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Symbol task_list_obj = task_list_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Erase_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				task_list_obj)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Erase_Task_List_Operator::display(Output_tee& ot) const
{
	ot << "Erase_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Erase_Task_List_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Erase_Task_List_Operator * p = dynamic_cast<const GOMS_Erase_Task_List_Operator *>(bp);
	if (!p)
		return false;
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) )
		return false;
	return true;
}

void GOMS_Get_Size_Of_Task_List_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Size_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Get_Size_Of_Task_List_Operator::display(Output_tee& ot) const
{
	ot << "Get_size_of_task_list ";
	task_list_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Get_Size_Of_Task_List_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Get_Size_Of_Task_List_Operator * p = dynamic_cast<const GOMS_Get_Size_Of_Task_List_Operator *>(bp);
	if (!p)
		return false;
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) )
		return false;
	return true;
}



void GOMS_Prepend_Item_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol item_name = task_list_item_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Prepend_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, item_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Prepend_Item_Operator::display(Output_tee& ot) const
{
	ot << "Prepend_item ";
	task_list_item_sptr->display(ot);
	ot << " to_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Prepend_Item_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Prepend_Item_Operator * p = dynamic_cast<const GOMS_Prepend_Item_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name and item name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!task_list_item_sptr->similar(p->task_list_item_sptr, cm) )
		return false;
	return true;
}

void GOMS_Append_Item_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol item_name = task_list_item_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Append_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, item_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Append_Item_Operator::display(Output_tee& ot) const
{
	ot << "Append_item ";
	task_list_item_sptr->display(ot);
	ot << " to_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Append_Item_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Append_Item_Operator * p = dynamic_cast<const GOMS_Append_Item_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name and item name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!task_list_item_sptr->similar(p->task_list_item_sptr, cm) )
		return false;
	return true;
}

void GOMS_Insert_Before_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol item_name = task_list_item_sptr->resolve(cog_proc_ptr);
	Symbol loc_item_name = task_list_location_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Insert_Before_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, item_name, loc_item_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Insert_Before_Operator::display(Output_tee& ot) const
{
	ot << "Insert_item ";
	task_list_item_sptr->display(ot);
	ot << "before ";
	task_list_location_sptr->display(ot);
	ot << " in_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Insert_Before_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Insert_Before_Operator * p = dynamic_cast<const GOMS_Insert_Before_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name, item name. and location name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm)  ||
		!task_list_item_sptr->similar(p->task_list_item_sptr, cm) ||
		!task_list_location_sptr->similar(p->task_list_location_sptr, cm) )
		return false;
	return true;
}


void GOMS_Find_Task_Item_Whose_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to Task processor.

	Symbol_list_t arg_props, arg_values; // lists of resolved property names and values
	
	Symbol_list_t::iterator prop_it, value_it;
	
	GOMS_Argument_Sptr_List_t::const_iterator pv_arg_it; //to iterate through pv_arg_list
	
	Symbol arg_prop, arg_value, tag_name;
//	GOMS_PV_Argument * pv_arg_ptr;
	
	// build lists of resolved property names and property values
	for(pv_arg_it = prop_value_arg_list.begin();
		pv_arg_it != prop_value_arg_list.end();
		pv_arg_it++) {
		
			arg_prop = (*pv_arg_it) -> resolve_property_name(cog_proc_ptr);
			arg_value = (*pv_arg_it) -> resolve_property_value(cog_proc_ptr);
			arg_props.push_back(arg_prop);
			arg_values.push_back(arg_value);
		}
		
	Symbol task_list_obj = task_list_name_sptr->resolve(cog_proc_ptr);
	tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
		cog_proc_ptr->schedule_event(
			new Task_List_Find_Item_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				task_list_obj, arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Find_Task_Item_Whose_Operator::display(Output_tee& ot) const
{
	ot << "Find_item_whose ";
	for (GOMS_Argument_Sptr_List_t::const_iterator itr = prop_value_arg_list.begin();
			itr != prop_value_arg_list.end(); itr++) {
		(*itr) ->display(ot);
		// is this the last pv arg?
		GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
		itr2++;
		if (itr2 != prop_value_arg_list.end())
			ot << ", and " ;
		}
	ot << " in_task_list ";
	task_list_name_sptr->display(ot);	
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Find_Task_Item_Whose_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Find_Task_Item_Whose_Operator * p = dynamic_cast<const GOMS_Find_Task_Item_Whose_Operator *>(bp);
	if (!p)
		return false;
	if (!GOMS_Memory_Access_Operator::similar(bp, cm))
		return false;
	return task_list_name_sptr->similar(p->task_list_name_sptr, cm);
}

void GOMS_Find_Item_Before_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol loc_item_name = task_item_name_sptr->resolve(cog_proc_ptr);
	Symbol tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Find_Before_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				loc_item_name, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Find_Item_Before_Operator::display(Output_tee& ot) const
{
	ot << "Find_item_before ";
	task_item_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Find_Item_Before_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Find_Item_Before_Operator * p = dynamic_cast<const GOMS_Find_Item_Before_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name, item name. and location name must be similar				
	if(!WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) ||
		!task_item_name_sptr->similar(p->task_item_name_sptr, cm) )
		return false;
	return true;
}

void GOMS_Find_Item_After_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol loc_item_name = task_item_name_sptr->resolve(cog_proc_ptr);
	Symbol tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Find_After_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				loc_item_name, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Find_Item_After_Operator::display(Output_tee& ot) const
{
	ot << "Find_item_after ";
	task_item_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Find_Item_After_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Find_Item_After_Operator * p = dynamic_cast<const GOMS_Find_Item_After_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name, item name. and location name must be similar				
	if(!WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) ||
		!task_item_name_sptr->similar(p->task_item_name_sptr, cm) )
		return false;
	return true;
}


void GOMS_Find_First_Item_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Find_First_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Find_First_Item_Operator::display(Output_tee& ot) const
{
	ot << "Find_first_item_in_task_list ";
	task_list_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Find_First_Item_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Find_First_Item_Operator * p = dynamic_cast<const GOMS_Find_First_Item_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name and tag name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) )
		return false;
	return true;
}


void GOMS_Find_Last_Item_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Find_Last_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, tag_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Find_Last_Item_Operator::display(Output_tee& ot) const
{
	ot << "Find_last_item_from_task_list ";
	task_list_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Find_Last_Item_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Find_Last_Item_Operator * p = dynamic_cast<const GOMS_Find_Last_Item_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name and tag name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) )
		return false;
	return true;
}


void GOMS_Remove_Item_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol list_name = task_list_name_sptr->resolve(cog_proc_ptr);
	Symbol item_name = task_list_item_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Remove_Item_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				list_name, item_name)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Remove_Item_Operator::display(Output_tee& ot) const
{
	ot << "Remove_item ";
	task_list_item_sptr->display(ot);
	ot << " from_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Remove_Item_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Remove_Item_Operator * p = dynamic_cast<const GOMS_Remove_Item_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// list name and item name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) ||
		!task_list_item_sptr->similar(p->task_list_item_sptr, cm) )
		return false;
	return true;
}


void GOMS_Remove_First_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol task_list_obj = task_list_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Remove_First_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				task_list_obj)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Remove_First_Operator::display(Output_tee& ot) const
{
	ot << "Remove_first_item_from_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Remove_First_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Remove_First_Operator * p = dynamic_cast<const GOMS_Remove_First_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// object name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) )
		return false;
	return true;
}



void GOMS_Remove_Last_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	// resolve each argument
	Symbol task_list_obj = task_list_name_sptr->resolve(cog_proc_ptr);
	
	cog_proc_ptr->schedule_event(
			new Task_List_Remove_Last_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_task_ptr(), 
				task_list_obj)
			); 	
	cog_proc_ptr->set_task_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Remove_Last_Operator::display(Output_tee& ot) const
{
	ot << "Remove_last_item_from_task_list ";
	task_list_name_sptr->display(ot);
}

bool GOMS_Remove_Last_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Remove_Last_Operator * p = dynamic_cast<const GOMS_Remove_Last_Operator *>(bp);
	// not similar if other is not the same type
	if (!p)
		return false;
	// object name must be similar				
	if(!task_list_name_sptr->similar(p->task_list_name_sptr, cm) )
		return false;
	return true;
}

