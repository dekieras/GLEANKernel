#ifndef MANUAL_MESSAGE_TYPES_H
#define MANUAL_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

// forward declarations
class Manual_processor;

class Manual_Keystroke_Command : public Manual_event {
public:
	
	Manual_Keystroke_Command(long t, Processor * r, const Symbol key_name) :
		Manual_event(t, r, key_name) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Type_in_Command : public Manual_event {
public:
	Manual_Type_in_Command(long t, Processor * r, const Symbol type_in_string) :
		Manual_event(t, r, type_in_string) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Home_Command : public Manual_event {
public:
	Manual_Home_Command(long t, Processor * r, const Symbol destination) :
		Manual_event(t, r, destination) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Hold_Command : public Manual_event {
public:
	Manual_Hold_Command(long t, Processor * r, const Symbol button_name) :
		Manual_event(t, r, button_name) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Release_Command : public Manual_event {
public:
	Manual_Release_Command(long t, Processor * r, const Symbol button_name) :
		Manual_event(t, r, button_name) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Click_Command : public Manual_event {
public:
	Manual_Click_Command(long t, Processor * r, const Symbol button_name) :
		Manual_event(t, r, button_name) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Double_Click_Command : public Manual_event {
public:
	Manual_Double_Click_Command(long t, Processor * r, const Symbol button_name) :
		Manual_event(t, r, button_name) {}
			
	virtual void handle_self(Manual_processor *) const;
};

class Manual_Point_Command : public Manual_event {
public:
	Manual_Point_Command(long t, Processor * r, const Symbol destination) :
		Manual_event(t, r, destination) {}
			
	virtual void handle_self(Manual_processor *) const;
};


#endif
