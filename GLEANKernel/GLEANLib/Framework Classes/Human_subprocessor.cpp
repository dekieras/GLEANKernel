#include "Human_subprocessor.h"
#include "Human_processor.h"
#include "Assert_throw.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <functional>

using std::endl;
using std::list;
using std::string;
using std::for_each;
using std::mem_fun; using std::bind2nd;
//#include <typeinfo>



void Human_subprocessor::initialize()
{
	Assert(human_ptr);
	clear_views();
//	describe_parameters(Normal_out);
}

/* View maintenance and notification functions */


void Human_subprocessor::add_view(View_base * view_ptr)
{
	views.push_back(view_ptr);
}
	
void Human_subprocessor::remove_view(View_base * view_ptr)
{
	views.remove(view_ptr);
}

void Human_subprocessor::clear_views()
{
	// tell all the associated views to clear themselves
	for(View_list_t::const_iterator it = views.begin(); it != views.end(); it++)
		(*it)->clear();
}

string Human_subprocessor::processor_info() const
{
	string result = human_ptr->processor_info();
	result += get_name();
	result += ':';
	return result;
}


void Human_subprocessor::broadcast_to_recorders(const Event * event_ptr) const
{
	human_ptr->broadcast_to_recorders(event_ptr);
}

