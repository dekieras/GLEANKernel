#ifndef HUMAN_SUBPROCESSOR_H
#define HUMAN_SUBPROCESSOR_H

/*
This class is a subclass of Processor to be used for the components of a Human Processor. It has
facilities for communications with a containing Human processor. It intercepts no events.
Since these are unique objects, copy and assignment is disallowed.
*/

#include "Processor.h"

#include <list>
#include <string>

class Human_processor;
class View_base;

class Human_subprocessor : public Processor {
public:
	Human_subprocessor(const std::string& name_, Human_processor * human_ptr_) :
		Processor(name_), human_ptr(human_ptr_)
		{}
	
	virtual void initialize();
	
	virtual std::string processor_info() const;	// produce string containing output info

	// connect to another processor via this pointer
	Human_processor * get_human_ptr() const
		{return human_ptr;}
	
	// public because non-descendants might need to request this service
	void broadcast_to_recorders(const Event * event_ptr) const;
		
	// View maintenance functions
	void add_view(View_base * view_ptr);
	void remove_view(View_base * view_ptr);
	void clear_views();	// tell all of the views to clear themselves

protected:
	// services to derived classes
	typedef std::list<View_base *> View_list_t;
	const View_list_t& get_view_list() const
		{return views;}

	// call these functions to notify the views; func is a member function of View_base
	// note some View_base functions cause a conflict
	template <typename F, typename A1>
	void notify(F func, A1 a1)
	{
		for(View_list_t::const_iterator it = views.begin(); it != views.end(); it++)
			((*it)->*func)(a1);
	}

	template <typename F, typename A1, typename A2>
	void notify(F func, A1 a1, A2 a2) const
	{
		for(View_list_t::const_iterator it = views.begin(); it != views.end(); it++)
			((*it)->*func)(a1, a2);
	}

	template <typename F, typename A1, typename A2, typename A3>
	void notify(F func, A1 a1, A2 a2, A3 a3)
	{
		for(View_list_t::const_iterator it = views.begin(); it != views.end(); it++)
			((*it)->*func)(a1, a2, a3);
	}

private:
	// connnections
	Human_processor * human_ptr;
		
	View_list_t views;

	// no copy, assignment
	Human_subprocessor(const Human_subprocessor&);
	Human_subprocessor& operator= (const Human_subprocessor&);
};

#endif




