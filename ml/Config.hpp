#pragma once

#include <cstdlib>
#include <memory>

// To avoid duplicate definitions of global variables and to avoid
// having to declare the variables as extern, I'll use a 
// singleton to define the varaibles.
//
// To access the variables use:
//		Config::global-><name>;
class Config {
public:

	// Pointer to the singleton instance.
	// As this is a shared pointer, it is ok to define
	// shared pointers to this object for ease of use.
	//
	//	e.g.
	//		std::shared_ptr<Config> cfg{Config::global};
	static std::shared_ptr<Config> global;

	double differenceCoef = 0.1;

	// Mutation settings
	bool mutateBestNetwork = false;
	size_t cloneAmount = 1;
	size_t smallChangeAmount = 5;

	// Display settings
	int outputInterval = 1;

};

// Out of line initalization for static variable Config::global.
std::shared_ptr<Config> Config::global{new Config{}};
