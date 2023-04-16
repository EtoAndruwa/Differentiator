#include "Differentiator.h"

int main()
{
    Tree* tree_ptr_1 = tree_ctor();
    get_vars(tree_ptr_1);
    get_eq_string(tree_ptr_1);
    tree_ptr_1->root = get_recur_tree(tree_ptr_1);
    // tree_ptr_1->root = shortener(tree_ptr_1, tree_ptr_1->root);

    // get_tokens(tree_ptr_1);
    // tree_ptr_1->root = input_tree(tree_ptr_1);
    create_graph_jpg(tree_ptr_1, "tree_ptr_1");
    double result = eval(tree_ptr_1, tree_ptr_1->root);
    printf("\nresult of eval = %lf\n", result);
    // tree_ptr_1->root = shortener(tree_ptr_1, tree_ptr_1->root);
    // create_graph_jpg(tree_ptr_1, "tree_ptr_1");
    // printf("\n\nResult: %lf\n\n", eval(tree_ptr_1->root));

    // Tree* tree_ptr_2 = tree_ctor();
    // get_vars(tree_ptr_2);
    // get_size(tree_ptr_2);
    // get_into_buff(tree_ptr_2);
    // get_tokens(tree_ptr_2);
    // tree_ptr_2->root = diff_tree(tree_ptr_2);
    // result = eval(tree_ptr_2, tree_ptr_2->root);
    // printf("\nresult of eval = %lf\n", result);
    // create_graph_jpg(tree_ptr_2, "tree_ptr_2");
    // 
    

    
    create_html(OUTPUT_NAME);

    // tree_dtor(tree_ptr_2);
    tree_dtor(tree_ptr_1);

    return 0;
}

