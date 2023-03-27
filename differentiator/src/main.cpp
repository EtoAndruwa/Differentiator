#include "Differentiator.h"

int main()
{
    Tree* tree_ptr = tree_ctor();

    Node* node_1 = create_node(tree_ptr, SUB, IS_OP);
    Node* node_2 = create_node(tree_ptr, 2.1);
    tree_ptr->root = create_node(tree_ptr, ADD, IS_OP, node_1, node_2);

    create_graph_jpg(tree_ptr, "");
    create_html(OUTPUT_NAME);
    

    tree_dtor(tree_ptr);

    return 0;
}

