#ifndef GLEAN_STANDARD_SYMBOLS_H
#define GLEAN_STANDARD_SYMBOLS_H
#include "Symbol.h"
#include "Standard_Symbols.h"
#include "Standard_utility_symbols.h"

// include this file in any C++ code that needs to use Symbols commonly used in GLEAN models
// The actual symbol string value is the same as the name without the "_c" suffix.
// Add new values to this set if they are likely to be re-used.

/* indicator values - defined in Standard_utility_symbols.h
extern const Symbol Nil_c; // "Nil"
extern const Symbol Absent_c; // "Absent"
extern const Symbol None_c("None");
extern const Symbol Empty_string_c; // ""
*/

// Conventional properties and values for memory objects
extern const Symbol Event_type_c;
extern const Symbol In_focus_c;
extern const Symbol Out_of_focus_c;
extern const Symbol Information_c;


// conventional properties for Task objects
extern const Symbol First_c;
extern const Symbol Last_c;
extern const Symbol In_list_c;
extern const Symbol Previous_c;
// extern const Symbol Next_c;	// see standard symbols

// Used in operator definition - use with caution elsewhere
extern const Symbol Main_c;
extern const Symbol Error_c;
extern const Symbol Interrupt_c;
extern const Symbol Check__c;
extern const Symbol Interrupts__c;
extern const Symbol Check_c;
extern const Symbol Interrupts_c;
extern const Symbol Exception_name_tag_c;
extern const Symbol Exception_condition_tag_c;
extern const Symbol Exception_current_thread_tag_c;
extern const Symbol Exception_current_goal_action_tag_c;
extern const Symbol Exception_current_goal_object_tag_c;
extern const Symbol Exception_current_step_tag_c;

#endif
