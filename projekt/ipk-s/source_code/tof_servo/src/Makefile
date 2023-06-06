CXXFLAGS += -Wall -O2 # Compiler flags

SRCS = main.cpp tof.cpp       # List of source files
OBJS = $(SRCS:.cpp=.o)        # List of corresponding object files

all: libtof.a tof_servo

libtof.a: tof.o
	ar rcs libtof.a tof.o

tof_servo: main.o
	$(CXX) main.o -L. -ltof -o tof_servo

tof.o: tof.cpp
	$(CXX) $(CXXFLAGS) -c tof.cpp -o tof.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -lrt -o main.o

clean:
	rm -f $(OBJS) libtof.a tof_servo
