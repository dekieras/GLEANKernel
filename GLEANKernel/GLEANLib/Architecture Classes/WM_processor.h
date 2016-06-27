#ifndef WM_PROCESSOR_H
#define WM_PROCESSOR_H

#include <set>
#include "Smart_Pointer.h"
#include "Processor.h"
#include "Object_Memory_Processor.h"
#include "WM_event_types.h"
#include "OPV_Containers.h"
#include "Symbol.h"


// WM_Objects contain the name of the object and a pointer to the processor
// responsible for them. They are simple objects, with default destructor,
// and default copy semantics.

struct WM_Object
{
	WM_Object(const Symbol& o_n, Object_Memory_Processor * proc_ptr) : 
		object_name(o_n), responsible_processor(proc_ptr) {}	

	Symbol object_name;
	Object_Memory_Processor * responsible_processor;
	
	bool operator< (const WM_Object& rhs) const
		{return object_name < rhs.object_name;}
};

/*
// a function object to compare PV_Pairs pointed to by Smart_Pointers
struct less_WM_Object
{
	bool operator() (const WM_Object& x, const WM_Object& y) const
	{ return  x.object_name < y.object_name; }
};
*/

// a container type for WM_Objects
//typedef std::set<WM_Object, less_WM_Object> WM_Object_set_t;
typedef std::set<WM_Object> WM_Object_set_t;


class WM_processor : public Human_subprocessor{
public:
	WM_processor(Human_processor * human_ptr) :
	Human_subprocessor("WM_processor", human_ptr), focus_messages_flag(true)
		{}
	
	virtual void initialize();
	void initialize_tag_store();

	void display_contents(Output_tee&);
	void display_tag_store_contents(Output_tee&);
	void display_object_store_contents(Output_tee&);
	void set_focus_message_desired(bool flag)
		{focus_messages_flag = flag;}
	virtual void accept_event(const WM_event *);
	virtual void handle_event(const WM_Store_Tag_event *);
	virtual void handle_event(const WM_Erase_Tag_event *);
	virtual void handle_event(const WM_Retrieve_Value_Tag_event *);
	virtual void handle_event(const WM_Store_Object_event *);
	virtual void handle_event(const WM_Erase_Object_event *);
	virtual void handle_event(const WM_Retrieve_Value_Object_event *);
	virtual void handle_event(const WM_Display_Request_event *);
	
	// immediate access functions
	void store_tag(const Symbol& tag_name, const Symbol& tag_value);
	Symbol retrieve_tag_value(const Symbol& tag_name);
	void erase_tag(const Symbol& tag_name);
   const PV_Container& get_tag_store() const { return tag_store; }
	Symbol_list_t get_object_name_list() const;

private:
	PV_Container tag_store;
	WM_Object_set_t object_store;
	// this switch is a kludge installed to support the interrupt rule checking - dk 5/4/00
	bool focus_messages_flag;	// flag to turn not-in-focus messages on & off
};


#endif
