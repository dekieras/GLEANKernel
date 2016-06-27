#ifndef TASK_MESSAGE_TYPES_H
#define TASK_MESSAGE_TYPES_H


#include "Event_types.h"
#include "Symbol.h"

//forward declarations
class Task_processor;

class Task_Get_Value_event : public Task_event {
public:
	Symbol object_name;
	Symbol property_name;
	Symbol WM_property_name;
		
	Task_Get_Value_event(long t, Processor * r, const Symbol& o, const Symbol& p, const Symbol& tg) : 
		Task_event(t, r), object_name(o), property_name(p), WM_property_name(tg) {}

	virtual ~Task_Get_Value_event () {}
		
	virtual void handle_self(Task_processor *) const;
};



// This event is a request to the Task processor to identify the object that matches on the property
// and value pairs, and then request that a WM_Object be stored under the tag_name.
class Task_Get_Object_event : public Task_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Task_Get_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, 
		const Symbol& tg) : 
		Task_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Task_Get_Object_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

// This event is a request to the Task processor to store the property-value pairs for a new object
class Task_Write_Object_event : public Task_event {
public:
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Task_Write_Object_event(long t, Processor * r, const Symbol_list_t& pl, const Symbol_list_t& vl, const Symbol& tg) : 
		Task_event(t, r), property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Task_Write_Object_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

// This event is a request to the Task processor to store the property-value pairs for the object
class Task_Update_Object_event : public Task_event {
public:
	Symbol object_name;
	Symbol_list_t property_names;
	Symbol_list_t property_values;
		
	Task_Update_Object_event(long t, Processor * r, const Symbol& o, const Symbol_list_t& pl, const Symbol_list_t& vl) : 
		Task_event(t, r), object_name(o), property_names(pl), property_values(vl) {}

	virtual ~Task_Update_Object_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

// This event is a request to the Task processor to erase the property for the object
class Task_Erase_Property_event : public Task_event {
public:
	Symbol object_name;
	Symbol property_name;
		
	Task_Erase_Property_event(long t, Processor * r, const Symbol& o, const Symbol& p) : 
		Task_event(t, r), object_name(o), property_name(p) {}

	virtual ~Task_Erase_Property_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

// This event is a request to the Task processor to erase the object
class Task_Erase_Object_event : public Task_event {
public:
	Symbol object_name;
		
	Task_Erase_Object_event(long t, Processor * r, const Symbol& o) : 
		Task_event(t, r), object_name(o) {}

	virtual ~Task_Erase_Object_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

/* Task List Events */

class Task_List_Create_event : public Task_event {
public:
	Symbol task_list_name;
		
	Task_List_Create_event(long t, Processor * r, const Symbol& tln) : 
		Task_event(t, r), task_list_name(tln) {}

	virtual ~Task_List_Create_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Erase_event : public Task_event {
public:
	Symbol task_list_name;
		
	Task_List_Erase_event(long t, Processor * r, const Symbol& tln) : 
		Task_event(t, r), task_list_name(tln) {}

	virtual ~Task_List_Erase_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Size_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol WM_tag_name;
		
	Task_List_Size_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& tg) : 
		Task_event(t, r), task_list_name(tln), WM_tag_name(tg) {}

	virtual ~Task_List_Size_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Prepend_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol task_item_name;
		
	Task_List_Prepend_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& itemn) : 
		Task_event(t, r), task_list_name(tln), task_item_name(itemn)
		{}

	virtual ~Task_List_Prepend_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Append_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol task_item_name;
		
	Task_List_Append_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& itemn) : 
		Task_event(t, r), task_list_name(tln), task_item_name(itemn)
		{}

	virtual ~Task_List_Append_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Insert_Before_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol task_item_name;
	Symbol loc_item_name;
		
	Task_List_Insert_Before_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& itemn, const Symbol& tlocn) : 
		Task_event(t, r), task_list_name(tln), task_item_name(itemn), loc_item_name(tlocn)
		{}

	virtual ~Task_List_Insert_Before_event () {}
		
	virtual void handle_self(Task_processor *) const;
};


class Task_List_Find_Item_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol_list_t property_names;
	Symbol_list_t property_values;
	Symbol WM_tag_name;
		
	Task_List_Find_Item_event(long t, Processor * r, const Symbol& tln, 
			const Symbol_list_t& pl, const Symbol_list_t& vl, const Symbol& tg) : 
		Task_event(t, r), task_list_name(tln), 
		property_names(pl), property_values(vl), WM_tag_name(tg) {}

	virtual ~Task_List_Find_Item_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Find_Before_event : public Task_event {
public:
	Symbol loc_item_name;
	Symbol WM_tag_name;
		
	Task_List_Find_Before_event(long t, Processor * r,
			 const Symbol& tlocn, const Symbol& tg) : 
		Task_event(t, r), loc_item_name(tlocn), WM_tag_name(tg) {}

	virtual ~Task_List_Find_Before_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Find_After_event : public Task_event {
public:
	Symbol loc_item_name;
	Symbol WM_tag_name;
		
	Task_List_Find_After_event(long t, Processor * r,
			 const Symbol& tlocn, const Symbol& tg) : 
		Task_event(t, r), loc_item_name(tlocn), WM_tag_name(tg) {}

	virtual ~Task_List_Find_After_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Find_First_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol WM_tag_name;
		
	Task_List_Find_First_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& tg) : 
		Task_event(t, r), task_list_name(tln), WM_tag_name(tg) {}

	virtual ~Task_List_Find_First_event () {}
		
	virtual void handle_self(Task_processor *) const;
};


class Task_List_Find_Last_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol WM_tag_name;
		
	Task_List_Find_Last_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& tg) : 
		Task_event(t, r), task_list_name(tln), WM_tag_name(tg) {}

	virtual ~Task_List_Find_Last_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

//need
class Task_List_Remove_Item_event : public Task_event {
public:
	Symbol task_list_name;
	Symbol task_item_name;
		
	Task_List_Remove_Item_event(long t, Processor * r, const Symbol& tln, 
			 const Symbol& itemn) : 
		Task_event(t, r), task_list_name(tln), task_item_name(itemn)
		{}

	virtual ~Task_List_Remove_Item_event () {}
		
	virtual void handle_self(Task_processor *) const;
};


class Task_List_Remove_First_event : public Task_event {
public:
	Symbol task_list_name;
		
	Task_List_Remove_First_event(long t, Processor * r, const Symbol& tln) : 
		Task_event(t, r), task_list_name(tln) {}

	virtual ~Task_List_Remove_First_event () {}
		
	virtual void handle_self(Task_processor *) const;
};

class Task_List_Remove_Last_event : public Task_event {
public:
	Symbol task_list_name;
		
	Task_List_Remove_Last_event(long t, Processor * r, const Symbol& tln) : 
		Task_event(t, r), task_list_name(tln) {}

	virtual ~Task_List_Remove_Last_event () {}
		
	virtual void handle_self(Task_processor *) const;
};



#endif
