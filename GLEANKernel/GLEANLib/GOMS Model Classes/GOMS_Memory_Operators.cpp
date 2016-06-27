#include <typeinfo>
#include "GOMS_Model.h"
#include "GOMS_Memory_Operators.h"
#include "Human_processor.h"
#include "Cognitive_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Event_types.h"
#include "Cognitive_event_types.h"
#include "WM_event_types.h"
#include "LTM_event_types.h"
#include "LTM_processor.h"
#include "Task_event_types.h"
#include "Task_processor.h"
#include "Visual_event_types.h"
#include "Visual_processor.h"
#include "Auditory_event_types.h"
#include "Auditory_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

//#include <DebugNew.h>
using namespace std;


// Argument functions
Symbol GOMS_Constant_Argument::resolve(Cognitive_processor *) const 
{
	return arg_value;
}	

	
// look up in WM, then return
Symbol GOMS_WM_Argument::resolve(Cognitive_processor * cog_proc_ptr) const
{
	WM_Retrieve_Value_Tag_event msg (
		cog_proc_ptr->get_time(), cog_proc_ptr->get_human_ptr()->get_wm_ptr(),  tag_name);
	cog_proc_ptr->call_with_event(msg);
	// desired result is now in the event.
	return msg.tag_value;
}

// Resolve each term in the "x of y" construct, then access the property of the object
Symbol GOMS_PO_Argument::resolve(Cognitive_processor * cog_proc_ptr) const
{ 
	Symbol prop_name = prop_name_sp->resolve(cog_proc_ptr);
	Symbol obj_name = obj_name_sp->resolve(cog_proc_ptr);
	WM_Retrieve_Value_Object_event msg (
		cog_proc_ptr->get_time(), cog_proc_ptr->get_human_ptr()->get_wm_ptr(),  obj_name, prop_name);
	cog_proc_ptr->call_with_event(msg);
	// desired result is now in the event.
	return msg.property_value;
}

// memory operator functions

void GOMS_Store_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
		
	WM_Store_Tag_event msg (
		cog_proc_ptr->get_step_end_time(),  cog_proc_ptr->get_human_ptr()->get_wm_ptr(), 
		WM_tag_name_sptr->get_symbol(), property_value_sptr->resolve(cog_proc_ptr));
	cog_proc_ptr->call_with_event(msg);
}

void GOMS_Store_Operator::display(Output_tee& ot) const
{
	ot << "Store ";
	property_value_sptr->display(ot);
	ot << " under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Store_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Store_Operator * p = dynamic_cast<const GOMS_Store_Operator *>(bp)) {
		// similar if GOMS arguments for WM tag name and property_value are similar
		// arguments here are pointers to GOMS_Arguments
		return WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm) && 
			   property_value_sptr->similar(p->property_value_sptr, cm);
		}
	return false;
}

void GOMS_Delete_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
		
	WM_Erase_Tag_event msg (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_wm_ptr(), 
		WM_tag_name_sptr->get_symbol());
	cog_proc_ptr->call_with_event(msg);
}

void GOMS_Delete_Operator::display(Output_tee& ot) const
{
	ot << "Delete ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Delete_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Delete_Operator * p = dynamic_cast<const GOMS_Delete_Operator *>(bp)) {
		// similar if GOMS arguments for WM tag name are similar
		// arguments here are pointers to GOMS_Arguments
		return WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm);
		}
	return false;
}

void GOMS_Log_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread *) 
{
	// output some useful information via the cognitive processor
	Symbol_list_t values;
	for (GOMS_Argument_Sptr_List_t::const_iterator itr = arg_list.begin();
			itr != arg_list.end(); itr++) {
		values.push_back((*itr)->resolve(cog_proc_ptr));
		}
	
	cog_proc_ptr->output_log(values);
}

void GOMS_Log_Operator::display(Output_tee& ot) const
{
	ot << "Log ";
	if (!arg_list.empty()) {
		for (GOMS_Argument_Sptr_List_t::const_iterator itr = arg_list.begin();
			itr != arg_list.end(); itr++) {
			(*itr) ->display(ot);
			// is this the last argument?
			GOMS_Argument_Sptr_List_t::const_iterator itr2 = itr;
			itr2++;
			if (itr2 != arg_list.end())
				ot << ", " ;
			}
		}
}

bool GOMS_Log_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	// log steps are psychologically irrelevant - here a similarity comparison is done, but shouldn't be
	// compared in the first place.
	if (const GOMS_Log_Operator * p = dynamic_cast<const GOMS_Log_Operator *>(bp)) {
		// pseudo arguments must match
		GOMS_Argument_Sptr_List_t::const_iterator itr1, itr2;
		for (itr1 = arg_list.begin(), itr2 = p->arg_list.begin();
			itr1 != arg_list.end() && itr2 != p->arg_list.end();
			itr1++, itr2++) 
				if (!(*itr1)->similar((*itr2), cm))
					return false;
		return true;
		}
	else
		return false;
}


void GOMS_Retrieve_LTM_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
		
	cog_proc_ptr->schedule_event(
		new LTM_Recall_Value_event (
			cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_ltm_ptr(), 
			object_name_sptr->resolve(cog_proc_ptr), property_name_sptr->resolve(cog_proc_ptr), 
			WM_tag_name_sptr->get_symbol())); 	
	cog_proc_ptr->set_ltm_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Retrieve_LTM_Operator::display(Output_tee& ot) const
{
	ot << "Retrieve_from_LTM ";
	property_name_sptr->display(ot);
	ot << " of ";
	object_name_sptr->display(ot);
	ot << " and_store_under ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Retrieve_LTM_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Retrieve_LTM_Operator * p = dynamic_cast<const GOMS_Retrieve_LTM_Operator *>(bp)) {
		// similar if GOMS arguments are similar
		// arguments here are pointers to GOMS_Arguments
		return	
				object_name_sptr->similar(p->object_name_sptr, cm) &&
				property_name_sptr->similar(p->property_name_sptr, cm) &&
				WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm)
				;
		}
	return false;
}


void GOMS_Retrieve_LTM_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_ltm();
}

void GOMS_Visual_Memory_Access_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_visual();
}

// tell the visual processor to look at the object whose name is in the tag, and store
// the result back in the same tag
void GOMS_Look_at_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	cog_proc_ptr->schedule_event(
		new Visual_Look_at_Object_event (
			cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_visual_ptr(), 
			WM_tag_name_sptr->resolve(cog_proc_ptr),
			WM_tag_name_sptr->get_symbol()			
			)); 	
	cog_proc_ptr->set_visual_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Look_at_Operator::display(Output_tee& ot) const
{
	ot << "Look_at ";
	WM_tag_name_sptr->display(ot);
}

bool GOMS_Look_at_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Look_at_Operator * p = dynamic_cast<const GOMS_Look_at_Operator *>(bp)) {
		// similar if GOMS arguments are similar
		// arguments here are pointers to GOMS_Arguments
		return	
				WM_tag_name_sptr->similar(p->WM_tag_name_sptr, cm)
				;
		}
	return false;
}

void GOMS_Look_at_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_visual();
}



void GOMS_Memory_Access_Operator::display(Output_tee& ot) const
{
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

// This function computes similarity for all derived classes
bool GOMS_Memory_Access_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Memory_Access_Operator * p = dynamic_cast<const GOMS_Memory_Access_Operator *>(bp);
	// not similar if other is not in the base class and exact types are not the same
	if (!p || typeid(*this) != typeid(*p))
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


void GOMS_Recall_LTM_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to LTM.

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
		
//	tag_name = WM_tag_name_sptr->resolve(cog_proc_ptr);
	
		cog_proc_ptr->schedule_event(
			new LTM_Recall_Object_event (
				cog_proc_ptr->get_step_end_time(),  cog_proc_ptr->get_human_ptr()->get_ltm_ptr(), 
				arg_props, arg_values, WM_tag_name_sptr->get_symbol()
				)
			); 	
	cog_proc_ptr->set_ltm_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Recall_LTM_Operator::display(Output_tee& ot) const
{
	ot << "Recall_LTM_item_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}

void GOMS_Recall_LTM_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_ltm();
}


void GOMS_Look_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to the visual processor.

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
			new Visual_Find_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_visual_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_visual_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Look_Operator::display(Output_tee& ot) const
{
	ot << "Look_for_object_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}

void GOMS_Check_Visual_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to the visual processor.

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
			new Visual_Get_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_visual_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_visual_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Check_Visual_Operator::display(Output_tee& ot) const
{
	ot << "Check_for_visual_object_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}

void GOMS_Wait_Visual_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to the visual processor.

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
			new Visual_Wait_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_visual_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_visual_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Wait_Visual_Operator::display(Output_tee& ot) const
{
	ot << "Wait_for_visual_object_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}

void GOMS_Auditory_Memory_Access_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_auditory();
}

void GOMS_Check_Auditory_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to the visual processor.

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
			new Auditory_Get_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_auditory_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_auditory_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Check_Auditory_Operator::display(Output_tee& ot) const
{
	ot << "Check_for_auditory_object_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}


void GOMS_Wait_Auditory_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
// resolve each term in the prop_value_arg_list and build a list of properties and values
// and set up a request to the visual processor.

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
			new Auditory_Wait_Object_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_auditory_ptr(), 
				arg_props, arg_values, tag_name
				)
			); 	
	cog_proc_ptr->set_auditory_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Wait_Auditory_Operator::display(Output_tee& ot) const
{
	ot << "Wait_for_auditory_object_whose ";
	GOMS_Memory_Access_Operator::display(ot);
}

// time less one to align with next cognitive cycle.
const int GOMS_Mental_Operator::default_mental_operator_time = 1200 - 1;

void GOMS_Mental_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	if(Trace_out && cog_proc_ptr->get_trace())
		Trace_out << cog_proc_ptr->processor_info()
		<< " Mental:" << mental_operator_name << ' ' << documentation_argument << endl;
	cog_proc_ptr->schedule_event(
			new Cognitive_Delay_event (
				cog_proc_ptr->get_step_end_time(), cog_proc_ptr, 
				default_mental_operator_time, thread_ptr)
			); 	
	cog_proc_ptr->set_cognitive_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Mental_Operator::display(Output_tee& ot) const
{
	ot << "Mental: " << mental_operator_name << ' ' << documentation_argument;
}

bool GOMS_Mental_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Mental_Operator * p = dynamic_cast<const GOMS_Mental_Operator *>(bp)) {
		// similar if mental operator names are similar; documentation ignored
		return ::similar(mental_operator_name, p->mental_operator_name, cm);
		}
	return false;
}

void GOMS_Mental_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_cognitive();
}


