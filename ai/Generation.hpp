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
			//general constants
			const size_t GEN_SIZE;	//the number of organisms in each generation
			const size_t GEN_KEEP;	//the number of organisms to keep

			//breeding constants
			const double DIFF_THRESHOLD;	//the distance two networks have to be to be considered different species
			const double BREED_CHANCE;		//the chance to breed a chosen pair of networks
			const double MUTATE_CHANCE;		//the chance to mutate a chosen network
			
			
			//all of the neural networks, separated into species
			std::vector<std::vector<NNetwork*>> networks;

			//lookup table of connections -> innovation numbers for all connections added this generation
			std::unordered_map<std::pair<size_t,size_t>,size_t> curgenes;
			size_t curinnov;	//the current highest innovation number


			//sorts the species by fitness value, descending
			void compete(std::vector<NNetwork*>&) = 0;
			

			//breeding functions
			/*
				//Separate all networks found in [networks] into species
				void separate();

				//evolves a species
				//same as calling breed() then mutate()
				void evolve(std::vector<NNetwork*>&);

				//(should these be inlined in evolve()?)
				//randomly breeds networks within a species and adds them to the species
				void breed(std::vector<NNetwork*>&);
				//randomly mutates networks within a species by calling NNetwork.mutate
				void mutate(std::vector<NNetwork*>&);
			*/

		public:	
			//Loads generation data from a directory
			//	- constants
			//	- all neural networks (which are put into a single species)
			Generation(const char *directory);
			void Generation::save(const char *directory);

			~Generation();

			//Evolves everything forward a generation.
			void step();

			//given an edge that a network wants to add, returns the innovation number that edge should take
			//to be called by NNetwork when mutating
			size_t getInnovNum(size_t endid, size_t startid);
	};
}

/*double NNetwork::difference (NNetwork& other) {
	size_t E = 0;
	size_t D = 0;
	size_t N = 0;


	double W = 0;
	size_t Wcount = 0;

	auto it = this.conns.begin();
	auto jt = other.conns.begin();

	if (it->innov == jt->innov) {
		W += abs(it->weight - jt->weight);
		Wcount++;
		it++;
		jt++;
	} else {
		D++;
		if (it->innov < jt->innov) {
			it++;
		} else {
			jt++;
		}
	}

}*/
#endif