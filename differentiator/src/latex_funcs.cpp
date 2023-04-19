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
    fprintf(tex_file_ptr, "\t\\begin{equation}\n");

    return RETURN_OK;
}

int print_footer(FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\t\\end{equation}\n");
    fprintf(tex_file_ptr, "\\end{document}\n");

    return RETURN_OK;
}

int create_latex()
{
    



}

int add_equation()
{



}





