CXX = g++
CXX_FLAGS = -std=c++14 -Wall

netdiff: netdiff.cpp
	$(CXX) $(CXX_FLAGS) -o netdiff netdiff.cpp ../lib/ailib.a ../lib/imlib.a
	
neteval: neteval.cpp
	$(CXX) $(CXX_FLAGS) -o neteval neteval.cpp ../lib/ailib.a ../lib/imlib.a

gensave: gensave.cpp
	$(CXX) $(CXX_FLAGS) -o gensave gensave.cpp ../lib/ailib.a ../lib/imlib.a

netbreed: netbreed.cpp
	$(CXX) $(CXX_FLAGS) -o netbreed netbreed.cpp ../lib/ailib.a ../lib/imlib.a

xortest: xortest.cpp
	$(CXX) $(CXX_FLAGS) -o xortest xortest.cpp ../lib/ailib.a ../lib/imlib.a
clean:
	rm *.exe