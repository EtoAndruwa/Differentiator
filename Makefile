EXE_FLAG = valgrind -s --leak-check=yes --show-leak-kinds=all --log-file="./logs/valgrind_log.txt"

CC = g++

DIR_GRAPHVIZ = ./graphviz
DIR_DIFFERRENTIATOR = ./differentiator
DIR_CPU = ./CPU


all:  Make_d

clean_d: 
	cd $(DIR_DIFFERRENTIATOR) && make clean 

clean_files: clean_df clean_gf

clean_gf:
	cd $(DIR_GRAPHVIZ) && make clean_files

clean_df:
	cd $(DIR_DIFFERRENTIATOR) && make clean_files 

clean_g:
	cd $(DIR_GRAPHVIZ) && make clean

clean_c:
	cd $(DIR_GRAPHVIZ) && make clean_obj

Make_g: 
	cd $(DIR_GRAPHVIZ) && make

Make_d: 
	cd $(DIR_DIFFERRENTIATOR) && make -w

Make_c:
	cd $(DIR_CPU) && make