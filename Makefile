CC = clang
CXX = clang++
LD = clang++

WARN = -Wall \
	   -Werror \
	   -Wno-unused-result \
       -Wno-unused-variable \
       -Wno-unused-function

INCLUDES = -I/usr/lib -lsfml-graphics -lsfml-window -lsfml-system \
		   -lm


OPTIMIZATION = 0

# http://stackoverflow.com/questions/714100/os-detecting-makefile
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	INCLUDES += -lGL -I.
endif
ifeq ($(UNAME_S),Darwin)
	INCLUDES += -framework OpenGL
endif
SFMT = -DSFMT_MEXP=19937
CFLAGS = -std=c99 -g -O$(OPTIMIZATION) $(WARN) $(SFMT)
LDFLAGS = $(INCLUDES)

CXXFLAGS = -std=c++11 -g -O$(OPTIMIZATION) $(WARN) $(SFMT)

SRCS = src/hexagon.cpp \
	   src/simulator.cpp \
	   src/framebuffer.c \
	   src/input.c \
	   src/game.c \
	   src/demo.c \
	   src/draw.c \
	   src/util.c

SRCS += SFMT/SFMT.c

OBJS = build/hexagon.o \
	   build/simulator.o \
	   build/framebuffer.o \
	   build/input.o \
	   build/game.o \
	   build/demo.o \
	   build/draw.o \
	   build/util.o

OBJS += SFMT/SFMT.o

PROJECT = hexagon

default: $(PROJECT)

$(PROJECT): $(OBJS) Makefile
	$(LD) $(LDFLAGS) $(OBJS) -o ${PROJECT}

build/%.o: src/%.c Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

build/%.o: src/%.cpp Makefile
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

run: $(PROJECT)
	./${PROJECT}

valgrind: clean $(PROJECT)
	@echo "valgrind logfile is 'valgrind.log'"
	valgrind --leak-check=full --track-origins=yes --log-file=dumps/valgrind.log ./$(PROJECT)
	@echo "done."

clean:
	$(RM) ${PROJECT}
	$(RM) $(OBJS)

