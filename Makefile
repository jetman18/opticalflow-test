CC = g++
CFLAGS = -g -Wall
SRCS = ./src/testcv2.cpp ./src/px4flow.cpp
INC = -I ./include
PROG =  ./build/testcv2

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(INC) $(LIBS)
