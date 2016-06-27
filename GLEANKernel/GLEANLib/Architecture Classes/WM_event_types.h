#ifndef WM_MESSAGE_TYPES_H
#define WM_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

// forward declarations
class WM_processor;
class Object_Memory_Processor;

// All WM_Tag_events contain a tag name
class WM_Tag_event : public WM_event {
public:
	Symbol tag_name;
		
	WM_Tag_event(long t, Processor * r) : 
		WM_event(t, r) {}

	WM_Tag_event(long t, Processor * r, const Symbol& tg) : 
		WM_event(t, r), tag_name(tg) {}
	// no override of handle_self function - this is an ABC
		
};


// Store a value under a tag
class WM_Store_Tag_event : public WM_Tag_event {
public:
	Symbol tag_value;
			
	WM_Store_Tag_event(long t, Processor * r, const Symbol& tg, const Symbol& vl ) : 
		WM_Tag_event(t, r, tg), tag_value(vl) {}

	virtual void handle_self(WM_processor *) const;

};

// erase the tag and its value
class WM_Erase_Tag_event : public WM_Tag_event {
public:
		
	WM_Erase_Tag_event(long t, Processor * r, const Symbol& tg) : 
		WM_Tag_event(t, r, tg) {}
		
	virtual void handle_self(WM_processor *) const;
};

// retrieve the value stored under the tag; value returned in this event object
class WM_Retrieve_Value_Tag_event : public WM_Tag_event {
public:
	Symbol tag_value;
		
	WM_Retrieve_Value_Tag_event(long t, Processor * r, const Symbol& tg) : 
		WM_Tag_event(t, r, tg) {}

	virtual void handle_self(WM_processor *) const;
};

// base class for events that use the in-focus WM_Object store
// data members are the object name and optionally and Processor * pointer.
// WM_processor creates the WM_Objects

class WM_Object_event : public WM_event {
public:

	Symbol object_name;
			
	WM_Object_event(long t, Processor * r, const Symbol& o_n) : 
		WM_event(t, r), object_name(o_n) {}  
		
	virtual ~WM_Object_event () {}

};

// Store the supplied WM_object under the object name
class WM_Store_Object_event : public WM_Object_event {
public:

	Object_Memory_Processor * responsible_processor;
			
	WM_Store_Object_event(long t, Processor * r, const Symbol& o_n, Object_Memory_Processor * p_p) : 
		WM_Object_event(t, r, o_n), responsible_processor(p_p) {}

	virtual void handle_self(WM_processor *) const;
};

// erase the WM_object stored under the object name
class WM_Erase_Object_event : public WM_Object_event {
public:
			
	WM_Erase_Object_event(long t, Processor * r, const Symbol& o_n) : 
		WM_Object_event(t, r, o_n) {}

	virtual void handle_self(WM_processor *) const;
};

// retrieve the value for the property of the object in the object store
// returned value is stored in this event
class WM_Retrieve_Value_Object_event : public WM_Object_event {
public:

	Symbol property_name;
	Symbol property_value;
			
	WM_Retrieve_Value_Object_event(long t, Processor * r, const Symbol& o_n, const Symbol& p_n) : 
		WM_Object_event(t, r, o_n), property_name (p_n) {}
	virtual void handle_self(WM_processor *) const;
};


class WM_Display_Request_event : public WM_event {
public:

	WM_Display_Request_event(long t, Processor * r) : 
		WM_event(t, r) {}

	virtual void handle_self(WM_processor *) const;
private:
	WM_Display_Request_event();

};

#endif
