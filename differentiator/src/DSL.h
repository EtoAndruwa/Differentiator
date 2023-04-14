#ifndef DSL_H
#define DSL_H

#define GET_CUR_TOK()                    \
    size_t cur_tok = tree_ptr->cur_tok;  \
    tree_ptr->cur_tok++;                 \

    
#define NUM_NODE(value) create_node(tree_ptr, value);
#define MUL_NODE(left_child, right_child) create_node(tree_ptr, Mul, IS_OP, nullptr, left_child, right_child);
#define ADD_NODE(left_child, right_child) create_node(tree_ptr, Add, IS_OP, nullptr, left_child, right_child);
#define SUB_NODE(left_child, right_child) create_node(tree_ptr, Sub, IS_OP, nullptr, left_child, right_child);
#define DIV_NODE(left_child, right_child) create_node(tree_ptr, Div, IS_OP, nullptr, left_child, right_child);
#define SIN_NODE(left_child)              create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_child);
#define COS_NODE(left_child)              create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_child);
#define SQRT_NODE(left_child)             create_node(tree_ptr, Sqrt, IS_FUNC, nullptr, left_child);
#define LN_NODE(left_child)               create_node(tree_ptr, Log, IS_FUNC, nullptr, left_child);
#define POW_NODE(base, exp)               create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp); 

#endif
