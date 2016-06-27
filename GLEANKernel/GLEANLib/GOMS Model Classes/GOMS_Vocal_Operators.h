#ifndef GOMS_VOCAL_OPERATORS_H
#define GOMS_VOCAL_OPERATORS_H

#include "GOMS_Model.h"

// forward reference
class Cognitive_processor;

// if addition vocal operators, 
// add a base class to emulate class structure of manual operators
class GOMS_Speak_Operator : public GOMS_Operator {
public:
	GOMS_Speak_Operator () : 
		specification_sptr(new GOMS_Constant_Argument) 
		{}
	GOMS_Speak_Operator (GOMS_Argument_Sptr_t wrd) :
		specification_sptr(wrd)
		{}

	virtual void execute(Cognitive_processor * cog_proc_ptr, GOMS_Execution_Thread * thread_ptr);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t word_sptr() const {return specification_sptr;}
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	virtual void request_processors(Processor_Request&) const;

private:
	GOMS_Argument_Sptr_t specification_sptr;
};

#endif
