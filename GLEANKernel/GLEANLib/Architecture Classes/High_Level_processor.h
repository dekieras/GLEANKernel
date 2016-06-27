#ifndef HIGH_LEVEL_PROCESSOR_H
#define HIGH_LEVEL_PROCESSOR_H

#include "Object_Memory_Processor.h"
#include "High_Level_event_types.h"
#include "Geometry.h"

// This class handles the High-Level GOMS interaction with the device

class High_Level_processor : public Object_Memory_Processor {
public:
	High_Level_processor(Human_processor * human_ptr) :
		Object_Memory_Processor("High_Level_processor", human_ptr),
		waiting_for_object(false)
		{}
	
	virtual void initialize();
	virtual void display_object_info(Output_tee&) const;
		
	virtual void accept_event(const High_Level_event *);

	// GOMS operator commands high-level output to the device
	virtual void handle_event(const High_Level_Put_Command *);
	
	// GOMS operator commands high-level input from the device
	virtual void handle_event(const High_Level_Get_Command *);
	
	// device supplies information to be added to memory
	virtual void handle_event(const High_Level_Input_event *);

	// device specifies information to be removed from memory
	virtual void handle_event(const High_Level_Erase_event *);
	

private:
	bool waiting_for_object;
	void send_operation_complete(int time_required);
};

#endif
