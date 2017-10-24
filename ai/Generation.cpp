#include "Generation.hpp"
#include <sstream>

//random number generation, replace it with something more C++-like if there is one
#include <stdlib>
inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

using namespace network;

Generation::Generation(const char *directory) {
	/*
	//load constants from constants.ini?
	
	//load innovation numbers from innovations.txt


	//load neural networks
	//create empty species vector
	std::vector<NNetwork*>& species;
	networks.push_back(species);

	while there are still *.txt files to read
		NNetwork* network = new NNetwork(filename);
		species.push_back(network);

	
	*/
}

Generation::~Generation() {
	for (auto species : networks) {
		for (auto network : species) {
			delete network;
		}
	}
}

void Generation::save(const char *directory) {
	//delete all files in the directory beforehand?

	size_t count = 1;
	std::ostringstream name;
	//save each network into a sequential text file
	for (auto species : networks) {
		for (auto network : species) {
			name << directory << '/' << "network" << count << ".txt";
			network.save(name.str());
			name.clear();
			count++;
		}
	}
}

void Generation::step() {
	//mutations only: assume there is only one species

	compete();	//sorts networks descending by fitness value
	std::vector<NNetwork*>& species = networks.front();
	
	//repopulate generation
	for(size_t i=GEN_KEEP; i<GEN_SIZE; i++) {	
		//delete network
		delete species[i];

		//fill in its spot
		NNetwork* chosen = species[rand() % GEN_KEEP];	//pick a random network
		NNetwork* a = new NNetwork(chosen);				//put a mutated copy of it back into species
		a.mutate(this);
		species[i] = a;
	}
}

size_t Generation::getInnovNum(size_t endid, size_t startid) {
	//	if that edge has already been added that generation, returns its value in [curgenes]
	//	otherwise, increments [curinnov] and adds the new edge to [curgenes]
	auto it = curgenes.find({endid, startid});
	if (it == curgenes.end()) {
		curinnov++;
		curgenes[{endid, startid}] = curinnov;
		return curinnov;
	} else {
		return *it;
	}
}

void compete() {


}
//breeding stuff

/*
	//void Generation::step() {
		//generate number of offspring based on sum of fitness values in species
		std::vector<NNetwork*> offspring;
		for (auto species : networks) {
			NNetwork* child = randnetwork1.breed(randnetwork2);
			offspring.push_back(child);
		}

		//delete all but one random organism in each species
		//delete()
		for (auto network : offspring) {
			bool sorted = false;
			for (auto species : networks) {
				if (species.front()->difference(network) < threshold) {
					network.push_back(species);
					sorted = true;
					break;
				}
			}

			if (not(sorted)) {
				std::vector<NNetwork*> newspecies;
				newspecies.push_back(network);
				network.push_back(newspecies);
			}
		}
		

	//complicated
	void Generation::separate();

	//to be implemented by specific instance
	//void Generation::compete(std::vector<NNetwork*>& species);

	void Generation::evolve(std::vector<NNetwork*>& species) {
		breed(species);		//do breeding
		mutate(species);	//do mutation
		//should these operations be inlined?
	}

	//don't know how it works
	void Generation::breed(std::vector<NNetwork*>& species) {
		//choose two networks somehow, p1 and p2
		if (rand_double() < breed_chance) {
			NNetwork* child = p1->breed(p2);
			species.push_back(child);
		}
	}

	void Generation::mutate(std::vector<NNetwork*>& species) {
		for (auto network : species) {
			if (rand_double() < mutate_chance) {
				network->mutate(this);
				//mutate then calls getInnovNum() when it tries to add any connections
			}
		}
	}
*/