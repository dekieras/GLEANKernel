//#include <iostream>

#include "Event_types.h"
#include "Processor.h"

// These functions must be defined separately from the declarations to avoid circularities
// in the header files.


void Start_event::send_self(Processor * proc_ptr) const 
{		
	proc_ptr->accept_event(this);
}


void Stop_event::send_self(Processor * proc_ptr) const 
{		
	proc_ptr->accept_event(this);
}

void Report_event::send_self(Processor * proc_ptr) const 
{		
	proc_ptr->accept_event(this);
}

void Cognitive_event::send_self(Processor * proc_ptr) const 
{		
	proc_ptr->accept_event(this);
}

void Manual_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void Vocal_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void WM_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void LTM_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void Task_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}


void Visual_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void Auditory_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void Device_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}

void High_Level_event::send_self(Processor * proc_ptr) const
{		
	proc_ptr->accept_event(this);
}
