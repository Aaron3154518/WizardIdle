CXX = g++
CXXFLAGS = 

INC = include
BIN = bin
OBJ = obj
SRC = src

SDL_INC = i686-w64-mingw32/include/SDL2
SDL_LIB = i686-w64-mingw32/lib

INCLUDE_PATHS = -I$(INC)/SDL2-2.0.12/$(SDL_INC) -I$(INC)/SDL2_image-2.0.5/$(SDL_INC) -I$(INC)/SDL2_ttf-2.0.15/$(SDL_INC)
LIBRARY_PATHS = -L$(INC)/SDL2-2.0.12/$(SDL_LIB) -L$(INC)/SDL2_image-2.0.5/$(SDL_LIB) -L$(INC)/SDL2_ttf-2.0.15/$(SDL_LIB)
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SOURCES = Rect Number Tools AssetManager Game Upgrade \
          Fireball Wizards WizardContext WizardIdle
OBJECTS = $(patsubst %, $(OBJ)/%.o, $(SOURCES))
DEPENDS = $(patsubst %, $(OBJ)/%.d, $(SOURCES))


wizard_idle: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN)/$@ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

-include $(DEPENDS)

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(BIN)/wizard_idle

$(OBJ)/%.o: $(SRC)/%.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

