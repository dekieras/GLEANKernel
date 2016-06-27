#include "Cognitive_event_types.h"
#include "Cognitive_processor.h"

// These definitions must be in a separate file from the event types declaration
// to avoid circular definitions.

void Cognitive_Step_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Stop_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Visual_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Auditory_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Manual_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Vocal_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Task_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_High_Level_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_LTM_Operation_Complete_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Delay_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

/*
void Cognitive_Accomplish_Goal_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Return_Goal_Accomplished_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}

void Cognitive_Abort_and_Restart_event::handle_self(Cognitive_processor * proc_ptr) const
{		
	proc_ptr->handle_event(this);
}
*/

