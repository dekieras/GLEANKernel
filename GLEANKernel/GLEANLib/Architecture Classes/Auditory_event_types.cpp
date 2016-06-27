#include "Auditory_event_types.h"
#include "Auditory_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.


void Auditory_Get_Value_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Auditory_Get_Object_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Auditory_Wait_Object_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Auditory_Input_Symbol_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Auditory_Input_Decay_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Auditory_Input_Spatial_event::handle_self(Auditory_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
