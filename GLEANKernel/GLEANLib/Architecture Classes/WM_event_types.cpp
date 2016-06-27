//#include <iostream>

#include "WM_event_types.h"
#include "WM_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void WM_Store_Tag_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Erase_Tag_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Retrieve_Value_Tag_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Store_Object_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Erase_Object_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Retrieve_Value_Object_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void WM_Display_Request_event::handle_self(WM_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
