#ifndef AUDITORY_RECORDERS_H
#define AUDITORY_RECORDERS_H

#include "Action_Recorders.h"
#include "Auditory_event_types.h"
#include <string>

class Auditory_Actions_Counter :public Action_Recorder
{
public:
	Auditory_Actions_Counter () : 
		Action_Recorder("Auditory_Actions_Counter")
		{
			initialize();
		}

	virtual void accept_event(const Auditory_event * event_ptr);
	virtual void accept_event(const Cognitive_event * event_ptr);

	virtual std::string get_report() const;
	// call this to embed the output in other output, duration supplied
	virtual std::string get_short_report(double duration) const;
};

#endif
