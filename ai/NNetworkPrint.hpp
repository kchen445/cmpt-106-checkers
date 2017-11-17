namespace network {
	template <size_t In, size_t Out>
	void print_network(const NNetwork<In,Out> &net) {
		std::cout << "Nodes: " << net.numNodes << " / Inputs: " << In << " / Outputs: " << Out << std::endl;
		std::cout << "Biases: " << net.biases.size() << " / Groups: " << net.connsize << std::endl;
		
		std::cout << "Biases: ";
		for (auto bias : net.biases) {
			std::cout << bias << ' ';
		}
		std::cout << std::endl;
		
		for (auto group : net.conns) {
			std::cout << group[0].startid << " | ";
			for (auto conn : group) {
				if (conn.enabled) {
					std::cout << "(" << conn.endid << ',' << conn.weight << ") ";
				}
			}
			std::cout << std::endl;
		}
	}
}