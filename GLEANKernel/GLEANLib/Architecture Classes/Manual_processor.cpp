#include "Manual_processor.h"
#include "Manual_event_types.h"
#include "Device_event_types.h"
#include "Device_processor.h"
#include "Cognitive_event_types.h"
#include "Human_processor.h"
#include "Visual_processor.h"
#include "Coordinator.h"
#include "Geometry.h"
#include "Numeric_utilities.h"
#include "Symbol_Geometry_utilities.h"
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include "Glean_standard_symbols.h"

#include <iostream>
#include <string>
#include <cmath>
//#include <cassert>

namespace GU = Geometry_Utilities;

//#include <DebugNew.h>
using namespace std;

//notes 7/23/98
// modified_Welfords_Fitts_law assumes one feature prepared

// values for motor processor constants - reviewed 7/23/98
// time less one to align with next cognitive cycle.
// calculated times are checked and 1 substracted if necessary
const int Manual_processor::keystroke_time = 280;
const int Manual_processor::home_time = 400 - 1;
const int Manual_processor::button_time = 100 - 1;
const int Manual_processor::default_point_time = 1100 - 1;
const int Manual_processor::point_minimum = 100;
const int Manual_processor::point_coefficient = 100;
const int Manual_processor::button_held_point_coefficient = 145; 
const int Manual_processor::initiation_time = 50;
const int Manual_processor::feature_preparation_time = 50;

const Symbol Keyboard_c("Keyboard");
const Symbol Mouse_c("Mouse");

void Manual_processor::initialize()
{
	right_hand_position = Keyboard_c;
}

void Manual_processor::accept_event(const Manual_event * event_ptr)
{
	broadcast_to_recorders(event_ptr);
	// tell the event to handle itself with yourself!
	event_ptr->handle_self(this);
}

void Manual_processor::handle_event(const Manual_Keystroke_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Keystroke " << event_ptr->specification << endl;

	// Check the hand position and home if necessary
	int completion_time = get_time() + auto_home_to_keyboard() + keystroke_time;
		
	// send the keystroke at the completion time to the device
	
	Event * output_event_ptr = new Device_Keystroke_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	
	Coordinator::get_instance().schedule_event(output_event_ptr);
	
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Type_in_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Type_in " << event_ptr->specification << endl;
	// Check the hand position and home if necessary
	// specification is a string of characters to be typed
	// note that the specification could contain a number that doesn't have a c-string internal representation

	int completion_time = static_cast<int>(
		get_time() + auto_home_to_keyboard() + keystroke_time * (event_ptr->specification).str().length()
			);
// time less one to align with next cognitive cycle.
	if ((completion_time % 50) == 0)
		completion_time--;

	Event * output_event_ptr = new Device_Type_In_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Home_Command * event_ptr)
{	
	if(Trace_out && get_trace())
		Trace_out << processor_info() <<  " Home_to " << event_ptr->specification << endl;
	// specification is a destination for the movement
	right_hand_position = event_ptr->specification;
	int completion_time = get_time() + home_time;
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Hold_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Hold "	<< event_ptr->specification << endl;
	button_held = true;
	// specification is which button
	int completion_time = get_time() + auto_home_to_mouse() + button_time;
	Event * output_event_ptr = new Device_Hold_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Release_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Release " << event_ptr->specification << endl;
	button_held = false;
 	// specification is which button
	int completion_time = get_time() + auto_home_to_mouse() + button_time;
	Event * output_event_ptr = new Device_Release_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Click_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Click "	<< event_ptr->specification << endl;
	// specification is which button
	int completion_time = get_time() + auto_home_to_mouse() + 2 * button_time;
	Event * output_event_ptr = new Device_Click_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

void Manual_processor::handle_event(const Manual_Double_Click_Command * event_ptr)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Double Click "	<< event_ptr->specification << endl;
	// specification is which button
	int completion_time = get_time() + auto_home_to_mouse() + 4 * button_time;
	Event * output_event_ptr = new Device_Double_Click_event(completion_time, get_human_ptr()->get_device_ptr(), event_ptr->specification);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}



void Manual_processor::handle_event(const Manual_Point_Command * event_ptr)
{
	Symbol target_name = event_ptr->specification;
	Symbol phys_name = get_human_ptr()->get_Visual_ptr()->get_physical_name(target_name);
	
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Point to " 
		<< phys_name << '/' << target_name << endl;
	// specification is a destination for the movement
	// access destination size; if no size specified, default pointing time will be used and no locations will be accessed.
	int point_time;
	Symbol obj_size = get_human_ptr()->get_Visual_ptr()->retrieve(target_name, Size_c);
	if(obj_size.has_pair_numeric_value()) {
		GU::Size object_size = to_Size(obj_size);
		// access locations; if not specified, default to origins just to keep going
		Symbol cur_loc = get_human_ptr()->get_Visual_ptr()->retrieve(Cursor_name_c, Location_c);
		GU::Point cursor_location = (cur_loc.has_Point_numeric_value()) ? cur_loc.get_Point() : GU::Point(0, 0);		
		Symbol obj_loc = get_human_ptr()->get_Visual_ptr()->retrieve(target_name, Location_c);
		GU::Point object_location = (obj_loc.has_Point_numeric_value()) ? obj_loc.get_Point() : GU::Point(0, 0);
		point_time = long(pointing_time(cursor_location, object_location, object_size));
		if(Trace_out && get_trace())
			Trace_out << processor_info() << " pointing from: " << cursor_location << " to: " << object_location
			<< " with size: "<< object_size << " in " << point_time << " ms" << endl;
		}
	else {
		point_time = default_point_time;
		if(Trace_out && get_trace())
			Trace_out << processor_info() << 
			" pointing from and to unspecified locations with unspecified target size in " << point_time << " ms" << endl;
		}
		
	int completion_time = get_time() + auto_home_to_mouse() + point_time;
	
// time less one to align with next cognitive cycle.
	if ((completion_time % 50) == 0)
		completion_time--;
	Event * output_event_ptr = new Device_Point_event(completion_time, get_human_ptr()->get_device_ptr(), phys_name);
	Coordinator::get_instance().schedule_event(output_event_ptr);
	send_acknowledgement(completion_time);
	
}

// the target size and position is given by a location point and a size point
// the location (x, y) is the upper left corner
// the size (w, h) is the width, height
// the movement is made from the cursor location to the target center,
// the center is given by (x + w/2, y - h/2)


double Manual_processor::pointing_time(GU::Point cursor_location, GU::Point target_location, GU::Size target_size)
{
	// use coefficient depending on whether the button is being held down
	double coefficient = double((button_held) ? button_held_point_coefficient : point_coefficient);
	double target_d, target_s;
	
	// if the starting point is outside the target, calculate s & d, and corresponding Fitts time
	// if not, then pointing time is the minimum - why are we pointing to it if already there?
	if (get_movement_d_and_s (cursor_location,target_location, target_size, target_d, target_s))
		return modified_Welfords_Fitts_law(target_d, target_s, point_minimum, coefficient);
	else
		return point_minimum;
}

/*
Calculate the target size along the path of intersection using a specialization of
of the Cohen-Sutherland clipping algorithm (see Foley, van Damm, Feiner, & Hughes).

The mouse movement path is from the start point P0 to the end point P1, which is the
center of the rectangular target region, whose boundaries are min_edges.x, max_edges.x, 
min_edges.y, max_edges.y.

Since P1 is always inside the rectangle, we check only for whether P0 is also inside,
and then which edge of the rectangle the line crosses, and compute the point of
intersection.

cursor location is (x, y) of cursor; target_location is (x, y) of upper left corner, 
target_size is (x size, y size).
return true if starting point is outside and d and s values are valid; false otherwise
*/



// If the cursor is outside the target, return d, the distance from the cursor location 
// to the center of the target, and s, the size of the target along the line of motion.
// If the cursor is inside the target, return false;
bool Manual_processor::get_movement_d_and_s(GU::Point cursor_location, GU::Point target_location, GU::Size target_size, double& d, double& s)
{
	GU::Line_segment movement_line(cursor_location, target_location);
	GU::Line_segment clipped_line;
	if(compute_center_intersecting_line(movement_line, target_size, clipped_line)){
		// here if cursor location is outside the target
		d = movement_line.get_length();
		s = 2.* clipped_line.get_length();
		return true;
		}
	return false;
}



// modified Welford's law, parameterized
// index_of_difficulty = (distance/s) + .5
// check if target closer than S, 
//	if index_of_difficulty < 1., t = 0.
//	else, t = if slope * logb2(index_of_difficulty) // logb2 is my function for base two log
// pointing time = max(minimum, t)

double Manual_processor::modified_Welfords_Fitts_law(double d, double s, double minimum_time, double coefficient)
{
	double id = (d/s) + .5;
	double t_raw = (id < 1.) ? 0. : coefficient * logb2(id);	// log2 is C9X standard function
	t_raw = (t_raw > minimum_time) ? t_raw : minimum_time; // correct for minimum movement duration
// the feature time has been removed - dk, 6/9/07
//	double t = t_raw + initiation_time + feature_preparation_time; // approximate time for motor movement - only one feature?
	double t = t_raw + initiation_time;

	if(Trace_out && get_trace()) {
		Trace_out << processor_info() << " pointing d: " << d << " s: " << s
		<< " minimum: "<< minimum_time << " coeff: " << coefficient << " raw time: " << t_raw 
		<< " initiation: " << initiation_time << " total time: " << t << endl;
		}

	return t;
}

//double Manual_processor::logb2(double x) {return (log (x) / log(2.));}


void Manual_processor::send_acknowledgement(int completion_time)
{	
	Event * event_ptr = new Cognitive_Manual_Operation_Complete_event (completion_time, get_human_ptr()->get_cognitive_ptr());
	Coordinator::get_instance().schedule_event(event_ptr); 
}

int Manual_processor::auto_home_to_keyboard()
{
	if (right_hand_position != Keyboard_c) {
		right_hand_position = Keyboard_c;
		if(Trace_out && get_trace()) 
			Trace_out << processor_info() << " auto-homing right hand to keyboard" << endl;
		return home_time;
		}
	else return 0;
}

int Manual_processor::auto_home_to_mouse()
{
	if (right_hand_position != Mouse_c) {
		right_hand_position = Mouse_c;
		if(Trace_out && get_trace()) 
			Trace_out << processor_info() << " auto-homing right hand to mouse" << endl;
		return home_time;
		}
	else return 0;
}

