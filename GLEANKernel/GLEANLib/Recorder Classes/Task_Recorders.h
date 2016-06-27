#ifndef TASK_RECORDERS_H
#define TASK_RECORDERS_H

#include "Recorder.h"
#include "Task_event_types.h"
#include "Cognitive_event_types.h"


class Task_Timer : public Recorder {
public:
	Task_Timer () :
		Recorder("Task_Timer"), start_time(0), stop_time(0), total_time(0), started(false)
		{}
		
	virtual void initialize();
	
	// if we ask for a task object, it must be the beginning of a new task!
	virtual void accept_event(const Task_event * event_ptr);
	virtual void accept_event(const Cognitive_event * event_ptr);
	virtual void accept_event(const Stop_event * event_ptr);
			
private:
	int start_time, stop_time, total_time;
	bool started;
};

#endif
