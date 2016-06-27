#ifndef AUDITORY_MESSAGE_TYPES_H
#define AUDITORY_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

//forward declarations
class Auditory_processor;

// Used to supply external input: value replaces value under property_name of object_name
class Auditory_Input_Symbol_event : public Auditory_event {
public:
		
	Auditory_Input_Symbol_event(long t, Processor * r, const Symbol& i_s) : 
		Auditory_event(t, r), input_symbol(i_s) {}
		
	virtual void handle_self(Auditory_processor *) const;
	
	const Symbol& get_input_symbol() const {return input_symbol;}

private:
	const Symbol input_symbol;
};

// Used to specify decay of external input object_name
class Auditory_Input_Decay_event : public Auditory_event {
public:
	Symbol object_name;
		
	Auditory_Input_Decay_event(long t, Processor * r, const Symbol& o) : 
		Auditory_event(t, r), object_name(o) {}
		
	virtual void handle_self(Auditory_processor *) const;
};

// Used to supply external input: value replaces value under property_name of object_name
class Auditory_Input_Spatial_event : public Auditory_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol property_value;	// should store a pair of doubles
		
	Auditory_Input_Spatial_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& v) : 
		Auditory_event(t, r), object_name(o), property_name(p), property_value(v) {}
		
	virtual void handle_self(Auditory_processor *) const;
};

class Auditory_Get_Value_event : public Auditory_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol WM_property_name;
		
	Auditory_Get_Value_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& tg) : 
		Auditory_event(t, r), object_name(o), property_name(p), WM_property_name(tg) {}
		
	virtual void handle_self(Auditory_processor *) const;
};



// This event is a request to Visual processor to identify the object that matches on the property
// and value pairs, and then request that a WM_Object be stored under the tag_name.
class Auditory_Get_Object_event : public Auditory_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Auditory_Get_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Auditory_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}
		
	virtual void handle_self(Auditory_processor *) const;
};

// This event is a request to Visual processor to try to identify the object 
// that matches on the property and value pairs. If not present, the requestor waits
// until it is. Then a WM_Object is stored under the tag_name.
class Auditory_Wait_Object_event : public Auditory_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Auditory_Wait_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Auditory_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}
		
	virtual void handle_self(Auditory_processor *) const;
};

#endif
