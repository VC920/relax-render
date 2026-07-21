CC = gcc
CFLAGS = -Wall -Wextra

TAR = rg

SRCS = \
thirdparty/glad/src/glad.c \
code/game/*.c \
code/camera/*.c \
code/map/*.c \
code/entity/*.c \
code/light/*.c \
code/render/*.c \
code/input/*.c

INCLUDES = \
-Ithirdparty/cglm/include \
-Ithirdparty/glad/include \
-Ithirdparty/stb/include \
-Icode/game \
-Icode/camera \
-Icode/map \
-Icode/entity \
-Icode/light \
-Icode/render \
-Icode/input

LIBS = -lSDL2 -lm

all: $(SRCS)
	$(CC) $(CFLAGS) -o $(TAR) $(SRCS) $(INCLUDES) $(LIBS)

run: $(TAR)
	./$(TAR)

clean: $(TAR)
	rm -rf ./$(TAR)
