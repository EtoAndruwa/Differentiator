#include "Differentiator.h"

int print_header(FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\\documentclass[a4paper, 12pt]{article}\n");
    fprintf(tex_file_ptr, "\\usepackage[english]{babel}\n");
    fprintf(tex_file_ptr, "\\usepackage{amsfonts}\n");
    fprintf(tex_file_ptr, "\\usepackage{pgfplots}\n");
    fprintf(tex_file_ptr, "\\usepackage[utf8]{inputenc}\n");
    fprintf(tex_file_ptr, "\\usepackage{amsmath,amssymb}\n");

    fprintf(tex_file_ptr, "\\begin{document}\n");

    return RETURN_OK;
}

int print_footer(FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\\end{document}\n");

    return RETURN_OK;
}

int create_latex(Node* root_node_ptr)
{
    FILE* tex_file_ptr = fopen(LATEX_FILE_NAME, "w");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    if(print_header(tex_file_ptr) != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, ERR_PRINT_LATEX)
        return ERR_PRINT_LATEX;
    }

    add_equation(root_node_ptr, tex_file_ptr);

    if(print_footer(tex_file_ptr) != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, ERR_PRINT_LATEX)
        return ERR_PRINT_LATEX; 
    }

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }
}

int add_equation(Node* node_ptr, FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\t\\begin{equation}\n");

    print_latex_eq(node_ptr, tex_file_ptr);

    fprintf(tex_file_ptr, "\t\\end{equation}\n");
}

int print_latex_eq(Node* node_ptr, FILE* tex_file_ptr)
{
    if(node_ptr == nullptr)
    {
        return 0;
    }
    else if(node_ptr->type == IS_VAL)
    {
        fprintf(tex_file_ptr, "%lf", node_ptr->value.node_value);
    }
    else if(node_ptr->type == IS_VARIB)
    {
        fprintf(tex_file_ptr, "%s", node_ptr->value.text);
    }
    else if(node_ptr->type == IS_OP)
    {
        switch(node_ptr->value.op_number)
        {
        case Mul:
            {
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, "\\cdot ");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                break;
            }
        case Add:
            {
                fprintf(tex_file_ptr, "(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, " + ");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");
                break;
            }
        case Sub:
            {
                fprintf(tex_file_ptr, "(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, " - ");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");
                break;
            }
        case Div:
            {
                fprintf(tex_file_ptr, "\\frac{");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, "}");
                fprintf(tex_file_ptr, "{");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                fprintf(tex_file_ptr, "}");
                break;
            }
        default:
            fprintf(tex_file_ptr, "ERR_UNKNOWN_OPERATOR");
            ERROR_MESSAGE(stderr, ERR_UNKNOWN_OPERATOR)
            break;
        }
    }
    else 
    {
        switch(node_ptr->value.op_number)
        {
        case Sin:
            {
                fprintf(tex_file_ptr, "sin(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");
                break;
            }
        case Cos:
            {
                fprintf(tex_file_ptr, "cos(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");
                break;
            }
        case Tan:
            {
                fprintf(tex_file_ptr, "tan(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");
                break;
            }
        default:
            fprintf(tex_file_ptr, "ERR_UNKNOWN_FUNC");
            ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
            break;
        }
    }
}



