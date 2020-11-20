CXX = g++
CXXFLAGS = -I.

INC = include
BIN = bin
OBJ = obj
SRC = src

SDL_INC = i686-w64-mingw32/include/SDL2
SDL_LIB = i686-w64-mingw32/lib

INCLUDE_PATHS = -I$(INC)/SDL2-2.0.12/$(SDL_INC) -I$(INC)/SDL2_image-2.0.5/$(SDL_INC) -I$(INC)/SDL2_ttf-2.0.15/$(SDL_INC)
LIBRARY_PATHS = -L$(INC)/SDL2-2.0.12/$(SDL_LIB) -L$(INC)/SDL2_image-2.0.5/$(SDL_LIB) -L$(INC)/SDL2_ttf-2.0.15/$(SDL_LIB)
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

_DEPS = Rect Number Tools AssetManager Game \
       WizardContext WizardIdle
DEPS = $(patsubst %, $(OBJ)/%.o, $(_DEPS))

wizard_idle: $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $^ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
.PHONY: clean

clean:
	rm -f bin/wizard_idle
	rm -f obj/%
