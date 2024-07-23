CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS =

SRCS = main.cpp Order.cpp Limit.cpp Book.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = trading_app

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean

