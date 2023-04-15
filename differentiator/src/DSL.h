#ifndef DSL_H
#define DSL_H

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_ptr->cur_tok;  \
    tree_ptr->cur_tok++;                 \

    
#define NUM_NODE(value)                   create_node(tree_ptr, value);
#define MUL_NODE(left_child, right_child) create_node(tree_ptr, Mul, IS_OP, nullptr, left_child, right_child);
#define ADD_NODE(left_child, right_child) create_node(tree_ptr, Add, IS_OP, nullptr, left_child, right_child);
#define SUB_NODE(left_child, right_child) create_node(tree_ptr, Sub, IS_OP, nullptr, left_child, right_child);
#define DIV_NODE(left_child, right_child) create_node(tree_ptr, Div, IS_OP, nullptr, left_child, right_child);
#define SIN_NODE(left_child)              create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_child);
#define COS_NODE(left_child)              create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_child);
#define SQRT_NODE(left_child)             create_node(tree_ptr, Sqrt, IS_FUNC, nullptr, left_child);
#define LN_NODE(left_child)               create_node(tree_ptr, Log, IS_FUNC, nullptr, left_child);
#define POW_NODE(base, exp)               create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp); 

#define PRINT_PARSE_LOG(log_ptr, rule_name, rule_wait)                                                                           \
    fprintf(log_ptr, "Called rule name: %s(Waiting for: %s). Current position in line: %ld. Current char string[%ld] = '%c'\n",  \
        rule_name, rule_wait, tree_ptr->cur_pos_str, tree_ptr->cur_pos_str, tree_ptr->tree_buff[tree_ptr->cur_pos_str]);         \   

#define RULE_E "E"
#define RULE_G "G"
#define RULE_P "P"
#define RULE_N "N"
#define RULE_T "T"

#define RULE_E_WAIT "+/-"
#define RULE_G_WAIT "rule E or end of line"
#define RULE_P_WAIT "( or )"
#define RULE_T_WAIT "mul/div"
#define RULE_N_WAIT "number"
#define STRING(pos)  tree_ptr->tree_buff[pos]
#define POSITION     tree_ptr->cur_pos_str
#define FUNC_NAME    __func__

#endif
