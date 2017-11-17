CXX = g++
CXX_FLAGS = -std=c++14 -Wall

# netdiff: netdiff.cpp ../lib/ailib.a ../lib/imlib.a
	# $(CXX) $(CXX_FLAGS) -o netdiff netdiff.cpp ../lib/ailib.a ../lib/imlib.a

# gensave: gensave.cpp
	# $(CXX) $(CXX_FLAGS) -o gensave gensave.cpp ../lib/ailib.a ../lib/imlib.a
	
# netbreed: netbreed.cpp
	# $(CXX) $(CXX_FLAGS) -o netbreed netbreed.cpp ../lib/ailib.a ../lib/imlib.a

# xortest: xortest.cpp
	# $(CXX) $(CXX_FLAGS) -o xortest xortest.cpp ../lib/ailib.a ../lib/imlib.a

# netmutate: netmutate.cpp ../lib/ailib.a ../lib/imlib.a
	# $(CXX) $(CXX_FLAGS) -o netmutate netmutate.cpp ../lib/ailib.a ../lib/imlib.a

#main testing files
tests: net_testmutate.cpp net_testsolve.cpp
	$(CXX) $(CXX_FLAGS) -o testmutate net_testmutate.cpp
	$(CXX) $(CXX_FLAGS) -o testsolve net_testsolve.cpp

#sample executables
neteval: neteval.cpp
	$(CXX) $(CXX_FLAGS) -o neteval neteval.cpp

genrand: genrand.cpp
	$(CXX) $(CXX_FLAGS) -o genrand genrand.cpp ../lib/ailib.a ../lib/imlib.a

clean:
	rm *.exe
	rm net_z*.txt