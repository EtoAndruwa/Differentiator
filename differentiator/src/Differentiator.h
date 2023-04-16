#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tree.h"
#include "DSL.h"
#include "../../graphviz/src/graphviz.h"

#define EPS 1e-7
#define PI  3.14159265

const int MAX_LEN_FSCANF = 11;

enum return_codes
{
    NULL_PTR_NODE  = 1,
    NOT_ALL_DIGITS = 2,
    IS_INT         = 3,
    IS_FLOAT       = 4,
    VALID_TOK      = 5,
    INVALID_TOK    = 6,
    EXISTING_FUNC  = 7,
    NON_EXIST_FUNC = 8,
};

enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = -1,
    ERR_CANNOT_CLOSE_OUTPUT = -2, 
    ERR_CANNOT_OPEN_INPUT   = -3,
    ERR_CANNOT_CLOSE_INPUT  = -4,
    ERR_INVALID_VAR_NUM     = -5,
    ERR_TO_CALLOC_VARS      = -6,
    ERR_INVALID_VAR_TEXT    = -7,
    ERR_TO_CALLOC_TOKS      = -8,
    ERR_INVALID_TOKEN       = -9,
    ERR_OPEN_PARSER_LOG     = -10,
    ERR_CLOSE_PARSER_LOG    = -11,
    ERR_NO_END_OF_LINE      = -12,
    ERR_NO_CLOSING_BRACKETS = -12,
};


size_t print_recur_tree(const Node* const node_ptr, FILE* file_ptr);
size_t output_tree(const Node* const root_node_ptr);
double eval(const Tree* const tree_ptr, const Node* const node_ptr);
void   print_recur_code(const Node* const node_ptr, FILE* file_ptr);
size_t generate_cpu_code(const Node* const root_node_ptr);

double func_Add(double value_1, double value_2);
double func_Sub(double value_1, double value_2);
double func_Mul(double value_1, double value_2);
double func_Div(double value_1, double value_2);

double func_Cos(double value_1, double value_2 = 0); // ok
double func_Sin(double value_1, double value_2 = 0); // ok
double func_Tan(double value_1, double value_2 = 0); // ok
double func_Asin(double value_1, double value_2 = 0); // ok
double func_Acos(double value_1, double value_2 = 0); // ok
double func_Sqrt(double value_1, double value_2 = 0); // ok
double func_Exp(double value_1, double value_2 = 0); // ok
double func_Log(double value_1, double value_2 = 0); // ok
double func_Cot(double value_1, double value_2 = 0); // ok
// double func_Log10(double value_1, double value_2 = 0); // ok
double func_Pow(double value_1, double value_2); // ok

Node*  input_tree(Tree* tree_ptr);
size_t check_is_int(char* num_text);
size_t check_is_float(char* num_text);
int    get_into_buff(Tree* tree_ptr);
int    get_size(Tree* tree_ptr);
int    get_tokens(Tree* tree_ptr);
int    realloc_toks(Tree* tree_ptr, size_t i);
void   print_toks(Tree* tree_ptr);
Node*  diff_tree(Tree* tree_ptr);
int    get_vars(Tree* tree_ptr);
Node*  shortener(Tree* tree_ptr, Node* node_ptr);
int    get_eq_string(Tree* const tree_ptr);

Node*  rule_N(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_G(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_E(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_T(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_P(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_V(Tree* const tree_ptr, FILE* log_ptr);
Node*  rule_F(Tree* const tree_ptr, FILE* log_ptr);
Node* rule_Pow(Tree* const tree_ptr, FILE* log_ptr);

Node*  get_recur_tree(Tree* const tree_ptr);
size_t length_double(char* str_double);



#endif
