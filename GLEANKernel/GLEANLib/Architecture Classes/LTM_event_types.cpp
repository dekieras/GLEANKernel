#include "LTM_event_types.h"
#include "LTM_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void LTM_Recall_Value_event::handle_self(LTM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void LTM_Recall_Object_event::handle_self(LTM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
