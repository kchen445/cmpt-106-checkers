#pragma once

#include <memory>
#include "../MLFramework.hpp"
#include "CLEntity.hpp"
#include "CLGameTemplate.hpp"

namespace ml {

    // Template specification:
    //      Player should be a subclass of CLEntity<In, Out> 
    //      and whatever class CLGameTemplate requires
    //
    //      In, Out are the size of the network to be used.
    //      Should match the In, Out for CLEntity inherited
    //      by player.
    template<class Player, size_t In, size_t Out>
    class CLSet : public LearningSet<Player, NUM_ENTITIES_PER_SET> {
    public:

        std::shared_ptr<CLGameTemplate<Player>> game;
        std::shared_ptr<Player> referenceEntity;

        CLSet (std::shared_ptr<CLGameTemplate<Player>> game,
               std::shared_ptr<Player> referenceEntity)
            : LearningSet<Player, NUM_ENTITIES_PER_SET>(),
              game(game),
              referenceEntity(referenceEntity)
        {
            // TODO: populate the entities array with CLEntity objects
        }

        CLSet (std::shared_ptr<NetworkType<In, Out>> seed,
               std::shared_ptr<CLGameTemplate<Player>> game,
               std::shared_ptr<Player> referenceEntity)
            : LearningSet<Player, NUM_ENTITIES_PER_SET>(seed),
              game(game),
              referenceEntity(referenceEntity)
        {}

    };

}
