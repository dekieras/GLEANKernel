#ifndef VOCAL_RECORDERS_H
#define VOCAL_RECORDERS_H

#include "Action_Recorders.h"
#include "Vocal_event_types.h"
#include "Cognitive_event_types.h"

class Vocal_Actions_Counter :public Action_Recorder
{
public:
	Vocal_Actions_Counter () :
		Action_Recorder("Vocal_Actions_Counter")
		{
			initialize();
		}

	virtual void accept_event(const Vocal_event * event_ptr);
	virtual void accept_event(const Cognitive_event * event_ptr);
	
	virtual std::string get_report() const;
	// call this to embed the output in other output, duration supplied
	virtual std::string get_short_report(double duration) const;
	
};

#endif
