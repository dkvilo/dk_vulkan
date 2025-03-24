CC      := clang
CFLAGS  := -std=c99 -g
HEDERS  := `pkg-config --cflags glfw3` -I./include
LIBS    := `pkg-config --libs glfw3`
LIBS_DIR := libs
BIN_NAME := vk_app

all: build

shaders:
	glslc res/shaders/shader.frag -o frag.spv
	glslc res/shaders/shader.vert -o vert.spv

build:
	$(CC) $(CFLAGS) source/main.c $(HEDERS) $(LIBS) -L$(LIBS_DIR) -lvulkan.1.2.189 -o $(BIN_NAME) -DDEBUG -Wl,-rpath,@executable_path/libs && make shaders