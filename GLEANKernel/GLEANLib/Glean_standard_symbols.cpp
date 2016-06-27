#include "Glean_standard_symbols.h"

// include this file in any C++ code that needs to use Symbols commonly used in GLEAN models
// The actual symbol string value is the same as the name without the "_c" suffix.
// Add new values to this set if they are likely to be re-used.

// conventional properties for Task objects
const Symbol First_c("First");
const Symbol Last_c("Last");
const Symbol In_list_c("In_list");
const Symbol Previous_c("Previous");
// const Symbol Next_c("Next");

// Conventional properties and values for memory objects
const Symbol Event_type_c ("Event_type");
const Symbol In_focus_c("In_focus");
const Symbol Out_of_focus_c("Out_of_focus");
const Symbol Information_c("Information");

// Used in operator definition - use with caution elsewhere
const Symbol Main_c("Main");
const Symbol Error_c("Error");
const Symbol Interrupt_c("Interrupt");

const Symbol Check__c("Check_");
const Symbol Interrupts__c("Interrupts_");
const Symbol Check_c("Check");
const Symbol Interrupts_c("Interrupts");
const Symbol Exception_name_tag_c("<Exception_name>");
const Symbol Exception_condition_tag_c("<Exception_condition>");
const Symbol Exception_current_thread_tag_c("<Exception_current_thread>");
const Symbol Exception_current_goal_action_tag_c("<Exception_current_goal_action>");
const Symbol Exception_current_goal_object_tag_c("<Exception_current_goal_object>");
const Symbol Exception_current_step_tag_c("<Exception_current_step>");

