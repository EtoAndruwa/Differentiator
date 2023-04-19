#ifndef DSL_H
#define DSL_H

/*####################################################################################################################################################################*/

#define STRING(pos)                       tree_ptr->tree_buff[pos]
#define POSITION                          tree_ptr->cur_pos_str
#define FUNC_NAME                         __func__
#define NODE_LEFT_CHILD                   node_ptr->left_child
#define NODE_RIGHT_CHILD                  node_ptr->right_child
#define SHORT_CHILD(child_node)           shortener(tree_ptr, child_node)

/*####################################################################################################################################################################*/

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_ptr->cur_tok;  \
    tree_ptr->cur_tok++;                 \

#define PRINT_PARSE_LOG(log_ptr, rule_name, rule_wait, error_name)                                                                   \
    fprintf(log_ptr, "Called rule name: %s(Waiting for: %s). Current position in line: %ld. Current char string[%ld] = '%c'.(%s)\n", \
        rule_name, rule_wait, tree_ptr->cur_pos_str, tree_ptr->cur_pos_str, tree_ptr->tree_buff[tree_ptr->cur_pos_str], error_name); \   

/*####################################################################################################################################################################*/    

#define NUM_NODE(value)                   create_node(tree_ptr, value);
#define MUL_NODE(left_child, right_child) create_node(tree_ptr, Mul, IS_OP, nullptr, left_child, right_child);
#define ADD_NODE(left_child, right_child) create_node(tree_ptr, Add, IS_OP, nullptr, left_child, right_child);
#define SUB_NODE(left_child, right_child) create_node(tree_ptr, Sub, IS_OP, nullptr, left_child, right_child);
#define DIV_NODE(left_child, right_child) create_node(tree_ptr, Div, IS_OP, nullptr, left_child, right_child);
#define SIN_NODE(left_child)              create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_child);
#define COS_NODE(left_child)              create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_child);
#define SQRT_NODE(left_child)             create_node(tree_ptr, Sqrt, IS_FUNC, nullptr, left_child);
#define TAN_NODE(left_child)              create_node(tree_ptr, Tan, IS_FUNC, nullptr, left_child);
#define COT_NODE(left_child)              create_node(tree_ptr, Cot, IS_FUNC, nullptr, left_child);
#define ASIN_NODE(left_child)             create_node(tree_ptr, Asin, IS_FUNC, nullptr, left_child);
#define ACOS_NODE(left_child)             create_node(tree_ptr, Acos, IS_FUNC, nullptr, left_child);
#define LN_NODE(left_child)               create_node(tree_ptr, Log, IS_FUNC, nullptr, left_child);
#define POW_NODE(base, exp)               create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp);
#define EXP_NODE(exp)                     create_node(tree_ptr, Exp, IS_FUNC, nullptr, exp);
#define VARIB_NODE(text)                  create_node(tree_ptr, 0, IS_VARIB,  text);
#define CNST_VARIB_NODE(text)             create_node(tree_ptr, 0, IS_CNST_VAR, text);

/*####################################################################################################################################################################*/

#define RULE_E "E"
#define RULE_G "G"
#define RULE_P "P"
#define RULE_N "N"
#define RULE_T "T"
#define RULE_V "V"
#define RULE_F "F"

#define RULE_E_WAIT  "+/-"
#define RULE_G_WAIT  "rule E or end of line"
#define RULE_P_WAIT  "( or )"
#define RULE_T_WAIT  "mul/div"
#define RULE_N_WAIT  "number"
#define RULE_V_WAIT  "variable"
#define RULE_F_WAIT  "function"

#define RULE_OK      "NO ERRORS DETECTED"
#define RULE_N_ERR   "NOT A NUMBER"
#define RULE_G_ERR   "NO END OF FILE"
#define RULE_E_ERR   "NULL PTR CHILD"
#define RULE_T_ERR   "NULL PTR CHILD"
#define RULE_P_ERR   "NO CLOSING BRACKETS"
#define RULE_F_ERR   "NO CLOSING BRACKETS OR INNER PROBLEMS OR NON EXISTING FUNC"
#define RULE_V_ERR   "NOT A VARIABLE"

/*####################################################################################################################################################################*/

#endif
