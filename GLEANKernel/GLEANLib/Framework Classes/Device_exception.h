#ifndef DEVICE_EXCEPTION_H
#define DEVICE_EXCEPTION_H

#include "Glean_exceptions.h"
#include "Device_base.h"

// The base class for Epic exceptions - this has a constructor to automatically
// prepend information about the processor throwing the exception
class Device_exception : public Glean_exception {
public:
	Device_exception(): Glean_exception("Unspecified Device_exception") {}
	Device_exception(const std::string& msg_) : Glean_exception(msg_) {}
	Device_exception(const Device_base * proc_ptr, const std::string& msg_)
		{msg = proc_ptr->processor_info() + msg_;}
};

#endif
