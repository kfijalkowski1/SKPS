CXXFLAGS += -Wall -O2 # Compiler flags

SRCS = main.cpp tof.cpp       # List of source files
OBJS = $(SRCS:.cpp=.o)        # List of corresponding object files

all: libtof.a tof

libtof.a: tof.o
	ar rcs libtof.a tof.o

tof: main.o
	$(CXX) main.o -L. -ltof -o tof

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) libtof.a tof
