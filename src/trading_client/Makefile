# Makefile for compiling producer and consumer

# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -I../common `pkg-config --cflags --libs glib-2.0 rdkafka`

# Source files
SRCS = producer.cpp ../common/Order.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Targets
TARGETS = producer

all: $(TARGETS)

producer: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	rm -f $(TARGETS) *.o ../common/*.o

.PHONY: all clean
