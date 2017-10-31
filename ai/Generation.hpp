#ifndef NNETWORK_GENERATION_HPP
#define NNETWORK_GENERATION_HPP

//Generation reads from a directory

#include <vector>
//#include <unordered_map>
#include <map>

#include "NNetwork.hpp"
#include "NNetworkOutputType.hpp"

namespace network {
	class Generation {
		public:	
			NNetworkOutputType* outputDevice;
			
			//all of the neural networks, separated into species
			std::vector<std::vector<NNetwork*>> networks;

			//lookup table of connections -> innovation numbers for all connections
			//!!! change to map for now -- specify hash function for pair for unordered_map
			std::map<std::pair<size_t,size_t>,size_t> curgenes;
			size_t maxinnov = 0;	//the current highest innovation number
			
		/* Saving, loading */
			//Create an empty generation
			Generation(NNetworkOutputType* outputDevice);
			
			//Load a generation from a directory
			Generation(NNetworkOutputType* outputDevice, const std::string &directory);
			//Save a generation to a directory
			void save(const std::string &directory);

			//Destructor
			~Generation();
			
		/* Evolution functions */
			//Evolve forward a generation
			void step();

			//Sorts the species by fitness value, descending
			void compete();
			
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
			
			
		/* Helper function for innovation numbers */
			//Given an edge that a network wants to add, returns the innovation number that edge should take
			//	(to be called by NNetwork when mutating)
			size_t getInnovNum(size_t startid, size_t endid);


			//breeding functions

		
	};
}
#endif