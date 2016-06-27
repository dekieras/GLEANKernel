/*
The Action_Recorder class defines a common type of recorder that tallies the
frequencies, total duration, and average duration of a set of actions.

*/

#ifndef ACTION_RECORDER_H
#define ACTION_RECORDER_H

#include "Recorder.h"
#include <string>

class Action_Recorder : public Recorder {
public:
	Action_Recorder (const std::string& id) : 
		Recorder(id)
		{}
	virtual ~Action_Recorder() {}
	
	virtual void initialize();
	int get_frequency() const
		{return number_of_actions_finished;}
	double get_total_time() const
		{return total_time / 1000.;}	// for seconds
	double get_duration() const
		{return (overall_stop_time - overall_start_time) / 1000.;}	// for seconds
	double get_avg() const
		{return (get_frequency()) ? get_total_time()/number_of_actions_finished : 0.0;}
	double get_percent() const
		{return (get_frequency()) ? (100. * (get_total_time() / get_duration())) : 0.0;}
	// calculate the percentage using a supplied duration
	double get_percent(double duration) const
		{return 100. * (get_total_time() / duration);}
	
	virtual void accept_event(const Start_event * event_ptr);
	virtual void accept_event(const Stop_event * event_ptr);
	virtual void accept_event(const Report_event * event_ptr);

	// stand-alone reports
	virtual void report() const;
	virtual std::string get_report() const = 0;
	// call these to embed the output in other output, duration supplied
	virtual void short_report(double duration) const;
	virtual std::string get_short_report(double duration) const = 0;

protected:
	// services for concrete subclasses
	void update_action_start();
	void update_action_end();
	// generate a string containing data with supplied title
	std::string generate_long_report(const char * title) const;
	std::string generate_short_report(const char * title, double duration) const;
	
	// protected member variable are poor style - need to fix
	// set to zero when created or initialized
	bool recording;	// true if started, false if finished
	long action_start_time;
	int number_of_actions_started;
	int number_of_actions_finished;
	double total_time;
	long overall_start_time;
	long overall_stop_time;
}; 

#endif

