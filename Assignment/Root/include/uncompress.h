#pragma once
#ifndef UNCOMPRESS_H
#define UNCOMPRESS_H

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include "Huffman.h"  


// Funktion zum Dekodieren der Binärdaten von einer Datei, die über stdin übergeben wird
Huffman::TreeNBitVector decompressFromStdin(std::istream& name);

#endif // UNCOMPRESS_H
