#ifndef WM_RECORDERS_H
#define WM_RECORDERS_H

#include "Recorder.h"
#include "Symbol.h"
#include "WM_event_types.h"
#include <set>


class WM_Tag_Counter :public Recorder
{
public:
	// default is to accumulate
	WM_Tag_Counter (bool accum = true) :
		Recorder("WM_Tag_Counter"), accumulate(accum)
		{initialize();}

	virtual void initialize();
	
	double get_total_time() const
		{return total_time / 1000.;}	// for seconds
	double get_duration() const
		{return (overall_stop_time - overall_start_time) / 1000.;}	// for seconds
	int get_number_stores() const
		{return number_stores;}	
	int get_number_replacements() const
		{return number_replacements;}	
	int get_number_deletes() const
		{return number_deletes;}
	int get_peak_items() const
		{return peak_items;}
	double get_avg() const
		{return total_items_x_time / total_time;}

	virtual void start_accumulating();
	virtual void stop_accumulating();

	virtual void accept_event(const Start_event * event_ptr);
	virtual void accept_event(const WM_event * event_ptr);
	virtual void accept_event(const Stop_event * event_ptr);
	virtual void accept_event(const Report_event * event_ptr);

	virtual void report() const;
	virtual void short_report() const;
	virtual std::string get_report() const;
	virtual std::string get_short_report() const;
	
private:	
	// set to zero when created or initialized
	long overall_start_time;
	long overall_stop_time;
	// 
	bool accumulate;	// default is to accumulate
	long accumulate_start_time;
	long accumulate_stop_time;
		
	long last_change_time;
	long last_number_of_items;
	double total_items_x_time;
	double total_time;
	int number_stores;
	int number_replacements;
	int number_deletes;
	int peak_items;
	
	std::set<Symbol> tags;

	void update_items_x_time();
};

#endif
