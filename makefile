driver: driver.o
	g++ -std=c++17 -o driver driver.o

driver.o: driver.cpp
	g++ -c driver.cpp

run: 
	./driver

clean:
	rm *.o driver
