#ifndef GOMS_TASK_OPERATORS_H
#define GOMS_TASK_OPERATORS_H

#include "GOMS_Model.h"
#include "GOMS_Memory_Operators.h"

// forward reference
class Cognitive_processor;

class GOMS_Task_Operator : public GOMS_Operator {
public:

	virtual void request_processors(Processor_Request&) const;
};

class GOMS_Task_Memory_Access_Operator : public GOMS_Memory_Access_Operator {
public:
	GOMS_Task_Memory_Access_Operator(GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) : 
		GOMS_Memory_Access_Operator(plp,tp) {}

	virtual void request_processors(Processor_Request&) const;
};

class GOMS_Get_Task_Operator : public GOMS_Task_Memory_Access_Operator {
public:
	GOMS_Get_Task_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Task_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};

// These classes modify the task description
class GOMS_Write_Task_Operator : public GOMS_Task_Operator {
public:
	GOMS_Write_Task_Operator ()
		{}
	GOMS_Write_Task_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		prop_value_arg_list(plp), WM_tag_name_sptr(tp)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_List_t prop_value_arg_list;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
	
};

class GOMS_Update_Task_Operator : public GOMS_Task_Operator {
public:
	GOMS_Update_Task_Operator ()
		{}
	GOMS_Update_Task_Operator (GOMS_Argument_Sptr_t& op, GOMS_Argument_Sptr_List_t& plp) :
		object_name_sptr(op), prop_value_arg_list(plp)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t object_name_sptr;
	GOMS_Argument_Sptr_List_t prop_value_arg_list;
	
};

class GOMS_Erase_Task_Prop_Operator : public GOMS_Task_Operator {
public:
	GOMS_Erase_Task_Prop_Operator ()
		{}
	GOMS_Erase_Task_Prop_Operator (GOMS_Argument_Sptr_t& op, GOMS_Argument_Sptr_t& propp) :
		object_name_sptr(op), property_name_sptr(propp)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t object_name_sptr;
	GOMS_Argument_Sptr_t property_name_sptr;	
};

class GOMS_Erase_Task_Obj_Operator : public GOMS_Task_Operator {
public:
	GOMS_Erase_Task_Obj_Operator ()
		{}
	GOMS_Erase_Task_Obj_Operator (GOMS_Argument_Sptr_t& op) :
		object_name_sptr(op)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t object_name_sptr;
};



class GOMS_Create_Task_List_Operator : public GOMS_Task_Operator {
public:
	GOMS_Create_Task_List_Operator ()
		{}
	GOMS_Create_Task_List_Operator (GOMS_Argument_Sptr_t& tlist_arg_p) :
		task_list_name_sptr(tlist_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	
};

class GOMS_Erase_Task_List_Operator : public GOMS_Task_Operator {
public:
	GOMS_Erase_Task_List_Operator ()
		{}
	GOMS_Erase_Task_List_Operator (GOMS_Argument_Sptr_t& tlist_arg_p) :
		task_list_name_sptr(tlist_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	
};

class GOMS_Get_Size_Of_Task_List_Operator : public GOMS_Task_Operator {
public:
	GOMS_Get_Size_Of_Task_List_Operator ()
		{}
	GOMS_Get_Size_Of_Task_List_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& tp) :
		task_list_name_sptr(tlist_arg_p), WM_tag_name_sptr(tp)

		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
	
};

class GOMS_Prepend_Item_Operator : public GOMS_Task_Operator {
public:
	GOMS_Prepend_Item_Operator ()
		{}
	GOMS_Prepend_Item_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& titem_arg_p) :
		task_list_name_sptr(tlist_arg_p), task_list_item_sptr(titem_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t task_list_item_sptr;	
};

class GOMS_Append_Item_Operator : public GOMS_Task_Operator {
public:
	GOMS_Append_Item_Operator ()
		{}
	GOMS_Append_Item_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& titem_arg_p) :
		task_list_name_sptr(tlist_arg_p), task_list_item_sptr(titem_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t task_list_item_sptr;	
};

class GOMS_Insert_Before_Operator : public GOMS_Task_Operator {
public:
	GOMS_Insert_Before_Operator ()
		{}
	GOMS_Insert_Before_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, 
		GOMS_Argument_Sptr_t& titem_arg_p, GOMS_Argument_Sptr_t& titem_loc_arg_p) :
		task_list_name_sptr(tlist_arg_p), 
			task_list_item_sptr(titem_arg_p), task_list_location_sptr(titem_loc_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t task_list_item_sptr;	
	GOMS_Argument_Sptr_t task_list_location_sptr;	
};


class GOMS_Find_Task_Item_Whose_Operator : public GOMS_Task_Memory_Access_Operator {
public:
	GOMS_Find_Task_Item_Whose_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, 
			GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Task_Memory_Access_Operator(plp,tp), task_list_name_sptr(tlist_arg_p) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
};

class GOMS_Find_Item_Before_Operator : public GOMS_Task_Operator {
public:
	GOMS_Find_Item_Before_Operator ()
		{}
	GOMS_Find_Item_Before_Operator (GOMS_Argument_Sptr_t& titem_arg_p, GOMS_Argument_Sptr_t& tag_p) :
		task_item_name_sptr(titem_arg_p), WM_tag_name_sptr(tag_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_item_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
};

class GOMS_Find_Item_After_Operator : public GOMS_Task_Operator {
public:
	GOMS_Find_Item_After_Operator ()
		{}
	GOMS_Find_Item_After_Operator (GOMS_Argument_Sptr_t& titem_arg_p, GOMS_Argument_Sptr_t& tag_p) :
		task_item_name_sptr(titem_arg_p), WM_tag_name_sptr(tag_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_item_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
};

class GOMS_Find_First_Item_Operator : public GOMS_Task_Operator {
public:
	GOMS_Find_First_Item_Operator ()
		{}
	GOMS_Find_First_Item_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& tag_p) :
		task_list_name_sptr(tlist_arg_p), WM_tag_name_sptr(tag_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
};

class GOMS_Find_Last_Item_Operator : public GOMS_Task_Operator {
public:
	GOMS_Find_Last_Item_Operator ()
		{}
	GOMS_Find_Last_Item_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& tag_p) :
		task_list_name_sptr(tlist_arg_p), WM_tag_name_sptr(tag_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
};

class GOMS_Remove_Item_Operator : public GOMS_Task_Operator {
public:
	GOMS_Remove_Item_Operator ()
		{}
	GOMS_Remove_Item_Operator (GOMS_Argument_Sptr_t& tlist_arg_p, GOMS_Argument_Sptr_t& titem_arg_p) :
		task_list_name_sptr(tlist_arg_p), task_list_item_sptr(titem_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	GOMS_Argument_Sptr_t task_list_item_sptr;	
};

class GOMS_Remove_First_Operator : public GOMS_Task_Operator {
public:
	GOMS_Remove_First_Operator ()
		{}
	GOMS_Remove_First_Operator (GOMS_Argument_Sptr_t& tlist_arg_p) :
		task_list_name_sptr(tlist_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	
};

class GOMS_Remove_Last_Operator : public GOMS_Task_Operator {
public:
	GOMS_Remove_Last_Operator ()
		{}
	GOMS_Remove_Last_Operator (GOMS_Argument_Sptr_t& tlist_arg_p) :
		task_list_name_sptr(tlist_arg_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t task_list_name_sptr;
	
};



#endif
