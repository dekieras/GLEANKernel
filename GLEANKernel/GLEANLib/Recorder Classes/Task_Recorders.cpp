#include "Task_Recorders.h"

#include "Output_tee.h"
#include "Output_tee_globals.h"
#include <iostream>

using namespace std;


void Task_Timer::initialize()
{
	start_time = 0;
	stop_time = 0;
	total_time = 0;
	started = false;
}
	
// if we ask for a task object, it must be the beginning of a new task!
void Task_Timer::accept_event(const Task_event * event_ptr)
{
	if (!started) {
		started = true;
		start_time = event_ptr->get_time();
		Normal_out << processor_info() << " Starting task clock at " << start_time << endl;
		}
	else {
		stop_time = event_ptr->get_time();
		Normal_out << processor_info() << " Stopping task clock at " << stop_time << endl;
		int task_time = stop_time - start_time;
		total_time += task_time;
		Normal_out << processor_info() << " Task time: " << task_time << ", Total time: " << total_time << endl;
		start_time = event_ptr->get_time();
		Normal_out << processor_info() << " Starting task clock at " << start_time << endl;
		}
}

void Task_Timer::accept_event(const Cognitive_event * event_ptr)
{
	if (started && dynamic_cast<const Cognitive_Stop_event *> (event_ptr) ) {
		stop_time = event_ptr->get_time();
		Normal_out << processor_info() << " Stopping task clock at " << stop_time << endl;
		int task_time = stop_time - start_time;
		total_time += task_time;
		Normal_out << processor_info() << " Task time: " << task_time << ", Total time: " << total_time << endl;
		started = false; // turn off after first report
		}
}
			
void Task_Timer::accept_event(const Stop_event *)
{
//	if (started) {
//		Normal_out << processor_info() << " Total time: " << total_time << endl;
//	}
}

