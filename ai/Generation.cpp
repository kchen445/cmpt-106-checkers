#include "Generation.hpp"
#include <sstream>
#include <fstream>
#include "../include/dirent.h"

#include <stdlib.h>
//random number generation, replace it with something more C++-like if there is one
inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

using namespace network;

Generation::Generation(const char *directory) {
	//load constants from constants.ini?
	
	//load innovation numbers from innovations.txt
	std::ostringstream filename;
	filename << directory << '/' << "innovations.txt";
	std::ifstream file(filename.str(), std::ifstream::in);
	filename.clear();
	
	while (file.good()) {
		size_t innov, startid, endid;
		file >> innov >> startid >> endid >> std::ws;	//skip trailing whitespace too
		curgenes[{startid, endid}] = innov;
	}
	file.close();
	
	//load neural networks
	//create empty species vector
	std::vector<NNetwork*>& species;
	networks.push_back(species);

	//copied from ls.c test script for dirent
	DIR *dir;
    struct dirent *ent;
	dir = opendir(directory);
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG && ent->d_name[0] == 'n') {
				filename << directory << '/' << ent->d_name;
				NNetwork* network = new NNetwork(filename.str());
				filename.clear();
				
				species.push_back(network);
			}
        }
        closedir(dir);
    } else {
		std::ostringstream error;
		error << "Cannot open directory " << directory;
        throw std::runtime_error(error.str());
    }
	//while there are still *.txt files to read
}

Generation::~Generation() {
	for (auto species : networks) {
		for (auto network : species) {
			delete network;
		}
	}
}

void Generation::save(const char *directory) {
	//prepare directory
	//create directory, if it doesn't already exist
	{
	std::ostringstream command;
	command << "mkdir " << directory;	//name is the same on windows/UNIX :D
	system(command);
	}	//braces so stringstream goes out of scope
	//delete all files in directory (?)
	
	//save innovation numbers
	//open file
	std::ostringstream filename;
	filename << directory << '/' << "innovations.txt";
	std::ofstream file(filename.str(), std::ifstream::out);
	filename.clear();
	//test if file was opened -> directory is available
	if (!file.good()) {				
		std::ostringstream error;
		error << "Cannot open directory " << directory;
        throw std::runtime_error(error.str());
	}
	//save innovation number data
	for (auto it=curgenes.begin(); it!=curgenes.end(); ++it) {
		file << it->second << ' '				//innovation number
			 << it->first->first << ' '			//start id
			 << it->first->second << std::endl;	//end id
	}
	file.close();
	
	//save networks into sequential text files
	size_t count = 1;
	for (auto species : networks) {
		for (auto network : species) {
			filename << directory << '/' << "network" << count << ".txt";
			network.save(filename.str());
			filename.clear();
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
	
	//depending on whether innovation numbers 
	curgenes.clear();
}

size_t Generation::getInnovNum(size_t startid, size_t endid) {
	//	if that edge has already been added that generation, returns its value in [curgenes]
	//	otherwise, increments [curinnov] and adds the new edge to [curgenes]
	auto it = curgenes.find({startid, endid});
	if (it == curgenes.end()) {
		curinnov++;
		curgenes[{startid, endid}] = curinnov;
		return curinnov;
	} else {
		return *it;
	}
}

//void compete();


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