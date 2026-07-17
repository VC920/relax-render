CC = gcc
CFLAGS = -Wall -Wextra

TAR = rg

SRCS = \
thirdparty/glad/src/glad.c \
code/game/main.c \
code/game/game.c \
code/camera/camera.c \
code/map/map.c \
code/object/entity.c \
code/object/light.c \
code/render/*.c \
code/input/input.c

INCLUDES = \
-Ithirdparty/cglm/include \
-Ithirdparty/glad/include \
-Ithirdparty/stb/include \
-Icode/game \
-Icode/camera \
-Icode/map \
-Icode/object \
-Icode/render \
-Icode/input

LIBS = -lSDL2 -lm

all: $(SRCS)
	$(CC) $(CFLAGS) -o $(TAR) $(SRCS) $(INCLUDES) $(LIBS)

run: $(TAR)
	./$(TAR)

clean: $(TAR)
	rm -rf ./$(TAR)
