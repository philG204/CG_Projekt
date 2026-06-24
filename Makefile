CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -ldl -lm

LDFLAGS += $(shell pkg-config --libs glfw3)
LDFLAGS += $(shell pkg-config --libs glew)

SRC = \
	src/core/input.c \
	src/core/window.c \
	src/utilities/config.c \
	src/utilities/fileOperations.c \
	src/math/matrixTransformation.c \
	src/renderer/loadObj.c \
	src/renderer/loadShader.c \
	src/renderer/mesh.c \
	src/renderer/postProcessing.c \
	src/renderer/shader.c \
	src/renderer/texture.c \
	src/scene/camera.c \
	src/scene/light.c \
	src/scene/object.c \
	src/scene/scene.c \
	src/scene/loadObjectList.c \
	src/main.c

HEADERS = \
	headers/core/input.h \
	headers/core/window.h \
	headers/math/matrixTransformation.h \
	headers/renderer/loadObj.h \
	headers/renderer/loadShader.h \
	headers/renderer/mesh.h \
	headers/renderer/postProcessing.h \
	headers/renderer/shader.h \
	headers/renderer/texture.h \
	headers/scene/camera.h \
	headers/scene/light.h \
	headers/scene/loadObjectList.h \
	headers/scene/object.h \
	headers/scene/scene.h \
	headers/utilities/config.h \
	headers/utilities/fileOperations.h
	  
OBJ = $(SRC:.c=.o)

TEST_SRC = \
	src/math/matrixTransformation.c \
	test/framework.c \
	test/main.c \
	test/test_matrixTransformation.c

TEST_OBJ = $(TEST_SRC:.c=.o)

TARGET = demo
TEST_TARGET = run_tests

.PHONY: all run test clean format docs docs_open

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

format:
	clang-format --style=GNU -i $(SRC) $(HEADERS) $(TEST_SRC)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $(TEST_TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

docs:
	doxygen Doxyfile
	xdg-open docs/html/index.html

docs_open:
	xdg-open docs/html/index.html