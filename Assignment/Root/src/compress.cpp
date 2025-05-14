#include <iostream>
#include "../include/Diff.h"
#include "../include/Huffman.h"
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <utility>

//Comprimierung  um binärdaten des Graphs auf std::cout auszugeben.



void bitDot(Huffman::TreeNBitVector& daten, std::ostream& os){
 // std::ofstream outFile("output.bin", std::ios::binary);


  //Werte für Huffmans-Baums erstmal schreiben
  for (const auto& val : daten.first) {
    os.write(reinterpret_cast<const char*>(&val), sizeof(val));
  }

  //Für daten.second
  char byte = 0;
  int bitcounter = 0;

  for(bool bit : daten.second){
    byte = (byte << 1) | bit;  //schieben des bits in das aktuellen byte 
    bitcounter++;

    if(bitcounter == 8) {
      os.write(&byte, sizeof(char)); //Schreiben des bytes auf std::cout
      byte = 0;
      bitcounter = 0;

    }
  }
  
  //Wenn nicht volle bits geschrieben
  if(bitcounter > 0) {
    byte <<= (8 - bitcounter); //Mit null auffüllen wie gesagt im Labor
    os.write(&byte, sizeof(char));
  }
   
    
}

