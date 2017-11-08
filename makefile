all:  main

CC = gcc -g -O0
INCLUDES = -Iinclude 
LINKER_FLAGS = -std=c++14 -lstdc++ -fopenmp -lXt -lm -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lGLEW
CFLAGS = $(INCLUDES) $(LINKER_FLAGS)
DEPS = include/*

INTERNAL_INCLUDES = main.cpp src/*
EXTERNAL_INCLUDES = 

# To check changes in headers
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


main: $(INTERNAL_INCLUDES) $(EXTERNAL_INCLUDES)
	$(CC) -Wall -o main -DGL_GLEXT_PROTOTYPES $(INTERNAL_INCLUDES) $(CFLAGS)

clean:
	rm main


# TODO Clean this up. Below looks clean. 

# common_inc = ../common
# glad_inc = $(common_inc)/glad
# LINK_FLAGS = -lglfw3 -lopengl32 -lglu32 -lgdi32
# INCLUDES = -I$(common_inc) -I$(glad_inc)

# cpp_files = main.cpp
# objects = $(cpp_files:.cpp=.o)
# headers =

# all: main.exe

# main.exe: $(objects) glad.o
# 	$(CXX) $(LIBRARIES) -o main.exe $(objects) glad.o $(LINK_FLAGS)

# $(objects): %.o: %.cpp $(headers) makefile
# 	$(CXX) $(COMPILE_FLAGS) $(INCLUDES) -c -o $@ $<

# glad.o: glad.c
# 	$(CC) $(COMPILE_FLAGS) $(INCLUDES) -c -o glad.o glad.c