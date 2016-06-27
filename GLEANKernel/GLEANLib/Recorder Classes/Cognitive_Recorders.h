#ifndef COGNITIVE_RECORDERS_H
#define COGNITIVE_RECORDERS_H

#include "Recorder.h"
#include "Cognitive_event_types.h"


struct Accumulator {
    Accumulator() : total_time(0.0), frequency (0) {}
    Accumulator(double v) : total_time(v), frequency (1) {}
    double total_time;
    int frequency;
};

class Cognitive_Method_Profiler :public Recorder
{
public:
	Cognitive_Method_Profiler () : 
		Recorder("Cognitive_Method_Profiler"),
		overall_start_time(0), overall_stop_time(0)
		{}

	virtual void initialize();

	virtual void accept_event(const Start_event * event_ptr);
	virtual void accept_event(const Stop_event * event_ptr);

	virtual void accept_event(const Cognitive_event * event_ptr);
	
	virtual void report() const;

private:
	// set to zero when created or initialized
	// set at Start time
	long overall_start_time;
	// set at Stop time
	long overall_stop_time;
	long first_AG_time;	// set when first Accomplish_Goal recorded
	long last_RGA_time;	// set when last Return_with_Goal_Accomplished recorded
			

	std::map <GOMS_Goal, Accumulator, std::less<GOMS_Goal> > profile;
	std::map <GOMS_Goal, long, std::less<GOMS_Goal> > start_time;
	
};

#endif
