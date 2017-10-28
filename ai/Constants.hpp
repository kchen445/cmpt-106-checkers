#ifndef NNETWORK_CONSTANTS_HPP
#define NNETWORK_CONSTANTS_HPP

namespace network {
	//Generation breeding constants
	const double KEEP_RATIO = 0.6;		//the top percentage of organisms to keep after evolution cutoff
	
	const double DIFF_THRESHOLD = 0.5;	//the distance two networks have to be to be considered different species
	const double BREED_CHANCE = 0.69;	//the chance to breed a chosen pair of networks
	const double MUTATE_CHANCE = 0.24;	//the chance to mutate a chosen network
	
	//NNetwork difference constants
	const double c1 = 6;		//excess 
	const double c2 = 0.06;		//disjoint
	const double c3 = 0.0001;	//weight sum
	
	//NNetwork mutate constants
	const int CHANGE_WEIGHT_CHANCE = 20;
	const int ADD_NODE_CHANCE = 5;
	const int ADD_CONN_CHANCE = 5;
	
	const int RANDOM_WEIGHT_CHANCE = 20;
}

#endif