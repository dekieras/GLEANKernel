#include "Device_Object_Store_Base.h"
#include "Human_processor.h"
#include "Geometry.h"
#include "Symbol_Geometry_utilities.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_standard_symbols.h"

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
//#include <typeinfo>
//#include <DebugNew.h>
using namespace std;

namespace GU =  Geometry_Utilities;


void Device_Object_Store_Base::initialize()
{
	Device_base::initialize();
	Object_Store::initialize();
}



void Device_Object_Store_Base::display() const
{
	Device_base::display();
	device_out << "Device Object Store Contents:" << endl;
	Object_Store::display_contents(device_out);
}

// Tell the simulated human we have a new visual object with unspecified location and size
void Device_Object_Store_Base::make_visual_object_appear(const Symbol& object_name)
{
	Device_base::make_visual_object_appear(object_name);
	create_object(object_name);
}

// Tell the simulated human we have a new visual object with specified location and size
void Device_Object_Store_Base::make_visual_object_appear(const Symbol& object_name, 
	Geometry_Utilities::Point location, Geometry_Utilities::Size size)
{
	Device_base::make_visual_object_appear(object_name, location, size);
	create_object(object_name);
	store(object_name, Location_c, to_Symbol(location));
	store(object_name, Size_c, to_Symbol(size));
}

// Tell the simulated human we have a value for a property of a visual object
void Device_Object_Store_Base::set_visual_object_property(const Symbol& object_name, 
	const Symbol& property_name, const Symbol& property_value)
{
	Device_base::set_visual_object_property(object_name, property_name, property_value);	
	store(object_name, property_name, property_value);
}

// Tell the simulated human that a visual object is gone
void Device_Object_Store_Base::make_visual_object_disappear(const Symbol& object_name)
{
	Device_base::make_visual_object_disappear(object_name);
	erase(object_name);
}

