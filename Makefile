CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = server

$(TARGET): main.o system_functions.o http_functions.o
	$(CXX) $(CXXFLAGS) *.o -o $(TARGET)

system_functions.o: system_functions.hpp system_functions.cpp
	$(CXX) $(CXXFLAGS) -c system_functions.cpp

http_functions.o: http_functions.hpp http_functions.cpp
	$(CXX) $(CXXFLAGS) -c http_functions.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f *.o

distclean:
	rm -f $(TARGET) *.o
