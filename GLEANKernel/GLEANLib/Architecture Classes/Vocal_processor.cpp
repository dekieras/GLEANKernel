#include "Vocal_processor.h"
#include "Coordinator.h"
#include "Vocal_event_types.h"
#include "Device_event_types.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "Syllable_counter.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"

#include <iostream>
#include <string>
#include <cmath>
//#include <cassert>
#include <list>

//#include <DebugNew.h>
using namespace std;

// make output pointers into a list to allow multiple recipients 6/14/99

// notes 7/23/98
// Speak assumes one feature prepared
// values for motor processor constants - reviewed 7/23/98

// calculated times are less one to align with next cognitive cycle.
const int Vocal_processor::initiation_time = 50;
const int Vocal_processor::feature_preparation_time = 50;
const int Vocal_processor::syllable_time = 150;

void Vocal_processor::initialize()
{
	word_counter = 0;
}

void Vocal_processor::accept_event(const Vocal_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	event_ptr->handle_self(this);
}

void Vocal_processor::handle_event(const Vocal_Speak_Command * event_ptr)
{
	long speaking_time = syllable_time * count_total_syllables((event_ptr->specification).str());
	
	if(Trace_out && get_trace()) 
		Trace_out << processor_info() << " Output: " << event_ptr->specification << " duration: " << speaking_time << endl;
	
	long completion_time = get_time() + initiation_time + feature_preparation_time + speaking_time;
	
	// time less one to align with next cognitive cycle.
	if ((completion_time % 50) == 0)
		completion_time--;

	// send the event at the completion time to the device
	word_counter++;
	
	Event * output_event_ptr = new Device_Vocal_event(completion_time, get_human_ptr()->get_device_ptr(), 
		event_ptr->specification);
		Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

void Vocal_processor::send_acknowledgement(int completion_time)
{	

	Event * event_ptr = new Cognitive_Vocal_Operation_Complete_event (completion_time, get_human_ptr()->get_cognitive_ptr());
		
	Coordinator::get_instance().schedule_event(event_ptr); 
	
}

