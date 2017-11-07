#include "Generation.hpp"
#include "NNetwork.hpp"
#include "Constants.hpp"

#include <fstream>
#include <string>

#ifdef _WIN32
#include "../include/dirent.h"
#else
#include <dirent.h>
#endif
#include <stdlib.h>
inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

using namespace network;

//https://stackoverflow.com/questions/12764645/good-hash-function-with-2-integer-for-a-special-key
//https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
Generation::Generation(NNetworkOutputType* outputDevice) : outputDevice(outputDevice) {
	
}
	
Generation::Generation(NNetworkOutputType* outputDevice, const std::string &directory) : outputDevice(outputDevice), networks(1) {	
	//load innovation numbers
	std::ifstream file(directory + "/_innovations.ini");
	if (!file) {
		throw std::runtime_error("Generation: unable to read " + directory + "/_innovations.ini");
	}
	
	while (file.good()) {
		size_t innov, startid, endid;
		file >> innov >> startid >> endid >> std::ws;
		curgenes[{startid, endid}] = innov;
		maxinnov = (maxinnov > innov) ? maxinnov : innov;
	}
	file.close();
	
	//load neural networks
	//std::vector<NNetwork*> species;		//create empty species vector
	//networks.push_back(species);
	
	DIR *dir;
    struct dirent *ent;
	dir = opendir(directory.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG && ent->d_name[0] != '_') {
				NNetwork* network = new NNetwork(outputDevice, directory + '/' + ent->d_name);
				networks.front().push_back(network);
			}
        }
        closedir(dir);
    } else {
        throw std::runtime_error("Generation: cannot open directory " + directory);
    }
}

Generation::~Generation() {
	for (auto species : networks) {
		for (auto network : species) {
			delete network;
		}
	}
}

void Generation::save(const std::string &directory) {
	//save innovation numbers
	std::ofstream file(directory + "/_innovations.ini");
	if (!file) {
		throw std::runtime_error("Generation: unable to save to " + directory + "/_innovations.ini");
	}
	
	for (auto& kv : curgenes) {
		file << kv.second << ' '				//innovation number
			 << kv.first.first << ' '			//start id
			 << kv.first.second << std::endl;	//end id
	}
	file.close();
	
	//save networks into sequential text files
	size_t count = 1;
	for (auto species : networks) {
		for (auto network : species) {
			network->save(directory + "/network" + std::to_string(count) + ".txt");
			count++;
		}
	}
}

void Generation::step() {
	//mutations only: assume there is only one species

	compete();
	std::vector<NNetwork*>& species = networks.front();
	
	//repopulate generation
	size_t GEN_KEEP = species.size()*KEEP_RATIO;
	for(size_t i=GEN_KEEP; i<species.size(); i++) {
		delete species[i];		//delete network
								//fill in its spot with a mutated copy of a good network
		NNetwork* chosen = species[rand() % GEN_KEEP];
		NNetwork* off = new NNetwork(*chosen);
		off->mutate(*this);
		species[i] = off;
	}
	
	/*for(size_t i=0; i<GEN_KEEP; i++) {
		species[i]->mutate(*this);
	}*/
	//depending on whether innovation numbers are saved between generations
	//curgenes.clear();
}

size_t Generation::getInnovNum(size_t startid, size_t endid) {
	//	if that edge has already been added that generation, returns its value in [curgenes]
	//	otherwise, increments [maxinnov] and adds the new edge to [curgenes]
	auto it = curgenes.find({startid, endid});
	if (it == curgenes.end()) {
		maxinnov++;
		curgenes[{startid, endid}] = maxinnov;
		return maxinnov;
	} else {
		return it->second;
	}
}

/*bool Generation::addGene(size_t startid, size_t endid, size_t innov) {
	curgenes[{startid, endid}] = innov;
}
			void addGenes(NNetwork *net);*/
			
//void Generation::compete() {
 	//sorts networks descending by fitness value
//}


//breeding stuff
//If the maximum fitness of a species did not improve in 15 generations, the networks in the stagnant species were not allowed to reproduce
//. The champion of each species with more than five networks was copied into the next generation unchanged.
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

			if (!sorted) {
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
