
#include <iostream>
#include "../include/ISCAS.h"
#include "../include/ROBDD.h"
#include "../include/FuncOpt.h"
#include <map>
#include <stdexcept>
#include <algorithm>   
#include <random>       
#include<chrono>
#include <array>
#include <climits>
using namespace std;

/*Für robdd-opt: Aufgabe3*/

     
void FuncOpt::StrukturOpt(const ISCAS::Assignments& assignments, ROBDD& robdd){
      for (const ISCAS::Assignment& assignment : assignments) {
        operationenFuncOpt(robdd, std::get<0>(assignment),(ISCAS::Kind)std::get<1>(assignment),std::get<2>(assignment)); 
      }
    }

    //Inputs var in map einfügen
    void FuncOpt::inputsVariablesOpt(ROBDD& robdd, std::vector<std::string> Inputs){
      unsigned i=0;
      for(const std::string& str : Inputs){  
        if(m_Inputs->find(str)==m_Inputs->end()){
          m_Variablen->insert(pair<unsigned,std::string>(i,str));
          m_Inputs->insert(pair<std::string,const ROBDD::Func&>(str,robdd.genVar(i++)));
        }
      }
    }
    
  //ite Funktionen 
    const ROBDD::Func& FuncOpt::andFunc(ROBDD& robdd,const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, crArg2, robdd.genFalse());
    }

    const ROBDD::Func& FuncOpt::notFunc(ROBDD& robdd, const ROBDD::Func& crArg) {
      return robdd.ite(crArg, robdd.genFalse(), robdd.genTrue());
    }

    const ROBDD::Func& FuncOpt::nandFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, robdd.ite(crArg2, robdd.genFalse(), robdd.genTrue()), robdd.genTrue());
    }

    const ROBDD::Func& FuncOpt::orFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, robdd.genTrue(), crArg2);
    }

    const ROBDD::Func& FuncOpt::xorFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, robdd.ite(crArg2, robdd.genFalse(), robdd.genTrue()), crArg2);
    }

    const ROBDD::Func& FuncOpt::norFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, robdd.genFalse(), robdd.ite(crArg2, robdd.genFalse(), robdd.genTrue()));
    }

    const ROBDD::Func& FuncOpt::impFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, crArg2, robdd.genTrue());
    }

    const ROBDD::Func& FuncOpt::eqFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
      return robdd.ite(crArg1, crArg2, robdd.ite(crArg2, robdd.genFalse(), robdd.genTrue()));
    }

    //das ist die Funktionen zuwweisung
    void FuncOpt::operationenFuncOpt(ROBDD& robdd, const std::string& name, const ISCAS::Kind& kind, const ISCAS::StrVec& Vec) {
       
      if (kind == 0 && Vec.size() > 1) {
        int S = Vec.size();
        std::map<int, const ROBDD::Func&> Func;
        const ROBDD::Func& And = andFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]));
        Func.insert(pair<int, const ROBDD::Func&>(0, And));
        int P = 0;
        int q = 0;
        for (int i = 3; i <= S; ++i) {
            P = i - 2;
            q = i - 3;
            Func.insert(pair<int, const ROBDD::Func&>(P, andFunc(robdd, Func.at(q), m_Inputs->at(Vec[i - 1]))));
        }
        int L = S - 2;
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, Func.at(L)));
      }
      else if (kind == 1 && Vec.size() > 1) {
        int S = Vec.size();
        std::map<int, const ROBDD::Func&> Func;
        const ROBDD::Func& Nand = nandFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]));
        Func.insert(pair<int, const ROBDD::Func&>(0, Nand));
        int P = 0;
        int q = 0;
        for (int i = 3; i <= S; ++i) {
            P = i - 2;
            q = i - 3;
            Func.insert(pair<int, const ROBDD::Func&>(P, nandFunc(robdd, Func.at(q), m_Inputs->at(Vec[i - 1]))));
        } 
        int L = S - 2;
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, Func.at(L)));
      }
      else if (kind == 2 && Vec.size() > 1) {
        int S = Vec.size();
        std::map<int, const ROBDD::Func&> Func;
        const ROBDD::Func& OR = orFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]));
        Func.insert(pair<int, const ROBDD::Func&>(0, OR));
        int P = 0;
        int q = 0;
        for (int i = 3; i <= S; ++i) {
            P = i - 2;
            q = i - 3;
            Func.insert(pair<int, const ROBDD::Func&>(P, orFunc(robdd, Func.at(q), m_Inputs->at(Vec[i - 1]))));
        }
        int L = S - 2;
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, Func.at(L)));
      }
      else if (kind == 3 && Vec.size() > 1) {
        int S = Vec.size();
        std::map<int, const ROBDD::Func&> Func;
        const ROBDD::Func& Xor = xorFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]));
        Func.insert(pair<int, const ROBDD::Func&>(0, Xor));
        int P = 0;
        int q = 0;
        for (int i = 3; i <= S; ++i) {
            P = i - 2;
            q = i - 3;
            Func.insert(pair<int, const ROBDD::Func&>(P, xorFunc(robdd, Func.at(q), m_Inputs->at(Vec[i - 1]))));
        }
        int L = S - 2;
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, Func.at(L)));
      }
      else if (kind == 4 && Vec.size() > 1) {
        int S = Vec.size();
        std::map<int, const ROBDD::Func&> Func;
        const ROBDD::Func& Nor = norFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]));
        Func.insert(pair<int, const ROBDD::Func&>(0, Nor));
        int P = 0;
        int q = 0;
        for (int i = 3; i <= S; ++i) {
            P = i - 2;
            q = i - 3;
            Func.insert(pair<int, const ROBDD::Func&>(P, norFunc(robdd, Func.at(q), m_Inputs->at(Vec[i - 1]))));
        }
        int L = S - 2;
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, Func.at(L)));
      }
      else if (kind == 5 && Vec.size() == 1) {
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, notFunc(robdd, m_Inputs->at(Vec[0]))));
      }
      else if (kind == 6 && Vec.size() == 2) {
        if(m_Inputs->find(name)==m_Inputs->end())
          m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, impFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]))));
      }
      else if (kind == 7 && Vec.size() == 2) {
       if(m_Inputs->find(name)==m_Inputs->end())
        m_Inputs->insert(pair<std::string, const ROBDD::Func&>(name, eqFunc(robdd, m_Inputs->at(Vec[0]), m_Inputs->at(Vec[1]))));
      } 
      else {
        std::cerr << "außer Bereich" << std::endl;
      }
    }

  


//unsere Graph auf std::cout

void FuncOpt::drawGraphDotOpt(ROBDD& robdd, const ROBDD::Func& crArg1, std::map<const ROBDD::Func*, int>& Funktionen, int& i, int& node_counter, std::ostream& os) {
  //Else verzweigung
  if (!robdd.isConstant(robdd.getElse(crArg1))) {
    if (Funktionen.count(&robdd.getElse(crArg1)) < 1 ) {
      Funktionen.insert({&robdd.getElse(crArg1), i++});
      node_counter++;
      os << Funktionen.at(&robdd.getElse(crArg1)) << " [label=" << m_Variablen->at(robdd.getVar(robdd.getElse(crArg1))) << "];" << std::endl;
      drawGraphDotOpt(robdd, robdd.getElse(crArg1), Funktionen, i, node_counter, os);
    }
    os << Funktionen.at(&crArg1) << "->" << Funktionen.at(&robdd.getElse(crArg1)) << "[label=0, color=red];" << std::endl;
  } else {
    if (robdd.isFalse(robdd.getElse(crArg1))) {
      os << Funktionen.at(&crArg1) << "->null[label=0, color=red];" << std::endl;
    } else if (robdd.isTrue(robdd.getElse(crArg1))) {
      os << Funktionen.at(&crArg1) << "->ein[label=0, color=red];" << std::endl;
    }
  }

  //Then verzweigung
  
  if (!robdd.isConstant(robdd.getThen(crArg1))) {
    if (Funktionen.count(&robdd.getThen(crArg1)) < 1) {
      Funktionen.insert({&robdd.getThen(crArg1), i++});
      node_counter++;
      os << Funktionen.at(&robdd.getThen(crArg1)) << " [label=\"" << m_Variablen->at(robdd.getVar(robdd.getThen(crArg1))) << "\"];" << std::endl;
      drawGraphDotOpt(robdd, robdd.getThen(crArg1), Funktionen, i, node_counter, os);
    }
    os << Funktionen.at(&crArg1) << "->" << Funktionen.at(&robdd.getThen(crArg1)) << "[label=1];" << std::endl;
  } else {
    if (robdd.isFalse(robdd.getThen(crArg1))) {
      os << Funktionen.at(&crArg1) << "->null[label=1];" << std::endl;
    } else if (robdd.isTrue(robdd.getThen(crArg1))) {
      os << Funktionen.at(&crArg1) << "->ein[label=1];" << std::endl;
    }
  }
}


void FuncOpt::printDotOpt(ROBDD& robdd, ISCAS::StrVec str, std::ostream& os) {
  int node_counter = 0;

  os << "digraph ROBDD{" << std::endl;
  std::map<const ROBDD::Func*, int> Funktionen;
  os << "subgraph cluster_0{" << std::endl;
  os << "style=invis" << std::endl;
  os << "}" << std::endl;
    
  os << "null [shape=box, label=0];" << std::endl;
  os << "ein [shape=box, label=1];" << std::endl;

  int i = 0;

  for (std::string& Output : str) {
    os << Output << "[shape=plaintext];" << std::endl;
    if (Funktionen.find(&m_Inputs->at(Output)) == Funktionen.end()) {
      Funktionen.insert({&m_Inputs->at(Output), i});
      node_counter++;
      ++i;
      
      if (!robdd.isConstant(m_Inputs->at(Output))) {
        drawGraphDotOpt(robdd, m_Inputs->at(Output), Funktionen, i, node_counter, os);
      }
    }
    
    if (!robdd.isConstant(m_Inputs->at(Output))) {
      os << Funktionen.at(&m_Inputs->at(Output)) << " [label=" << m_Variablen->at(robdd.getVar(m_Inputs->at(Output))) << "];";
      os << Output << "->" << Funktionen.at(&m_Inputs->at(Output)) << ";" << std::endl;
    } else {
      if (robdd.isFalse(m_Inputs->at(Output))) {
        os << Output << "->null;" << std::endl;
      } else if (robdd.isTrue(m_Inputs->at(Output))) {
        os << Output << "->ein;" << std::endl;
      }
    }
  }
  os << "}" << std::endl;
   
} 

//size Berechnen mit counter um den knoten zu zählen

void FuncOpt::sizeBerechnen(ROBDD& robdd, const ROBDD::Func& crArg1, std::vector<const ROBDD::Func* >& Funktionen, int& node_counter) {

   if (std::find_if(Funktionen.begin(), Funktionen.end(), [&crArg1](const ROBDD::Func *ptr)
                     { return ptr == &crArg1; }) != Funktionen.end()) {
                      return;
    }
        

        Funktionen.push_back(&crArg1);
        node_counter++;
        
   if(robdd.isConstant(crArg1)) {
    return;
   }


   const ROBDD::Func& then = robdd.getThen(crArg1);
   const ROBDD::Func& elseB = robdd.getElse(crArg1);

   sizeBerechnen(robdd, then, Funktionen, node_counter);
   sizeBerechnen(robdd, elseB, Funktionen, node_counter);
   
   
   //std::cout << "size ist : " << node_counter <<std::endl;
}


//size als int zurückgeben 

int FuncOpt::size(ROBDD& robdd, ISCAS::StrVec inputs, std::map<std::string, const ROBDD::Func&>* FOutputs){
  int counter = 0;
  std::vector<const ROBDD::Func*> speich;
  std::map<string,int>sizeMap;

  for(const auto &out : *FOutputs) {
    sizeBerechnen(robdd, out.second, speich, counter);
    //sizeMap[out.first] = counter;
  }

  //std::cout << "VariablenOrdnung der Permutation ist ";
  for (const auto &in : inputs) {
  // std::cout << in << "";
  }
 // std::cout << " und Größe des ROBDD ist : " << counter / 2 << std::endl;
  
  return counter;
}

//Um permutation zu berechnen von bibliothek algorithms

void FuncOpt::switchup(ISCAS::StrVec &inputs) {
  
next_permutation(inputs.begin(), inputs.end());



}

//logic
void FuncOpt::finalGr(ROBDD &robdd, ISCAS::StrVec &inputs, ISCAS::StrVec &outputs, ISCAS::Assignments &assignments, std::ostream& os) {

  int counter = 0;
  long double faktorial = 1;
  int max = INT_MAX;   //Max werte
 // int min = size(robdd, inputs, m_Inputs); // aktuelle größe 
  ISCAS::StrVec var; //Aktuelle variable

  for(unsigned i = 1; i <= inputs.size(); i++) faktorial *= i; 
  
  

  while(counter < faktorial) {
    //m_Inputs->clear();
    if(counter != 0) switchup(inputs);
     m_Inputs->clear();
     m_Variablen->clear();
    inputsVariablesOpt(robdd, inputs);
    
    StrukturOpt(assignments, robdd);
       
      
    size(robdd, inputs, m_Inputs);
    counter++;

     int min = size(robdd, inputs, m_Inputs); // aktuelle größe

    if(min <  max) {
      max = min;
      var = inputs;
      //Wenn ich hier die funktion prinDot aufrufe wird das für jedes mi gezeichnet
    }
  } 

  if(!var.empty()) {
    m_Inputs->clear();
    m_Variablen->clear();
    inputsVariablesOpt(robdd, var);
    StrukturOpt(assignments, robdd);
    printDotOpt(robdd, outputs, os);
  }
} 
