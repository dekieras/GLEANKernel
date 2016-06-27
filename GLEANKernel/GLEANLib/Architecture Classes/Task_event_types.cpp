#include "Task_event_types.h"
#include "Task_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.


void Task_Get_Value_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_Get_Object_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_Write_Object_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_Update_Object_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_Erase_Property_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_Erase_Object_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}


void Task_List_Create_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Erase_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Size_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Prepend_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Append_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Insert_Before_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Find_Item_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Find_First_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Find_Last_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Find_Before_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Find_After_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
/*
void Task_List_Write_Item_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
*/


void Task_List_Remove_Item_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Remove_First_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Task_List_Remove_Last_event::handle_self(Task_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

