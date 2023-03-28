#include "Differentiator.h"

int main()
{
    Tree* tree_ptr = tree_ctor();
    get_size(tree_ptr);
    get_into_buff(tree_ptr);
    get_tokens(tree_ptr);
    print_toks(tree_ptr);
    tree_ptr->root = input_tree(tree_ptr);
    // input_tree(tree_ptr);
    // Node* node_1 = create_node(tree_ptr, 9);
    // Node* node_2 = create_node(tree_ptr, 100);
    // Node* node_4 = create_node(tree_ptr, 3);
    // Node* node_3 = create_node(tree_ptr, MUL, IS_OP, node_1, node_2);
    // tree_ptr->root = create_node(tree_ptr, ADD, IS_OP, node_3, node_4);
    // print_leaves(tree_ptr->root);
    // printf("\n");
    // print_inorder(tree_ptr->root);
    // printf("\n\n");

    // printf("\n");
    // print_preorder(tree_ptr->root);
    // printf("\n\n");

    // printf("\n");
    // print_postorder(tree_ptr->root);
    // printf("\n\n");


    create_graph_jpg(tree_ptr, "");
    create_html(OUTPUT_NAME);
    

    tree_dtor(tree_ptr);

    return 0;
}

