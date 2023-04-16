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
    char    text[4] = {};
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
    #define DEF_OP(name_op, code_op, ...) name_op = code_op,
    #define DEF_FUNC(name_func, code_func, ...) name_func = code_func,
    #include "def_cmd.h"
    #undef DEF_OP
    #undef DEF_FUNC
};

enum node_type
{
    IS_OP       = 1,
    IS_VAL      = 2,
    IS_VARIB    = 3,
    IS_FUNC     = 4,
    IS_CNST_VAR = 5,
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef union Tok_data
{
    int   int_val;
    double flt_val;
}Tok_data;

typedef struct tokens
{
    Tok_data  value;         /// \brief The value reponsible for asm code of the token
    size_t    type = 0;     /// \brief The value responsible for type of token ('cmd','reg', 'val', 'flg')
    char      text[8];
}tokens;

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

typedef struct Var
{
    char  var_text[2];
    double var_value = 0;
};

/**
 * @brief This struct contains error code of the tree and the pointer to the tree's node
 * 
 */
typedef struct Tree
{
    Node*   root        = nullptr;
    size_t  error_code  = TREE_OK;
    char*   tree_buff   = nullptr;
    char    size        = 0;
    char    num_of_toks = 1;
    tokens* toks        = nullptr;
    size_t  cur_tok     = 0;
    Var*    vars        = nullptr;
    size_t  num_of_vars = 0;
    size_t  cur_pos_str = 0;
    FILE*   file_ptr    = nullptr;
}Tree;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

Tree* tree_ctor();
void  tree_dtor(Tree* tree_ptr);
void  dtor_childs(Node* node_ptr);
Node* create_node(Tree* tree_ptr, double node_value, int node_type = IS_VAL, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);
void  print_leaves(Node* node_ptr);
void  print_inorder(Node* node_ptr);
void  print_preorder(Node* node_ptr);
void  print_postorder(Node* node_ptr);


#endif
