CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../common
LDFLAGS =

SRCS = main.cpp ../common/Order.cpp Limit.cpp Book.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = trading_app

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
