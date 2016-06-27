// Event pointers are passed around. The events are const objects;
// once created, their contents cannot be altered.
// All members of Events are public for convenience in writing the many functions
// that need to access their content.
// The originator of an event calls the create function to create an event, 
// which returns a pointer to a const Event.
// Event receivers must declare parameter as const Event_type *.

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#include "Event_base.h"

#include "Symbol.h"

// forward declarations
class Processor;
class WM_processor;	
class LTM_processor;
class Task_processor;
class Eye_processor;	
class Visual_processor;	
class Visual_sensory_store;	
class Visual_perceptual_processor;	
class Visual_perceptual_store;	

class Auditory_processor;	
class Auditory_store;
class Auditory_physical_store;
class Ear_processor;	
class Auditory_sensory_store;	
class Auditory_perceptual_processor;	
class Auditory_perceptual_store;	

class Motor_processor;	
class Manual_processor;	
//class Ocular_processor;	
//class Invol_ocular_processor;	
class Vocal_processor;	

class Cognitive_processor;	

class Device_processor;	
class High_Level_processor;

// These event classes are used in all processors and are not
// base classes.

class Start_event : public Event {
public:
	Start_event(long t, Processor * d) : Event(t, d) {}		
	virtual void send_self(Processor *) const;
};

class Stop_event : public Event {
public:
	Stop_event(long t, Processor * r) : Event(t, r) {}	
	virtual void send_self(Processor *) const;
};

class Report_event : public Event {
public:
	Report_event(long t, Processor * r, long d) :
		Event(t, r), duration(d) {}
	
	long duration;
	virtual void send_self(Processor *) const;
};

class Cognitive_event : public Event {
public:
	Cognitive_event(long t, Processor * r) : Event(t, r) {}
	virtual ~Cognitive_event () {}
		
	virtual void send_self(Processor *) const;
	virtual void handle_self(Cognitive_processor *) const = 0;	// This is an abstract class
};


class Manual_event : public Event {
public:
	Symbol specification;
	
	Manual_event(long t, Processor * r, const Symbol& spc) : 
		Event(t, r), specification (spc) {}

	virtual ~Manual_event () {}
		
	virtual void send_self(Processor *) const;
	virtual void handle_self(Manual_processor *) const = 0;	// This is an abstract class
};

class Vocal_event : public Event {
public:
	Symbol specification;
	
	Vocal_event(long t, Processor * r, const Symbol& spc) : 
		Event(t, r), specification (spc) {}
		
	virtual void send_self(Processor *) const;
	virtual void handle_self(Vocal_processor *) const = 0;	// This is an abstract class
};

class WM_event : public Event {
public:
		
	WM_event(long t, Processor * r) : 
		Event(t, r) {}

	virtual ~WM_event () {}
		
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(WM_processor *) const = 0;	// This is an abstract class
};

class LTM_event : public Event {
public:
	
	LTM_event(long t, Processor * r) :
		Event(t, r) {}
		
	virtual ~LTM_event () {}
	
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(LTM_processor *) const = 0;	// This is an abstract class
};

class Task_event : public Event {
public:
	
	Task_event(long t, Processor * r) :
		Event(t, r) {}
		
	virtual ~Task_event () {}
	
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(Task_processor *) const = 0;	// This is an abstract class
};

class Visual_event : public Event {
public:
	
	Visual_event(long t, Processor * r) :
		Event(t, r) {}
		
	virtual ~Visual_event () {}
	
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(Visual_processor *) const = 0;	// This is an abstract class
};

class Auditory_event : public Event {
public:
	
	Auditory_event(long t, Processor * r) :
		Event(t, r) {}
			
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(Auditory_processor *) const = 0;	// This is an abstract class
};

class High_Level_event : public Event {
public:
	
	High_Level_event(long t, Processor * r) :
		Event(t, r) {}
			
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(High_Level_processor *) const = 0;	// This is an abstract class
};

class Device_event : public Event {
public:
	
	Device_event(long t, Processor * r) :
		Event(t, r) {}
		
	virtual ~Device_event () {}
	
	virtual void send_self(Processor *) const; 			// supports derived classes
	virtual void handle_self(Device_processor *) const = 0;		// This is an abstract class
};

#endif

