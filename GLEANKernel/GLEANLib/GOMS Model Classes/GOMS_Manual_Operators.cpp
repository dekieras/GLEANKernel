#include "GOMS_Manual_Operators.h"
//#include <typeinfo>
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Manual_event_types.h"
#include "Manual_processor.h"
#include "Glean_standard_symbols.h"
#include "Output_tee.h"

//#include <DebugNew.h>
using namespace std;

// manual operator functions	

bool GOMS_Manual_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Manual_Operator * p = dynamic_cast<const GOMS_Manual_Operator *>(bp)) {
		// similar if specifications are similar
		return specification_sptr->similar(p->specification_sptr, cm);
		}
	return false;
}

void GOMS_Manual_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_manual();
}

void GOMS_Manual_Operator::finish_operation(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr, Event * event_ptr)
{
	cog_proc_ptr->schedule_event(event_ptr);
	cog_proc_ptr->set_manual_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Keystroke_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Symbol key_name = key_name_sptr()->resolve(cog_proc_ptr);
	if(get_faulted() || key_name == "XXX" /* testing purposes only */) {
		cog_proc_ptr->raise_exception(thread_ptr, Symbol("Keystroke_Error"));
		return;
		}

	Event * event_ptr = new Manual_Keystroke_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		key_name_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Keystroke_Operator::display(Output_tee& ot) const
{
	ot << "Keystroke ";
	key_name_sptr()->display(ot);
}

void GOMS_Type_In_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Manual_Type_in_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		type_in_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Type_In_Operator::display(Output_tee& ot) const
{
	ot << "Type_in ";
	type_in_sptr()->display(ot);
}

void GOMS_Home_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Manual_Home_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		dest_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Home_Operator::display(Output_tee& ot) const
{
	ot << "Home ";
	dest_sptr()->display(ot);
}

void GOMS_Hold_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Manual_Hold_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		button_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Hold_Operator::display(Output_tee& ot) const
{
	ot << "Hold_down ";
	button_sptr()->display(ot);
}

void GOMS_Release_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Manual_Release_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		button_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Release_Operator::display(Output_tee& ot) const
{
	ot << "Release  ";
	button_sptr()->display(ot);
}

void GOMS_Click_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	if(get_faulted()) {
		cog_proc_ptr->raise_exception(thread_ptr, Symbol("Click_Error"));
		return;
		}
	Event * event_ptr = new Manual_Click_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		button_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Click_Operator::display(Output_tee& ot) const
{
	ot << "Click ";
	button_sptr()->display(ot);
}

void GOMS_Double_Click_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Manual_Double_Click_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		button_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Double_Click_Operator::display(Output_tee& ot) const
{
	ot << "Double_Click ";
	button_sptr()->display(ot);
}

void GOMS_Point_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	if(get_faulted() || dest_sptr()->resolve(cog_proc_ptr) == Absent_c) {
			cog_proc_ptr->raise_exception(thread_ptr, Symbol("Point_Error"));
			return;
			}

	Event * event_ptr = new Manual_Point_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_manual_ptr(), 
		dest_sptr()->resolve(cog_proc_ptr));
	
	finish_operation(cog_proc_ptr, thread_ptr, event_ptr);
}

void GOMS_Point_Operator::display(Output_tee& ot) const
{
	ot << "Point_to ";
	dest_sptr()->display(ot);
}
