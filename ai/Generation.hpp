#ifndef NNETWORK_GENERATION_HPP
#define NNETWORK_GENERATION_HPP

//Generation reads from a directory
//	constants.ini: initializes constants
//	*.txt: loads it as a neural network

#include <vector>
#include <unordered_map>

#include "NNetwork.hpp"

namespace network {
	class Generation {
		private:
			const double diff_threshold;	//the distance two networks have to be to be considered different species
			const double breed_chance;		//the chance to breed a chosen pair of networks
			const double mutate_chance;		//the chance to mutate a chosen network
			
			//all of the neural networks, separated into species
			std::vector<std::vector<NNetwork*>> networks;
			//lookup table of connections -> innovation numbers for all connections added this generation
			std::unordered_map<std::pair<size_t,size_t>,size_t> curgenes;
			size_t curinnov;	//the current highest innovation number

			//Separate all networks found in [networks] into species
			void separate();

			//sorts the species by fitness value, descending
			void compete(std::vector<NNetwork*>&) = 0;
			
			//evolves a species
			//same as calling breed() then mutate()
			void evolve(std::vector<NNetwork*>&);

			//(should these be inlined in evolve()?)
			//randomly breeds networks within a species and adds them to the species
			void breed(std::vector<NNetwork*>&);
			//randomly mutates networks within a species by calling NNetwork.mutate
			void mutate(std::vector<NNetwork*>&);

		public:	
			//Loads generation data from a directory
			//	- constants
			//	- all neural networks (which are put into a single species)
			Generation(const char *directory);

			//Evolves everything forward a generation
			//	Calls separate(), then compete() on each species, then evolve() on each species
			//	clears out curgenes after finished
			void step();

			//given an edge that a network wants to add, returns the innovation number that edge should take
			//	if that edge has already been added that generation, returns its value in [curgenes]
			//	otherwise, increments [curinnov] and adds the new edge to [curgenes]
			//to be called by NNetwork when mutating
			size_t getInnovNum(size_t endid, size_t startid);
	};
}
#endif