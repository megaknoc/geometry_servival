CC = clang
CXX = clang++
LD = clang++

WARN = -Wall \
	   -Werror \
	   -Wno-unused-result \
       -Wno-unused-variable \
       -Wno-unused-function

INCLUDES = -I/usr/lib -lsfml-graphics -lsfml-window -lsfml-system \
		   -lGL -lm

OPTIMIZATION = 0

CFLAGS = -std=c99 -g -O$(OPTIMIZATION) $(WARN)
LDFLAGS = $(INCLUDES)

CXXFLAGS = -std=c++11 -g -O$(OPTIMIZATION) $(WARN)

SRCS = src/hexagon.cpp \
	   src/simulator.cpp \
	   src/framebuffer.c \
	   src/input.c \
	   src/game.c \
	   src/draw.c \
	   src/util.c

OBJS = build/hexagon.o \
	   build/simulator.o \
	   build/framebuffer.o \
	   build/input.o \
	   build/game.o \
	   build/draw.o \
	   build/util.o

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

