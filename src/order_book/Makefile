# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../common $(shell pkg-config --cflags glib-2.0 rdkafka)
LDFLAGS = $(shell pkg-config --libs glib-2.0 rdkafka)

# Source files
SRCS = main.cpp Limit.cpp Book.cpp Match.cpp ../common/Order.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Targets
TARGETS = trading_app

all: $(TARGETS)

trading_app: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Pattern rule for compiling .cpp files to .o files
%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Custom rule for compiling Order.cpp to Order.o in the ../common directory
../common/Order.o: ../common/Order.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	rm -f $(TARGETS) $(OBJS) ../common/*.o

.PHONY: all clean
