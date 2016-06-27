#ifndef MANUAL_RECORDERS_H
#define MANUAL_RECORDERS_H

#include "Action_Recorders.h"
#include "Manual_event_types.h"
#include "Cognitive_event_types.h"

class Manual_Actions_Counter :public Action_Recorder
{
public:
	Manual_Actions_Counter () : 
		Action_Recorder("Manual_Actions_Counter")
		{
			initialize();
		}

	virtual void accept_event(const Manual_event * event_ptr);
	virtual void accept_event(const Cognitive_event * event_ptr);
	
	virtual std::string get_report() const;
	// call this to embed the output in other output, duration supplied
	virtual std::string get_short_report(double duration) const;

};

#endif
