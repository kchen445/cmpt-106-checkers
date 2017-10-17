#include "Generation.hpp"

//random number generation, replace it with something more C++-like if there is one
#include <stdlib>
inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

using namespace network;

Generation::Generation(const char *directory) {
	/*
	//load constants from constants.ini

	//load neural networks
	//create empty species vector
	std::vector<NNetwork*>& species;
	networks.push_back(species);

	while there are still *.txt files to read
		NNetwork* network = new NNetwork(filename);
		species.push_back(network);

	//all networks are loaded into one species, but these are separated when you call step()
	*/
}

void Generation::step() {
	separate();
	for (auto species : networks) {
		compete(species);
	}
	for (auto species : networks) {
		evolve(species);
	}
	curgenes.clear();	//generation is done; empty out all created edges?
}

size_t Generation::getInnovNum(size_t endid, size_t startid) {
	auto it = curgenes.find({endid, startid});
	if (it == curgenes.end()) {
		curinnov++;
		curgenes[{endid, startid}] = curinnov;
		return curinnov;
	} else {
		return *it;
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