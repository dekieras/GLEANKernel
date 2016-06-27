#include "Glean_exceptions.h"
#include "Processor.h"

Glean_exception::Glean_exception(const Processor * proc_ptr, const std::string& msg_) : 
		Exception(proc_ptr->processor_info() + " " + msg_)
{}
