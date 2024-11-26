#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "header.c"

/* Function prototypes */
char* base64(const word* w); /*converting to base64*/
macro newmac(char* name, char* body); /*new macro */
void Kdam_Assembler(const char *input_file); /*first pass on file to detected macros and handle*/
void twoPasses(); /*second and mainly part of project */
OperandInfo getOperandType(const char *token); /*get the operand type*/
void handleOperand1(code* w, char* token, int *a, char **b); /*handle operand number 1*/
void handleOperand2(code* w, char* token, int *a, char **b);/*handle operand number 2*/
code **addToCodeList(code **code_list, code *w, int *code_count, int *IC); /*add code line to code list*/
void addSymbolToTable(char *token, symbol **Symbol_table, int *symbolcnt, int IC, int DC, Symboltype status, int *symbol_table_size); /*add symbol to symbol list*/

#endif

