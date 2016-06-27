#include "Vocal_event_types.h"
#include "Vocal_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void Vocal_Speak_Command::handle_self(Vocal_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

