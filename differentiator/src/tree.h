#ifndef TREE_H
#define TREE_H

/*####################################################################################################################################################################*/

const int POISON = 0xDEAD; /// \brief The Poison value which is used in order to delete node's values  

/*####################################################################################################################################################################*/

/**
 * @brief This union is used in order to store the number of arithmetical operator or the value of the node
 */
typedef union Node_data
{
    int     op_number;      // The number of operator
    double  node_value;     // The value of the constant
    char    text[4] = {};   // ???????
}Node_data;

/**
 * @brief 
 */
enum error_codes
{
    TREE_OK                = 0,
    ERR_TO_CALLOC_ROOT     = 1,
    ERR_TO_CALLOC_NODE     = 2,
    ERR_TO_CALLOC_TREE     = 3,
};


/**
 * @brief This enum contains the names and numbers of funcs and operators
 */
enum op_func_numbers 
{
    #define DEF_OP(name_op, code_op, ...) name_op = code_op,
    #define DEF_FUNC(name_func, code_func, ...) name_func = code_func,
    #include "def_cmd.h"
    #undef DEF_OP
    #undef DEF_FUNC
};

/**
 * @brief This enum stores the types of the nodes
 */
enum node_type
{
    IS_OP       = 1,    /// \brief Operator
    IS_VAL      = 2,    /// \brief Value
    IS_VARIB    = 3,    /// \brief Variable
    IS_FUNC     = 4,    /// \brief Function
    IS_CNST_VAR = 5,    /// \brief Constant variable
};

/*####################################################################################################################################################################*/

/**
 * @brief 
 */
typedef union Tok_data
{
    int    int_val;
    double flt_val;
}Tok_data;

/**
 * @brief 
 */
typedef struct tokens
{
    Tok_data  value;    /// \brief The value reponsible for asm code of the token
    size_t    type = 0; /// \brief The value responsible for type of token ('cmd','reg', 'val', 'flg')
    char      text[8];  /// \brief
}tokens;

/**
 * @brief The struct of the node 
 */
typedef struct Node
{
    Node_data value;                 /// \brief The value of the node which is union
    char      type        = IS_VAL;  /// \brief The type of the node
    Node*     left_child  = nullptr; /// \brief The pointer to the left child of the node
    Node*     right_child = nullptr; /// \brief The pointer to the right child of the node
}Node;

/**
 * @brief 
 */
typedef struct Var
{
    char   var_text[10];     /// \brief
    double var_value = 0;   /// \brief
};

/**
 * @brief This struct contains all information of the tree
 */
typedef struct Tree
{
    Node*   root        = nullptr;  /// \brief The pointer to the root node of the tree 
    char*   tree_buff   = nullptr;  /// \brief The pointer to the buffer with the tree equation
    FILE*   file_ptr    = nullptr;  /// \brief The pointer to the file with the tree equation
    tokens* toks        = nullptr;  /// \brief The pointer to the array of token structs
    Var*    vars        = nullptr;  /// \brief The pointer to the array of var structs

    char    size        = 0;        /// \brief The total size of the buffer with the tree equation
    char    num_of_toks = 1;        /// \brief The total number of tokens
    size_t  cur_tok     = 0;        /// \brief The current number of tok in the array of tokens
    size_t  num_of_vars = 0;        /// \brief The total number of variables in the tree
    size_t  cur_pos_str = 0;        /// \brief The current position in the buffer with the tree equation
    size_t  error_code  = TREE_OK;  /// \brief The error code of the struct
}Tree;

/*####################################################################################################################################################################*/

/**
 * @brief Creates and initializes the tree struct
 * @return The pointer to the Tree struct which is containing all information about tree
 */
Tree* tree_ctor();

/*####################################################################################################################################################################*/

/**
 * @brief 
 * @param tree_ptr The pointer to the Tree struct
 */
void tree_dtor(Tree* tree_ptr);

/*####################################################################################################################################################################*/

/**
 * @brief Deletes the node's data and free's all its children and itself
 * @param node_ptr The pointer to the node of the tree (or subtree)
 */
void dtor_childs(Node* node_ptr);

/*####################################################################################################################################################################*/

/**
 * @brief Takes all information about future node and creates it
 * 
 * @param tree_ptr The pointer to the Tree struct
 * @param node_value The value of the node (op_number or node_value)
 * @param node_type The type of the node (fun, op, val, varib)
 * @param text The text of the node if needed (for varib)
 * @param left_child  The pointer to the left child of the node to create link between them
 * @param right_child The pointer to the right child of the node to create link between them
 * @return The pointer to the created node 
 */
Node* create_node(Tree* tree_ptr, double node_value, int node_type = IS_VAL, char* text = nullptr, Node* left_child = nullptr, Node* right_child = nullptr);

/*####################################################################################################################################################################*/

/**
 * @brief Prints all leaves of the node (or subtree)
 * @param node_ptr The pointer to the node (or subtree)
 */
void print_leaves(Node* node_ptr);

/*####################################################################################################################################################################*/

/**
 * @brief Prints the nodes in inorder format
 * @param node_ptr The pointer to the node (or subtree). Usually pointer to the tree's root node
 */
void print_inorder(Node* node_ptr);

/*####################################################################################################################################################################*/

/**
 * @brief Prints the nodes in preorder format
 * @param node_ptr The pointer to the node (or subtree). Usually pointer to the tree's root node
 */
void print_preorder(Node* node_ptr);

/*####################################################################################################################################################################*/

/**
 * @brief Prints the nodes in postorder format
 * @param node_ptr The pointer to the node (or subtree). Usually pointer to the tree's root node
 */
void print_postorder(Node* node_ptr);

/*####################################################################################################################################################################*/

#endif
