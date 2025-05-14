
#include <iostream>
#include "../include/Diff.h"
#include "../include/Huffman.h" // Passe Pfad zur Huffman-Klasse an
#include "../include/uncompress.h"
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <utility>
#include <list>


Huffman::TreeNBitVector decompressFromStdin(std::istream& name) {
    
    std::vector<bool> M;
    Huffman::TreeNBitVector Daten; 

    for(int index = 0; index < Huffman::MAX; ++index) {
        std::streampos oldPos = name.tellg();

        int j = 0;
        name.read(reinterpret_cast<char*>(&j), sizeof(j));

        std::streamsize bytesRead = name.gcount();
        std::streampos newPos = name.tellg();

        if(!name.good()){
            std::cerr <<"Fehler beim lesen des Baum Nur " << index
             << " int(s) gelesen!\n"
             << "    bytesRead=" << bytesRead
             << ", oldPos=" << oldPos
             << ", newPos= " << newPos
             << ", eof=" << name.eof()
             <<", fail=" << name.fail()
             <<", bad=" << name.bad()
             << std::endl;
             break;
            
        }
        Daten.first[index] = j;
    }

    
    char bytes;

    while (name.read(reinterpret_cast<char*>(&bytes), sizeof(bytes)))
    {
        for(int i = 7; i >= 0; --i) {
            bool bit = (bytes >> i) & 1;
            Daten.second.push_back(bit);
        }
        
    }


    return Daten;
   
}

