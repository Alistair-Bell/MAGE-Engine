CMP = clang
CST = -std=c89
FLAGS = -g -Wall -Werror -O3 
LIB_DIR = -L./build/lib/ -lmageCore
LIBS = $(LIB_DIR) -lvulkan -lglfw3 -lX11 -ldl -lm -lopenal -lpthread 
BF_DIR = build
BIN_DIR = $(BF_DIR)/bin
OBJ_DIR = $(BF_DIR)/obj/Client
TARGET = Client
OBJS = clientMain.o

.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS) 
	$(CMP) $(CST) $(FLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.o $(LIBS)

clientMain.o: Client/ClientMain.c
	$(CMP) $(CST) $(FLAGS) -c Client/ClientMain.c -o $(OBJ_DIR)/clientMain.o 


clean:

	rm $(OBJ_DIR)/*.o && rm $(BIN_DIR)/*
