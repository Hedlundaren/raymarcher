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
