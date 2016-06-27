// Simulation.cpp
#include "Simulation.h"
#include <string>
#include "Output_tee.h"
#include "Output_tee_globals.h"
#include <fstream>

using namespace std;

// static variables for output file streams
ofstream Simulation::glean_output_file;
ofstream Simulation::trace_output_file;
ofstream Simulation::detail_output_file;
ofstream Simulation::debug_output_file;

// prototype for top-level function for PCCTS-generated parser
GOMS_Model_Sptr_t parse_GOMSL_file(const char * filename);

Simulation::Simulation() :
execute_enabled(false), single_step(true)
{
//	Normal_out.add_stream(Normal_out);
}

Simulation::~Simulation()
{
	if (glean_output_file.is_open())
		glean_output_file.close();
	if (trace_output_file.is_open())
		trace_output_file.close();
	if (detail_output_file.is_open())
		detail_output_file.close();
	if (debug_output_file.is_open())
		debug_output_file.close();
}
	
void Simulation::run_top_level(bool workload_reports)	// top-level command-handling loop
{
	bool single_step = true;	
	char command;
	bool run = true;
	while (run) {
		Normal_out << "Enter command: c, l, d, r, s, q, R, ?: ";
		cin >> command;
		switch (command) {
			case 'c':
				compile();
				break;
			case 'l':
				calculate_learning_time();
				break;
			case 'd':
				display();
				break;
			case 'r':
				execute(workload_reports);
				break;
			case 's':
				if (single_step) {
					single_step = false;
					Normal_out << "single_step is off" << endl;
					set_single_step(single_step);
					}
				else {
					single_step = true;
					Normal_out << "single_step is on" << endl;
					set_single_step(single_step);
					}
				break;
			case 'q':
				Normal_out << "Quitting" << endl;
				run = false;
				break;
			case 'R':
				// ask for random seed
				Normal_out << "Enter a random generator seed value (integer): ";
				unsigned seed;
				cin >> seed;
				Normal_out << "Random seed: " << seed << endl;
				srand(seed);
				break;
			case '?':	// output list of commands
			default:
				Normal_out << "Valid commands:\n"
					<< "c(ompile) - compile method files\n"
					<< "l(earning analysis) - produce learning time analysis\n"
					<< "d(isplay) - show contents of GOMS model\n"
					<< "r(un) - execute GOMS model\n"
					<< "s(ingle step toggle) - change single step mode\n"
					<< "q(uit) - exit\n"
					<< "R(andom seed) - set the random generator seed\n"
					<< "? - this list"
					<< endl;
				break;
			}
		}
}
	
// function for doing output set-up
void Simulation::setup_output() 
{
	Normal_out << "Normal output will be sent to display." << endl;
	char command;
	Normal_out << "Normal, trace, detail, and debug will not be saved\n"
		 << "Change output settings? y(es) or n(o): ";
	cin >> command;
	if (command != 'y') {
		Normal_out << "Settings unchanged.\n" << endl;
		return;
		}
		
	Normal_out << "Save normal output only in file \"glean.out\"? y(es) or n(o): ";
	cin >> command;
	if (command == 'y') {
		if (open_file_stream(glean_output_file, "glean.out"))
			Normal_out.add_stream(glean_output_file);
		}
		
	Normal_out << "Save normal & trace output in file \"trace.out\"? y(es) or n(o): ";
	cin >> command;
	if (command == 'y') {
		if (open_file_stream(trace_output_file, "trace.out")) {
			Normal_out.add_stream(trace_output_file);
			Trace_out.add_stream(trace_output_file);
			}
		}
		
/*	Normal_out << "Save normal, trace, & detail output in file \"detail.out\"? y(es) or n(o): ";
	cin >> command;
	if (command == 'y') {
		if (open_file_stream(detail_output_file, "detail.out")) {
			Normal_out.add_stream(detail_output_file);	
			Trace_out.add_stream(detail_output_file);
			Detail_out.add_stream(detail_output_file);
			}
		}
*/		
/*	Normal_out << "Save normal, trace, & debug output in file \"debug.out\"? y(es) or n(o): ";
	cin >> command;
	if (command == 'y') {
		if (open_file_stream(debug_output_file, "debug.out")) {
			Normal_out.add_stream(debug_output_file);	
			Trace_out.add_stream(debug_output_file);
//			Debug_out.add_stream(debug_output_file);
			}
		}
*/
}

bool Simulation::open_file_stream(ofstream& ofs, const string& fn)
{
	ofs.open(fn.c_str());
	if (!ofs.good()) {
		Normal_out << "Could not open " << fn << endl;
		return false;
		}
	return true;
}


bool Simulation::compile(const string& model_filename, GOMS_Model_Sptr_t& model_sptr)
{
	Normal_out << "\nCompiling " << model_filename << endl;
	model_sptr = 0;
	model_sptr = parse_GOMSL_file(model_filename.c_str());	
	if (model_sptr) {
		Normal_out << "Model is executable." << endl;
		return true;
		}
	else {
		Normal_out << "Model is invalid due to errors." << endl;
		return false;
		}
}

void Simulation::calculate_learning_time(const string& model_filename, const GOMS_Model_Sptr_t model_sptr)
{
	if (model_sptr) {
		Normal_out << "\n" << model_filename << endl;
		Normal_out << model_sptr->get_learning_report();
		}
	else
		Normal_out << "Model is invalid - learning results can not be computed." << endl;
}

void Simulation::display(const string& model_filename, const GOMS_Model_Sptr_t model_sptr)
{
	if (model_sptr) {
		Normal_out << "\n" << model_filename << endl;
		model_sptr->display(Normal_out);
		(model_sptr->get_LTM()).display_contents(Normal_out);
		(model_sptr->get_initial_WM_contents()).display_contents(Normal_out);
		}
	else
		Normal_out << "Model is invalid - contents not shown." << endl;
}

