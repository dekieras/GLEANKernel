#ifndef GOMS_MEMORY_OPERATORS_H
#define GOMS_MEMORY_OPERATORS_H

#include "GOMS_Model.h"

// forward reference
class Cognitive_processor;


// memory operator classes

class GOMS_Store_Operator : public GOMS_Operator {
public:
	GOMS_Store_Operator () : 
		property_value_sptr(new GOMS_Constant_Argument),
		WM_tag_name_sptr(new GOMS_Constant_Argument)
		{}
	GOMS_Store_Operator (GOMS_Argument_Sptr_t pp, GOMS_Argument_Sptr_t  tp) :
		property_value_sptr(pp), WM_tag_name_sptr(tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;

private:
	GOMS_Argument_Sptr_t property_value_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;
};

class GOMS_Delete_Operator : public GOMS_Operator {
public:
	GOMS_Delete_Operator () : 
		WM_tag_name_sptr(new GOMS_Constant_Argument)
		{}
	GOMS_Delete_Operator (GOMS_Argument_Sptr_t tp) :
		WM_tag_name_sptr(tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_t WM_tag_name_sptr;
};

class GOMS_Log_Operator : public GOMS_Operator {
public:
	GOMS_Log_Operator ()
		{}
	GOMS_Log_Operator (GOMS_Argument_Sptr_List_t& args) :
		arg_list(args) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee&) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
private:
	GOMS_Argument_Sptr_List_t arg_list;
};

class GOMS_Retrieve_LTM_Operator : public GOMS_Operator {
public:
	GOMS_Retrieve_LTM_Operator () : 
		property_name_sptr(new GOMS_Constant_Argument),
		object_name_sptr(new GOMS_Constant_Argument),
		WM_tag_name_sptr(new GOMS_Constant_Argument)
		{}
	GOMS_Retrieve_LTM_Operator (GOMS_Argument_Sptr_t prop_p, GOMS_Argument_Sptr_t obj_p, GOMS_Argument_Sptr_t tag_p) :
		property_name_sptr(prop_p),
		object_name_sptr(obj_p),
		WM_tag_name_sptr(tag_p) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;


private:
	GOMS_Argument_Sptr_t property_name_sptr;
	GOMS_Argument_Sptr_t object_name_sptr;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;
};

class GOMS_Look_at_Operator : public GOMS_Operator {
public:
	GOMS_Look_at_Operator () : 
		WM_tag_name_sptr(new GOMS_Constant_Argument)
		{}
	GOMS_Look_at_Operator (GOMS_Argument_Sptr_t tag_p) :
		WM_tag_name_sptr(tag_p)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;
private:
	GOMS_Argument_Sptr_t WM_tag_name_sptr;
};


// An ABC for a set of operators that differ only in which data source they access

class GOMS_Memory_Access_Operator : public GOMS_Operator {
public:
	GOMS_Memory_Access_Operator ()
		{}
	GOMS_Memory_Access_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		prop_value_arg_list(plp), WM_tag_name_sptr(tp) 
		{}
	virtual ~GOMS_Memory_Access_Operator () 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr) = 0; // an ABC
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;

protected:	
	GOMS_Argument_Sptr_List_t prop_value_arg_list;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;
};


class GOMS_Recall_LTM_Operator : public GOMS_Memory_Access_Operator {
public:
	GOMS_Recall_LTM_Operator ()
		{}
	GOMS_Recall_LTM_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual void request_processors(Processor_Request&) const;
};

class GOMS_Visual_Memory_Access_Operator : public GOMS_Memory_Access_Operator {
public:
	GOMS_Visual_Memory_Access_Operator(GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) : 
		GOMS_Memory_Access_Operator(plp,tp) {}

	virtual void request_processors(Processor_Request&) const;
};

class GOMS_Look_Operator : public GOMS_Visual_Memory_Access_Operator {
public:
//	GOMS_Look_Operator ()
//		{}
	GOMS_Look_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Visual_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};

class GOMS_Check_Visual_Operator : public GOMS_Visual_Memory_Access_Operator {
public:
//	GOMS_Check_Visual_Operator ()
//		{}
	GOMS_Check_Visual_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Visual_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};


class GOMS_Wait_Visual_Operator : public GOMS_Visual_Memory_Access_Operator {
public:
//	GOMS_Wait_Visual_Operator ()
//		{}
	GOMS_Wait_Visual_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Visual_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};


class GOMS_Auditory_Memory_Access_Operator : public GOMS_Memory_Access_Operator {
public:
	GOMS_Auditory_Memory_Access_Operator(GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) : 
		GOMS_Memory_Access_Operator(plp,tp) {}

	virtual void request_processors(Processor_Request&) const;
};


class GOMS_Check_Auditory_Operator : public GOMS_Auditory_Memory_Access_Operator {
public:
//	GOMS_Check_Auditory_Operator ()
//		{}
	GOMS_Check_Auditory_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Auditory_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};



class GOMS_Wait_Auditory_Operator : public GOMS_Auditory_Memory_Access_Operator {
public:
//	GOMS_Wait_Auditory_Operator ()
//		{}
	GOMS_Wait_Auditory_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		GOMS_Auditory_Memory_Access_Operator(plp,tp) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
};

class GOMS_Mental_Operator : public GOMS_Operator {
public:
	GOMS_Mental_Operator ()
		{}
//	GOMS_Mental_Operator (const Symbol& mon, GOMS_Argument_Sptr_t& lp) :
	GOMS_Mental_Operator (const Symbol& mon, const Symbol& da) :
		mental_operator_name(mon),	documentation_argument(da) 
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;
private:
	Symbol mental_operator_name;
	Symbol documentation_argument;
//	GOMS_Argument_Sptr_t label_sptr;
	static const int default_mental_operator_time;
};

#endif
