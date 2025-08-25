CC        := clang
CFLAGS    := -std=c99 -g
HEDERS    := `pkg-config --cflags glfw3` -I./include
LIBS      := `pkg-config --libs glfw3` -lm
LIBS_DIR  := libs
BIN_NAME  := vk_app

UNAME     := $(shell uname)
VULKAN_LIB :=
RPATH     :=

ifeq ($(UNAME), Darwin)
	VULKAN_LIB := -lvulkan.1.2.189
	RPATH      := -Wl,-rpath,@executable_path/$(LIBS_DIR)
	GLSLC      := glslc
else ifeq ($(UNAME), Linux)
	VULKAN_LIB := -lvulkan
	GLSLC      := ./tools/glslc
	RPATH      :=
endif

all: build

shaders:
	$(GLSLC) res/shaders/shader.frag -o frag.spv
	$(GLSLC) res/shaders/shader.vert -o vert.spv

build:
	$(CC) $(CFLAGS) source/main.c $(HEDERS) $(LIBS) -L$(LIBS_DIR) $(VULKAN_LIB) -o $(BIN_NAME) -DDEBUG $(RPATH) && make shaders

clean:
	rm -f $(BIN_NAME) *.spv
