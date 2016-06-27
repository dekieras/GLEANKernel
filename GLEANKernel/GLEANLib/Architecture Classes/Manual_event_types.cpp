#include "Manual_event_types.h"
#include "Manual_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void Manual_Keystroke_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Type_in_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Home_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Hold_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Release_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Click_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Double_Click_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Manual_Point_Command::handle_self(Manual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

