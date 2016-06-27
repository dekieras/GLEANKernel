//#include <typeinfo>
#include "GOMS_Model.h"
#include "GOMS_Vocal_Operators.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "GOMS_Execution_Thread.h"
#include "Vocal_event_types.h"
#include "Vocal_processor.h"
#include "Output_tee.h"

//#include <DebugNew.h>
using namespace std;

// vocal operator functions	

void GOMS_Speak_Operator::execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) 
{
	Event * event_ptr = new Vocal_Speak_Command (
		cog_proc_ptr->get_step_end_time(), cog_proc_ptr->get_human_ptr()->get_vocal_ptr(), 
		word_sptr()->resolve(cog_proc_ptr));
	
	cog_proc_ptr->schedule_event(event_ptr);
	cog_proc_ptr->set_vocal_busy(thread_ptr);
	thread_ptr->new_operation_pending();
}

void GOMS_Speak_Operator::display(Output_tee& ot) const
{
	ot << "Speak ";
	word_sptr()->display(ot);
}

bool GOMS_Speak_Operator::similar(const GOMS_Operator * bp, const Correspondence_map_t& cm) const
{
	if (const GOMS_Speak_Operator * p = dynamic_cast<const GOMS_Speak_Operator *>(bp)) {
		// similar if specifications are similar
		return specification_sptr->similar(p->specification_sptr, cm);
		}
	return false;
}

void GOMS_Speak_Operator::request_processors(Processor_Request& pr) const
{
	pr.request_vocal();
}
