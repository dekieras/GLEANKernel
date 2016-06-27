#ifndef MANUAL_PROCESSOR_H
#define MANUAL_PROCESSOR_H

#include "Human_subprocessor.h"
#include "Manual_event_types.h"
#include "Geometry.h"

namespace GU = Geometry_Utilities;

// all output events are sent to the Device_processor associated with the Human_processor
// of which this is a member.

class Manual_processor : public Human_subprocessor {
public:
	Manual_processor(Human_processor * human_ptr) :
		Human_subprocessor("Manual_processor", human_ptr), 
		right_hand_position("keyboard"), button_held(false)
		{}
	~Manual_processor() {}
	
	virtual void initialize();

	virtual void accept_event(const Manual_event *);
	virtual void handle_event(const Manual_Keystroke_Command *);
	virtual void handle_event(const Manual_Type_in_Command *);
	virtual void handle_event(const Manual_Home_Command *);
	virtual void handle_event(const Manual_Hold_Command *);
	virtual void handle_event(const Manual_Release_Command *);
	virtual void handle_event(const Manual_Click_Command *);
	virtual void handle_event(const Manual_Double_Click_Command *);
	virtual void handle_event(const Manual_Point_Command *);

private:
	// private constants initialized in Manual_processor.cpp
	//static const int keystroke_time = 280;
	static const int keystroke_time;
	static const int home_time;
	static const int button_time;
	//static const int button_time = 100 - 1;
	static const int default_point_time;
	static const int point_minimum;
	static const int point_coefficient;
	static const int button_held_point_coefficient;
	static const int initiation_time;
	static const int feature_preparation_time;

	void send_acknowledgement(int time_required);
	int auto_home_to_keyboard();
	int auto_home_to_mouse();
	double pointing_time(GU::Point cursor_location, GU::Point target_location, GU::Size target_size);
	bool get_movement_d_and_s(GU::Point cursor_location, GU::Point target_location, GU::Size target_size, double& d, double& s);
	double modified_Welfords_Fitts_law(double d, double s, double minimum_time, double coefficient);
//	double logb2(double x);


	Symbol right_hand_position;
	bool button_held;
};

#endif
