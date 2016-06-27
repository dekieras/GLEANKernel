#ifndef GOMS_MANUAL_OPERATORS_H
#define GOMS_MANUAL_OPERATORS_H

#include "GOMS_Model.h"

// forward reference
class Cognitive_processor;
class Event;

class GOMS_Manual_Operator : public GOMS_Operator {
public:
	GOMS_Manual_Operator () : 
		specification_sptr(new GOMS_Constant_Argument) 
		{}
	GOMS_Manual_Operator (GOMS_Argument_Sptr_t spec) :
		specification_sptr(spec)
		{}
	virtual ~GOMS_Manual_Operator () 
		{}
	virtual bool similar(const GOMS_Operator *, const Correspondence_map_t&) const;
	
	virtual void request_processors(Processor_Request&) const;
	
protected:
	GOMS_Argument_Sptr_t specification_sptr;
	virtual void finish_operation(Cognitive_processor *, GOMS_Execution_Thread *, Event *);

};


class GOMS_Keystroke_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Keystroke_Operator () 
		{}
	GOMS_Keystroke_Operator (GOMS_Argument_Sptr_t kp) :
		GOMS_Manual_Operator(kp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t key_name_sptr() const {return specification_sptr;}
};

class GOMS_Type_In_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Type_In_Operator () 
		{}
	GOMS_Type_In_Operator (GOMS_Argument_Sptr_t tp) :
		GOMS_Manual_Operator(tp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t type_in_sptr() const {return specification_sptr;}
};

class GOMS_Home_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Home_Operator () 
		{}
	GOMS_Home_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t dest_sptr() const {return specification_sptr;}
};


class GOMS_Hold_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Hold_Operator () 
		{}
	GOMS_Hold_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t button_sptr() const {return specification_sptr;}
};

class GOMS_Release_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Release_Operator () 
		{}
	GOMS_Release_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}
	~GOMS_Release_Operator () 
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t button_sptr() const {return specification_sptr;}
};

class GOMS_Click_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Click_Operator () 
		{}
	GOMS_Click_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t button_sptr() const {return specification_sptr;}
};

class GOMS_Double_Click_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Double_Click_Operator () 
		{}
	GOMS_Double_Click_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t button_sptr() const {return specification_sptr;}
};


class GOMS_Point_Operator : public GOMS_Manual_Operator {
public:
	GOMS_Point_Operator ()
		{}
	GOMS_Point_Operator (GOMS_Argument_Sptr_t dp) :
		GOMS_Manual_Operator(dp)
		{}

	virtual void execute(Cognitive_processor *, GOMS_Execution_Thread *);
	virtual void display(Output_tee& ot) const;
	GOMS_Argument_Sptr_t dest_sptr() const {return specification_sptr;}

};

#endif
