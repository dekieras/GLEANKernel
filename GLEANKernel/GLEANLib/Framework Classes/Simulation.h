// Simulation.h
/*
An abstract base class for encapsulating a simulation
*/

#ifndef SIMULATION_H
#define SIMULATION_H
#include <iostream>
#include <string>
#include "GOMS_Model.h"
#include <list>

class View_base;

class Simulation {
public:
	Simulation();
	virtual ~Simulation();
	
	virtual void run_top_level(bool workload_reports = false);	// top-level command-handling loop
	virtual bool compile() = 0;		// abstract class
	virtual void execute(bool workload_reports = false) = 0;
	virtual void calculate_learning_time() = 0;
 	virtual void display() = 0;
 	virtual void set_single_step(bool ss) {single_step = ss;}

	// For managing attached visual views
	virtual void add_visual_view(View_base* view)    { visualViews.push_back(view); }
	virtual void remove_visual_view(View_base* view) { visualViews.remove(view);     }

// function for doing output set-up
	static void setup_output();

// static variables for output file streams
	static std::ofstream glean_output_file;
	static std::ofstream trace_output_file;
	static std::ofstream detail_output_file;
	static std::ofstream debug_output_file;
	static bool open_file_stream(std::ofstream& ofs, const std::string& fn);

protected:
	// services for subclasses
	bool compile(const std::string& model_filename, GOMS_Model_Sptr_t& model_sptr);
	void calculate_learning_time(const std::string& model_filename, const GOMS_Model_Sptr_t model_sptr);
	void display(const std::string& model_filename, const GOMS_Model_Sptr_t model_sptr);

	bool execute_enabled;	// false unless both models are good
	bool single_step;		// current single-step state for simulation as a whole

	std::list<View_base*> visualViews;
};


#endif

