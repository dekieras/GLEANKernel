#include "High_Level_event_types.h"
#include "High_Level_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.


void High_Level_Put_Command::handle_self(High_Level_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void High_Level_Get_Command::handle_self(High_Level_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void High_Level_Input_event::handle_self(High_Level_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void High_Level_Erase_event::handle_self(High_Level_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

