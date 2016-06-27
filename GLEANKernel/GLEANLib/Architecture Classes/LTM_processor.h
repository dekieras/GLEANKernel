#ifndef LTM_PROCESSOR_H
#define LTM_PROCESSOR_H

#include "Object_Memory_Processor.h"
#include "LTM_event_types.h"
#include "Symbol.h"


class LTM_processor : public Object_Memory_Processor{
public:
	LTM_processor(Human_processor * human_ptr) :
		Object_Memory_Processor("LTM_processor", human_ptr)
		{}
			
	virtual void initialize();
	void display_contents(Output_tee&) const;
	virtual void display_object_info(Output_tee&) const;
	virtual void accept_event(const LTM_event *);	// override Processor definition

//	These events ask for a slow memory retrieval and deposit the result in WM tag_store
	virtual void handle_event(const LTM_Recall_Value_event *);
	virtual void handle_event(const LTM_Recall_Object_event *);
	
private:
	// private constant initialized in LTM_processor.cpp
	static const int default_retrieval_time;

	Symbol in_focus_object_name;	// holds the object name of the current in_focus object

	void send_operation_complete(long completion_time);
	void change_focus(const Symbol& object_name, long change_time);
	void remove_focus(const Symbol& object_name, long change_time);
};

#endif
