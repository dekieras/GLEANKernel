#ifndef HUMAN_BASE_H
#define HUMAN_BASE_H

#include "Symbol.h"
#include "Geometry.h"
namespace GU = Geometry_Utilities;


// This class defines an interface to a Human in terms of functions to be called to supply input to the
// human. Cf. Device_base which provides a similar functional interface for input to the device, along
// with functions to call to create inputs to the human, which call these functions.

class Human_base  {
public:
	// inputs - Tells the simulated human about a sensory event
	// A new visual object with unspecified location and size
	virtual void make_visual_object_appear(const Symbol& obj_name) {}
	// A new visual object with specified location and size
	virtual void make_visual_object_appear(const Symbol& obj_name, GU::Point location, GU::Size size) {}
	// A visual object is gone
	virtual void make_visual_object_disappear(const Symbol& obj_name) {}
	// The location of a visual object has changed
	virtual void set_visual_object_location(const Symbol& obj_name, GU::Point location) {}
	// The size of a visual object has changed
	virtual void set_visual_object_size(const Symbol& obj_name, GU::Size size) {}
	// A property of a visual object has changed
	virtual void set_visual_object_property(const Symbol& obj_name, const Symbol& propname, const Symbol& propvalue) {}

	// A new auditory source with location and size
	virtual void create_auditory_source(const Symbol& name, GU::Point location, GU::Size size = GU::Size()) {}
	// An auditory source is gone
	virtual void destroy_auditory_source(const Symbol& name) {}
	// The location of an auditory source has changed
	virtual void set_auditory_source_location(const Symbol& name, GU::Point location) {}
	// The size of an auditory source has changed
	virtual void set_auditory_source_size(const Symbol& name, GU::Size size) {}
	// A property of an auditory source has changed
	virtual void set_auditory_source_property(const Symbol& name, const Symbol& propname, const Symbol& propvalue) {}
	
	// An auditory event with a "message" as a simple signal
	virtual void make_auditory_event(const Symbol& message) {}
	// An auditory event with timbre, source, duration, and optional intrinsic duration
	virtual void make_auditory_sound_event(const Symbol& name, const Symbol& timbre, const Symbol& source, long duration, long intrinsic_duration = 0) {}
	// A new auditory sound with timbre, source, and instrinsic duration has started
	virtual void make_auditory_sound_start(const Symbol& name, const Symbol& timbre, const Symbol& source, long intrinsic_duration) {}
	// The auditory sound has stopped
	virtual void make_auditory_sound_stop(const Symbol& name) {}
	// A property of the auditory sound has changed
	virtual void set_auditory_sound_property(const Symbol& name, const Symbol& propname, const Symbol& propvalue) {}
	// A speech event with source, content (utterance), and duration
	virtual void make_auditory_speech_sound_event(const Symbol& name, const Symbol& source, const Symbol& utterance, long duration) {}
	
	// A high-level input event
	virtual void make_high_level_input_appear(const Symbol& object_name, const Symbol_list_t& props, const Symbol_list_t& values, const Symbol& tag) {}
	// A high-level input disappears
	virtual void make_high_level_input_disappear(const Symbol& object_name) {}
	
	// Make a report
	virtual void make_report(long duration) {};
	
protected:
	Human_base() {}		// this class serves only as a base for derived classes
	virtual ~Human_base() {}
};

#endif

