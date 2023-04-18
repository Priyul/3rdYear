CXX = g++
CXXFLAGS = -std=c++11 -Wall

driver: driver.o
	$(CXX) $(CXXFLAGS) -o driver driver.o

driver.o: driver.cpp
	$(CXX) $(CXXFLAGS) -c driver.cpp

clean:
	rm -f driver driver.o
