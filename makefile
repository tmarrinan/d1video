############
MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL
	OPENGL_LIB= -framework OpenGL
	SDL_INC= `sdl2-config --cflags`
	SDL_LIB= `sdl2-config --libs` -lSDL2_ttf
	IMG_INC= -I/usr/local/include
	IMG_LIB= -L/usr/local/lib -lpng -ljpeg
else
	OPENGL_INC= -I/usr/X11R6/include
	OPENGL_LIB= -L/usr/lib64 -lGL -lGLU
	SDL_INC= `sdl2-config --cflags`
	SDL_LIB= `sdl2-config --libs` -lSDL2_ttf
	IMG_INC= -I/usr/include
	IMG_LIB= -L/usr/lib64 -lpng -ljpeg
endif

# object files have corresponding source files
BINDIR= bin

OBJDIR_C= d1vconverter/objs
OBJS_C= $(addprefix $(OBJDIR_C)/, main.o d1vConverter.o)
INCLUDE_C= $(IMG_INC)
LIBS_C= $(IMG_LIB)

OBJDIR_P= d1vplayer/objs
OBJS_P= $(addprefix $(OBJDIR_P)/, main.o d1vPlayer.o)
INCLUDE_P= $(SDL_INC) $(OPENGL_INC) 
LIBS_P= $(SDL_LIB) $(OPENGL_LIB)

CXX= g++
COMPILER_FLAGS= -g

EXEC_C= $(BINDIR)/d1vconverter
EXEC_P= $(BINDIR)/d1vplayer


all: d1vconverter d1vplayer

d1vconverter: $(BINDIR) $(EXEC_C)

d1vplayer: $(BINDIR) $(EXEC_P)


$(BINDIR):
	mkdir -p $(BINDIR)


$(EXEC_C): $(OBJS_C)
	$(CXX) $(COMPILER_FLAGS) -o $(EXEC_C) $(OBJS_C) $(LIBS_C)

$(OBJDIR_C)/%.o: d1vconverter/src/%.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE_C)

$(OBJS_C): | $(OBJDIR_C)

$(OBJDIR_C):
	mkdir -p $(OBJDIR_C)


$(EXEC_P): $(OBJS_P)
	$(CXX) $(COMPILER_FLAGS) -o $(EXEC_P) $(OBJS_P) $(LIBS_P)

$(OBJDIR_P)/%.o: d1vplayer/src/%.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE_P)

$(OBJS_P): | $(OBJDIR_P)

$(OBJDIR_P):
	mkdir -p $(OBJDIR_P)

	
clean:
	rm -f $(EXEC_C) $(OBJS_C) $(EXEC_P) $(OBJS_P)
