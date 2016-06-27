/*
The Recorder class defines a common interface for Recorders which is a subtype of Processors.
Recorders can be copied and assigned.
*/

#ifndef RECORDER_H
#define RECORDER_H

#include "Processor.h"
#include <string>

class Recorder : public Processor {
public:
	Recorder (const std::string& id) : 
		Processor(id, false) // do not add this processor to the processor list in the coordinator
		{}
	virtual ~Recorder() {}
	
	virtual void initialize() = 0;	// abstract base class

	virtual void report() const {}
	// return the string version of the report
	virtual std::string get_report() const 
		{return std::string();}
};

#endif

