#include "Visual_event_types.h"
#include "Visual_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void Visual_Input_Object_Appearance_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Input_Object_Change_Symbol_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

/*
void Visual_Input_Object_Change_Spatial_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
*/

void Visual_Input_Object_Disappearance_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Event_Type_Erasure_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Input_Erasure_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Get_Value_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Find_Object_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Get_Object_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Wait_Object_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Visual_Look_at_Object_event::handle_self(Visual_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
