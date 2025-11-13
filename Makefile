CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

OBJS = main.o Cache.o MainMemory.o
TARGET = CacheSim.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Cache.h MainMemory.h Utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Cache.o: Cache.cpp Cache.h MainMemory.h Utils.h
	$(CXX) $(CXXFLAGS) -c Cache.cpp

MainMemory.o: MainMemory.cpp MainMemory.h
	$(CXX) $(CXXFLAGS) -c MainMemory.cpp

clean:
	del /Q $(OBJS) $(TARGET)
