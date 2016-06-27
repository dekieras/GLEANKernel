/* This serves as the base class for the device implementation that Device_processor forwards
events to in the form of simple function calls. This class structure is intended to insulate
the coding of a specific device implementation from the internal structure of simulation
system as much as possible. Thus this class does not inherit from any class defined
within the simulation system, and uses only a few utility classes from the simulation system.

This class provides some basic facilities for interacting with the simulated human. The default
behaviors defined in this implementation simply echo the supplied input and generate no output.
The output functions are provided as an inheritable facility. 

This subclass of Device_base provides a built in store of objects which can have properties
and values, in which is automatically stored the objects, properties and values for visual inputs
sent to the simulated human. This allows the device code to easily reference information
about the current input situation, but at the expense of using somewhat clumsy facilities.
At this time only visual inputs are stored.

Using this facility is probably not a good idea for new code; it would be better to use conventional
data structures to represent the device's state.
*/

#ifndef DEVICE_OBJECT_STORE_BASE_H
#define DEVICE_OBJECT_STORE_BASE_H

#include "Device_base.h"
#include "Symbol.h"
#include "Geometry.h"
#include "Output_tee.h"
#include "Object_Store.h"

#include <string>
#include <map>	// for the default parameter facility

class Device_processor;

class Device_Object_Store_Base : public Device_base, public Object_Store {
public:
	// constructor with empty initial states
	Device_Object_Store_Base(const std::string& id, Output_tee& ot) :
		Device_base(id, ot)
		{}
		
	virtual ~Device_Object_Store_Base() {}
	
	virtual void initialize();
	virtual void display() const;
		
	// services for derived and related classes
	// in this class, visual objects and their properties are kept in the local Object_Store
	// See the Object_Store class for the member functions
	
	// functions to manipulate the Device's display and output
	// Tell the simulated human we have a new visual object with unspecified location and size
	void make_visual_object_appear(const Symbol& object_name);
	// Tell the simulated human we have a new visual object with specified location and size
	void make_visual_object_appear(const Symbol& object_name, Geometry_Utilities::Point location,
		Geometry_Utilities::Size size);
	// Tell the simulated human we have a value for a property of a visual object
	void set_visual_object_property(const Symbol& object_name, 
		const Symbol& property_name, const Symbol& property_value);
	// the following assume that the property name is appropriate for the value being sent,
	// 		which is encoded identically in the Symbol for the value.	
	// Tell the simulated human we have a Point value for a property of a visual object
	void set_visual_object_property(const Symbol& object_name, 
		const Symbol& property_name, Geometry_Utilities::Point pv)
		{set_visual_object_property(object_name, property_name, Symbol(pv.x, pv.y));}
	// Tell the simulated human we have a Size value for a property of a visual object
	void set_visual_object_property(const Symbol& object_name, 
		const Symbol& property_name, Geometry_Utilities::Size pv)
		{set_visual_object_property(object_name, property_name, Symbol(pv.h, pv.v));}
	// Tell the simulated human we have two numerical values for a property of a visual object
	void set_visual_object_property(const Symbol& object_name, 
		const Symbol& property_name, double x, double y)
		{set_visual_object_property(object_name, property_name, Symbol(x, y));}
	// Tell the simulated human that a visual object is gone
	void make_visual_object_disappear(const Symbol& object_name);
};

#endif

