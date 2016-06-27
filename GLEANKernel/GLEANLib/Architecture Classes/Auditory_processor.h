#ifndef AUDITORY_PROCESSOR_H
#define AUDITORY_PROCESSOR_H

#include "Object_Memory_Processor.h"
#include "Auditory_event_types.h"
#include "Symbol.h"

class Auditory_processor : public Object_Memory_Processor {
public:
	Auditory_processor(Human_processor * human_ptr) :
		Object_Memory_Processor("Auditory_processor", human_ptr),
		waiting_for_object(false), input_object_number(0)
		{}
	
	virtual void initialize();
	virtual void display_contents(Output_tee&) const;
	virtual void display_object_info(Output_tee&) const;
	virtual void accept_event(const Auditory_event *);	// override Processor definition
	virtual void handle_event(const Auditory_Input_Symbol_event *);
	virtual void handle_event(const Auditory_Input_Decay_event *);
	virtual void handle_event(const Auditory_Input_Spatial_event *);
	virtual void handle_event(const Auditory_Get_Value_event *);
	virtual void handle_event(const Auditory_Get_Object_event *);
	virtual void handle_event(const Auditory_Wait_Object_event *);
	
private:
	// private constant
	static const int auditory_find_time;
	static const int auditory_decay_time;

	OPV_Container auditory_encodings;	// holds encoding specs
	
	bool waiting_for_object;
	Symbol_list_t waiting_for_property_names;
	Symbol_list_t waiting_for_property_values;
	Symbol waiting_for_tagname;
	int input_object_number;	// used for a unique object name for each input
	
	void do_auditory_encoding (const Symbol& object_name, const Symbol& input_symbol);
	void send_operation_complete(long completion_time);
};

#endif
