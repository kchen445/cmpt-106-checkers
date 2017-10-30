CXX = g++
CXX_FLAGS = -std=c++14 -Wall

netdiff: netdiff.cpp
	$(CXX) $(CXX_FLAGS) -o netdiff netdiff.cpp ../lib/ailib.a ../lib/imlib.a
	
neteval: neteval.cpp
	$(CXX) $(CXX_FLAGS) -o neteval neteval.cpp ../lib/ailib.a ../lib/imlib.a

gensave: gensave.cpp
	$(CXX) $(CXX_FLAGS) -o gensave gensave.cpp ../lib/ailib.a ../lib/imlib.a

clean:
	rm *.exe