#ifndef VISUAL_PROCESSOR_H
#define VISUAL_PROCESSOR_H

#include "Object_Memory_Processor.h"
#include "Visual_event_types.h"
#include "Symbol.h"
#include "Name_map.h"

class Visual_processor : public Object_Memory_Processor {
public:
	// no public default constructor
	// initialize both the regular memory and the initial value memory
	Visual_processor(Human_processor * human_ptr) :
		Object_Memory_Processor("Visual_processor", human_ptr),
		waiting_for_object(false), psychobj_counter(0)
		{}
	
	virtual void initialize();
	virtual void display_contents(Output_tee&) const;
	virtual void display_object_info(Output_tee&) const;
	virtual void accept_event(const Visual_event *);	// override Processor definition
	virtual void handle_event(const Visual_Input_Object_Appearance_event *);
	virtual void handle_event(const Visual_Input_Object_Change_Symbol_event *);
//	virtual void handle_event(const Visual_Input_Object_Change_Spatial_event *);
	virtual void handle_event(const Visual_Input_Object_Disappearance_event *);
	virtual void handle_event(const Visual_Event_Type_Erasure_event *);
	virtual void handle_event(const Visual_Input_Erasure_event *);
	virtual void handle_event(const Visual_Get_Value_event *);
	virtual void handle_event(const Visual_Find_Object_event *);
	virtual void handle_event(const Visual_Get_Object_event *);
	virtual void handle_event(const Visual_Wait_Object_event *);
	virtual void handle_event(const Visual_Look_at_Object_event *);
	
	// accessor for name of object that is currently in focus
	const Symbol& get_in_focus_object_name() const {return in_focus_object_name;}
	// acccessor for physical name of object
	Symbol get_physical_name(const Symbol& psych_name) const
		{return name_map.get_physical_name(psych_name);}

private:

	// private constants
	static const int visual_find_time;
	static const int visual_waitfor_time;
	static const int visual_refind_time;
	static const int visual_check_time;
	static const int visual_erasure_time;
	static const int visual_event_type_retention_time;
	static const int visual_fixation_duration;

	Symbol in_focus_object_name;	// holds the object name of the current in_focus object
	GU::Point current_eye_location;	// set to location of in_focus_object whenever changed

	bool waiting_for_object;
	Symbol_list_t waiting_for_property_names;
	Symbol_list_t waiting_for_property_values;
	Symbol waiting_for_tagname;
	long psychobj_counter;	// counter for new object psychologial names
	Name_map name_map;	// name map for visual objects
	
	// send an operation complete event
	void send_operation_complete(long completion_time);
	void change_focus(const Symbol& object_name);
	bool check_for_salient_change(const Symbol& object_name, const Symbol& property_name);
	Symbol change_related_object_name(const Symbol& prop_name, const Symbol& prop_value);
	// functions for estimating search time
	struct Search_object {
		Search_object(const Symbol& name_, GU::Point location_, double size_) :
			name(name_), location(location_), size(size_) {}
		Symbol name;
		GU::Point location;
		double size;
		Symbol color;
	};

	typedef std::vector<Search_object> Search_objects_t;

	long estimate_search_time(const Symbol& target_name, const Symbol_list_t& prop_names, const Symbol_list_t prop_values);
	int estimate_fixations_required(const Symbol& target_name, const Symbol_list_t& prop_names, const Symbol_list_t prop_values);
	bool accuity_function(double eccentricity, double obj_size);

};

#endif
