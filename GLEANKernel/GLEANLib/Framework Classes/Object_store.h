#ifndef OBJECT_STORE_H
#define OBJECT_STORE_H

#include "OPV_Containers.h"
#include "Symbol.h"

class Output_tee;
/*
Object_Store is a mixin base class for Processors that keep a store of objects with properties and
values.  This class provides services for such Processors. 
*/

class Object_Store {
public:
	Object_Store() {}
	virtual ~Object_Store() {}
	
	void set_initial_contents(const OPV_Container& initial_contents);
	
	virtual void initialize();
		
	virtual void display_contents(Output_tee&) const;
	virtual void display_object_info(Output_tee& ot, const char * title = "Object_Store") const;

	// immediate store and retrieve functions for non-event access
	void create_object(const Symbol& object_name);
	void erase(const Symbol& object_name);
	void erase(const Symbol& object_name, const Symbol& property_name);
	void clear(); // empty the container
	int get_number()	// return the number of objects in the container
		{return objects.size();}

	void store(const Symbol& object_name, const Symbol& property_name, const Symbol& property_value);
	void store(const Symbol& object_name, 
		const Symbol_list_t& property_names, const Symbol_list_t& property_values);

	bool is_present (const Symbol& object_name) const;
	Symbol retrieve(const Symbol& object_name, const Symbol& property_name) const;
	// find and return name of first object matching property-value pair
	Symbol find_object(const Symbol& prop, const Symbol& value) const;
	// find and return name of first object matching property-value list
	Symbol find_object(const Symbol_list_t& props, const Symbol_list_t& values) const;
	// find and return all names of objects
	Symbol_list_t get_all_object_names() const;
	// find and return all names of objects matching property-value list
	Symbol_list_t find_all_objects(const Symbol_list_t& props, const Symbol_list_t& values) const;
	

    bool equals( const Object_Store & other ) const;

    const Object_Store * delta( const Object_Store & other ) const;
	
    const OPV_Container& get_objects() const 
    {
       return objects;
    }

protected:  // protected member variables - not a good idea
	OPV_Container initial_objects;
	OPV_Container objects;
};

#endif
