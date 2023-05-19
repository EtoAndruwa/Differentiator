#include "Differentiator.h"

int main()
{
    Tree* tree_ptr_1 = tree_ctor();

    get_eq_string(tree_ptr_1, "input_tree.txt");
    get_vars(tree_ptr_1);

    tree_ptr_1->root = get_recur_tree(tree_ptr_1);
    create_graph_jpg(tree_ptr_1, "tree_ptr_1");

    full_diff(tree_ptr_1);
    create_graph_jpg(tree_ptr_1, "tree_ptr_1 final");

    create_html(OUTPUT_NAME);
    tree_dtor(tree_ptr_1);
    
    return 0;
}

