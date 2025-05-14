#include <iostream>
#include <sstream>
#include "../include/ISCAS.h"
#include "../include/ROBDD.h"
#include "../include/Func.h"
#include "../include/FuncOpt.h"
#include "../include/uncompress.h"
#include "../include/compare.h"
#include "../include/compress.h"
#include <iterator>
#include <map>
#include <stdexcept>
#include <filesystem>
#include <climits>
using namespace std;

int main(int argc,char *argv[]) {

  
  //getname
  std::string name = filesystem::path(argv[0]).filename().string(); //Get name
  
  std::ostringstream output; // für uncompress
  std::ostringstream out; //für dot ausgabe der binär
  
   //std::map<std::string, const ROBDD::Func*>m_Inputs;
//Rufe bevor einlesen von iscas sonst kriege ich fehler

  //4
  if(name == "uncompress") {
      Huffman::TreeNBitVector results = decompressFromStdin(std::cin);
      Huffman::decompress(results, std::cout);
     // std::cout << output.str();
      //compare
  } else if(name == "compare") {
      std::string file1 = argv[1]; 
      std::string file2 = argv[2];
      std::cout << "Datei1" << file1 << " ; und 2: " << file2<< std::endl;
      compare(file1, file2);
      //std::cout << "compare" << std::endl;
    }
  //ISCAS einlesen
	ISCAS iscas(stdin);
  ROBDD robdd;
   

//	if (iscas.isValid()){
    Func dot; //Objekte anlegen Für ROBDD
    FuncOpt opt; //Für optimierung der ROBDD

    const ISCAS::Assignments& assignments = iscas.assignments();

    ISCAS::StrVec outputs = iscas.outputs();

    std::vector<std::string> inputs = iscas.inputs();

 
    int max = INT_MAX; //Für optimierung


    if(argc < 1) {
      std::cerr << "Error: No input file provided." << std::endl;
      return 1;
    }
    

      //1
      if(name == "robdd"){

       dot.inputsVariables(robdd, inputs);
       
       try{
        dot.Struktur(assignments, robdd);
       } catch(const exception& e){
        std::cerr << "Caught exception: " << e.what() << std::endl;
        throw ("Semantikfehler aufgetreten.");
        return -1;

      }
      
      dot.printDot(robdd, iscas.outputs(), out); //war Für Graph zum testen
      //std::cout << out.str();

      std::istringstream in(out.str());

      Huffman::TreeNBitVector treeNBit = Huffman::compress(in);
      
      bitDot(treeNBit, std::cout); //Binäre auf console

    //2

    } else if(name == "robdd-sizes"){

      ROBDD bdd;
      ISCAS::StrVec outputs2 = iscas.outputs();

     ISCAS::StrVec inputs2 = iscas.inputs();
      
     ISCAS::Assignments assignments2 = iscas.assignments();
       
     dot.finalGr(bdd, inputs2, outputs2, assignments2);

            
      
      //3
    } else if(name == "robdd-opt") {
      
      
      ISCAS::StrVec outputs2 = iscas.outputs();

     ISCAS::StrVec inputs2 = iscas.inputs();
      
     ISCAS::Assignments assignments2 = iscas.assignments();
      opt.finalGr(robdd, inputs2, outputs2, assignments2, out);
      /*opt.printDotOpt(robdd, iscas.outputs(), out);*/
      
      //std::cout << out.str(); für Graph um zu testen
      std::istringstream input(out.str());

      Huffman::TreeNBitVector treeNBit = Huffman::compress(input);
      bitDot(treeNBit, std::cout);

    } 
    
    
	return 0;
}


