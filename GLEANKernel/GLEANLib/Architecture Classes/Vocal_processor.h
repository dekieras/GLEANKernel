#ifndef VOCAL_PROCESSOR_H
#define VOCAL_PROCESSOR_H

#include "Human_subprocessor.h"
#include "Vocal_event_types.h"
#include "Cognitive_processor.h"
#include "Device_processor.h"
#include "Auditory_processor.h"
#include "Syllable_counter.h"
#include <list>


// all output events are sent to the Device_processor associated with the Human_processor
// of which this is a member.

class Vocal_processor : public Human_subprocessor {
public:
	Vocal_processor(Human_processor * human_ptr) :
		Human_subprocessor("Vocal_processor", human_ptr),
		word_counter(0)
		{}
	~Vocal_processor() {}
	
	virtual void initialize();

	virtual void accept_event(const Vocal_event *);
	virtual void handle_event(const Vocal_Speak_Command *);

private:
	// private constants initialized in Vocal_processor.cpp
	static const int initiation_time;
	static const int feature_preparation_time;
	static const int syllable_time;

	void send_acknowledgement(int time_required);
	
	int word_counter;
};

#endif
