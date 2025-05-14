#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
 
//string name("output.bin");
int L = 500 + std::rand() % (100002 - 500 + 1) ;

std::vector<char>array(L);
std::cin >> array;
/*fstream file(name, fstream::in | fstream::binary);
if(!file) {
  std::cout <<"Datei konnte nicht zum lesen geöffnet oder existiert nicht" << std::endl;
}
if(file) {
  file.seekg(0);
file.read(array.data(), array.size());
std::cout << "wurde gelesen" << std::endl;

int bitpos = 0;
for(int i = 0; i <= L; ++i) {
int value = 0;

}
}*/


return 0;
}
