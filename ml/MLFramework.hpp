// Precompiled header for the machine learning framework system.

// Since the all code for the machine learning system is in
// header files, as the majority of it is templated, precompiled
// headers are used to speed up compile time.
//
// For faster compiling, other files should include this header
// and NOT any of the ones in the framework/ directory. This
// goes for the subsystems TargetedLearning and CompetitveLearning
// as well.

#include "framework/Config.hpp"
#include "framework/Display.hpp"
#include "framework/Flags.hpp"
#include "framework/LearningCore.hpp"
#include "framework/LearningEntity.hpp"
#include "framework/LearningSet.hpp"
#include "framework/LearningThread.hpp"
#include "framework/NetworkType.hpp"
#include "framework/NeuralIOSet.hpp"
#include "framework/util.hpp"
