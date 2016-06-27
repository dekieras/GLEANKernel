#ifndef GLEAN_EXCEPTIONS_H
#define GLEAN_EXCEPTIONS_H

#include "Exception.h"
#include <string>

class Processor;

// The base class for Glean exceptions - this has a constructor to automatically
// prepend information about the processor throwing the exception
class Glean_exception : public Exception {
public:
	Glean_exception(): Exception("Unspecified Glean_exception") {}
	Glean_exception(const std::string& msg_) : Exception(msg_) {}
	Glean_exception(const Processor * proc_ptr, const std::string& msg_);
};


/*class Device_exception : public Glean_exception {
public:
	Device_exception(const std::string& msg_) :
		Glean_exception(msg_)
		{}
	Device_exception(const Processor * proc_ptr, const std::string& msg_) :
		Glean_exception(proc_ptr, msg_)
		{}
};
*/
class Glean_user_error : public Glean_exception {
public:
	Glean_user_error(): Glean_exception("Unspecified Glean_user_error") {}
	Glean_user_error(const std::string& msg_) : Glean_exception(msg_) {}
};


class Glean_internal_error : public Glean_exception {
public:
	Glean_internal_error(): Glean_exception("Unspecified Glean_internal_error") {}
	Glean_internal_error(const std::string& msg_) : Glean_exception(msg_) {}
	Glean_internal_error(const Processor * proc_ptr, const std::string& msg_) :
		Glean_exception(proc_ptr, std::string("*** Glean Internal Error ") + msg_)
		{}
};

#endif
