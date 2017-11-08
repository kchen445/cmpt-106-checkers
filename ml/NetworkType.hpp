#ifndef ML_NETWORK_TYPE
#define ML_NETWORK_TYPE

#include <array>
#include <memory>
#include <string>

namespace ml {


    template<size_t In, size_t Out>
    class NetworkType {
    public:

        // Abstract method to solve this network.
        virtual std::array<double, Out> solve (std::array<double, In> const &input) = 0;

        // Mutation methods.
        virtual void tweakWeight (int chance, double range) = 0;
        virtual void randomizeWeight (int chance) = 0;

        virtual void tweakBias (int chance, double range) = 0;
        virtual void randomizeBias (int chance) = 0;

        // Should return a network with the same configuration.
        // The returned network should be an isolated entity that
        // shares no pointers or anything with the base network.
        //
        // The network should be declared used the new operator.
        virtual std::shared_ptr<NetworkType<In, Out>> clone () = 0;

        // Saves this network.
        virtual void save (std::string const &filename) = 0;

        virtual ~NetworkType () {}

    };

}


namespace network {

	// using declaration in network namespace for convenience.
	template<size_t In, size_t Out>
	using NetworkType = ml::NetworkType<In, Out>;

}

#endif
