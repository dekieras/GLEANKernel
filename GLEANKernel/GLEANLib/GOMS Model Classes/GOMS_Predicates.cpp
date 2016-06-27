#include <set>

//#include <typeinfo>
#include "GOMS_Model.h"
#include "GOMS_Predicates.h"
#include "Cognitive_processor.h"
#include "Human_processor.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

using namespace std;

// predicate functions

void GOMS_Is_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is ";
	second_arg->display(ot);
 }

void GOMS_Is_Not_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is_not ";
	second_arg->display(ot);
}

void GOMS_Is_Greater_Than_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is_greater_than ";
	second_arg->display(ot);
}

void GOMS_Is_Greater_Than_Or_Equal_To_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is_greater_than_or_equal_to ";
	second_arg->display(ot);
}

void GOMS_Is_Less_Than_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is_less_than ";
	second_arg->display(ot);
}

void GOMS_Is_Less_Than_Or_Equal_To_Predicate::display(Output_tee& ot) const
{
	first_arg->display(ot);
	ot << " is_less_than_or_equal_to ";
	second_arg->display(ot);
}

void GOMS_Exists_Predicate::display(Output_tee& ot) const
{
	ot << "Exists " ;
	first_arg->display(ot);
	ot << " with ";
	for (GOMS_Predicate_Sptr_list_t::const_iterator itr1 = predicates.begin();
			itr1 != predicates.end(); itr1++) {
		(*itr1) ->display(ot);
		// is this the last predicate?
		GOMS_Predicate_Sptr_list_t::const_iterator itr2 = itr1;
		itr2++;
		if (itr2 != predicates.end())
			ot << ", and " ;
		else
			ot << ' ' ;
		}
}

// GOMS_Exists_Predicate is all similar if type is the same and predicates are similar
bool GOMS_Exists_Predicate::similar(const GOMS_Predicate * bp, const Correspondence_map_t& cm) const
{
	const GOMS_Exists_Predicate * p = dynamic_cast<const GOMS_Exists_Predicate *>(bp);
	
	if (p && (first_arg->similar(p->first_arg, cm))) {
		GOMS_Predicate_Sptr_list_t::const_iterator it1, it2;
		for (
			it1 = predicates.begin(), it2 = p->predicates.begin();
			it1 != predicates.end() && it1 != p->predicates.end();
			it1++, it2++) {
				if (!((*it1)->similar(*it2, cm)))
					return false;
				}
		return true;
		}
	else
		return false;
}

bool GOMS_Exists_Predicate::evaluate(Cognitive_processor * cog_proc_ptr)
{
	Symbol tagname = first_arg->get_symbol();
	// save the previous value of the tagname 
	Symbol old_value = cog_proc_ptr->get_human_ptr()->get_Wm_ptr()->retrieve_tag_value(tagname);
	// get a list of the object names in the object store
//	Symbol_list_t object_names = cog_proc_ptr->get_human_ptr()->get_wm_ptr()->get_object_name_list();

	// print the list - debugging only!
	const set<Symbol> & object_names = cog_proc_ptr->get_salient_objects();
/*	if(Normal_out && !object_names.empty()) {
		Normal_out << cog_proc_ptr->get_time() << "Test Exists " << tagname << ": ";
		set<Symbol>::const_iterator it;
	
		for (it = object_names.begin(); it != object_names.end(); it++)
			Normal_out << *it << ' ';
		Normal_out << endl;
		}
*/	
	// try for each object in the object store, setting the tagname to the object name
	for(set<Symbol>::const_iterator o_it = object_names.begin(); o_it != object_names.end(); o_it++) {
		const Symbol& new_value = *o_it;
		cog_proc_ptr->get_human_ptr()->get_Wm_ptr()->store_tag(tagname, new_value);
	
		// now evaluate the predicates with the tag value as currently set
		bool flag = true;
		for (GOMS_Predicate_Sptr_list_t::iterator itr1 = predicates.begin();
				itr1 != predicates.end(); itr1++) {
			// if any predicate is false, quit
			if (! ((*itr1) ->evaluate(cog_proc_ptr)) ) {
				flag = false;
				break;
				}
			}
		// if all predicates are true, leave the tagname containing the object name
		if(flag) {
			if(Trace_out && cog_proc_ptr->get_trace()) {
				Trace_out << cog_proc_ptr->processor_info() << "Exists match: " << tagname << ' ' << new_value << endl;
				}
			return true;
			}
		}
	// here if a match was never found; restore the previous value
	cog_proc_ptr->get_human_ptr()->get_Wm_ptr()->store_tag(tagname, old_value);
	return false;
}

