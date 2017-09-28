nodes = ConnectionType.hpp NodeType.hpp InputNode.hpp OutputNode.hpp ThresholdNode.hpp

main2: main2.o NNetwork.o
	g++ -Wall -o main2 main2.o NNetwork.o
	
NNetwork.o: NNetwork.cpp $(nodes)
main2.o: main2.cpp

clean:
	rm NNetwork.o main2.o main2