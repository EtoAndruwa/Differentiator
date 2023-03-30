#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tree.h"
#include "../../graphviz/src/graphviz.h"

#define EPS 1e-7
#define PI  3.14159265

const int MAX_LEN_FSCANF = 11;

enum return_codes
{
    NULL_PTR_NODE  = 1,
    ALL_DIGITS     = 2,
    NOT_ALL_DIGITS = 3,
};

enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = 1,
    ERR_CANNOT_CLOSE_OUTPUT = 2, 
    ERR_CANNOT_OPEN_INPUT   = 3,
    ERR_CANNOT_CLOSE_INPUT  = 4,
};

size_t print_recur_tree(const Node* const node_ptr, FILE* file_ptr);
size_t output_tree(const Node* const root_node_ptr);
double eval(const Node* const node_ptr, double var_value);
void   print_recur_code(const Node* const node_ptr, FILE* file_ptr);
size_t generate_cpu_code(const Node* const root_node_ptr);

double func_Add(double value_1, double value_2);
double func_Sub(double value_1, double value_2);
double func_Mul(double value_1, double value_2);
double func_Div(double value_1, double value_2);

double func_Cos(double value_1, double value_2 = 0);
double func_Sin(double value_1, double value_2 = 0);
double func_Tan(double value_1, double value_2 = 0);
double func_Asin(double value_1, double value_2 = 0);
double func_Acos(double value_1, double value_2 = 0);
double func_Sqrt(double value_1, double value_2 = 0);
double func_Exp(double value_1, double value_2 = 0);
double func_Log(double value_1, double value_2 = 0);
double func_Cot(double value_1, double value_2 = 0);
double func_Log10(double value_1, double value_2 = 0);
double func_Pow(double value_1, double value_2);

Node*  input_tree(Tree* tree_ptr);
size_t check_is_number(char* num_text);
size_t get_into_buff(Tree* tree_ptr);
size_t get_size(Tree* tree_ptr);
size_t get_tokens(Tree* tree_ptr);
size_t realloc_toks(Tree* tree_ptr, size_t i);
void   print_toks(Tree* tree_ptr);
Node*  diff_tree(Tree* tree_ptr);

#endif
