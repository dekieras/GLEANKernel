#ifndef VOCAL_MESSAGE_TYPES_H
#define VOCAL_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

// forward declarations
class Vocal_processor;

class Vocal_Speak_Command : public Vocal_event {
public:
	
	Vocal_Speak_Command(long t, Processor * r, const Symbol spc) :
		Vocal_event(t, r, spc) {}
	
		
	virtual void handle_self(Vocal_processor *) const;
};


#endif
