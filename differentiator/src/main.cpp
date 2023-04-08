#include "Differentiator.h"

int main()
{
    Tree* tree_ptr_1 = tree_ctor();
    get_vars(tree_ptr_1);
    get_size(tree_ptr_1);
    get_into_buff(tree_ptr_1);
    get_tokens(tree_ptr_1);
    tree_ptr_1->root = input_tree(tree_ptr_1);
    create_graph_jpg(tree_ptr_1, "tree_ptr_1");
    // printf("\n\nResult: %lf\n\n", eval(tree_ptr_1->root));

    Tree* tree_ptr_2 = tree_ctor();
    get_vars(tree_ptr_2);
    get_size(tree_ptr_2);
    get_into_buff(tree_ptr_2);
    get_tokens(tree_ptr_2);
    tree_ptr_2->root = diff_tree(tree_ptr_2);
    create_graph_jpg(tree_ptr_2, "tree_ptr_2");
    print_inorder(tree_ptr_2->root);
    
    create_html(OUTPUT_NAME);

    // tree_dtor(tree_ptr_2);
    tree_dtor(tree_ptr_1);

    return 0;
}

