#ifndef LTM_MESSAGE_TYPES_H
#define LTM_MESSAGE_TYPES_H


#include "Event_types.h"
#include "Symbol.h"

//forward declarations
class LTM_processor;


class LTM_Recall_Value_event : public LTM_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol WM_property_name;
		
	LTM_Recall_Value_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& tg) : 
		LTM_event(t, r), object_name(o), property_name(p), WM_property_name(tg) {}
		
	virtual void handle_self(LTM_processor *) const;
};



// This event is a request to LTM to identify the object that matches on the property
// and value pairs, and then request that a WM_Object be stored under the tag_name.
class LTM_Recall_Object_event : public LTM_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	LTM_Recall_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		LTM_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}
		
	virtual void handle_self(LTM_processor *) const;
};






#endif
