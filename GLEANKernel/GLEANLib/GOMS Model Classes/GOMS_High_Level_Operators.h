#ifndef GOMS_HIGH_LEVEL_OPERATORS_H
#define GOMS_HIGH_LEVEL_OPERATORS_H

#include "GOMS_Model.h"

class GOMS_HLGet_Device_Operator : public GOMS_Operator {
public:
	GOMS_HLGet_Device_Operator ()
		{}
	GOMS_HLGet_Device_Operator (GOMS_Argument_Sptr_List_t& plp, GOMS_Argument_Sptr_t& tp) :
		prop_value_arg_list(plp), WM_tag_name_sptr(tp)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;

private:
	GOMS_Argument_Sptr_List_t prop_value_arg_list;
	GOMS_Argument_Sptr_t WM_tag_name_sptr;	
};

class GOMS_HLPut_Device_Operator : public GOMS_Operator {
public:
	GOMS_HLPut_Device_Operator ()
		{}
	GOMS_HLPut_Device_Operator (GOMS_Argument_Sptr_List_t& plp) :
		prop_value_arg_list(plp)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;
private:
	GOMS_Argument_Sptr_List_t prop_value_arg_list;	
};


#endif
