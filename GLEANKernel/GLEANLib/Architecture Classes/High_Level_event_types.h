#ifndef HIGH_LEVEL_MESSAGE_TYPES_H
#define HIGH_LEVEL_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

//forward declarations
class High_Level_processor;

// GOMS operator commands high-level output to the device
class High_Level_Put_Command : public High_Level_event {
public:
	Symbol_list_t props;
	Symbol_list_t values;
		
	High_Level_Put_Command(long t, Processor * r, 
			const Symbol_list_t & ps, const Symbol_list_t& vs) : 
		High_Level_event(t, r), props(ps), values(vs) {}
		
	virtual void handle_self(High_Level_processor *) const;
};

// GOMS operator commands high-level input from the device
class High_Level_Get_Command : public High_Level_event {
public:
	Symbol_list_t props;
	Symbol_list_t values;
	Symbol tag;
		
	High_Level_Get_Command(long t, Processor * r, 
			const Symbol_list_t & ps, const Symbol_list_t& vs, const Symbol& tg) : 
		High_Level_event(t, r), props(ps), values(vs), tag(tg) {}
		
	virtual void handle_self(High_Level_processor *) const;
};

// device supplies information to be added to the object store
class High_Level_Input_event : public High_Level_event {
public:
	Symbol object_name;
	Symbol_list_t props;
	Symbol_list_t values;
	Symbol tag;
		
	High_Level_Input_event(long t, Processor * r, const Symbol& obj,
			const Symbol_list_t & ps, const Symbol_list_t& vs, const Symbol& tg) : 
		High_Level_event(t, r), object_name(obj), props(ps), values(vs), tag(tg) {}
		
	virtual void handle_self(High_Level_processor *) const;	
};

class High_Level_Erase_event : public High_Level_event {
public:
	Symbol object_name;
		
	High_Level_Erase_event(long t, Processor * r, const Symbol& obj) : 
		High_Level_event(t, r), object_name(obj) {}
		
	virtual void handle_self(High_Level_processor *) const;
};

#endif
