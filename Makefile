CC = g++
CFLAGS = -g -Wall
SRCS = ./src/main.cpp ./src/px4flow.cpp ./src/debug.cpp
INC = -I ./include
PROG =  ./build/main

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(INC) $(LIBS)
