/* 
This is a Object Adapter class that adapts the pointed-to Device_base object into the
Processor event interface. The events are accepted by this class, which then forwards them
as function calls to the Device_base object. Thus this class provides an interface to the 
Device_base from the GLEAN event system.

It accepts events which are forwarded to the Device_base object, and has functions that 
the Device_base class calls to create either Device_delay events or events for the simulated human. 
This class is supposed to be a leaf class only. 

After creation, this processor must be connected to a Device_base object, checked for by
initialize(), and optionally to a Human processor, checked at function-call time. 
Because devices should be unique in the domain, copy and assignment are forbidden.
*/

#ifndef DEVICE_PROCESSOR_H
#define DEVICE_PROCESSOR_H

#include "Processor.h"
#include "Device_event_types.h"
#include "Geometry.h"
#include <string>

class Human_processor;
class Human_base;
class Device_base;

class Device_processor : public Processor {
public:
	// This constructor requires that Device_base device and Human_processor must be created first
	Device_processor(const std::string& id);
	
	virtual void initialize();
	virtual void display() const;
	virtual std::string processor_info() const;
	virtual void set_parameter_string(const std::string&);
	virtual std::string get_parameter_string() const;
	
	void connect(Device_base * device_interface_ptr_);
	Device_base * get_device_ptr() const
		{return device_interface_ptr;}
	// the device interacts with at most one human
	void connect(Human_base * human_interface_ptr_);
	Human_base * get_human_ptr() const
		{return human_interface_ptr;}
	
	// event acceptance -- forwarded to device implementation
	virtual void accept_event(const Start_event *); 	// override Processor definition
	virtual void accept_event(const Stop_event *);
	virtual void accept_event(const Device_event *);

	// event handling
	virtual void handle_event(const Device_Delay_event *);
	virtual void handle_event(const Device_Keystroke_event *);
	virtual void handle_event(const Device_Type_In_event *);
	virtual void handle_event(const Device_Hold_event *);
	virtual void handle_event(const Device_Release_event *);
	virtual void handle_event(const Device_Click_event *);
	virtual void handle_event(const Device_Double_Click_event *);
	virtual void handle_event(const Device_Ply_event *);
	virtual void handle_event(const Device_Point_event *);
	virtual void handle_event(const Device_Vocal_event *);
	virtual void handle_event(const Device_VisualFocusChange_event *);
	virtual void handle_event(const Device_Eyemovement_Start_event *);
	virtual void handle_event(const Device_Eyemovement_End_event *);
	virtual void handle_event(const Device_HLGet_event *);
	virtual void handle_event(const Device_HLPut_event *);

	// functions for creating device delay events to arrive at this processor after a delay
	void schedule_delay_event(long delay);
	void schedule_delay_event(long delay, const Symbol& delay_type, const Symbol& delay_datum);
	void schedule_delay_event(long delay, const Symbol& delay_type, 
		const Symbol& object_name, const Symbol& property_name, const Symbol& property_value);
		
	// functions for creating inputs to the simulated human
	// inputs - Tells the simulated human about a sensory event
	// A new visual object with unspecified location and size
	void make_visual_object_appear(const Symbol& obj_name);
	// A new visual object with specified location and size
	void make_visual_object_appear(const Symbol& obj_name, GU::Point location, GU::Size size);
	// A visual object is gone
	void make_visual_object_disappear(const Symbol& obj_name);
	// The location of a visual object has changed
	void set_visual_object_location(const Symbol& obj_name, GU::Point location);
	// The size of a visual object has changed
	void set_visual_object_size(const Symbol& obj_name, GU::Size size);
	// A property of a visual object has changed
	void set_visual_object_property(const Symbol& obj_name, const Symbol& propname, const Symbol& propvalue);

	// A new auditory source with location and size
	void create_auditory_source(const Symbol& name, GU::Point location, GU::Size size = GU::Size());
	// An auditory source is gone
	void destroy_auditory_source(const Symbol& name);
	// The location of an auditory source has changed
	void set_auditory_source_location(const Symbol& name, GU::Point location);
	// The size of an auditory source has changed
	void set_auditory_source_size(const Symbol& name, GU::Size size);
	// A property of an auditory source has changed
	void set_auditory_source_property(const Symbol& name, const Symbol& propname, const Symbol& propvalue);
	
	// An auditory event with a "message" as a simple signal
	void make_auditory_event(const Symbol& message);
	// An auditory event with timbre, source, duration, and optional intrinsic duration
	void make_auditory_sound_event(const Symbol& name, const Symbol& timbre, const Symbol& source, long duration, long intrinsic_duration = 0);
	// A new auditory sound with timbre, source, and instrinsic duration has started
	void make_auditory_sound_start(const Symbol& name, const Symbol& timbre, const Symbol& source, long intrinsic_duration);
	// The auditory sound has stopped
	void make_auditory_sound_stop(const Symbol& name);
	// A property of the auditory sound has changed
	void set_auditory_sound_property(const Symbol& name, const Symbol& propname, const Symbol& propvalue);
	// A speech event with source, content (utterance), and duration
	void make_auditory_speech_sound_event(const Symbol& name, const Symbol& source, const Symbol& utterance, long duration);
	
	// A high-level input event
	void make_high_level_input_appear(const Symbol& object_name, const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag);
	// A high-level input disappears
	void make_high_level_input_disappear(const Symbol& object_name);
	
	// Make a report
	void make_report(long time, long duration);
	
	// stop the simulation - separate function to allow trace message
	void do_stop_simulation();

private:
	Human_processor * human_ptr;
	Human_base * human_interface_ptr;					// destination for visual and auditory output
	Device_base * device_interface_ptr;				// the Device object
	
	Device_processor(const Device_processor&);
	Device_processor& operator= (const Device_processor&);
};

// provide a definition for this function to create a specific subclass of device
Device_base * create_specific_device();

#endif
