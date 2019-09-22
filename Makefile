CXX= g++
CXXFLAGS = -Wall

make: MedianHeap.o City.o test1.cpp
	$(CXX)  $(CXXFLAGS) City.o MedianHeap.o test1.cpp -o run

City.o: City.h City.cpp
	$(CXX) $(CXXFLAGS) City.h City.cpp -c

MedianHeap.o: MedianHeap.h MedianHeap.cpp
	$(CXX) $(CXXFLAGS) MedianHeap.cpp  MedianHeap.h -c

clean:
	rm *.o output
