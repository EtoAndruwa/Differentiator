#include "Differentiator.h"

int main()
{
    Tree* tree_ptr_1 = tree_ctor();
    // Tree* tree_ptr_3 = tree_ctor();
    get_eq_string(tree_ptr_1, "input_tree.txt");
    tree_ptr_1->root = get_recur_tree(tree_ptr_1);
    create_graph_jpg(tree_ptr_1, "tree_ptr_1");

    output_tree(tree_ptr_1->root, "full_brack.txt");
    Tree* tree_ptr_2 = tree_ctor();
    get_eq_string(tree_ptr_2, "full_brack.txt");
    get_vars(tree_ptr_2);
    get_tokens(tree_ptr_2);
    tree_ptr_2->root = input_tree(tree_ptr_2);
    create_graph_jpg(tree_ptr_2, "tree_ptr_2");

    // tree_ptr_3->root = shortener(tree_ptr_2, tree_ptr_2->root);
    // create_graph_jpg(tree_ptr_3, "tree_ptr_3");
    // tree_ptr_4->root = shortener(tree_ptr_3, tree_ptr_3->root);
    // create_graph_jpg(tree_ptr_4, "tree_ptr_4");

    // tree_ptr_2->root = copy_subtree(tree_ptr_2, tree_ptr_2->root);
    // create_graph_jpg(tree_ptr_2, "tree_ptr_3");

    // printf("\nResult: %lf\n", eval(tree_ptr_2, tree_ptr_2->root));

    tree_ptr_2->root = full_diff(tree_ptr_2);
    create_graph_jpg(tree_ptr_2, "tree_ptr_2");
    create_latex(tree_ptr_2->root);

    create_html(OUTPUT_NAME);
    tree_dtor(tree_ptr_1);
    tree_dtor(tree_ptr_2);
    // tree_dtor(tree_ptr_3);

    return 0;
}

