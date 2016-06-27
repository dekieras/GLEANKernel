#ifndef GOMS_PREDICATES_H
#define GOMS_PREDICATES_H

#include "GOMS_Model.h"

// forward reference
class Cognitive_processor;

// predicate classes

class GOMS_Is_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Predicate () 
		{}
	GOMS_Is_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) == (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

class GOMS_Is_Not_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Not_Predicate () 
		{}
	GOMS_Is_Not_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) != (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

class GOMS_Is_Greater_Than_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Greater_Than_Predicate () 
		{}
	GOMS_Is_Greater_Than_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) > (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

class GOMS_Is_Greater_Than_Or_Equal_To_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Greater_Than_Or_Equal_To_Predicate () 
		{}
	GOMS_Is_Greater_Than_Or_Equal_To_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) >= (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

class GOMS_Is_Less_Than_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Less_Than_Predicate () 
		{}
	GOMS_Is_Less_Than_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) < (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

class GOMS_Is_Less_Than_Or_Equal_To_Predicate : public GOMS_Predicate {
public:
	GOMS_Is_Less_Than_Or_Equal_To_Predicate () 
		{}
	GOMS_Is_Less_Than_Or_Equal_To_Predicate (const GOMS_Argument_Sptr_t& x, const GOMS_Argument_Sptr_t& y) :
		GOMS_Predicate(x, y)
		{}
	virtual bool evaluate(Cognitive_processor * cp)
		{return (first_arg->resolve(cp)) <= (second_arg->resolve(cp));}
	virtual void display(Output_tee&) const;
	
};

// this predicate is different - has only one argument (a tagname) and then a list of other predicates
class GOMS_Exists_Predicate : public GOMS_Predicate {
public:
	GOMS_Exists_Predicate () 
		{}
	GOMS_Exists_Predicate (const GOMS_Argument_Sptr_t& tag, const GOMS_Predicate_Sptr_list_t& pl) :
		GOMS_Predicate(tag, tag), predicates(pl)
		{}
	virtual bool evaluate(Cognitive_processor * cp);
	virtual void display(Output_tee&) const;
	bool similar(const GOMS_Predicate *, const Correspondence_map_t&) const;
	
protected:
	GOMS_Predicate_Sptr_list_t predicates;
};

#endif

