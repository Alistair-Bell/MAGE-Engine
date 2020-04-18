CMP = clang
CST = -std=c89
FLAGS = -g -Wall
LIBS = -lGL -lGLEW -lglfw3 -ldl -lm -lopenal -lpthread 
LIB_TYPE = -shared
BF_DIR = build
BIN_DIR = $(BF_DIR)/bin
LIB_DIR = $(BF_DIR)/lib
OBJ_DIR = $(BF_DIR)/obj
TARGET = mage
OBJS = mageMain.o mageMemoryDebug.o mageWindow.o mageInput.o mageMaths.o mageGeneric.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CMP) $(CST) $(FLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.* $(LIBS)

mageMain.o: Source/mageMain.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageMain.c -o $(OBJ_DIR)/mageMain.o

mageWindow.o: Source/mageAPI.h Source/mageWindow.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageWindow.c -o $(OBJ_DIR)/mageWindow.o

mageMemoryDebug.o: Source/mageAPI.h Source/mageMemoryDebug.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageMemoryDebug.c -o $(OBJ_DIR)/mageMemoryDebug.o

mageInput.o: Source/mageAPI.h Source/mageInput.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageInput.c -o $(OBJ_DIR)/mageInput.o

mageMaths.o: Source/mageAPI.h Source/mageMaths.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageMaths.c -o $(OBJ_DIR)/mageMaths.o

mageGeneric.o: Source/mageAPI.h Source/mageGeneric.c
	$(CMP) $(CST) $(FLAGS) -c Source/mageGeneric.c -o $(OBJ_DIR)/mageGeneric.o


clean:
	rm $(OBJ_DIR)/*.* $(BIN_DIR)/$(TARGET)

