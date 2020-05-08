CMP = clang
CST = -std=c89
FLAGS = -g -Wall 
LIBS = -lvulkan -lglfw3 -lX11 -ldl -lm -lopenal -lpthread -lvulkan_radeon
LIB_TYPE = -shared
BF_DIR = build
BIN_DIR = $(BF_DIR)/bin
LIB_DIR = $(BF_DIR)/lib
OBJ_DIR = $(BF_DIR)/obj
TARGET = mage
OBJS = mageMain.o mageWindow.o mageInput.o mageMaths.o mageGeneric.o mageRenderer.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CMP) $(CST) $(FLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.* $(LIBS)

mageMain.o: Core/mageMain.c
	$(CMP) $(CST) $(FLAGS) -c Core/mageMain.c -o $(OBJ_DIR)/mageMain.o

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
	
clean:
	rm $(OBJ_DIR)/*.* $(BIN_DIR)/$(TARGET)
