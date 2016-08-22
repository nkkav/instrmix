======================
 instrmix user manual
======================

+-------------------+----------------------------------------------------------+
| **Title**         | instrmix (Instruction mix extraction pass for Machine    |
|                   | SUIF)                                                    |
+-------------------+----------------------------------------------------------+
| **Author**        | Nikolaos Kavvadias 2004, 2005, 2006, 2007, 2008, 2009,   |
|                   | 2010, 2011, 2012, 2013, 2014, 2015, 2016                 |
+-------------------+----------------------------------------------------------+
| **Contact**       | nikos@nkavvadias.com                                     |
+-------------------+----------------------------------------------------------+
| **Website**       | http://www.nkavvadias.com                                |
+-------------------+----------------------------------------------------------+
| **Release Date**  | 23 September 2014                                        |
+-------------------+----------------------------------------------------------+
| **Version**       | 1.1.1                                                    |
+-------------------+----------------------------------------------------------+
| **Rev. history**  |                                                          |
+-------------------+----------------------------------------------------------+
|        **v1.1.1** | 2014-09-23                                               |
|                   |                                                          |
|                   | Updated headers in all source code files. Added File     |
|                   | Listing section in README; new files AUTHORS, LICENSE    |
|                   | and VERSION.                                             |
+-------------------+----------------------------------------------------------+
|        **v1.1.0** | 2014-02-24                                               |
|                   |                                                          |
|                   | Changed documentation format to RestructuredText.        |
+-------------------+----------------------------------------------------------+
|        **v1.0.0** | 2004-07-18                                               |
|                   |                                                          |
|                   | Initial release.                                         |
+-------------------+----------------------------------------------------------+


1. Introduction
===============

``instrmix`` is an analysis pass that generates the SUIFvm (SUIF virtual machine)
instruction mix for a given input source file. A textual report is written to
file ``instr_mix_report.rpt``.

Instructions are categorized as follows:

arithmetic: 
  ADD, SUB, NEG, ABS, MIN, MAX, MOV, LDC, LDA;
logical: 
  NOT, AND, IOR, XOR;
comparison: 
  SEQ, SNE, SL, SLE; (set instructions)
shift: 
  ASR, LSL, LSR, ROT;
division: 
  DIV, REM, MOD;
multiply: 
  MUL;
load: 
  LOD;
store: 
  STR;
ubr: 
  JMP, JMPI; (unconditional branch/jump)
cbr: 
  BFALSE, BTRUE, BEQ, BNE, BGE, BGT, BLE, BLT; (cond. branch/jump)
call: 
  CAL;
return: 
  RET;
other: 
  any instruction not fitting into the previous categories.

This pass works for the SUIF virtual machine instruction set (SUIFvm).
Also, the ``instrmix`` pass has been tested with MachSUIF 2.02.07.15.


2. File listing
===============

The ``instrmix`` distribution includes the following files:
   
+-----------------------+------------------------------------------------------+
| /instrmix             | Top-level directory                                  |
+-----------------------+------------------------------------------------------+
| AUTHORS               | List of ``instrmix`` authors.                        |
+-----------------------+------------------------------------------------------+
| LICENSE               | The modified BSD license governs ``instrmix``.       |
+-----------------------+------------------------------------------------------+
| README.rst            | This file.                                           |
+-----------------------+------------------------------------------------------+
| README.html           | HTML version of README.                              |
+-----------------------+------------------------------------------------------+
| README.pdf            | PDF version of README.                               |
+-----------------------+------------------------------------------------------+
| VERSION               | Current version of the project sources.              |
+-----------------------+------------------------------------------------------+
| instrmix.cpp          | Implementation of the ``instrmix`` pass.             |
+-----------------------+------------------------------------------------------+
| instrmix.h            | C++ header file containing declarations and          |
|                       | prototypes for the above.                            |
+-----------------------+------------------------------------------------------+
| rst2docs.sh           | Bash script for generating the HTML and PDF versions |
|                       | of the documentation (README).                       |
+-----------------------+------------------------------------------------------+
| suif_main.cpp         | Entry point for building the standalone program      |
|                       | ``do_instrmix`` that implements the pass.            |
+-----------------------+------------------------------------------------------+
| suif_pass.cpp         | Define the SUIF pass built as the dynamically        |
|                       | loadable library ``libinstrmix.so``.                 |
+-----------------------+------------------------------------------------------+
| suif_main.h           | C++ header file for the above.                       |
+-----------------------+------------------------------------------------------+
| utils.h               | C header file with implementations of auxiliary      |
|                       | functions.                                           |
+-----------------------+------------------------------------------------------+


3. Installation
===============

Unpack the instrmix archive wherever you like, e.g. in ``$MACHSUIFHOME/instrmix``.
You don't need to modify anything in the Makefile, if you have a working
MachSUIF 2 installation.

The program binary (``do_instrmix``) will be installed at ``$NCIHOME/bin`` and 
the shared library (``libinstrmix.so``) at ``$NCIHOME/solib``, where NCIHOME is 
the SUIF 2 top-level directory.


4. Usage details
================

The pass accepts an input file in CFG form to operate. You don't have to define
any output files since their naming convention has been hardwired.

Usage synopsys:

| ``$ ./do_instrmix test.cfg``
