#ifndef NNETWORK_CONSTANTS_HPP
#define NNETWORK_CONSTANTS_HPP

namespace network {
	//(in parens: value used in NEAT paper)
	
	
	//Generation mutation-only evolution constants
	const double KEEP_RATIO = 0.6;		//the top percentage of organisms to keep after evolution cutoff
	
	//Generation breeding evolution constants
	const double DIFF_THRESHOLD = 3;	//the distance two networks have to be to be considered different species	(3)
	const double MUTATION_RATIO = 0.25;	//percentage of offspring created from mutation	rather than breeding		(0.25)
	const double INTERBREED_CHANCE = 0.001;	//interspecies mating chance											(0.001)
	
	//NNetwork difference constants
	const double c1 = 1;	//excess 		(1)
	const double c2 = 1;	//disjoint		(1)
	const double c3 = 0.4;	//weight sum	(0.4)
	
	//NNetwork mutate constants
	const int CHANGE_WEIGHT_CHANCE = 80;	//chance a weight value will be modified		(80)
	const int RANDOM_WEIGHT_CHANCE = 20;	//chance for completely random weight value		(10)
	const double MAX_WEIGHT_DIFF = 0.5;		//maximum change in weights if not randomly assigned
	
	const int ADD_NODE_CHANCE = 5;			//	(3)
	const int ADD_CONN_CHANCE = 5;			//	(5, or 30)
	
	//There was a 75% chance that an inherited gene was disabled if it was disabled in either parent.
	//In each generation, 25% of offspring resulted from mutation without crossover.
	//The interspecies mating rate was 0.001.
	
	//In smaller populations, the probability of adding a new node was 0.03 and the probability of a new link mutation was 0.05.
	//In the larger population, the probability of adding a new link was 0.3, because a larger population can tolerate a larger number of prospective species and greater topological diversity.
}

#endif