#ifndef TREE_H
#define TREE_H

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef char* Node_data;                                         /// \brief The type of the node's value
const int POISON = 0xDED;                                /// \brief The Poison value which is used in order to delete node's values  

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief The struct of the node 
 * 
 */
typedef struct Node
{
    Node_data node_value = nullptr;
    Node* left_child  = nullptr;
    Node* right_child = nullptr;
}Node;

/**
 * @brief This struct contains error code of the tree and the pointer to the tree's node
 * 
 */
typedef struct Tree
{
    Node* root = nullptr;
    size_t error_code = TREE_OK;
}Tree;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif
