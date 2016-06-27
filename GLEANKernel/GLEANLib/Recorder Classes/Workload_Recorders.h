#ifndef WORKLOAD_RECORDERS_H
#define WORKLOAD_RECORDERS_H

#include "Recorder.h"
#include "Cognitive_event_types.h"
#include "GOMS_Model.h"	// for definition of GOMS_Goal
// these also include the relevant event type headers
// not duplicated here
#include "Visual_Recorders.h"
#include "Auditory_Recorders.h"
#include "Manual_Recorders.h"
#include "Vocal_Recorders.h"
#include "WM_Recorders.h"

#include <map>
#include <stack>
#include <list>


struct Workload_Accumulator {
    Workload_Accumulator();
	
    double total_time;
    int frequency;
    
    Visual_Actions_Counter visual_actions;
    Auditory_Actions_Counter auditory_actions;
    Manual_Actions_Counter manual_actions;
    Vocal_Actions_Counter vocal_actions;
    WM_Tag_Counter WM_tags;
};

class Workload_Method_Profiler :public Recorder
{
public:
	Workload_Method_Profiler () : 
		Recorder("Workload_Method_Profiler"),
		overall_start_time(0), overall_stop_time(0)
		{
			initialize();
		}

	virtual void initialize();
	
	// each embedded recorder will get the Start, Stop, and Cognitive Events
	// and then the event for its modality

	virtual void accept_event(const Start_event * event_ptr);
	virtual void accept_event(const Stop_event * event_ptr);
//	virtual void accept_event(const Report_event *);

	virtual void accept_event(const Visual_event * event_ptr);
	virtual void accept_event(const Auditory_event * event_ptr);
	virtual void accept_event(const Manual_event * event_ptr);
	virtual void accept_event(const Vocal_event * event_ptr);
	virtual void accept_event(const WM_event * event_ptr);
	virtual void accept_event(const Cognitive_event * event_ptr);
	
	virtual void report() const;
	virtual std::string get_report() const;
	
	// accessor for the profile container
	const std::map <GOMS_Goal, Workload_Accumulator, std::less<GOMS_Goal> > & get_profile() const
		{return profile;}


private:
	// set to zero when created or initialized
	// set at Start time
	long overall_start_time;
	// set at Stop time
	long overall_stop_time;
	long first_AG_time;	// set when first Accomplish_Goal recorded
	long last_RGA_time;	// set when last Return_with_Goal_Accomplished recorded
	bool accumulate;	// true if events being handled

	GOMS_Goal current_goal;	// a separate variable to simplify debugging and verification
	std::map <GOMS_Goal, Workload_Accumulator, std::less<GOMS_Goal> > profile;
	std::map <GOMS_Goal, long, std::less<GOMS_Goal> > start_time;
	// profligate but simple - our local goal stack
	// use a list so that we can iterate through it
	std::list<GOMS_Goal> goal_stack;
	
};

#endif
