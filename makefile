CMP = clang
CST = -std=c89
FLAGS = -g -Wall
LIBS = -lvulkan_radeon -lglfw3 -ldl -lm -lopenal -lpthread 
LIB_TYPE = -shared
BF_DIR = build
BIN_DIR = $(BF_DIR)/bin
LIB_DIR = $(BF_DIR)/lib
OBJ_DIR = $(BF_DIR)/obj
TARGET = mage
OBJS = mageMain.o mageWindow.o mageInput.o mageMaths.o mageGeneric.o mageRenderer.o mageBuffers.o mageShaders.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CMP) $(CST) $(FLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.* $(LIBS)

mageMain.o: Source/mageMain.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageMain.c -o $(OBJ_DIR)/mageMain.o

mageWindow.o: Source/mageAPI.h Source/mageWindow.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageWindow.c -o $(OBJ_DIR)/mageWindow.o

mageInput.o: Source/mageAPI.h Source/mageInput.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageInput.c -o $(OBJ_DIR)/mageInput.o

mageMaths.o: Source/mageAPI.h Source/mageMaths.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageMaths.c -o $(OBJ_DIR)/mageMaths.o

mageGeneric.o: Source/mageAPI.h Source/mageGeneric.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageGeneric.c -o $(OBJ_DIR)/mageGeneric.o

mageRenderer.o: Source/mageAPI.h Source/mageRenderer.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageRenderer.c -o $(OBJ_DIR)/mageRenderer.o

mageBuffers.o: Source/mageAPI.h Source/mageBuffers.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageBuffers.c -o $(OBJ_DIR)/mageBuffers.o

mageShaders.o: Source/mageAPI.h Source/mageShaders.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageShaders.c -o $(OBJ_DIR)/mageShaders.o

clean:
	rm $(OBJ_DIR)/*.* $(BIN_DIR)/$(TARGET)

