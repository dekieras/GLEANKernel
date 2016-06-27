#ifndef COGNITIVE_MESSAGE_TYPES_H
#define COGNITIVE_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"
#include "GOMS_Model.h"

// forward declarations
class Cognitive_processor;
class GOMS_Execution_Thread;

class Cognitive_Step_event : public Cognitive_event {
public:
	Cognitive_Step_event(long t, Processor * r) : Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Stop_event : public Cognitive_event {
public:
	Cognitive_Stop_event(long t, Processor * r) : Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};


class Cognitive_Operation_Complete_event : public Cognitive_event {
public:
	GOMS_Execution_Thread * thread_ptr;
	Cognitive_Operation_Complete_event(long t, Processor * r, GOMS_Execution_Thread * gt) : 
		Cognitive_event(t, r), thread_ptr(gt) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Visual_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_Visual_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Auditory_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_Auditory_Operation_Complete_event(long t, Processor * r) :
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Manual_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_Manual_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Vocal_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_Vocal_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_Task_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_Task_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_High_Level_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_High_Level_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};

class Cognitive_LTM_Operation_Complete_event : public Cognitive_event {
public:
	Cognitive_LTM_Operation_Complete_event(long t, Processor * r) : 
		Cognitive_event(t, r) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};


class Cognitive_Delay_event : public Cognitive_event {
public:
	long delay;
	GOMS_Execution_Thread * thread_ptr;
	
	Cognitive_Delay_event(long t, Processor * r, long dly, GOMS_Execution_Thread * gt = 0) : 
		Cognitive_event(t, r), delay(dly), thread_ptr(gt) {}
		
	virtual void handle_self(Cognitive_processor *) const;
};


class Cognitive_Accomplish_Goal_event : public Cognitive_event {
public:
	GOMS_Goal goal;
	GOMS_Execution_Thread * thread_ptr;
	
	Cognitive_Accomplish_Goal_event(long t, Processor * r, GOMS_Execution_Thread * gt, const GOMS_Goal& g) : 
		Cognitive_event(t, r), goal(g), thread_ptr(gt) {}
		
	virtual void handle_self(Cognitive_processor *) const {}
};

class Cognitive_Return_Goal_Accomplished_event : public Cognitive_event {
public:
	GOMS_Goal goal;
	GOMS_Execution_Thread * thread_ptr;
	
	Cognitive_Return_Goal_Accomplished_event(long t, Processor * r, GOMS_Execution_Thread * gt, const GOMS_Goal& g) : 
		Cognitive_event(t, r), goal(g), thread_ptr(gt) {}
		
	virtual void handle_self(Cognitive_processor *) const {}
};

class Cognitive_Abort_and_Restart_event : public Cognitive_event {
public:
	GOMS_Goal goal;
	
	Cognitive_Abort_and_Restart_event(long t, Processor * r, const GOMS_Goal& g) : 
		Cognitive_event(t, r), goal(g) {}
		
	virtual void handle_self(Cognitive_processor *) const {}
};


#endif
