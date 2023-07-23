CXXFLAGS += -std=c++17
LIBS += -pthread

myfind: main.o mysearch.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^