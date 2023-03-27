CFLAGS = -g -c -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline \
		 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
 		 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy \
		 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
		 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
		 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

EXE_FLAG = valgrind -s --leak-check=yes --show-leak-kinds=all --log-file="./logs/valgrind_log.txt"

CC = g++
TARGET = Differentiator

PREF_OBJ = ./obj/
PREF_SRC = ./src/
PREF_GRV= ../graphviz/src/

DIFF_SRC = $(wildcard $(PREF_SRC)*.cpp)
DIFF_OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(DIFF_SRC))

GRV_CPP = $(wildcard $(PREF_GRV)*.cpp)
GRV_OBJ = $(patsubst $(PREF_GRV)%.cpp, $(PREF_OBJ)%.o, $(GRV_CPP))

OBJ = $(GRV_OBJ) $(DIFF_OBJ)

all : $(TARGET) run

$(TARGET) : $(OBJ) 
	$(CC) $(OBJ) -o $(TARGET).exe

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(PREF_OBJ)%.o : $(PREF_GRV)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm $(TARGET).exe $(PREF_OBJ)*.o

run:
	$(EXE_FLAG) ./$(TARGET).exe