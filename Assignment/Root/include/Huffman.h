#pragma once

#include <vector>
#include <array>

class Huffman {
public: // types

	static const unsigned MAX = 512;
	// std::array<int,MAX> is the Huffman encoding tree
	// std::vector<bool> is the compressed text
	using TreeNBitVector = std::pair<std::array<int,MAX>,std::vector<bool>>;
	
public: // methods

	static TreeNBitVector compress(std::istream&);

	static void decompress(const TreeNBitVector&,std::ostream&);
};
