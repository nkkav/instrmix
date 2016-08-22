/* file "instrmix/instrmix.cpp" */
/*
 *     Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 *                   2011, 2012, 2013, 2014, 2015, 2016 Nikolaos Kavvadias
 *
 *     This software was written by Nikolaos Kavvadias, Ph.D. candidate
 *     at the Physics Department, Aristotle University of Thessaloniki,
 *     Greece (at the time).
 *
 *     This software is provided under the terms described in
 *     the "machine/copyright.h" include file.
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <ctype.h>
//#include <string.h>

#include <machine/copyright.h>

#ifdef USE_PRAGMA_INTERFACE
#pragma implementation "instrmix/instrmix.h"
#endif

#include <machine/machine.h>
#include <suifvm/suifvm.h>
#include <cfg/cfg.h>

#include <instrmix/instrmix.h>
#include "utils.h"

#ifdef USE_DMALLOC
#include <dmalloc.h>
#define new D_NEW
#endif

FILE *f_rpt_file;

bool is_legal_opnd(Opnd opnd)
{
  if ((is_reg(opnd) || is_var(opnd)) && !is_null(opnd))
    return true;
  else
    return false;
}

bool is_real_instruction(Instr *mi)
{
  if ((is_label(mi) || is_dot(mi)) && !is_null(mi))
    return false;
  else
    return true;
}

bool is_arith_instruction(Instr *mi)
{
  return mi->get_opcode() == suifvm::ADD
      || mi->get_opcode() == suifvm::SUB
      || mi->get_opcode() == suifvm::NEG
      || mi->get_opcode() == suifvm::ABS
      || mi->get_opcode() == suifvm::MIN
      || mi->get_opcode() == suifvm::MAX
      || mi->get_opcode() == suifvm::MOV
      || mi->get_opcode() == suifvm::LDC
      || mi->get_opcode() == suifvm::LDA;
}

bool is_logical_instruction(Instr *mi)
{
  return mi->get_opcode() == suifvm::NOT
      || mi->get_opcode() == suifvm::AND
      || mi->get_opcode() == suifvm::IOR
      || mi->get_opcode() == suifvm::XOR;
}

bool is_set_instruction(Instr *mi)
{  
  return mi->get_opcode() == suifvm::SEQ
      || mi->get_opcode() == suifvm::SNE
      || mi->get_opcode() == suifvm::SL
      || mi->get_opcode() == suifvm::SLE;
}

bool is_shift_instruction(Instr *mi)
{  
  return mi->get_opcode() == suifvm::ASR
      || mi->get_opcode() == suifvm::LSL
      || mi->get_opcode() == suifvm::LSR
      || mi->get_opcode() == suifvm::ROT;
}

bool is_division_instruction(Instr *mi)
{  
  return mi->get_opcode() == suifvm::DIV
      || mi->get_opcode() == suifvm::REM
      || mi->get_opcode() == suifvm::MOD;
}

bool is_multiply_instruction(Instr *mi)
{  
  return mi->get_opcode() == suifvm::MUL;
}


void InstrMix::do_opt_unit(OptUnit *unit)
{
    OptUnit *cur_unit;
    /**/
    int bb_num = 0;           // Number of the current basic block
    char bb_num_s[25];        // String equivalent to bb_num
   
    // Open instuction mix report file
    f_rpt_file = fopen("instr_mix_report.rpt","a");

    // Make local copy of the optimization unit
    cur_unit = unit;

    // Get the name of the optimization unit
    IdString name = get_name(get_proc_sym(cur_unit));

    // Report name of the CFG under processing
    fprintf(stdout, "Processing CFG \"%s\"\n", name.chars());
    const char *cur_proc_name = get_name(cur_unit).chars();

    // Get the body of the OptUnit
    AnyBody *cur_body = get_body(cur_unit);

    // verify that it is a CFG
    claim(is_kind_of<Cfg>(cur_body), "expected OptUnit's body in CFG form");
    //remove_unreachable_nodes(unit_cfg);

    // Create a local copy of the input CFG
    Cfg *cfg = (Cfg *)cur_body;

    // Iterate through the nodes of the CFG
    for (CfgNodeHandle cfg_nh=nodes_start(cfg); cfg_nh!=nodes_end(cfg); ++cfg_nh)
    {
      // Get the current node
      CfgNode* cnode = get_node(cfg, cfg_nh);

      /****************************************************************/
      /********************* INSIDE A CFGNODE ***********************/
      /****************************************************************/

      int i = 0;

      // Declarations for the instruction type counters
      int cnt_arith, cnt_logical, cnt_multiply, cnt_division, cnt_other;
      int cnt_set, cnt_load, cnt_store, cnt_ubr, cnt_cbr, cnt_call, cnt_return;
      int cnt_shift;

      // Initialize instruction type counters
      cnt_arith = 0;
      cnt_logical = 0;
      cnt_shift = 0;
      cnt_multiply = 0;
      cnt_division = 0;
      cnt_set = 0;
      cnt_load = 0;
      cnt_store = 0;
      cnt_ubr = 0;
      cnt_cbr = 0;
      cnt_call = 0;
      cnt_return = 0;
      cnt_other = 0;


     // Calculate instruction mix statistics
     for (InstrHandle h = instrs_start(cnode); h != instrs_end(cnode); ++h)
     {
        Instr *mi = *h;

        if (is_real_instruction(mi))
        {
            // Stat arithmetic instructions
            if(is_arith_instruction(mi))
              cnt_arith++;
            // Stat logical instructions
            else if (is_logical_instruction(mi))
              cnt_logical++;
            // Stat shift instructions
            else if (is_shift_instruction(mi))
              cnt_shift++;
            // Stat set instructions
            else if (is_set_instruction(mi))
              cnt_set++;
            // Stat multiply instructions
            else if (is_multiply_instruction(mi))
              cnt_multiply++;
            // Stat division instructions
            else if (is_division_instruction(mi))
              cnt_division++;
            // Stat load instructions
            else if (reads_memory(mi))
              cnt_load++;
            // Stat store instructions
            else if (writes_memory(mi))
              cnt_store++;
            // Stat unconditional branch/jump instructions
            else if (is_ubr(mi))
              cnt_ubr++;
            // Stat conditional branch/jump instructions
            else if (is_cbr(mi))
              cnt_cbr++;
            // Stat call instructions
            else if (is_call(mi))
              cnt_call++;
            // Stat return instructions
            else if (is_return(mi))
              cnt_return++;
            // Stat all remaining instructions
            else
              cnt_other++;
        }

        i++;
     }

     // Obtain bb_num_s alphanumeric representation of bb_num integer
     itoa(bb_num, bb_num_s);

     // Create the actual name of the VCG representation the current BB
     char *bb_name = new char[strlen(cur_proc_name) + strlen(bb_num_s) + 10];
     // Print the correct string for "bb_name" e.g. main_001
     sprintf(bb_name, "%s_%s", cur_proc_name, bb_num_s);

     // Print results to report file
     fprintf(f_rpt_file,"%s:\tarith:%d\tlogical:%d\tshift:%d\t",bb_name,cnt_arith,cnt_logical,cnt_shift);
     fprintf(f_rpt_file,"set:%d\tmultiply:%d\tdivision:%d\t",cnt_set,cnt_multiply,cnt_division);
     fprintf(f_rpt_file,"load:%d\tstore:%d\tubr:%d\tcbr:%d\t",cnt_load,cnt_store,cnt_cbr,cnt_ubr);
     fprintf(f_rpt_file,"call:%d\treturn:%d\tother:%d\n",cnt_call,cnt_return,cnt_other);

      // The number of current basic block
      bb_num++;

      /****************************************************************/
      /****************** end of INSIDE A CFGNODE *********************/
      /****************************************************************/

    } // end of for each CFGnode


    // Close instruction mix report file
    fclose(f_rpt_file);


}   /*** END OF instrmix.cpp ***/
