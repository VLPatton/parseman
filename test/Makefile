SOURCES=main.cpp

STDVER?=gnu++20

CXXFLAGS=-I../src -std=$(STDVER) -Wall
CXX=g++
LDFLAGS=

.PHONY: all run clean

all: run

run: test
	./$<

test: $(SOURCES:.cpp=.o)
	$(CXX) -o $@ $(CXXFLAGS) $^

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	rm test $(SOURCES:.cpp=.o)
