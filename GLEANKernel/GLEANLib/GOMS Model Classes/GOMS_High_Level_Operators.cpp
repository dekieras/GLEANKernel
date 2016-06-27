#include "GOMS_High_Level_Operators.h"
#include "GOMS_Model.h"
#include "GOMS_Memory_Operators.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Event_types.h"
#include "Cognitive_event_types.h"
#include "High_Level_event_types.h"
#include "WM_event_types.h"
#include "Task_event_types.h"
#include "Task_processor.h"
#include "Device_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include <typeinfo>


//#include <DebugNew.h>
using namespace std;

void GOMS_HLGet_Device_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_high_level();
}


/* These operators support High-Level GOMS models - the device is commanded to insert
an object directly into the human's task memory, or given a abstract command.
*/

// write to the task description is a different kind of operator
void GOMS_HLGet_Device_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
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

	// at this point we have to talk to the device, it will send its result back, 
	// and get that written ...
	cog_proc_ptr->schedule_event(
			new High_Level_Get_Command (
				cog_proc_ptr->get_step_end_time(), 
				cog_proc_ptr->get_human_ptr()->get_high_level_ptr(),  arg_props, arg_values, tag_name)
			); 	
	cog_proc_ptr->set_high_level_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_HLGet_Device_Operator::display(Output_tee& ot) const
{
	ot << "Get_from_device ";
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

bool GOMS_HLGet_Device_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_HLGet_Device_Operator * p = dynamic_cast<const GOMS_HLGet_Device_Operator *>(bp);
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


void GOMS_HLPut_Device_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_high_level();
}


// send a property-value list to the device
void GOMS_HLPut_Device_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
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
			new High_Level_Put_Command (
				cog_proc_ptr->get_step_end_time(), 
				cog_proc_ptr->get_human_ptr()->get_high_level_ptr(),  arg_props, arg_values)
			); 	
	cog_proc_ptr->set_high_level_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_HLPut_Device_Operator::display(Output_tee& ot) const
{
	ot << "Put_to_device ";
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

bool GOMS_HLPut_Device_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	const GOMS_HLPut_Device_Operator * p = dynamic_cast<const GOMS_HLPut_Device_Operator *>(bp);
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
	return true;
}
