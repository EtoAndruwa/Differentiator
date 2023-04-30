EXE_FLAG = valgrind -s --leak-check=yes --show-leak-kinds=all --log-file="./logs/valgrind_log.txt"

CC = g++
TARGET = Dot

PREF_OBJ  = ./obj/
PREF_SRC  = ./src/


SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

all : $(TARGET) run

$(TARGET) : $(OBJ) 
	$(CC) $(OBJ) -o $(TARGET).exe

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC) -c $< -o $@

clean: 
	rm $(TARGET).exe $(PREF_OBJ)*.o ./graph_dumps/images/*.jpg ./graph_dumps/txt/*.txt ./graph_dumps/diff_tree.html ./graph_dumps/image_list.txt 	
clean_files:
	rm ./graph_dumps/images/*.jpg ./graph_dumps/txt/*.txt ./graph_dumps/giff_tree.html ./graph_dumps/image_list.txt ./graph_dumps/diff_tree.html

run:
	$(EXE_FLAG) ./dot.exe 

