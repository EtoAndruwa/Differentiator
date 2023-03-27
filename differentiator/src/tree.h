#ifndef TREE_H
#define TREE_H

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

const int POISON = 0xDED;                                /// \brief The Poison value which is used in order to delete node's values  

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief Union is used in order to store the number of arithmetical operator or the value of the node
 * 
 */
typedef union Node_data
{
    int     op_number;
    double  node_value;
}Node_data;

/**
 * @brief 
 * 
 */
enum error_codes
{
    TREE_OK                = 0,
    ERR_TO_CALLOC_ROOT     = 3,
    ERR_TO_CALLOC_NODE     = 4,
    ERR_TO_CALLOC_TREE     = 5,
};


enum op_numbers
{
    ADD  = 1,
    SUB  = 2,
    DIV  = 3,
    MUL  = 4,
};

enum node_type
{
    IS_OP  = 1,
    IS_VAL = 2,
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief The struct of the node 
 * 
 */
typedef struct Node
{
    Node_data value;   
    char      type        = IS_VAL;    
    Node*     left_child  = nullptr;
    Node*     right_child = nullptr;
}Node;

/**
 * @brief This struct contains error code of the tree and the pointer to the tree's node
 * 
 */
typedef struct Tree
{
    Node*  root = nullptr;
    size_t error_code = TREE_OK;
}Tree;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Tree* tree_ctor();
void  tree_dtor(Tree* tree_ptr);
void  dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = IS_VAL, Node* left_child = nullptr, Node* right_child = nullptr);
void  link_node_left(Node* parent_ptr, Node* child_ptr);
void  link_node_right(Node* parent_ptr, Node* child_ptr);
Node* search_node(Node* node_ptr, Node_data search_value);
void  print_leaves(Node* node_ptr);

#endif
