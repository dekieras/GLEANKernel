/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   /Users/kieras/Documents/Programming/pccts 1.33mr33/bin/antlr GOMSL.txt -CC -k 2 -nopurify
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "tokens.h"

#include "Symbol.h"
#include "Smart_Pointer.h"
#include "GOMS_Model.h"
#include "GOMS_Control_Operators.h"
#include "GOMS_Predicates.h"
#include "GOMS_Memory_Operators.h"
#include "GOMS_Task_Operators.h"
#include "GOMS_Manual_Operators.h"
#include "GOMS_Vocal_Operators.h"
#include "GOMS_High_Level_Operators.h"
#include "Output_tee_globals.h"

#include <iostream>
using std::endl;

// GOMS_Model_Sptr_t parse_GOMSL_file(char * filename);	// main function prototype

  
#include "AParser.h"
#include "GOMSL_parser.h"
#include "DLexerBase.h"
#include "ATokPtr.h"

#include "DLGLexer.h"		/* include definition of DLGLexer.
* This cannot be generated automatically because
* ANTLR has no idea what you will call this file
* with the DLG command-line options.
*/

typedef ANTLRCommonToken ANTLRToken;

#include <iostream>
//#include <assert>
using namespace std;

GOMS_Model_Sptr_t parse_GOMSL_file(const char * filename);

GOMS_Model_Sptr_t parse_GOMSL_file(const char * filename)
{
  ANTLRTokenPtr aToken = new ANTLRToken;
  FILE * input_file = fopen(filename, "r");
  if (input_file == NULL) {
    Normal_out << "Could not open input file: " << filename << endl;
    return 0;
  }
  //assert(input_file != NULL);
  DLGFileInput in(input_file);
  DLGLexer scan(&in);
  ANTLRTokenBuffer pipe(&scan);
  scan.setToken(mytoken(aToken));
  GOMSL_parser parser(&pipe);
  parser.set_file_name(filename);
  parser.init();
  GOMS_Model_Sptr_t model_sptr;
  model_sptr = parser.model();
  Normal_out << "Parsing done." << endl;
  fclose(input_file);
  
	if (model_sptr == 0) {
    Normal_out << "Discovered " << parser.get_number_of_syntax_errors() << " syntax errors - model is not valid!" << endl;
    return 0;
  }
  else {
    Normal_out << "Model was parsed successfully." << endl;
    return model_sptr;
  }
}

  

GOMS_Goal
GOMSL_parser::goal(void)
{
  GOMS_Goal   _retv;
  zzRULE;
  ANTLRTokenPtr _t11=NULL,_t12=NULL;
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t12 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  _retv = GOMS_Goal(_t11->getText(), _t12->getText());
  // $g.display();
  // Normal_out << "goal found" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** goal not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x1);
  return _retv;
}

GOMS_Argument_Sptr_t
GOMSL_parser::simple_arg(void)
{
  GOMS_Argument_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  if ( (LA(1)==TAGNAME) ) {
    zzmatch(TAGNAME);    
    if ( !guessing ) {
     _t11 = (ANTLRTokenPtr)LT(1);
    }

    if ( !guessing ) {
    
    _retv = new GOMS_WM_Argument(_t11->getText());
    }
 consume();
  }
  else {
    if ( (LA(1)==STRING) ) {
      zzmatch(STRING);      
      if ( !guessing ) {
       _t11 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      
      _retv = new GOMS_Constant_Argument(_t11->getText());
      }
 consume();
    }
    else {
      if ( (LA(1)==IDENTIFIER) ) {
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t11 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        
        _retv = new GOMS_Constant_Argument(_t11->getText());
        }
 consume();
        if ( !guessing ) {
        
        // Normal_out << "simple_arg built" << endl;
        // $a_p->display();
        }
      }
      else {FAIL(1,err1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** simple_arg not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x2);
  return _retv;
}

void
GOMSL_parser::simple_arg_p(void)
{
  zzRULE;
  if ( (LA(1)==TAGNAME) ) {
    zzmatch(TAGNAME); consume();
  }
  else {
    if ( (LA(1)==STRING)
 ) {
      zzmatch(STRING); consume();
    }
    else {
      if ( (LA(1)==IDENTIFIER) ) {
        zzmatch(IDENTIFIER); consume();
      }
      else {FAIL(1,err2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x4);
}

GOMS_Argument_Sptr_t
GOMSL_parser::step_specifier(void)
{
  GOMS_Argument_Sptr_t   _retv;
  zzRULE;
  {
    ANTLRTokenPtr _t21=NULL;
    if ( (LA(1)==TAGNAME) ) {
      zzmatch(TAGNAME);      
      if ( !guessing ) {
       _t21 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      
      _retv = new GOMS_WM_Argument(_t21->getText());
      }
 consume();
    }
    else {
      if ( (LA(1)==STRING) ) {
        zzmatch(STRING);        
        if ( !guessing ) {
         _t21 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        
        _retv = new GOMS_Constant_Argument(_t21->getText());
        }
 consume();
      }
      else {
        if ( (LA(1)==IDENTIFIER) ) {
          zzmatch(IDENTIFIER);          
          if ( !guessing ) {
           _t21 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          
          _retv = new GOMS_Constant_Argument(_t21->getText());
          }
 consume();
        }
        else {
          if ( (LA(1)==NUMBER)
 ) {
            zzmatch(NUMBER);            
            if ( !guessing ) {
             _t21 = (ANTLRTokenPtr)LT(1);
            }

            if ( !guessing ) {
            
            _retv = new GOMS_Constant_Argument(_t21->getText());
            }
 consume();
          }
          else {FAIL(1,err3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
  }
  if ( !guessing ) {
  
  // Normal_out << "step_specifier built" << endl;
  // $a_p->display();
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** step_specifier argument not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x8);
  return _retv;
}

GOMS_Argument_Sptr_t
GOMSL_parser::x_of_y_arg(void)
{
  GOMS_Argument_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t x_p; GOMS_Argument_Sptr_t y_p;
  if ( !guessing ) {
     x_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(OF); consume();
  if ( !guessing ) {
     y_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_PO_Argument(x_p, y_p); 
  // Normal_out << "x_of_y_arg built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** x_of_y_arg not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x10);
  return _retv;
}

void
GOMSL_parser::x_of_y_arg_p(void)
{
  zzRULE;
  simple_arg_p();
  zzmatch(OF); consume();
  simple_arg_p();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x20);
}

GOMS_Argument_Sptr_t
GOMSL_parser::any_inp_arg(void)
{
  GOMS_Argument_Sptr_t   _retv;
  zzRULE;
  zzGUESS_BLOCK
  zzGUESS
  if ( !zzrv && (setwd1[LA(1)]&0x40) && (LA(2)==OF) ) {
    {
      x_of_y_arg_p();
    }
    zzGUESS_DONE
    if ( !guessing ) {
       _retv  = x_of_y_arg();

    } else {
      x_of_y_arg();
    }
  }
  else {
    if ( !zzrv ) zzGUESS_DONE;
    if ( (setwd1[LA(1)]&0x80) && (setwd2[LA(2)]&0x1) ) {
      if ( !guessing ) {
         _retv  = simple_arg();

      } else {
        simple_arg();
      }
      if ( !guessing ) {
      
      // Normal_out << "any_inp_arg built" << endl;
      }
    }
    else {FAIL(2,err4,err5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** any_inp_arg not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x2);
  return _retv;
}

void
GOMSL_parser::any_inp_arg_p(void)
{
  zzRULE;
  zzGUESS_BLOCK
  zzGUESS
  if ( !zzrv && (setwd2[LA(1)]&0x4) && (LA(2)==OF) ) {
    {
      x_of_y_arg_p();
    }
    zzGUESS_DONE
    x_of_y_arg_p();
  }
  else {
    if ( !zzrv ) zzGUESS_DONE;
    if ( (setwd2[LA(1)]&0x8) && (LA(2)==1) ) {
      simple_arg_p();
    }
    else {FAIL(2,err6,err7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x10);
}

GOMS_Argument_Sptr_t
GOMSL_parser::prop_value_arg(void)
{
  GOMS_Argument_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t x_p; GOMS_Argument_Sptr_t y_p;
  if ( !guessing ) {
     x_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(IS); consume();
  if ( !guessing ) {
     y_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_PV_Argument(x_p, y_p); 
  // Normal_out << "prop_value built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** prop_value_arg not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x20);
  return _retv;
}

GOMS_Argument_Sptr_List_t
GOMSL_parser::prop_value_arg_list(void)
{
  GOMS_Argument_Sptr_List_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  if ( !guessing ) {
     a_p  = prop_value_arg();

  } else {
    prop_value_arg();
  }
  if ( !guessing ) {
  
  _retv.push_back(a_p);
  // Normal_out << "prop_value_arg_list item built" << endl;
  }
  {
    while ( (LA(1)==COMMA)
 ) {
      {
        if ( (LA(1)==COMMA) && (setwd2[LA(2)]&0x40) ) {
          zzmatch(COMMA); consume();
        }
        else {
          if ( (LA(1)==COMMA) && (LA(2)==AND) ) {
            {
              zzmatch(COMMA); consume();
              zzmatch(AND); consume();
            }
          }
          else {FAIL(2,err8,err9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      if ( !guessing ) {
         a_p  = prop_value_arg();

      } else {
        prop_value_arg();
      }
      if ( !guessing ) {
      
      _retv.push_back(a_p);
      // Normal_out << "prop_value_arg_list item built" << endl;
      }
    }
  }
  if ( !guessing ) {
  
  // Normal_out << "prop_value_arg_list found" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** prop_value_arg_list not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x80);
  return _retv;
}

GOMS_Argument_Sptr_List_t
GOMSL_parser::psdo_param_list(void)
{
  GOMS_Argument_Sptr_List_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  a_p = new GOMS_WM_Argument(_t11->getText());
  _retv.push_back(a_p);
  // Normal_out << "psdo_param_list item built" << endl;
  }
 consume();
  {
    ANTLRTokenPtr _t22=NULL;
    while ( (LA(1)==COMMA) ) {
      {
        if ( (LA(1)==COMMA) && (LA(2)==TAGNAME) ) {
          zzmatch(COMMA); consume();
        }
        else {
          if ( (LA(1)==COMMA) && 
(LA(2)==AND) ) {
            {
              zzmatch(COMMA); consume();
              zzmatch(AND); consume();
            }
          }
          else {FAIL(2,err10,err11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      zzmatch(TAGNAME);      
      if ( !guessing ) {
       _t22 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      
      a_p = new GOMS_WM_Argument(_t22->getText());
      _retv.push_back(a_p);
      // Normal_out << "psdo_param_list item built" << endl;
      }
 consume();
    }
  }
  if ( !guessing ) {
  
  // Normal_out << "psdo_param_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** psdo_param_list not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x1);
  return _retv;
}

GOMS_Argument_Sptr_List_t
GOMSL_parser::psdo_arg_list(void)
{
  GOMS_Argument_Sptr_List_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  if ( !guessing ) {
     a_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv.push_back(a_p);
  // Normal_out << "psdo_arg_list item built" << endl;
  }
  {
    while ( (LA(1)==COMMA) ) {
      {
        if ( (LA(1)==COMMA) && (setwd3[LA(2)]&0x2) ) {
          zzmatch(COMMA); consume();
        }
        else {
          if ( (LA(1)==COMMA) && (LA(2)==AND) ) {
            {
              zzmatch(COMMA); consume();
              zzmatch(AND); consume();
            }
          }
          else {FAIL(2,err12,err13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      if ( !guessing ) {
         a_p  = any_inp_arg();

      } else {
        any_inp_arg();
      }
      if ( !guessing ) {
      
      _retv.push_back(a_p);
      // Normal_out << "psdo_arg_list item built" << endl;
      }
    }
  }
  if ( !guessing ) {
  
  // Normal_out << "psdo_arg_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** psdo_arg_list not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x4);
  return _retv;
}

void
GOMSL_parser::ltm_def(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  Symbol o_s;
  {
    ANTLRTokenPtr _t23=NULL;
    zzmatch(DEFLTM); consume();
    zzmatch(COLON); consume();
    zzmatch(IDENTIFIER);    
    if ( !guessing ) {
     _t23 = (ANTLRTokenPtr)LT(1);
    }

    if ( !guessing ) {
    o_s = _t23->getText();
    }
 consume();
  }
  {
    int zzcnt=1;
    do {
      ltm_prop_value( model_sptr, o_s );
    } while ( (LA(1)==IDENTIFIER) );
  }
  if ( !guessing ) {
  
  // Normal_out << "ltm_def found" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** ltm_def not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x8);
}

void
GOMSL_parser::ltm_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol v_s;
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IS); consume();
  {
    if ( (LA(1)==IDENTIFIER)
 ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        v_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING) ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          v_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  (model_sptr->theLTM).store(o_s, _t11->getText(), v_s);
  // Normal_out << "ltm_prop_value found" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** ltm_prop_value not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x10);
}

void
GOMSL_parser::task_def(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  Symbol o_s;
  {
    ANTLRTokenPtr _t23=NULL;
    zzmatch(DEFTASK); consume();
    zzmatch(COLON); consume();
    zzmatch(IDENTIFIER);    
    if ( !guessing ) {
     _t23 = (ANTLRTokenPtr)LT(1);
    }

    if ( !guessing ) {
    o_s = _t23->getText();
    }
 consume();
  }
  {
    int zzcnt=1;
    do {
      task_prop_value( model_sptr, o_s );
    } while ( (LA(1)==IDENTIFIER) );
  }
  if ( !guessing ) {
  
  // Normal_out << "task_def found" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** task_def not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x20);
}

void
GOMSL_parser::task_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol v_s;
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IS); consume();
  {
    if ( (LA(1)==IDENTIFIER) ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        v_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING) ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          v_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err15,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  (model_sptr->task_description).store(o_s, _t11->getText(), v_s);
  // Normal_out << "task_prop_value found" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** task_prop_value not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x40);
}

void
GOMSL_parser::visual_def(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  Symbol o_s;
  {
    ANTLRTokenPtr _t23=NULL;
    zzmatch(DEFVISUAL); consume();
    zzmatch(COLON); consume();
    zzmatch(IDENTIFIER);    
    if ( !guessing ) {
     _t23 = (ANTLRTokenPtr)LT(1);
    }

    if ( !guessing ) {
    o_s = _t23->getText();
    }
 consume();
  }
  {
    int zzcnt=1;
    do {
      visual_prop_value( model_sptr, o_s );
    } while ( (LA(1)==IDENTIFIER)
 );
  }
  if ( !guessing ) {
  
  // Normal_out << "visual_def found" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** visual_def not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x80);
}

void
GOMSL_parser::visual_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol v_s;
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IS); consume();
  {
    if ( (LA(1)==IDENTIFIER) ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        v_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING) ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          v_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  (model_sptr->initial_visual_state).store(o_s, _t11->getText(), v_s);
  // Normal_out << "visual_prop_value found" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** visual_prop_value not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x1);
}

void
GOMSL_parser::auditory_def(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  Symbol o_s;
  zzmatch(DEFAUDITORY); consume();
  zzmatch(COLON); consume();
  {
    if ( (LA(1)==IDENTIFIER) ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        o_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING) ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          o_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err17,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    int zzcnt=1;
    do {
      auditory_prop_value( model_sptr, o_s );
    } while ( (LA(1)==IDENTIFIER)
 );
  }
  if ( !guessing ) {
  
  // Normal_out << "auditory_def found" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** auditory_def not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x2);
}

void
GOMSL_parser::auditory_prop_value(GOMS_Model_Sptr_t model_sptr,Symbol o_s)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol a_s;
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IS); consume();
  {
    if ( (LA(1)==IDENTIFIER) ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        a_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING) ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          a_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err18,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  (model_sptr->initial_auditory_state).store(o_s, _t11->getText(), a_s);
  // Normal_out << "auditory_prop_value found" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** auditory_prop_value not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x4);
}

void
GOMSL_parser::initial_wm_def(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  zzmatch(DEFWM); consume();
  zzmatch(COLON); consume();
  {
    int zzcnt=1;
    do {
      wm_prop_value( model_sptr );
    } while ( (LA(1)==TAGNAME) );
  }
  if ( !guessing ) {
  
  // Normal_out << "initial_wm_def found" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** initial_wm_def not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x8);
}

void
GOMSL_parser::wm_prop_value(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol v_s;
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  zzmatch(IS); consume();
  {
    if ( (LA(1)==IDENTIFIER) ) {
      {
        ANTLRTokenPtr _t31=NULL;
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t31 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        v_s = _t31->getText();
        }
 consume();
      }
    }
    else {
      if ( (LA(1)==STRING)
 ) {
        {
          ANTLRTokenPtr _t31=NULL;
          zzmatch(STRING);          
          if ( !guessing ) {
           _t31 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          v_s = _t31->getText();
          }
 consume();
        }
      }
      else {FAIL(1,err19,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  (model_sptr->initial_WM_contents).store(_t11->getText(), v_s);
  // Normal_out << "wm_prop_value found" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** wm_prop_value not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x10);
}

GOMS_Model_Sptr_t
GOMSL_parser::model(void)
{
  GOMS_Model_Sptr_t   _retv;
  zzRULE;
  GOMS_Model_Sptr_t model_sptr = new GOMS_Model; GOMS_Goal g;
  Symbol m_n;
  number_of_syntax_errors = 0;	// reset the error counter
  zzmatch(DEFMODEL); consume();
  zzmatch(COLON); consume();
  {
    ANTLRTokenPtr _t21=NULL;
    if ( (LA(1)==STRING) ) {
      zzmatch(STRING);      
      if ( !guessing ) {
       _t21 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      m_n = _t21->getText();
      }
 consume();
    }
    else {
      if ( (LA(1)==IDENTIFIER) ) {
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t21 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        m_n = _t21->getText();
        }
 consume();
      }
      else {FAIL(1,err20,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    zzmatch(STARTING_GOAL); consume();
    zzmatch(IS); consume();
    if ( !guessing ) {
       g  = goal();

    } else {
      goal();
    }
    zzmatch(PERIOD); consume();
  }
  {
    int zzcnt=1;
    do {
      if ( (LA(1)==MFG) ) {
        method( model_sptr );
      }
      else {
        if ( (LA(1)==SRFG) ) {
          srs( model_sptr );
        }
        else {
          if ( (LA(1)==IRS)
 ) {
            irs( model_sptr );
          }
          else {
            if ( (LA(1)==DEFLTM) ) {
              ltm_def( model_sptr );
            }
            else {
              if ( (LA(1)==DEFTASK) ) {
                task_def( model_sptr );
              }
              else {
                if ( (LA(1)==DEFVISUAL) ) {
                  visual_def( model_sptr );
                }
                else {
                  if ( (LA(1)==DEFAUDITORY) ) {
                    auditory_def( model_sptr );
                  }
                  else {
                    if ( (LA(1)==DEFWM)
 ) {
                      initial_wm_def( model_sptr );
                    }
                    /* MR10 ()+ */ else {
                      if ( zzcnt > 1 ) break;
                      else {FAIL(1,err21,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                    }
                  }
                }
              }
            }
          }
        }
      }
      zzcnt++;
    } while ( 1 );
  }
  zzmatch(1);
  if ( !guessing ) {
  
  if (number_of_syntax_errors) {
    Normal_out << "Found " << number_of_syntax_errors << " syntax errors; model not built." << endl;
    _retv = 0;
  }
  else {
    model_sptr->model_name = m_n;
    model_sptr->start_goal = g;
    //					// model_sptr->display();
    _retv = model_sptr;
    // Normal_out << "model built" << endl;
  }
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Define_model not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x20);
  return _retv;
}

void
GOMSL_parser::method(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  GOMS_Procedure_Sptr_t proc_sptr = new GOMS_Method; 
  GOMS_Goal g; GOMS_Goal error_g;
  GOMS_Argument_Sptr_List_t a_l_p;
  zzmatch(MFG); consume();
  zzmatch(COLON); consume();
  if ( !guessing ) {
     g  = goal();

  } else {
    goal();
  }
  {
    if ( (LA(1)==USING) ) {
      zzmatch(USING); consume();
      if ( !guessing ) {
         a_l_p  = psdo_param_list();

      } else {
        psdo_param_list();
      }
    }
    else {
      if ( (setwd4[LA(1)]&0x40) ) {
      }
      else {FAIL(1,err22,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    if ( (LA(1)==ONERROR) ) {
      zzmatch(ONERROR); consume();
      zzmatch(COLON); consume();
      if ( !guessing ) {
         error_g  = goal();

      } else {
        goal();
      }
    }
    else {
      if ( (LA(1)==STEP) ) {
      }
      else {FAIL(1,err23,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    int zzcnt=1;
    do {
      step( proc_sptr );
    } while ( (LA(1)==STEP)
 );
  }
  if ( !guessing ) {
  
  // why not a constructor here?
  proc_sptr->goal = g;
  proc_sptr->error_goal = error_g;
  proc_sptr->param_list = a_l_p;
  // Normal_out << "param_list size is %d\n", (proc_sptr->param_list).size());
  // proc_sptr->display();
  model_sptr->procedure_set.insert(proc_sptr);	// no check on success of insertion
  model_sptr->procedure_list.push_back(proc_sptr);
  // Normal_out << "method built" << endl;
  {
    //	Normal_out << "(mp1 If ((Goal " << g.action << ' ' << g.object << ")(Not (Step mp ??? ???))) Then ((Add (Step mp 1)))" << endl;
  }
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Method_for_goal not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x80);
}

void
GOMSL_parser::srs(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  
  GOMS_Goal g;
  GOMS_Conditional_Sptr_list_t c_l;
  GOMS_Conditional_Sptr_t c_p;
  GOMS_Operator_Sptr_t dec_op;
  GOMS_Operator_Sptr_t r_op;
  GOMS_Argument_Sptr_List_t a_l_p;
  int start_line = -1;
  zzmatch(SRFG);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  start_line = _t11->getLine();
  }
 consume();
  zzmatch(COLON); consume();
  if ( !guessing ) {
     g  = goal();

  } else {
    goal();
  }
  {
    if ( (LA(1)==USING) ) {
      zzmatch(USING); consume();
      if ( !guessing ) {
         a_l_p  = psdo_param_list();

      } else {
        psdo_param_list();
      }
    }
    else {
      if ( (LA(1)==IF) ) {
      }
      else {FAIL(1,err24,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    int zzcnt=1;
    do {
      if ( !guessing ) {
         c_p  = sel_rule();

      } else {
        sel_rule();
      }
      if ( !guessing ) {
      
      c_l.push_back(c_p);
      }
    } while ( (LA(1)==IF) );
  }
  if ( !guessing ) {
     r_op  = rga_op();

  } else {
    rga_op();
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  // construct a selection operator
  dec_op = new GOMS_Selection_Operator (c_l);
  // construct a step with special label
  GOMS_Step_Sptr_t step_sptr = new GOMS_Step("SR Selection");
  step_sptr->set_file_location(file_name, start_line);
  // put the selection operator in the operator list for the step
  step_sptr->operator_sptr_list.push_back(dec_op);
  // construct a selection rule set procedure with the goal
  GOMS_Procedure_Sptr_t proc_sptr = new GOMS_Selection_Rule_Set(g);
  // put the selection step into the procedure step list
  proc_sptr->step_sptr_list.push_back(step_sptr);
  // construct another step, put the rga operator in it, etc.
  step_sptr = new GOMS_Step("SR Return");
  step_sptr->set_file_location(file_name, _t11->getLine());
  step_sptr->operator_sptr_list.push_back(r_op);
  proc_sptr->step_sptr_list.push_back(step_sptr);
  proc_sptr->param_list = a_l_p;
  // put the procedure in the model
  model_sptr->procedure_set.insert(proc_sptr);	// no check on success of insertion
  model_sptr->procedure_list.push_back(proc_sptr);
  // Normal_out << "srs built" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Selection_rules_for_goal not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x1);
}

GOMS_Conditional_Sptr_t
GOMSL_parser::sel_rule(void)
{
  GOMS_Conditional_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Predicate_Sptr_list_t pr_l; 
  GOMS_Operator_Sptr_list_t op_l;
  GOMS_Operator_Sptr_t op;
  zzmatch(IF); consume();
  pred_list( pr_l );
  {
    if ( (LA(1)==COMMA) ) {
      zzmatch(COMMA); consume();
    }
    else {
      if ( (LA(1)==THEN)
 ) {
      }
      else {FAIL(1,err25,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(THEN); consume();
  if ( !guessing ) {
     op  = ag_op();

  } else {
    ag_op();
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  op_l.push_back(op);
  _retv = new GOMS_Conditional(pr_l, op_l);
  // Normal_out << "sr built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Selection rule not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x2);
  return _retv;
}

void
GOMSL_parser::irs(GOMS_Model_Sptr_t model_sptr)
{
  zzRULE;
  
  GOMS_Conditional_Sptr_list_t c_l;
  GOMS_Conditional_Sptr_t c_p;
  GOMS_Operator_Sptr_t interrupt_op;
  zzmatch(IRS); consume();
  {
    int zzcnt=1;
    do {
      if ( !guessing ) {
         c_p  = interrupt_rule();

      } else {
        interrupt_rule();
      }
      if ( !guessing ) {
      
      c_l.push_back(c_p);
      }
    } while ( (LA(1)==IF) );
  }
  if ( !guessing ) {
  
  // construct a interrupt rule set operator
  //interrupt_op = new GOMS_Interrupt_Rule_Set_Operator (c_l);
  // put the interrupt rule set in the model
  //model_sptr->interrupt_rule_set = interrupt_op;
  // construct a interrupt rule set operator
  interrupt_op = new GOMS_Interrupt_Rule_Set_Operator (c_l);
  // construct a step with special label
  Symbol check_step_label("Check");
  GOMS_Step_Sptr_t step_sptr = new GOMS_Step(check_step_label);
  // put the interrupt operator in the operator list for the step
  step_sptr->operator_sptr_list.push_back(interrupt_op);
  // construct an interrupt rule set procedure - it has its own goal
  GOMS_Procedure_Sptr_t proc_sptr = new GOMS_Interrupt_Rule_Set;
  // put the interrupt step into the procedure step list
  proc_sptr->step_sptr_list.push_back(step_sptr);
  // // construct another step, put Goto operator in it.
  // step_sptr = new GOMS_Step("Loop");
  // GOMS_Operator_Sptr_t goto_op = new GOMS_Goto_Operator(check_step_label);
  // step_sptr->operator_sptr_list.push_back(goto_op);
  // proc_sptr->step_sptr_list.push_back(step_sptr);
  // put the procedure in the model
  model_sptr->procedure_set.insert(proc_sptr);	// no check on success of insertion
  model_sptr->procedure_list.push_back(proc_sptr);
  // Normal_out << "irs built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Interrupt_rules not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x4);
}

GOMS_Conditional_Sptr_t
GOMSL_parser::interrupt_rule(void)
{
  GOMS_Conditional_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Predicate_Sptr_list_t pr_l; 
  GOMS_Operator_Sptr_list_t op_l;
  GOMS_Operator_Sptr_t op;
  zzmatch(IF); consume();
  {
    if ( (LA(1)==EXISTS) ) {
      exists_pred( pr_l );
    }
    else {
      if ( (setwd5[LA(1)]&0x8) ) {
        pred_list( pr_l );
      }
      else {FAIL(1,err26,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    if ( (LA(1)==COMMA) ) {
      zzmatch(COMMA); consume();
    }
    else {
      if ( (LA(1)==THEN)
 ) {
      }
      else {FAIL(1,err27,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(THEN); consume();
  {
    if ( (LA(1)==AAR) ) {
      if ( !guessing ) {
         op  = abort_restart_op();

      } else {
        abort_restart_op();
      }
    }
    else {
      if ( (LA(1)==AG) ) {
        if ( !guessing ) {
           op  = ag_op();

        } else {
          ag_op();
        }
      }
      else {FAIL(1,err28,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  op_l.push_back(op);
  _retv = new GOMS_Conditional(pr_l, op_l);
  //Normal_out << "interrupt_rule built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** interrupt_rule not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x10);
  return _retv;
}

void
GOMSL_parser::step(GOMS_Procedure_Sptr_t proc_sptr)
{
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  GOMS_Step_Sptr_t step_sptr = new GOMS_Step;
  GOMS_Operator_Sptr_list_t& op_l = step_sptr->operator_sptr_list;
  zzmatch(STEP);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  step_sptr->set_file_location(file_name, _t11->getLine());
  }
 consume();
  {
    ANTLRTokenPtr _t21=NULL;
    if ( (LA(1)==NUMBER) ) {
      zzmatch(NUMBER);      
      if ( !guessing ) {
       _t21 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      
      step_sptr->label = Symbol(_t21->getText());
      }
 consume();
    }
    else {
      if ( (LA(1)==IDENTIFIER) ) {
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t21 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        
        step_sptr->label = Symbol(_t21->getText());
        }
 consume();
      }
      else {
        if ( (LA(1)==PERIOD)
 ) {
        }
        else {FAIL(1,err29,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzmatch(PERIOD); consume();
  op_list( op_l );
  zzmatch(PERIOD);
  if ( !guessing ) {
  
  //				step_sptr->label = Symbol($2->getText());
  proc_sptr->step_sptr_list.push_back(step_sptr);
  // Normal_out << "step built" << endl;
  }
 consume();
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Step not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x20);
}

void
GOMSL_parser::op_list(GOMS_Operator_Sptr_list_t& o_l)
{
  zzRULE;
  if ( (LA(1)==DECIDE) ) {
    singleton_op( o_l );
  }
  else {
    if ( (setwd5[LA(1)]&0x40) ) {
      flow_op( o_l );
    }
    else {
      if ( (setwd5[LA(1)]&0x80) ) {
        {
          inter_op( o_l );
          {
            while ( (LA(1)==SEMICOLON) && (setwd6[LA(2)]&0x1) ) {
              zzmatch(SEMICOLON); consume();
              intra_op( o_l );
            }
          }
          {
            if ( (LA(1)==SEMICOLON) && 
(setwd6[LA(2)]&0x2) ) {
              zzmatch(SEMICOLON); consume();
              flow_op( o_l );
            }
            else {
              if ( (setwd6[LA(1)]&0x4) && (setwd6[LA(2)]&0x8) ) {
              }
              else {FAIL(2,err30,err31,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
      }
      else {
        if ( (setwd6[LA(1)]&0x10) ) {
          {
            intra_op( o_l );
            {
              {
                while ( (LA(1)==SEMICOLON) && (setwd6[LA(2)]&0x20) ) {
                  zzmatch(SEMICOLON); consume();
                  intra_op( o_l );
                }
              }
              {
                if ( (LA(1)==SEMICOLON) && (setwd6[LA(2)]&0x40) ) {
                  zzmatch(SEMICOLON); consume();
                  inter_op( o_l );
                  {
                    while ( (LA(1)==SEMICOLON) && 
(setwd6[LA(2)]&0x80) ) {
                      zzmatch(SEMICOLON); consume();
                      intra_op( o_l );
                    }
                  }
                }
                else {
                  if ( (setwd7[LA(1)]&0x1) && (setwd7[LA(2)]&0x2) ) {
                  }
                  else {FAIL(2,err32,err33,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
              }
              {
                if ( (LA(1)==SEMICOLON) && (setwd7[LA(2)]&0x4) ) {
                  zzmatch(SEMICOLON); consume();
                  flow_op( o_l );
                }
                else {
                  if ( (setwd7[LA(1)]&0x8) && (setwd7[LA(2)]&0x10) ) {
                  }
                  else {FAIL(2,err34,err35,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
              }
            }
          }
          if ( !guessing ) {
          
          // Normal_out << "op_list built" << endl;
          }
        }
        else {FAIL(1,err36,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** operator list not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd7, 0x20);
}

void
GOMSL_parser::singleton_op(GOMS_Operator_Sptr_list_t& o_l)
{
  zzRULE;
  GOMS_Operator_Sptr_t o_p;
  if ( !guessing ) {
     o_p  = decide_op();

  } else {
    decide_op();
  }
  if ( !guessing ) {
  
  o_l.push_back(o_p);
  // Normal_out << "singleton_op built" << endl;
  // o_p->display();
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Decide arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd7, 0x40);
}

void
GOMSL_parser::intra_op(GOMS_Operator_Sptr_list_t& o_l)
{
  zzRULE;
  GOMS_Operator_Sptr_t o_p;
  {
    if ( (LA(1)==STORE) ) {
      if ( !guessing ) {
         o_p  = store_op();

      } else {
        store_op();
      }
    }
    else {
      if ( (LA(1)==DELETE)
 ) {
        if ( !guessing ) {
           o_p  = delete_op();

        } else {
          delete_op();
        }
      }
      else {
        if ( (LA(1)==LOG) ) {
          if ( !guessing ) {
             o_p  = log_op();

          } else {
            log_op();
          }
        }
        else {FAIL(1,err37,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  if ( !guessing ) {
  
  o_l.push_back(o_p);
  // Normal_out << "intra_op built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Intrastep operator arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd7, 0x80);
}

void
GOMSL_parser::flow_op(GOMS_Operator_Sptr_list_t& o_l)
{
  zzRULE;
  GOMS_Operator_Sptr_t o_p;
  {
    if ( (LA(1)==AG) ) {
      if ( !guessing ) {
         o_p  = ag_op();

      } else {
        ag_op();
      }
    }
    else {
      if ( (LA(1)==AAG) ) {
        if ( !guessing ) {
           o_p  = also_ag_op();

        } else {
          also_ag_op();
        }
      }
      else {
        if ( (LA(1)==RGA) ) {
          if ( !guessing ) {
             o_p  = rga_op();

          } else {
            rga_op();
          }
        }
        else {
          if ( (LA(1)==GOTO)
 ) {
            if ( !guessing ) {
               o_p  = goto_op();

            } else {
              goto_op();
            }
          }
          else {
            if ( (LA(1)==AAR) ) {
              if ( !guessing ) {
                 o_p  = abort_restart_op();

              } else {
                abort_restart_op();
              }
            }
            else {
              if ( (LA(1)==RESUME) ) {
                if ( !guessing ) {
                   o_p  = resume_op();

                } else {
                  resume_op();
                }
              }
              else {
                if ( (LA(1)==RAISE) ) {
                  if ( !guessing ) {
                     o_p  = raise_op();

                  } else {
                    raise_op();
                  }
                }
                else {
                  if ( (LA(1)==STOP) ) {
                    if ( !guessing ) {
                       o_p  = stop_op();

                    } else {
                      stop_op();
                    }
                  }
                  else {FAIL(1,err38,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
              }
            }
          }
        }
      }
    }
  }
  if ( !guessing ) {
  
  o_l.push_back(o_p);
  // Normal_out << "flow_op built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Flow-of-control arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x1);
}

void
GOMSL_parser::inter_op(GOMS_Operator_Sptr_list_t& o_l)
{
  zzRULE;
  GOMS_Operator_Sptr_t o_p;
  {
    if ( (LA(1)==KEYSTROKE)
 ) {
      if ( !guessing ) {
         o_p  = key_op();

      } else {
        key_op();
      }
    }
    else {
      if ( (LA(1)==TYPE_IN) ) {
        if ( !guessing ) {
           o_p  = type_op();

        } else {
          type_op();
        }
      }
      else {
        if ( (LA(1)==HOME_TO) ) {
          if ( !guessing ) {
             o_p  = home_op();

          } else {
            home_op();
          }
        }
        else {
          if ( (LA(1)==POINT_TO) ) {
            if ( !guessing ) {
               o_p  = point_op();

            } else {
              point_op();
            }
          }
          else {
            if ( (LA(1)==HOLD_DOWN) ) {
              if ( !guessing ) {
                 o_p  = hold_op();

              } else {
                hold_op();
              }
            }
            else {
              if ( (LA(1)==RELEASE)
 ) {
                if ( !guessing ) {
                   o_p  = release_op();

                } else {
                  release_op();
                }
              }
              else {
                if ( (LA(1)==CLICK) ) {
                  if ( !guessing ) {
                     o_p  = click_op();

                  } else {
                    click_op();
                  }
                }
                else {
                  if ( (LA(1)==DBL_CLICK) ) {
                    if ( !guessing ) {
                       o_p  = dbl_click_op();

                    } else {
                      dbl_click_op();
                    }
                  }
                  else {
                    if ( (LA(1)==SPEAK) ) {
                      if ( !guessing ) {
                         o_p  = speak_op();

                      } else {
                        speak_op();
                      }
                    }
                    else {
                      if ( (LA(1)==RLTM) ) {
                        if ( !guessing ) {
                           o_p  = rltm_op();

                        } else {
                          rltm_op();
                        }
                      }
                      else {
                        if ( (LA(1)==LOOK_AT)
 ) {
                          if ( !guessing ) {
                             o_p  = look_at_op();

                          } else {
                            look_at_op();
                          }
                        }
                        else {
                          if ( (LA(1)==RECALL) ) {
                            if ( !guessing ) {
                               o_p  = recall_op();

                            } else {
                              recall_op();
                            }
                          }
                          else {
                            if ( (LA(1)==LOOK_FOR) ) {
                              if ( !guessing ) {
                                 o_p  = look_for_op();

                              } else {
                                look_for_op();
                              }
                            }
                            else {
                              if ( (LA(1)==CHECK_VISUAL) ) {
                                if ( !guessing ) {
                                   o_p  = check_visual_op();

                                } else {
                                  check_visual_op();
                                }
                              }
                              else {
                                if ( (LA(1)==WAIT_VISUAL) ) {
                                  if ( !guessing ) {
                                     o_p  = wait_visual_op();

                                  } else {
                                    wait_visual_op();
                                  }
                                }
                                else {
                                  if ( (LA(1)==CHECK_AUDITORY)
 ) {
                                    if ( !guessing ) {
                                       o_p  = check_auditory_op();

                                    } else {
                                      check_auditory_op();
                                    }
                                  }
                                  else {
                                    if ( (LA(1)==WAIT_AUDITORY) ) {
                                      if ( !guessing ) {
                                         o_p  = wait_auditory_op();

                                      } else {
                                        wait_auditory_op();
                                      }
                                    }
                                    else {
                                      if ( (LA(1)==WAIT_ANY_AUDITORY) ) {
                                        if ( !guessing ) {
                                           o_p  = wait_any_auditory_op();

                                        } else {
                                          wait_any_auditory_op();
                                        }
                                      }
                                      else {
                                        if ( (LA(1)==GET_TASK) ) {
                                          if ( !guessing ) {
                                             o_p  = get_task_op();

                                          } else {
                                            get_task_op();
                                          }
                                        }
                                        else {
                                          if ( (LA(1)==WRITE_TASK) ) {
                                            if ( !guessing ) {
                                               o_p  = write_task_op();

                                            } else {
                                              write_task_op();
                                            }
                                          }
                                          else {
                                            if ( (LA(1)==UPDATE_TASK)
 ) {
                                              if ( !guessing ) {
                                                 o_p  = update_task_op();

                                              } else {
                                                update_task_op();
                                              }
                                            }
                                            else {
                                              if ( (LA(1)==ERASE_PROP) ) {
                                                if ( !guessing ) {
                                                   o_p  = erase_task_prop_op();

                                                } else {
                                                  erase_task_prop_op();
                                                }
                                              }
                                              else {
                                                if ( (LA(1)==ERASE_OBJECT) ) {
                                                  if ( !guessing ) {
                                                     o_p  = erase_task_object_op();

                                                  } else {
                                                    erase_task_object_op();
                                                  }
                                                }
                                                else {
                                                  if ( (LA(1)==CREATE_TASK_LIST) ) {
                                                    if ( !guessing ) {
                                                       o_p  = create_task_list_op();

                                                    } else {
                                                      create_task_list_op();
                                                    }
                                                  }
                                                  else {
                                                    if ( (LA(1)==ERASE_TASK_LIST) ) {
                                                      if ( !guessing ) {
                                                         o_p  = erase_task_list_op();

                                                      } else {
                                                        erase_task_list_op();
                                                      }
                                                    }
                                                    else {
                                                      if ( (LA(1)==GET_SIZE_OF)
 ) {
                                                        if ( !guessing ) {
                                                           o_p  = get_size_of_task_list_op();

                                                        } else {
                                                          get_size_of_task_list_op();
                                                        }
                                                      }
                                                      else {
                                                        if ( (LA(1)==FIND_ITEM_WHOSE) ) {
                                                          if ( !guessing ) {
                                                             o_p  = find_task_item_whose_op();

                                                          } else {
                                                            find_task_item_whose_op();
                                                          }
                                                        }
                                                        else {
                                                          if ( (LA(1)==FIND_FIRST_ITEM) ) {
                                                            if ( !guessing ) {
                                                               o_p  = find_first_item_op();

                                                            } else {
                                                              find_first_item_op();
                                                            }
                                                          }
                                                          else {
                                                            if ( (LA(1)==FIND_LAST_ITEM) ) {
                                                              if ( !guessing ) {
                                                                 o_p  = find_last_item_op();

                                                              } else {
                                                                find_last_item_op();
                                                              }
                                                            }
                                                            else {
                                                              if ( (LA(1)==FIND_ITEM_BEFORE) ) {
                                                                if ( !guessing ) {
                                                                   o_p  = find_item_before_op();

                                                                } else {
                                                                  find_item_before_op();
                                                                }
                                                              }
                                                              else {
                                                                if ( (LA(1)==FIND_ITEM_AFTER)
 ) {
                                                                  if ( !guessing ) {
                                                                     o_p  = find_item_after_op();

                                                                  } else {
                                                                    find_item_after_op();
                                                                  }
                                                                }
                                                                else {
                                                                  if ( (LA(1)==APPEND_ITEM) ) {
                                                                    if ( !guessing ) {
                                                                       o_p  = append_item_op();

                                                                    } else {
                                                                      append_item_op();
                                                                    }
                                                                  }
                                                                  else {
                                                                    if ( (LA(1)==PREPEND_ITEM) ) {
                                                                      if ( !guessing ) {
                                                                         o_p  = preprend_item_op();

                                                                      } else {
                                                                        preprend_item_op();
                                                                      }
                                                                    }
                                                                    else {
                                                                      if ( (LA(1)==INSERT_ITEM) ) {
                                                                        if ( !guessing ) {
                                                                           o_p  = insert_item_before_op();

                                                                        } else {
                                                                          insert_item_before_op();
                                                                        }
                                                                      }
                                                                      else {
                                                                        if ( (LA(1)==REMOVE_ITEM) ) {
                                                                          if ( !guessing ) {
                                                                             o_p  = remove_item_op();

                                                                          } else {
                                                                            remove_item_op();
                                                                          }
                                                                        }
                                                                        else {
                                                                          if ( (LA(1)==REMOVE_FIRST)
 ) {
                                                                            if ( !guessing ) {
                                                                               o_p  = remove_first_op();

                                                                            } else {
                                                                              remove_first_op();
                                                                            }
                                                                          }
                                                                          else {
                                                                            if ( (LA(1)==REMOVE_LAST) ) {
                                                                              if ( !guessing ) {
                                                                                 o_p  = remove_last_op();

                                                                              } else {
                                                                                remove_last_op();
                                                                              }
                                                                            }
                                                                            else {
                                                                              if ( (LA(1)==GET_DEVICE) ) {
                                                                                if ( !guessing ) {
                                                                                   o_p  = get_device_op();

                                                                                } else {
                                                                                  get_device_op();
                                                                                }
                                                                              }
                                                                              else {
                                                                                if ( (LA(1)==PUT_DEVICE) ) {
                                                                                  if ( !guessing ) {
                                                                                     o_p  = put_device_op();

                                                                                  } else {
                                                                                    put_device_op();
                                                                                  }
                                                                                }
                                                                                else {
                                                                                  if ( (LA(1)==MENTAL) ) {
                                                                                    if ( !guessing ) {
                                                                                       o_p  = mental_op();

                                                                                    } else {
                                                                                      mental_op();
                                                                                    }
                                                                                  }
                                                                                  else {FAIL(1,err39,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                                                                                }
                                                                              }
                                                                            }
                                                                          }
                                                                        }
                                                                      }
                                                                    }
                                                                  }
                                                                }
                                                              }
                                                            }
                                                          }
                                                        }
                                                      }
                                                    }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if ( !guessing ) {
  
  o_l.push_back(o_p);
  // Normal_out << "inter_op built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** interstep operator arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x2);
}

GOMS_Operator_Sptr_t
GOMSL_parser::ag_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Goal g; GOMS_Argument_Sptr_List_t arg_list;
  zzmatch(AG); consume();
  zzmatch(COLON); consume();
  if ( !guessing ) {
     g  = goal();

  } else {
    goal();
  }
  {
    if ( (LA(1)==USING)
 ) {
      zzmatch(USING); consume();
      if ( !guessing ) {
         arg_list  = psdo_arg_list();

      } else {
        psdo_arg_list();
      }
    }
    else {
      if ( (setwd8[LA(1)]&0x4) ) {
      }
      else {FAIL(1,err40,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Accomplish_Goal_Operator(g, arg_list);
  // $op->display();
  // Normal_out << "ag_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Accomplish_goal arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::also_ag_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t15=NULL;
  GOMS_Goal g; Symbol thread_name; GOMS_Argument_Sptr_List_t arg_list;
  zzmatch(AAG); consume();
  zzmatch(COLON); consume();
  if ( !guessing ) {
     g  = goal();

  } else {
    goal();
  }
  zzmatch(AS); consume();
  zzmatch(IDENTIFIER);  
  if ( !guessing ) {
   _t15 = (ANTLRTokenPtr)LT(1);
  }
 consume();
  {
    if ( (LA(1)==USING) ) {
      zzmatch(USING); consume();
      if ( !guessing ) {
         arg_list  = psdo_arg_list();

      } else {
        psdo_arg_list();
      }
    }
    else {
      if ( (setwd8[LA(1)]&0x10) ) {
      }
      else {FAIL(1,err41,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  if ( !guessing ) {
  
  thread_name = Symbol(_t15->getText());
  _retv = new GOMS_Also_Accomplish_Goal_Operator(g, thread_name, arg_list);
  // $op->display();
  // Normal_out << "also_ag_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Also_accomplish_goal arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::rga_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  zzmatch(RGA);
  if ( !guessing ) {
  
  _retv = new GOMS_Return_Goal_Accomplished_Operator;
  // Normal_out << "rga_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Return_with_goal_accomplished not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::abort_restart_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  zzmatch(AAR);
  if ( !guessing ) {
  
  _retv = new GOMS_Abort_and_Restart_Operator;
  //Normal_out << "abort_restart_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Abort_and_restart not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::resume_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t thread_arg_p; 
  GOMS_Argument_Sptr_t goal_action_arg_p; GOMS_Argument_Sptr_t goal_object_arg_p;
  GOMS_Argument_Sptr_t step_arg_p;
  {
    zzmatch(RESUME); consume();
    if ( !guessing ) {
       thread_arg_p  = simple_arg();

    } else {
      simple_arg();
    }
    zzmatch(ACCOMPLISHING_GOAL); consume();
    if ( !guessing ) {
       goal_action_arg_p  = simple_arg();

    } else {
      simple_arg();
    }
    if ( !guessing ) {
       goal_object_arg_p  = simple_arg();

    } else {
      simple_arg();
    }
    zzmatch(AT); consume();
    if ( !guessing ) {
       step_arg_p  = step_specifier();

    } else {
      step_specifier();
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Resume_Operator(thread_arg_p, goal_action_arg_p, goal_object_arg_p, step_arg_p);
  // Normal_out << "Resume ... accomplishing_goal ... at ... built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Resume ... accomplishing_goal ... at ... built arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::raise_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t exception_arg_p;
  {
    zzmatch(RAISE); consume();
    if ( !guessing ) {
       exception_arg_p  = simple_arg();

    } else {
      simple_arg();
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Raise_Operator(exception_arg_p);
  // Normal_out << "Raise ... built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Raise ... arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::stop_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t message_arg_p;
  {
    zzmatch(STOP); consume();
    if ( !guessing ) {
       message_arg_p  = simple_arg();

    } else {
      simple_arg();
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Stop_Operator(message_arg_p);
  // Normal_out << "Stop ... built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Stop ... arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::goto_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  zzmatch(GOTO); consume();
  {
    ANTLRTokenPtr _t21=NULL;
    if ( (LA(1)==NUMBER) ) {
      zzmatch(NUMBER);      
      if ( !guessing ) {
       _t21 = (ANTLRTokenPtr)LT(1);
      }

      if ( !guessing ) {
      
      _retv = new GOMS_Goto_Operator(_t21->getText());
      // Normal_out << "goto_op built" << endl;
      }
 consume();
    }
    else {
      if ( (LA(1)==IDENTIFIER)
 ) {
        zzmatch(IDENTIFIER);        
        if ( !guessing ) {
         _t21 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        
        _retv = new GOMS_Goto_Operator(_t21->getText());
        // Normal_out << "goto_op built" << endl;
        }
 consume();
      }
      else {FAIL(1,err42,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Go_to arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::decide_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Conditional_Sptr_list_t d_i_l;
  zzmatch(DECIDE); consume();
  zzmatch(COLON); consume();
  {
    decide_if( d_i_l );
    {
      while ( (LA(1)==SEMICOLON) && (LA(2)==IF) ) {
        zzmatch(SEMICOLON); consume();
        decide_if( d_i_l );
      }
    }
    {
      if ( (LA(1)==SEMICOLON) && (LA(2)==ELSE) ) {
        zzmatch(SEMICOLON); consume();
        decide_else( d_i_l );
      }
      else {
        if ( (setwd9[LA(1)]&0x10) && (setwd9[LA(2)]&0x20) ) {
        }
        else {FAIL(2,err43,err44,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Decide_Operator (d_i_l);
  // Normal_out << "decide_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Decide arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x40);
  return _retv;
}

void
GOMSL_parser::decide_if(GOMS_Conditional_Sptr_list_t& d_i_l)
{
  zzRULE;
  
  GOMS_Conditional_Sptr_t c_p;
  GOMS_Predicate_Sptr_list_t pr_l; 
  GOMS_Operator_Sptr_list_t op_l;
  zzmatch(IF); consume();
  pred_list( pr_l );
  {
    if ( (LA(1)==COMMA) ) {
      zzmatch(COMMA); consume();
    }
    else {
      if ( (LA(1)==THEN)
 ) {
      }
      else {FAIL(1,err45,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(THEN); consume();
  op_list( op_l );
  if ( !guessing ) {
  
  c_p = new GOMS_Conditional(pr_l, op_l);
  d_i_l.push_back(c_p);
  // Normal_out << "decide_if built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Decide If/Then not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x80);
}

void
GOMSL_parser::exists_pred(GOMS_Predicate_Sptr_list_t& p_l)
{
  zzRULE;
  ANTLRTokenPtr _t12=NULL;
  GOMS_Argument_Sptr_t a1_p; GOMS_Predicate_Sptr_t p_p; 
  GOMS_Predicate_Sptr_list_t p_l2;
  zzmatch(EXISTS); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t12 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  a1_p = new GOMS_WM_Argument(_t12->getText());
  // Normal_out << "TAGNAME found" << endl;
  }
 consume();
  pred_list( p_l2 );
  if ( !guessing ) {
  
  p_p = new GOMS_Exists_Predicate(a1_p, p_l2);
  p_l.push_back(p_p);				
  // Normal_out << "exists_pred built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Exists predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd10, 0x1);
}

void
GOMSL_parser::pred_list(GOMS_Predicate_Sptr_list_t& p_l)
{
  zzRULE;
  GOMS_Predicate_Sptr_t p_p;
  predicate( p_l );
  {
    while ( (LA(1)==COMMA) && (setwd10[LA(2)]&0x2) ) {
      {
        if ( (LA(1)==COMMA) && (setwd10[LA(2)]&0x4) ) {
          zzmatch(COMMA); consume();
        }
        else {
          if ( (LA(1)==COMMA) && (LA(2)==AND) ) {
            {
              zzmatch(COMMA); consume();
              zzmatch(AND); consume();
            }
          }
          else {FAIL(2,err46,err47,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      predicate( p_l );
    }
  }
  if ( !guessing ) {
  
  // Normal_out << "pred_list built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** predicate list not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd10, 0x8);
}

void
GOMSL_parser::decide_else(GOMS_Conditional_Sptr_list_t& d_i_l)
{
  zzRULE;
  
  GOMS_Conditional_Sptr_t c_p;
  GOMS_Operator_Sptr_list_t op_l;
  zzmatch(ELSE); consume();
  op_list( op_l );
  if ( !guessing ) {
  
  c_p = new GOMS_Conditional(op_l);
  d_i_l.push_back(c_p);
  // Normal_out << "decide_else built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Decide Else not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd10, 0x10);
}

void
GOMSL_parser::predicate(GOMS_Predicate_Sptr_list_t& p_l)
{
  zzRULE;
  GOMS_Predicate_Sptr_t p_p; GOMS_Argument_Sptr_t a1_p;
  if ( !guessing ) {
     a1_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  {
    if ( (setwd10[LA(1)]&0x20) ) {
      if ( !guessing ) {
         p_p  = is_predicate( a1_p );

      } else {
        is_predicate( a1_p );
      }
    }
    else {
      if ( (setwd10[LA(1)]&0x40)
 ) {
        if ( !guessing ) {
           p_p  = is_not_predicate( a1_p );

        } else {
          is_not_predicate( a1_p );
        }
      }
      else {
        if ( (LA(1)==IS_GREATER_THAN) ) {
          if ( !guessing ) {
             p_p  = is_greater_than_predicate( a1_p );

          } else {
            is_greater_than_predicate( a1_p );
          }
        }
        else {
          if ( (LA(1)==IS_LESS_THAN) ) {
            if ( !guessing ) {
               p_p  = is_less_than_predicate( a1_p );

            } else {
              is_less_than_predicate( a1_p );
            }
          }
          else {
            if ( (LA(1)==IS_GREATER_THAN_OR_EQUAL_TO) ) {
              if ( !guessing ) {
                 p_p  = is_greater_than_or_equal_to_predicate( a1_p );

              } else {
                is_greater_than_or_equal_to_predicate( a1_p );
              }
            }
            else {
              if ( (LA(1)==IS_LESS_THAN_OR_EQUAL_TO) ) {
                if ( !guessing ) {
                   p_p  = is_less_than_or_equal_to_predicate( a1_p );

                } else {
                  is_less_than_or_equal_to_predicate( a1_p );
                }
              }
              else {FAIL(1,err48,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
      }
    }
  }
  if ( !guessing ) {
  
  p_l.push_back(p_p);				
  // Normal_out << "predicate built" << endl;
  }
  return;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd10, 0x80);
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  {
    if ( (LA(1)==IS)
 ) {
      zzmatch(IS); consume();
    }
    else {
      if ( (LA(1)==IS_EQUAL_TO) ) {
        zzmatch(IS_EQUAL_TO); consume();
      }
      else {FAIL(1,err49,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Predicate(a1_p, a2_p);
  // Normal_out << "is_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x1);
  return _retv;
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_not_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  {
    if ( (LA(1)==IS_NOT) ) {
      zzmatch(IS_NOT); consume();
    }
    else {
      if ( (LA(1)==IS_NOT_EQUAL_TO) ) {
        zzmatch(IS_NOT_EQUAL_TO); consume();
      }
      else {FAIL(1,err50,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Not_Predicate(a1_p, a2_p);
  // Normal_out << "is_not_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_not_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x2);
  return _retv;
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_greater_than_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  zzmatch(IS_GREATER_THAN); consume();
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Greater_Than_Predicate(a1_p, a2_p);
  // Normal_out << "is_greater_than_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_greater_than_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x4);
  return _retv;
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_greater_than_or_equal_to_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  zzmatch(IS_GREATER_THAN_OR_EQUAL_TO); consume();
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Greater_Than_Or_Equal_To_Predicate(a1_p, a2_p);
  // Normal_out << "is_greater_than_or_equal_to_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_greater_than_or_equal_to_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x8);
  return _retv;
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_less_than_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  zzmatch(IS_LESS_THAN); consume();
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Less_Than_Predicate(a1_p, a2_p);
  // Normal_out << "is_less_than_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_less_than_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x10);
  return _retv;
}

GOMS_Predicate_Sptr_t
GOMSL_parser::is_less_than_or_equal_to_predicate(GOMS_Argument_Sptr_t a1_p)
{
  GOMS_Predicate_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a2_p;
  zzmatch(IS_LESS_THAN_OR_EQUAL_TO); consume();
  if ( !guessing ) {
     a2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Is_Less_Than_Or_Equal_To_Predicate(a1_p, a2_p);
  // Normal_out << "is_less_than_or_equal_to_predicate built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** is_less_than_or_equal_to_predicate not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::store_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  GOMS_Argument_Sptr_t a_p, tag_p;
  zzmatch(STORE); consume();
  if ( !guessing ) {
     a_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Store_Operator(a_p, tag_p);
  // $op->display();
  // Normal_out << "store_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Store arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::delete_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t12=NULL;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(DELETE); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t12 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t12->getText());
  _retv = new GOMS_Delete_Operator(tag_p);
  // $op->display();
  // Normal_out << "delete_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Delete arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd11, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::log_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_List_t arg_list;
  zzmatch(LOG); consume();
  if ( !guessing ) {
     arg_list  = psdo_arg_list();

  } else {
    psdo_arg_list();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Log_Operator(arg_list);
  // $op->display();
  // Normal_out << "log_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Log arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::rltm_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t16=NULL;
  
  GOMS_Argument_Sptr_t prp_p, obj_p, tag_p;
  zzmatch(RLTM); consume();
  if ( !guessing ) {
     prp_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(OF); consume();
  if ( !guessing ) {
     obj_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t16 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t16->getText());
  _retv = new GOMS_Retrieve_LTM_Operator(prp_p, obj_p, tag_p);
  // $op->display();
  // Normal_out << "rltm_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Retrieve_from_LTM arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::look_at_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t12=NULL;
  
  GOMS_Argument_Sptr_t obj_p;
  zzmatch(LOOK_AT); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t12 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant - gets both resolved and not
  obj_p = new GOMS_WM_Argument(_t12->getText());
  _retv = new GOMS_Look_at_Operator(obj_p);
  // $op->display();
  // Normal_out << "look_at_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Look_at arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::recall_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(RECALL); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Recall_LTM_Operator(arg_list_p, tag_p);
  // Normal_out << "recall_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Recall_LTM_item_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::look_for_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(LOOK_FOR); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Look_Operator(arg_list_p, tag_p);
  // Normal_out << "look_for_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Look_for_object_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x10);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::check_visual_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(CHECK_VISUAL); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Check_Visual_Operator(arg_list_p, tag_p);
  // Normal_out << "check_visual_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Check_for_visual_object_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::wait_visual_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(WAIT_VISUAL); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Wait_Visual_Operator(arg_list_p, tag_p);
  // Normal_out << "wait_visual_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Wait_for_auditory_object_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::check_auditory_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(CHECK_AUDITORY); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Check_Auditory_Operator(arg_list_p, tag_p);
  // Normal_out << "check_auditory_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Check_for_auditory_object_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd12, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::wait_auditory_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(WAIT_AUDITORY); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Wait_Auditory_Operator(arg_list_p, tag_p);
  // Normal_out << "wait_auditory_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Wait_for_auditory_object_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::wait_any_auditory_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t13=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(WAIT_ANY_AUDITORY); consume();
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t13 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t13->getText());
  _retv = new GOMS_Wait_Auditory_Operator(arg_list_p, tag_p);
  // Normal_out << "wait_any_auditory_op built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Wait_for_any_auditory_object arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::get_task_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(GET_TASK); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Get_Task_Operator(arg_list_p, tag_p);
  // Normal_out << "Get_task_item_whose built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Get_task_item_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::write_task_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(WRITE_TASK); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Write_Task_Operator(arg_list_p, tag_p);
  //				Normal_out << "Write_task_item_whose built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Write_task_item_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::update_task_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t arg_p;
  zzmatch(UPDATE_TASK); consume();
  if ( !guessing ) {
     arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(THAT); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Update_Task_Operator(arg_p, arg_list_p);
  //				Normal_out << "write_task_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Update_for_task_item arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x10);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::erase_task_prop_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t arg1_p;
  GOMS_Argument_Sptr_t arg2_p;
  zzmatch(ERASE_PROP); consume();
  if ( !guessing ) {
     arg1_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
     arg2_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Erase_Task_Prop_Operator(arg1_p, arg2_p);
  //				Normal_out << "erase_task_prop_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Erase_task_item_property arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::erase_task_object_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t arg_p;
  zzmatch(ERASE_OBJECT); consume();
  if ( !guessing ) {
     arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Erase_Task_Obj_Operator(arg_p);
  //				Normal_out << "erase_task_object_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Erase_task_item arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::create_task_list_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(CREATE_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Create_Task_List_Operator(tlist_arg_p);
  //				Normal_out << "Create_task_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Create_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd13, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::erase_task_list_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(ERASE_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Erase_Task_List_Operator(tlist_arg_p);
  //				Normal_out << "Erase_task_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Erase_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::get_size_of_task_list_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(GET_SIZE_OF); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Get_Size_Of_Task_List_Operator(tlist_arg_p, tag_p);
  // Normal_out << "Get_size_of_task_list built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Get_size_of_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::find_task_item_whose_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t16=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(FIND_ITEM_WHOSE); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(IN_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t16 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t16->getText());
  _retv = new GOMS_Find_Task_Item_Whose_Operator(tlist_arg_p, arg_list_p, tag_p);
  // Normal_out << "Find_item_whose built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Find_item_whose arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::find_first_item_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(FIND_FIRST_ITEM); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Find_First_Item_Operator(tlist_arg_p, tag_p);
  // Normal_out << "Find_first_item_in_task_list built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Find_first_item_in_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::find_last_item_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(FIND_LAST_ITEM); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Find_Last_Item_Operator(tlist_arg_p, tag_p);
  // Normal_out << "Find_last_item_in_task_list built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Find_last_item_in_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x10);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::find_item_after_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  zzmatch(FIND_ITEM_AFTER); consume();
  if ( !guessing ) {
     titem_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Find_Item_After_Operator(titem_arg_p, tag_p);
  // Normal_out << "Find_item_after built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Find_item_after arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::find_item_before_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_t tag_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  zzmatch(FIND_ITEM_BEFORE); consume();
  if ( !guessing ) {
     titem_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_Find_Item_Before_Operator(titem_arg_p, tag_p);
  // Normal_out << "Find_item_before built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Find_item_before arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::append_item_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  zzmatch(APPEND_ITEM); consume();
  if ( !guessing ) {
     titem_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(TO_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Append_Item_Operator(tlist_arg_p, titem_arg_p);
  // Normal_out << "Append_item built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Append_item arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd14, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::preprend_item_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  zzmatch(PREPEND_ITEM); consume();
  if ( !guessing ) {
     titem_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(TO_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Prepend_Item_Operator(tlist_arg_p, titem_arg_p);
  // Normal_out << "Prepend_item built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Prepend_item arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::insert_item_before_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  GOMS_Argument_Sptr_t titem_loc_arg_p;
  zzmatch(INSERT_ITEM); consume();
  if ( !guessing ) {
     titem_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(BEFORE); consume();
  if ( !guessing ) {
     titem_loc_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  zzmatch(IN_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Insert_Before_Operator(tlist_arg_p, titem_arg_p, titem_loc_arg_p);
  // Normal_out << "Insert_item ... before built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Insert_item ... before arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::remove_item_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  GOMS_Argument_Sptr_t titem_arg_p;
  zzmatch(REMOVE_ITEM); consume();
  if ( !guessing ) {
     titem_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  zzmatch(FROM_TASK_LIST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Remove_Item_Operator(tlist_arg_p, titem_arg_p);
  // Normal_out << "Remove_item built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Remove_item arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::remove_first_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(REMOVE_FIRST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Remove_First_Operator(tlist_arg_p);
  //				Normal_out << "Remove_first_item_from_task_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Remove_first_item_from_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::remove_last_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_t tlist_arg_p;
  zzmatch(REMOVE_LAST); consume();
  if ( !guessing ) {
     tlist_arg_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Remove_Last_Operator(tlist_arg_p);
  //				Normal_out << "Remove_last_item_from_task_list built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Remove_last_item_from_task_list arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x10);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::get_device_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t14=NULL;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(GET_DEVICE); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  zzmatch(AND_STORE_UNDER); consume();
  zzmatch(TAGNAME);  
  if ( !guessing ) {
   _t14 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  // the tag argument is a constant; won't be resolved
  tag_p = new GOMS_Constant_Argument(_t14->getText());
  _retv = new GOMS_HLGet_Device_Operator(arg_list_p, tag_p);
  //				Normal_out << "Get_from_device built" << endl;
  }
 consume();
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Get_from_device arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::put_device_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  
  GOMS_Argument_Sptr_List_t arg_list_p;
  GOMS_Argument_Sptr_t tag_p;
  zzmatch(PUT_DEVICE); consume();
  if ( !guessing ) {
     arg_list_p  = prop_value_arg_list();

  } else {
    prop_value_arg_list();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_HLPut_Device_Operator(arg_list_p);
  //				Normal_out << "Put_to_device built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Put_to_device arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd15, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::mental_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  ANTLRTokenPtr _t11=NULL;
  Symbol name_arg, doc_arg;
  zzmatch(MENTAL);  
  if ( !guessing ) {
   _t11 = (ANTLRTokenPtr)LT(1);
  }

  if ( !guessing ) {
  
  name_arg = _t11->getText();
  }
 consume();
  {
    ANTLRTokenPtr _t21=NULL;
    if ( (setwd15[LA(1)]&0x80) ) {
    }
    else {
      if ( (LA(1)==STRING)
 ) {
        zzmatch(STRING);        
        if ( !guessing ) {
         _t21 = (ANTLRTokenPtr)LT(1);
        }

        if ( !guessing ) {
        
        doc_arg = _t21->getText();
        }
 consume();
      }
      else {
        if ( (LA(1)==IDENTIFIER) ) {
          zzmatch(IDENTIFIER);          
          if ( !guessing ) {
           _t21 = (ANTLRTokenPtr)LT(1);
          }

          if ( !guessing ) {
          
          doc_arg = _t21->getText();
          }
 consume();
        }
        else {FAIL(1,err51,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Mental_Operator(name_arg, doc_arg);
  // Normal_out << "mental_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** mental_op arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::key_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(KEYSTROKE); consume();
  if ( !guessing ) {
     a_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Keystroke_Operator(a_p);
  // $op->display();
  // Normal_out << "key_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Keystroke arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x2);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::type_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(TYPE_IN); consume();
  if ( !guessing ) {
     a_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Type_In_Operator(a_p);
  // $op->display();
  // Normal_out << "type_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Type_in arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x4);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::home_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(HOME_TO); consume();
  if ( !guessing ) {
     a_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Home_Operator(a_p);
  // $op->display();
  // Normal_out << "home_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Home_to arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x8);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::point_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  {
    zzmatch(POINT_TO); consume();
    if ( !guessing ) {
       a_p  = simple_arg();

    } else {
      simple_arg();
    }
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Point_Operator(a_p);
  // $op->display();
  // Normal_out << "point_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Point_to arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x10);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::hold_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(HOLD_DOWN); consume();
  if ( !guessing ) {
     a_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Hold_Operator(a_p);
  // $op->display();
  // Normal_out << "hold_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Hold_down arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x20);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::release_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(RELEASE); consume();
  if ( !guessing ) {
     a_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Release_Operator(a_p);
  // $op->display();
  // Normal_out << "release_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Release arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x40);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::click_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(CLICK); consume();
  if ( !guessing ) {
     a_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Click_Operator(a_p);
  // $op->display();
  // Normal_out << "click_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Click arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd16, 0x80);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::dbl_click_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(DBL_CLICK); consume();
  if ( !guessing ) {
     a_p  = simple_arg();

  } else {
    simple_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Double_Click_Operator(a_p);
  // $op->display();
  // Normal_out << "dbl_click_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Double_click arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd17, 0x1);
  return _retv;
}

GOMS_Operator_Sptr_t
GOMSL_parser::speak_op(void)
{
  GOMS_Operator_Sptr_t   _retv;
  zzRULE;
  GOMS_Argument_Sptr_t a_p;
  zzmatch(SPEAK); consume();
  if ( !guessing ) {
     a_p  = any_inp_arg();

  } else {
    any_inp_arg();
  }
  if ( !guessing ) {
  
  _retv = new GOMS_Speak_Operator(a_p);
  // $op->display();
  // Normal_out << "speak_op built" << endl;
  }
  return _retv;
fail:
  if ( guessing ) zzGUESS_FAIL;
  Normal_out << "*** Speak arguments not found" << endl;  
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd17, 0x2);
  return _retv;
}
