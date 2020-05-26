CMP = clang
CST = -std=c89
FLAGS = -g -Wall -Werror -O3 -fPIC
LD_FLAGS = -shared
BF_DIR = build
LIB_DIR = $(BF_DIR)/lib
OBJ_DIR = $(BF_DIR)/obj/Core
TARGET = libmageCore.a
OBJS = mageWindow.o mageInput.o mageMaths.o mageGeneric.o mageRenderer.o mageScene.o mageApplication.o mageDebug.o

.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CMP) $(CST) $(LD_FLAGS) -o $(LIB_DIR)/$(TARGET) $(OBJ_DIR)/*.* 

mageWindow.o: Core/mageAPI.h Core/mageWindow.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageWindow.c -o $(OBJ_DIR)/mageWindow.o

mageInput.o: Core/mageAPI.h Core/mageInput.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageInput.c -o $(OBJ_DIR)/mageInput.o

mageMaths.o: Core/mageAPI.h Core/mageMaths.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageMaths.c -o $(OBJ_DIR)/mageMaths.o

mageGeneric.o: Core/mageAPI.h Core/mageGeneric.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageGeneric.c -o $(OBJ_DIR)/mageGeneric.o

mageRenderer.o: Core/mageAPI.h Core/mageRenderer.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageRenderer.c -o $(OBJ_DIR)/mageRenderer.o

mageScene.o: Core/mageAPI.h Core/mageScene.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageScene.c -o $(OBJ_DIR)/mageScene.o

mageApplication.o: Core/mageAPI.h Core/mageApplication.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageApplication.c -o $(OBJ_DIR)/mageApplication.o

mageDebug.o: Core/mageAPI.h Core/mageDebug.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageDebug.c -o $(OBJ_DIR)/mageDebug.o

clean:
	rm $(OBJ_DIR)/*.o && rm $(LIB_DIR)/*.a
