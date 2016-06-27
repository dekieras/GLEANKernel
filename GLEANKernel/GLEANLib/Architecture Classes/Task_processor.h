#ifndef TASK_PROCESSOR_H
#define TASK_PROCESSOR_H

#include "Object_Memory_Processor.h"
#include "Task_event_types.h"
#include "Symbol.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

class Device_processor;

class Task_processor : public Object_Memory_Processor{
public:
	// no public default constructor
	Task_processor(Human_processor * human_ptr) :
		Object_Memory_Processor("Task_processor", human_ptr),
		input_object_number(0)
		{}
	
	virtual void initialize();
	
	virtual void display_contents(Output_tee&) const;
	virtual void display_object_info(Output_tee&) const;
	virtual void accept_event(const Task_event *);	// override Processor definition
	virtual void handle_event(const Task_Get_Value_event *);
	virtual void handle_event(const Task_Get_Object_event *);
	virtual void handle_event(const Task_Write_Object_event * event_ptr);
	virtual void handle_event(const Task_Update_Object_event * event_ptr);
	virtual void handle_event(const Task_Erase_Property_event * event_ptr);
	virtual void handle_event(const Task_Erase_Object_event * event_ptr);

	virtual void handle_event(const Task_List_Create_event * event_ptr);
	virtual void handle_event(const Task_List_Erase_event * event_ptr);
	virtual void handle_event(const Task_List_Size_event * event_ptr); // need

	virtual void handle_event(const Task_List_Prepend_event * event_ptr); // need
	virtual void handle_event(const Task_List_Append_event * event_ptr);
	virtual void handle_event(const Task_List_Insert_Before_event * event_ptr); // need

	virtual void handle_event(const Task_List_Find_Item_event * event_ptr);
	virtual void handle_event(const Task_List_Find_Before_event * event_ptr); // need
	virtual void handle_event(const Task_List_Find_After_event * event_ptr);// need
	virtual void handle_event(const Task_List_Find_First_event * event_ptr);
	virtual void handle_event(const Task_List_Find_Last_event * event_ptr);

	virtual void handle_event(const Task_List_Remove_Item_event * event_ptr);
	virtual void handle_event(const Task_List_Remove_First_event * event_ptr);
	virtual void handle_event(const Task_List_Remove_Last_event * event_ptr);
	
private:
	// private constant initialized in Task_processor.cpp
	static const int task_find_time;
	static const int task_store_time;
	static const int task_property_store_time;
	static const int task_property_erase_time;
	static const int task_object_erase_time;
	static const int task_list_create_time;
	static const int task_list_modification_time;
	static const int task_list_count_time;
	
	Symbol in_focus_object_name;	// holds the object name of the current in_focus object
	int input_object_number;	// used for a unique object name for each input

	Symbol make_new_item_name();
	void send_operation_complete(long completion_time);
	void change_focus(const Symbol& object_name, long change_time);
	void remove_focus(const Symbol& object_name, long change_time);
	
	void store_and_notify(const Symbol& obj_name, const Symbol& prop_name, const Symbol& prop_value);
	void store_and_notify(const Symbol& object_name, const Symbol_list_t& property_names, const Symbol_list_t& property_values);
	void erase_and_notify(const Symbol& obj_name);
	void erase_and_notify(const Symbol& obj_name, const Symbol& prop_name);
	
	void list_prepend(const Symbol& list_name, const Symbol& new_node);
	void list_append(const Symbol& list_name, const Symbol& new_node);
	void store_find_results(const Symbol& item_name, const Symbol& tag_name);
	void finish_remove_list_item(const Symbol& item_name);
	void remove_first(const Symbol& list_header);
	void remove_last(const Symbol& list_header);
};

#endif
