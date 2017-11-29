CXX = g++
CXX_FLAGS = -std=c++14 -Wall

#main testing files
tests: net_testmutate.cpp net_testsolve.cpp
	$(CXX) $(CXX_FLAGS) -o testmutate net_testmutate.cpp
	$(CXX) $(CXX_FLAGS) -o testsolve net_testsolve.cpp

testspeed: net_testspeed.cpp
	$(CXX) $(CXX_FLAGS) -o testspeed net_testspeed.cpp ../benchmark.cpp
	
#sample executables
neteval: neteval.cpp
	$(CXX) $(CXX_FLAGS) -o neteval neteval.cpp

genrand: genrand.cpp
	$(CXX) $(CXX_FLAGS) -o genrand genrand.cpp

clean:
	rm *.exe
	rm net_z*.txt