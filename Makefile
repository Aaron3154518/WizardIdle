CXX := g++
CXXFLAGS := 

# TODO: auto generate files
INC := include
BIN := bin
OBJ := obj
SRC := src

TARGET := $(BIN)/wizard-idle

SRCS := $(wildcard $(SRC)/*.cpp $(SRC)/**/*.cpp)
EXCLUDE := $(wildcard $(SRC)/*_unittest.cpp $(SRC)/**/*_unittest.cpp)
SRCS := $(filter-out $(EXCLUDE), $(SRCS))
OBJS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
DEPS := $(OBJS:%.o=%.d)

SDL_INC := i686-w64-mingw32/include/SDL2
SDL_LIB := i686-w64-mingw32/lib

INCLUDE_PATHS := -I$(INC)/SDL2-2.0.12/$(SDL_INC) -I$(INC)/SDL2_image-2.0.5/$(SDL_INC) -I$(INC)/SDL2_ttf-2.0.15/$(SDL_INC)
LIBRARY_PATHS := -L$(INC)/SDL2-2.0.12/$(SDL_LIB) -L$(INC)/SDL2_image-2.0.5/$(SDL_LIB) -L$(INC)/SDL2_ttf-2.0.15/$(SDL_LIB)
LINKER_FLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Building $@
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

-include $(DEPS)

.PHONY: clean


$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

clean:
	rm -rf $(OBJ)/* $(TARGET)
	@echo Cleaned
