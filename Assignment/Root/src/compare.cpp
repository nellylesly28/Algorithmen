#include <iostream>
#include "../include/Huffman.h" 
#include "../include/Diff.h"
#include "../include/uncompress.h"
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
using namespace std;

std::vector<std::string>read(const std::string& name){

  std::vector<std::string> datei;
  std::string line;

  std::fstream file(name, std::fstream::in);
  if(!file){
    std::cerr << "Fehler beim öffnen der Datei 2" << name << std::endl;
  }
  while(std::getline(file, line)){
    datei.push_back(line);

  }
  file.close();
  return datei;
  

}
void compare (const std::string& Datei1,const std::string& Datei2){
  std::vector<std::string> datei1 = read(Datei1);
  std::vector<std::string> datei2 = read(Datei2);

  /*std::string line;

  std::fstream file1(Datei1, std::fstream::in);
  if(!file1){
    std::cerr << "Fehler beim öffnen der Datei 2" << Datei1 << std::endl;
  }
  while(std::getline(file1, line)){
    datei1.push_back(line);

  }
  file1.close();
  


 std::fstream file2(Datei2, std::fstream::in);
  if(!file2){
    std::cerr << "Fehler beim öffnen der datei 2" << Datei1 << std::endl;
  }
  while(std::getline(file2, line)){
    datei2.push_back(line);

  }
  
  file2.close();
*/

    //Für nicht Komprimierte Datei
    std::vector<unsigned>differences = Diff::shortestPaths(datei1, datei2);
    std::cout << "unterschied: " << std::endl;
    for(const auto& diff : differences ) {
      std::cout << diff << " ";
    }

    cout << endl;

    unsigned x=0;  
    unsigned y=0;  

    unsigned prevx = 0; //startpunkt 
    unsigned prevy = 0; //startpunkt


    int size =static_cast<int>(differences.size());  //hier speicher wir die länge

    for(int i= 1; i < size ; ++i){ 
      unsigned P = differences.at(i); //umrechnung Knotennummer P in V:L P = y*N+x
      /*Da shortestpaths den kurzeste weg berchne un das wird in differences gespeichert ist das hier
      unserem matrix */

      x = P % (Datei1.size()+1); //Umrechnung Horizontale Koordinate
      y = P / (Datei1.size()+1); //Umrechnung vertikale koordinate


    if(x > prevx && y > prevy){
      //kein unterschied, Diagonale
      prevx++;
      prevy++;
    }else if(x > prevx ){
     if(prevx < datei1.size()){

      std:: cout<<" Zeile "<< prevx + 1 <<" aus "<< Datei1 <<"  löschen."<< datei1[prevx]<<std::endl;
     }
     
     prevx++;
    }else if(y > prevy){
      if(prevy < datei2.size()){
        std::cout <<" Zeile "<< prevy + 1 <<" aus "<< Datei2 
      << " in Zeile " << prevx + 1 <<" in "<< Datei1 << " einfügen"<< datei1[prevx] << std::endl;
      }
      
      prevy++;
    }
     
      //x1 = x;
      //y1 = y;
    }
   }
   
