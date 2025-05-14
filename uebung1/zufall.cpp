#include <iostream>
#include <random>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

//Methode die ein Zufällige Zahl zwischen minimum und max gibt
int zufallwerte(int min, int max) {

  int resultat =min + (rand() % (max - min + 1));

return resultat; 
}

//Funktion zur Zählung der Bits in V

int countBits(int zahl) {
  if(zahl == 0) return 1;
  int zaehler = 1; //start bei 1 für das Vorzeichen-Bit
  //absolut Wert für negativ zahlen
  if(zahl < 0) zahl = -zahl;

  while (zahl > 0) {
    zahl >>=1; //rechts Verschiebung der Bit
    ++zaehler; //Bits zähler     
  }
  return zaehler;
}

int main(){

  int L =zufallwerte(500, 10000);
  int M = zufallwerte(10, 1000);

  std::cout << "Zufallszahl von L: "<< L << std::endl;
  std::cout <<"Zufallszahl von M: "<< M << std::endl;

  //erzeugen einen Vector v der Länge L
  std::vector<int>V(L);

  //gebe des Werts des Laege des Vectors aus
  std::cout <<"laenge des Vectors: " << V.size() << std::endl;

  //Füllen des Vectors mit Zufälligen Wert zwischen -M und M
   
  for(int i = 0; i < L ; ++i) {

    V[i] =zufallwerte(-10, 1000);
//hier auf der Console ausgeben nur zu prüfen aber es ist noch nicht in binärform
    std::cout << "vector Werte: " << V[i] << std::endl;
  }

  //Berechnung benötigen anzahl der bits in Wertebereich -M und M für codierung einzelne zahl
  int anzahlBits = countBits(M);
  std::cout << "Anzahlbits: " << anzahlBits << std::endl;
//Berechnung der gesamte anzahl von bits, die wir in V speichern können
  int gesamtBits = L * anzahlBits;
  std::cout << "GesamtBits: " << gesamtBits << std::endl;

  // Danach in Byte konvertieren da wir keine einzelne bits schreiben.Zuvor in Byte Array schreiben aber bevor rechnen wir wie viel Bytes wir brauchen.
  int gesamtByte = (gesamtBits + 7) / 8; // +7 für aufrunde der nächste volle Byte-anzahl
  std::cout << "GesamtByte: " << gesamtByte << std::endl;
  std::vector<char>byteArray(gesamtByte);
 int bitPos = 0;
 for(int zahl: V){
   for(int i = anzahlBits - 1; i >= 0; --i) {
     bool bit = (zahl >> i) & 1;
     //wenn bit true ist wird 1 in array geschrieben
     if(bit) {
       byteArray[bitPos / 8] |= (1 << (7 - (bitPos % 8)));
     } else {
       //wenn bit 0(false) wird 0 im Array . ~:(Bitweise NOT) ist für invertiert
       byteArray[bitPos / 8] &= ~(1 << (7 - (bitPos % 8)));
     }
     ++bitPos;
   }
   std::cout.write(byteArray.data(), byteArray.size());
   std::cout << std::endl;
/*
   string name("output.bin");
   fstream file(name, fstream::out | fstream::binary);
    if(file) {
     file.write(byteArray.data(), byteArray.size());
    } else {
      std::cout <<"kann nicht geöffnet werden" << std::endl;
    }*/
      
     }


return 0;


}
