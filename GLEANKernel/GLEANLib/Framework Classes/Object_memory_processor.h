#ifndef OBJECT_MEMORY_PROCESSOR_H
#define OBJECT_MEMORY_PROCESSOR_H

#include "Human_subprocessor.h"
#include "Object_Store.h"

/*
Object_Memory is a base class for Human component Processors that keep a memory of objects 
with properties and values and need to communicate to a WM_processor and a Cognitive Processor. 
This class stores pointers to those processors, which have to be set before the functions are used.
*/

class Object_Memory_Processor : public Human_subprocessor, public Object_Store {
public:
	Object_Memory_Processor(const std::string& id, Human_processor * human_ptr) :
		Human_subprocessor(id, human_ptr)
		{}

	virtual ~Object_Memory_Processor() {}
	
	virtual void initialize();
	
	virtual void display_contents(Output_tee&) const;
	
protected:
	// services for derived classes
	
	// get the value under property name for the object, store in WM under tag name	
	void retrieve_and_store_value(const Symbol& object_name, const Symbol& property_name, 
		const Symbol& WM_property_name, long completion_time);			

	// find, store in object store, and under WM tag, and return name of object matching property-value list
	Symbol find_and_store_object(
		const Symbol_list_t& props, const Symbol_list_t& values, 
		const Symbol& WM_tag_name, long completion_time);
			
	// store name of object in WM object store
	void store_WM_object(const Symbol& object_name, long completion_time);
	
	// store name of object in WM under the tag
	void store_WM_object_under_tag(
		const Symbol& object_name, const Symbol& WM_tag_name, long completion_time);

	// send an operation complete event
//	void send_operation_complete(long completion_time);

	// remove tag from WM
	void erase_WM_tag(const Symbol& WM_tag_name, long completion_time);
	
	// remove WM object from WM
	void erase_WM_object(const Symbol& object_name, long completion_time);
};

#endif

