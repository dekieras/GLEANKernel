#ifndef VISUAL_MESSAGE_TYPES_H
#define VISUAL_MESSAGE_TYPES_H

#include "Event_types.h"
#include "Symbol.h"

//forward declarations
class Visual_processor;

// Used to signal that a new object is appearing.
class Visual_Input_Object_Appearance_event : public Visual_event {
public:
	Symbol object_name;
		
	Visual_Input_Object_Appearance_event(long t, Processor * r, const Symbol& o) : 
		Visual_event(t, r), object_name(o) {}

	virtual ~Visual_Input_Object_Appearance_event () {}
		
	virtual void handle_self(Visual_processor *) const;	
};

// Used to supply external input: value replaces value under property_name of object_name
// object is assumed to already exist
class Visual_Input_Object_Change_Symbol_event : public Visual_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol property_value;
		
	Visual_Input_Object_Change_Symbol_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& v) : 
		Visual_event(t, r), object_name(o), property_name(p), property_value(v) {}

	virtual ~Visual_Input_Object_Change_Symbol_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};

// Used to supply external input: value replaces value under property_name of object_name
// object is assumed to already exist
/*
class Visual_Input_Object_Change_Spatial_event : public Visual_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol property_value;

		
	Visual_Input_Object_Change_Spatial_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& v) : 
		Visual_event(t, r), object_name(o), property_name(p), property_value(v) {}

	virtual ~Visual_Input_Object_Change_Spatial_event () {}
		
	virtual void handle_self(Visual_processor *) const;

private:
	Visual_Input_Object_Change_Spatial_event();
};
*/

// Used to signal that an object is disappearing.
class Visual_Input_Object_Disappearance_event : public Visual_event {
public:
	Symbol object_name;
		
	Visual_Input_Object_Disappearance_event(long t, Processor * r, const Symbol& o) : 
		Visual_event(t, r), object_name(o) {}

	virtual ~Visual_Input_Object_Disappearance_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};


// Used to specify disappearance of new object status indicator
class Visual_Event_Type_Erasure_event : public Visual_event {
public:
	Symbol object_name;
		
	Visual_Event_Type_Erasure_event(long t, Processor * r, const Symbol& o) : 
		Visual_event(t, r), object_name(o) {}
		
	virtual void handle_self(Visual_processor *) const;
};

// Used to specify final erasure of external input object_name
class Visual_Input_Erasure_event : public Visual_event {
public:
	Symbol object_name;
		
	Visual_Input_Erasure_event(long t, Processor * r, const Symbol& o) : 
		Visual_event(t, r), object_name(o) {}
		
	virtual void handle_self(Visual_processor *) const;
};


class Visual_Get_Value_event : public Visual_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol WM_property_name;
		
	Visual_Get_Value_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& tg) : 
		Visual_event(t, r), object_name(o), property_name(p), WM_property_name(tg) {}

	virtual ~Visual_Get_Value_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};



// This event is a request to Visual processor to identify the object that matches on the property
// and value pairs, and then request that a WM_Object be stored under the tag_name.
// This action is used for a full, time-consuming, visual search.
class Visual_Find_Object_event : public Visual_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Visual_Find_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Visual_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Visual_Find_Object_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};

// This event is a request to Visual processor to identify the object that matches on the property
// and value pairs, and then request that a WM_Object be stored under the tag_name.
// This action is used only to access visual properties of objects that might be currently present,
// such as a newly-appeared object, and not when a search over the screen is required.
// I.e. This is for emulating attention-getting by some class of visual events.
class Visual_Get_Object_event : public Visual_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Visual_Get_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Visual_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Visual_Get_Object_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};

// This event is a request to Visual processor to try to identify the object 
// that matches on the property and value pairs. If not present, the requestor waits
// until it is. Then a WM_Object is stored under the tag_name.
class Visual_Wait_Object_event : public Visual_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Visual_Wait_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Visual_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Visual_Wait_Object_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};

// This event is a request to Visual processor to "refind" an object 
// given its name and make it the in-focus object. 
// If not present, the infocus object is changed to absent
class Visual_Look_at_Object_event : public Visual_event {
public:
	Symbol object_name;
	Symbol WM_tag_name;
		
	Visual_Look_at_Object_event(long t, Processor * r, const Symbol& o, const Symbol& tg) : 
		Visual_event(t, r), object_name(o), WM_tag_name(tg) {}

	virtual ~Visual_Look_at_Object_event () {}
		
	virtual void handle_self(Visual_processor *) const;
};

#endif

