#include "Device_base.h"
#include "Device_processor.h"
#include "Coordinator.h"
#include "Human_base.h"
//#include "Human_processor.h"
#include "Geometry.h"
#include "Symbol_Geometry_utilities.h"
#include "Output_tee_globals.h"
#include "Glean_standard_symbols.h"

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

using namespace std;
namespace GU =  Geometry_Utilities;


Device_base::Device_base(const string& id, Output_tee& ot) :
	device_out(ot), device_name(id), device_proc_ptr(0)
{
	// create a Device_processor and connect this device to it. 
	// Processor() automatically adds itself to the simulation
	device_proc_ptr = new Device_processor(device_name + "_interface");
	device_proc_ptr->connect(this);
}

Device_base::~Device_base()
{
	// ~Processor automatically removes itself from the simulation
	delete device_proc_ptr;
	device_proc_ptr = 0;
}

/*
// create a Device_processor and connect this device to it. 
// Device_processor() automatically adds itself to the simulation
void Device_base::create_and_connect_interface(const string& interface_name)
{
	device_proc_ptr = new Device_processor(device_proc_name);
	device_proc_ptr->connect(this);
	this->connect(device_proc_ptr);
}
*/

void Device_base::initialize()
{
	// make sure we are connected to the rest of the system
	Assert(device_proc_ptr);
}


// These dummy functions should be overridden to obtain more complex behavior.

void Device_base::handle_Start_event()
{
}

void Device_base::handle_Stop_event()
{
}

void Device_base::handle_Report_event(long)
{
}

void Device_base::handle_Delay_event(const Symbol& type, const Symbol& datum, 
		const Symbol& object_name, const Symbol& property_name, const Symbol& property_value)
{
}

void Device_base::handle_Keystroke_event(const Symbol& key_name)
{
}

void Device_base::handle_Type_In_event(const Symbol& type_in_string)
{
}

void Device_base::handle_Hold_event(const Symbol& button_name)
{
}

void Device_base::handle_Release_event(const Symbol& button_name)
{
}

void Device_base::handle_Click_event(const Symbol& button_name)
{
}

void Device_base::handle_Double_Click_event(const Symbol& button_name)
{
}

void Device_base::handle_Point_event(const Symbol& target_name)
{
}

void Device_base::handle_Ply_event(const Symbol& cursor_name, const Symbol& target_name,
		GU::Point new_location, GU::Polar_vector movement_vector)
{
}

void Device_base::handle_Vocal_event(const Symbol& vocal_input)
{
}

void Device_base::handle_Vocal_event(const Symbol& vocal_input, long duration)
{
}

void Device_base::handle_VisualFocusChange_event(const Symbol& object_name)
{
}

void Device_base::handle_Eyemovement_Start_event(const Symbol& target_name, GU::Point new_location)
{
}

void Device_base::handle_Eyemovement_End_event(const Symbol& target_name, GU::Point new_location)
{
}

void Device_base::handle_HLGet_event(const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag)
{
	// default response is to simply echo the information back with a dummy name
	make_high_level_input_appear(Symbol("HLDummyObject"), props, values, tag);
}

void Device_base::handle_HLPut_event(const Symbol_list_t& props, const Symbol_list_t& values)
{
}

/* Services for derived and related classes */

string Device_base::processor_info() const
{
	ostringstream oss;
	oss << get_time() << ' ' << device_name << ':';
	return oss.str();
}

long Device_base::get_time() const
{
	return Coordinator::get_instance().get_time();
}
	
bool Device_base::get_trace() const
{
	return device_proc_ptr->get_trace();
}

void Device_base::set_trace(bool trace_)
{
	device_proc_ptr->set_trace(trace_);
}

	
// Tell the simulated human we have a new visual object with unspecified location and size
void Device_base::make_visual_object_appear(const Symbol& object_name)
{
	device_proc_ptr->make_visual_object_appear(object_name);
}

// Tell the simulated human we have a new visual object with specified location and size
void Device_base::make_visual_object_appear(const Symbol& object_name, 
	Geometry_Utilities::Point location, Geometry_Utilities::Size size)
{
	device_proc_ptr->make_visual_object_appear(object_name, location, size);
}

void Device_base::set_visual_object_location(const Symbol& object_name, GU::Point location)
{
	device_proc_ptr->set_visual_object_location(object_name, location);
}

void Device_base::set_visual_object_size(const Symbol& object_name, GU::Size size)
{
	device_proc_ptr->set_visual_object_size(object_name, size);
}


// Tell the simulated human we have a value for a property of a visual object
void Device_base::set_visual_object_property(const Symbol& object_name, 
	const Symbol& property_name, const Symbol& property_value)
{
	Assert(object_name != Nil_c);
	device_proc_ptr->set_visual_object_property(object_name, property_name, property_value);
}

// Tell the simulated human that a visual object is gone
void Device_base::make_visual_object_disappear(const Symbol& object_name)
{
	device_proc_ptr->make_visual_object_disappear(object_name);
}


// Tell the simulated human that an auditory event is present 
void Device_base::make_auditory_event(const Symbol& message)
{
	device_proc_ptr->make_auditory_event(message);
}

// A speech event with source, content (utterance), and duration
void Device_base::make_auditory_speech_sound_event(const Symbol& name, const Symbol& source, const Symbol& utterance, long duration)
{
	device_proc_ptr->make_auditory_speech_sound_event(name, source, utterance, duration);
}

// Tell the simulated human that a High-Level input object with properties and values should be
// stored under a WM tag after a specified time delay from now

void Device_base::make_high_level_input_appear(const Symbol& object_name, const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag)
{
	device_proc_ptr->make_high_level_input_appear(object_name, props, values, tag);
}

// Tell the simulated human that a High-Level input object is gone
// A high-level input disappears after a specified delay
void Device_base::make_high_level_input_disappear(const Symbol& object_name)
{
	device_proc_ptr->make_high_level_input_disappear(object_name);
}


// send a Device_Delay_event to self
void Device_base::schedule_delay_event(long delay)
{	
	device_proc_ptr->schedule_delay_event(delay);
}

void Device_base::schedule_delay_event(long delay, const Symbol& delay_type, const Symbol& delay_datum)
{	
	device_proc_ptr->schedule_delay_event(delay, delay_type, delay_datum);
}

void Device_base::schedule_delay_event(long delay, const Symbol& delay_type, 
		const Symbol& object_name, const Symbol& property_name, const Symbol& property_value)
{			
	device_proc_ptr->schedule_delay_event(delay, delay_type, object_name, property_name, property_value);	
}

// Tell the simulated human that a report with the specified duration must be made
// this is done visually
void Device_base::make_report(long time, long duration)
{
	device_proc_ptr->make_report(time, duration);
}


// service for descendant classes to use to halt simulation.
void Device_base::stop_simulation()
{
	device_proc_ptr->do_stop_simulation();
}

