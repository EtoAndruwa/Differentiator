#ifndef DEF_CONFIG_DOT_H
#define DEF_CONFIG_DOT_H

/*####################################################################################################################################################################*/

#include "colors.h"

/*####################################################################################################################################################################*/

#define RANKDIR_DOT          "TB"                              // The format of displaying nodes
#define DEFAULT_BOX_FEATURES ""                                // The nodes' box features

#define DIR_TO_DUMPS         "../graphviz/graph_dumps/"        // The path to the graph_dumps folder for other needs
#define IMAGE_FOLDER         "../graphviz/graph_dumps/images/" // The path to the folder with the ouptput files
#define TXT_FOLDER           "../graphviz/graph_dumps/txt/"    // The path to the folder with input files

#define IMAGE_LIST_NAME      "image_list.txt"                  // The name of the file which is conatining the path to the images that was genearted by dot
#define OUTPUT_FORMAT_FLAG   " -Tjpg "                         // The key to the dot with the output format

#define OUTPUT_FORMAT        ".jpg"                            // The output format from the dot
#define INPUT_FORMAT         ".txt"                            // The input format to the dot                     
#define HTML                 ".html"                           // The html format

#define OUTPUT_NAME          "diff_tree"                       // The name of the output/input files for dot
#define DEFAULT_LEGEND_TEXT  "Empty legend"                    // The default legend's text   
#define SPLINES_DOT          "true"                            // Splines on/off (for edges)

/*####################################################################################################################################################################*/

#endif
