CC = gcc
CFLAGS = -Wall -Wextra

TAR = rg

SRCS = \
thirdparty/glad/src/glad.c \
src/game/main.c src/game/game.c \
src/camera/camera.c \
src/map/map.c \
src/object/entity.c src/object/light.c \
src/render/*.c

INCLUDES = \
-Ithirdparty/cglm/include \
-Ithirdparty/glad/include \
-Ithirdparty/stb/include \
-Isrc/game \
-Isrc/camera \
-Isrc/map \
-Isrc/object \
-Isrc/render

LIBS = -lSDL2 -lm

all: $(SRCS)
	$(CC) $(CFLAGS) -o $(TAR) $(SRCS) $(INCLUDES) $(LIBS)

run: $(TAR)
	./$(TAR)

clean: $(TAR)
	rm -rf ./$(TAR)
