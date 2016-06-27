#include <iostream>
//#include <typeinfo>
#include "Object_Store.h"
#include "Symbol.h"
#include "Output_tee.h"

#include <vector>

using namespace std;


void Object_Store::set_initial_contents(const OPV_Container& initial_contents)
{
	initial_objects = initial_contents;
}

void Object_Store::clear() {
		objects.clear();
		}

void Object_Store::initialize() {
		objects = initial_objects;
		}
		
void Object_Store::create_object(const Symbol& object_name)
{
	// object might already exist - create_object will return false, but no ill result
	// normally, so situation is ignored
	objects.create_object(object_name);
}

void Object_Store::erase(const Symbol& object_name)
{
	objects.erase(object_name);
}

void Object_Store::erase(const Symbol& object_name, const Symbol& property_name)
{
	objects.erase(object_name, property_name);
}

void Object_Store::store(const Symbol& object_name, 
	const Symbol& property_name, const Symbol& property_value)
{
	objects.store(object_name, property_name, property_value);
}

void Object_Store::store(const Symbol& object_name, 
	const Symbol_list_t& property_names, const Symbol_list_t& property_values)
{
	objects.store(object_name, property_names, property_values);
}
		

bool Object_Store::is_present (const Symbol& object_name) const
{
	return (objects.is_present(object_name));
}

Symbol Object_Store::retrieve(const Symbol& object_name, const Symbol& property_name) const
{
	return objects.retrieve(object_name, property_name);
}

// find and return name of first object matching property-value pair
Symbol Object_Store::find_object(const Symbol& prop, const Symbol& value) const
{
	Symbol object_name = objects.find(prop, value);
	return object_name;
}


// find and return name of first object matching property-value list
Symbol Object_Store::find_object(
	const Symbol_list_t& props, const Symbol_list_t& values) const
{
	
	// retrieve the object name from the lists of property names and values
	Symbol object_name = objects.find(props, values);
		return object_name;
}

// find and return all names of objects
Symbol_list_t Object_Store::get_all_object_names() const
{
	return objects.get_all_object_names();
}

// find and return all names of objects matching property-value list
Symbol_list_t Object_Store::find_all_objects(
	const Symbol_list_t& props, const Symbol_list_t& values) const
{
	
	// retrieve the object name from the lists of property names and values
	Symbol_list_t object_names = objects.find_all(props, values);
	return object_names;
}

// for each object in the memory, display its properties and values
void Object_Store::display_object_info(Output_tee& ot, const char * title) const
{
	Symbol_list_t object_names = objects.get_all_object_names();
	ot << title << ": ";
	if(object_names.empty()) {
		ot << "Empty" << endl;
		return;
		}
	ot << endl;

	for(Symbol_list_t::const_iterator object_it = object_names.begin(); 
			object_it != object_names.end(); ++object_it) {
		ot << "  " << (*object_it) << ": ";
		// get lists of all the properties and values for each object
		Symbol_list_t prop_names, prop_values;
		objects.get_pv_lists(*object_it, prop_names, prop_values);
		Symbol_list_t::const_iterator p_it = prop_names.begin();
		Symbol_list_t::const_iterator v_it = prop_values.begin();
		for(; p_it != prop_names.end() && v_it != prop_values.end(); ++p_it, ++v_it) {
			ot << ' ' << *p_it << ':' << *v_it;
			}
		ot << endl;
		}
}


void Object_Store::display_contents(Output_tee& ot) const
{
	if (!initial_objects.empty()) {
		ot << "Initial contents:" << endl;
		initial_objects.display_contents(ot);
		}

	if (!objects.empty()) {
		ot << "Current contents:" << endl;
		objects.display_contents(ot);
		}
}

bool Object_Store::equals( const Object_Store & other ) const
{
    return objects.equals( other.get_objects() );
}

/**
 * @return A pointer to a new Object_Store instance containing objects that
 *         have either been added or changed in "other".
 */
 
 // this is an especially ugly piece of code that should not have been necessary
const Symbol REMOVE_C("REMOVE");
const Object_Store * Object_Store::delta( const Object_Store & other ) const {
    
    std::map< Symbol, PV_Container >::const_iterator i = get_objects().begin();
    std::map< Symbol, PV_Container >::const_iterator j = other.get_objects().begin();

    Object_Store * result = new Object_Store();
    OPV_Container symbol_map = result->get_objects();

    for( ; i != get_objects().end() && j != other.get_objects().end(); ) {

        if( i->first < j->first ) {

            symbol_map.add( i->first, PV_Container(REMOVE_C, REMOVE_C) );
            ++i;
        }
        else if( j->first < i->first ) {

            symbol_map.add( j->first, j->second );
            ++j;
        }
        else if( i->second != j->second ) {

            symbol_map.add( j->first, j->second );
            ++i; ++j;
        }
        else {
            ++i; ++j;
        }
    }

    for( ; i != get_objects().end(); i++ ) {
        symbol_map.add( i->first, i->second );
    }

    for( ; j != other.get_objects().end(); j++ ) {
        symbol_map.add( j->first, j->second );
    }

    result->set_initial_contents( symbol_map );
    result->initialize();

    return result;
}

