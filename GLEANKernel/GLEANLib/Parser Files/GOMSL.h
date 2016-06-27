/*
 * GOMSL: P a r s e r  H e a d e r 
 *
 * Generated from: gomslparser.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#ifndef GOMSL_h
#define GOMSL_h

#ifndef ANTLR_VERSION
#define ANTLR_VERSION 13333
#endif

#include "AParser.h"


#include "Symbol.h"
#include "Smart_Pointer.h"
#include "GOMS_Model.h"
#include "GOMS_Control_Operators.h"
#include "GOMS_Predicates.h"
#include "GOMS_Memory_Operators.h"
#include "GOMS_Task_Operators.h"
#include "GOMS_Manual_Operators.h"
#include "GOMS_Vocal_Operators.h"
#include "Output_tee_globals.h"
// GOMS_Model_Sptr_t parse_GOMSL_file(char * filename);	// main function prototype

  
class GOMSL : public ANTLRParser {
public:
	static  const ANTLRChar *tokenName(int tk);
	enum { SET_SIZE = 118 };
protected:
	static const ANTLRChar *_token_tbl[];
private:

private:
int number_of_syntax_errors;
std::string file_name;
public:
int get_number_of_syntax_errors () {return number_of_syntax_errors;}

void set_file_name(const std::string& f)
{
  file_name = f;
}

// override ANTLRParser's standard error reporting function to increment an error counter
virtual void syn(_ANTLRTokenPtr tok, ANTLRChar *egroup, SetWordType *eset, ANTLRTokenType etok, int k)
{
number_of_syntax_errors++;
// call the standard error reporting function
ANTLRParser::syn(tok, egroup, eset, etok, k);
}
protected:
	static SetWordType err1[16];
	static SetWordType err2[16];
	static SetWordType err3[16];
	static SetWordType setwd1[118];
	static SetWordType err4[16];
	static SetWordType err5[16];
	static SetWordType err6[16];
	static SetWordType err7[16];
	static SetWordType err8[16];
	static SetWordType err9[16];
	static SetWordType err10[16];
	static SetWordType err11[16];
	static SetWordType setwd2[118];
	static SetWordType err12[16];
	static SetWordType err13[16];
	static SetWordType err14[16];
	static SetWordType err15[16];
	static SetWordType err16[16];
	static SetWordType setwd3[118];
	static SetWordType err17[16];
	static SetWordType err18[16];
	static SetWordType err19[16];
	static SetWordType err20[16];
	static SetWordType err21[16];
	static SetWordType err22[16];
	static SetWordType err23[16];
	static SetWordType err24[16];
	static SetWordType setwd4[118];
	static SetWordType err25[16];
	static SetWordType err26[16];
	static SetWordType err27[16];
	static SetWordType err28[16];
	static SetWordType err29[16];
	static SetWordType setwd5[118];
	static SetWordType err30[16];
	static SetWordType err31[16];
	static SetWordType setwd6[118];
	static SetWordType err32[16];
	static SetWordType err33[16];
	static SetWordType err34[16];
	static SetWordType err35[16];
	static SetWordType err36[16];
	static SetWordType err37[16];
	static SetWordType err38[16];
	static SetWordType setwd7[118];
	static SetWordType err39[16];
	static SetWordType err40[16];
	static SetWordType err41[16];
	static SetWordType setwd8[118];
	static SetWordType err42[16];
	static SetWordType err43[16];
	static SetWordType err44[16];
	static SetWordType err45[16];
	static SetWordType setwd9[118];
	static SetWordType err46[16];
	static SetWordType err47[16];
	static SetWordType err48[16];
	static SetWordType err49[16];
	static SetWordType setwd10[118];
	static SetWordType err50[16];
	static SetWordType setwd11[118];
	static SetWordType setwd12[118];
	static SetWordType setwd13[118];
	static SetWordType setwd14[118];
	static SetWordType err51[16];
	static SetWordType setwd15[118];
	static SetWordType setwd16[118];
private:
	void zzdflthandlers( int _signal, int *_retsignal );

public:
	GOMSL(ANTLRTokenBuffer *input);
	GOMS_Goal goal(void);
	GOMS_Argument_Sptr_t simple_arg(void);
	void simple_arg_p(void);
	GOMS_Argument_Sptr_t step_specifier(void);
	GOMS_Argument_Sptr_t x_of_y_arg(void);
	void x_of_y_arg_p(void);
	GOMS_Argument_Sptr_t any_inp_arg(void);
	void any_inp_arg_p(void);
	GOMS_Argument_Sptr_t prop_value_arg(void);
	GOMS_Argument_Sptr_List_t prop_value_arg_list(void);
	GOMS_Argument_Sptr_List_t psdo_param_list(void);
	GOMS_Argument_Sptr_List_t psdo_arg_list(void);
	void ltm_def(GOMS_Model_Sptr_t model_sptr);
	void ltm_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s);
	void task_def(GOMS_Model_Sptr_t model_sptr);
	void task_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s);
	void visual_def(GOMS_Model_Sptr_t model_sptr);
	void visual_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s);
	void auditory_def(GOMS_Model_Sptr_t model_sptr);
	void auditory_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s);
	void initial_wm_def(GOMS_Model_Sptr_t model_sptr);
	void wm_prop_value(GOMS_Model_Sptr_t model_sptr);
	GOMS_Model_Sptr_t model(void);
	void method(GOMS_Model_Sptr_t model_sptr);
	void srs(GOMS_Model_Sptr_t model_sptr);
	GOMS_Conditional_Sptr_t sel_rule(void);
	void irs(GOMS_Model_Sptr_t model_sptr);
	GOMS_Conditional_Sptr_t interrupt_rule(void);
	void step(GOMS_Procedure_Sptr_t proc_sptr);
	void op_list(GOMS_Operator_Sptr_list_t& o_l);
	void singleton_op(GOMS_Operator_Sptr_list_t& o_l);
	void intra_op(GOMS_Operator_Sptr_list_t& o_l);
	void flow_op(GOMS_Operator_Sptr_list_t& o_l);
	void inter_op(GOMS_Operator_Sptr_list_t& o_l);
	GOMS_Operator_Sptr_t ag_op(void);
	GOMS_Operator_Sptr_t also_ag_op(void);
	GOMS_Operator_Sptr_t rga_op(void);
	GOMS_Operator_Sptr_t abort_restart_op(void);
	GOMS_Operator_Sptr_t resume_op(void);
	GOMS_Operator_Sptr_t raise_op(void);
	GOMS_Operator_Sptr_t stop_op(void);
	GOMS_Operator_Sptr_t goto_op(void);
	GOMS_Operator_Sptr_t decide_op(void);
	void decide_if(GOMS_Conditional_Sptr_list_t& d_i_l);
	void exists_pred(GOMS_Predicate_Sptr_list_t& p_l);
	void pred_list(GOMS_Predicate_Sptr_list_t& p_l);
	void decide_else(GOMS_Conditional_Sptr_list_t& d_i_l);
	void predicate(GOMS_Predicate_Sptr_list_t& p_l);
	GOMS_Predicate_Sptr_t is_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Predicate_Sptr_t is_not_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Predicate_Sptr_t is_greater_than_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Predicate_Sptr_t is_greater_than_or_equal_to_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Predicate_Sptr_t is_less_than_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Predicate_Sptr_t is_less_than_or_equal_to_predicate(GOMS_Argument_Sptr_t a1_p);
	GOMS_Operator_Sptr_t store_op(void);
	GOMS_Operator_Sptr_t delete_op(void);
	GOMS_Operator_Sptr_t log_op(void);
	GOMS_Operator_Sptr_t rltm_op(void);
	GOMS_Operator_Sptr_t look_at_op(void);
	GOMS_Operator_Sptr_t recall_op(void);
	GOMS_Operator_Sptr_t look_for_op(void);
	GOMS_Operator_Sptr_t check_visual_op(void);
	GOMS_Operator_Sptr_t wait_visual_op(void);
	GOMS_Operator_Sptr_t check_auditory_op(void);
	GOMS_Operator_Sptr_t wait_auditory_op(void);
	GOMS_Operator_Sptr_t wait_any_auditory_op(void);
	GOMS_Operator_Sptr_t get_task_op(void);
	GOMS_Operator_Sptr_t write_task_op(void);
	GOMS_Operator_Sptr_t update_task_op(void);
	GOMS_Operator_Sptr_t erase_task_prop_op(void);
	GOMS_Operator_Sptr_t erase_task_object_op(void);
	GOMS_Operator_Sptr_t create_task_list_op(void);
	GOMS_Operator_Sptr_t erase_task_list_op(void);
	GOMS_Operator_Sptr_t get_size_of_task_list_op(void);
	GOMS_Operator_Sptr_t find_task_item_whose_op(void);
	GOMS_Operator_Sptr_t find_first_item_op(void);
	GOMS_Operator_Sptr_t find_last_item_op(void);
	GOMS_Operator_Sptr_t find_item_after_op(void);
	GOMS_Operator_Sptr_t find_item_before_op(void);
	GOMS_Operator_Sptr_t append_item_op(void);
	GOMS_Operator_Sptr_t preprend_item_op(void);
	GOMS_Operator_Sptr_t insert_item_before_op(void);
	GOMS_Operator_Sptr_t remove_item_op(void);
	GOMS_Operator_Sptr_t remove_first_op(void);
	GOMS_Operator_Sptr_t remove_last_op(void);
	GOMS_Operator_Sptr_t mental_op(void);
	GOMS_Operator_Sptr_t key_op(void);
	GOMS_Operator_Sptr_t type_op(void);
	GOMS_Operator_Sptr_t home_op(void);
	GOMS_Operator_Sptr_t point_op(void);
	GOMS_Operator_Sptr_t hold_op(void);
	GOMS_Operator_Sptr_t release_op(void);
	GOMS_Operator_Sptr_t click_op(void);
	GOMS_Operator_Sptr_t dbl_click_op(void);
	GOMS_Operator_Sptr_t speak_op(void);
};

#endif /* GOMSL_h */
