#pragma once
#ifndef _FUNCOPT_H
#define _FUNCOPT_H

#include "ROBDD.h"
#include "ISCAS.h"
#include <set>
#include <unordered_map>
#include <iostream>
#include <map>
#include <fstream>

class FuncOpt {
  
    public:
    //Konstruktor
   FuncOpt() : m_Inputs(new std::map<std::string, const ROBDD::Func&>),
            m_Variablen(new std::map<unsigned, std::string>){}
     
     //destruktor
     ~FuncOpt() {
      delete m_Inputs;
      delete m_Variablen;
    }

       //ite Funktion
       const ROBDD::Func& andFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& nandFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& notFunc(ROBDD& robdd, const ROBDD::Func& crArg);
       const ROBDD::Func& orFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& xorFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& norFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& impFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       const ROBDD::Func& eqFunc(ROBDD& robdd, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2);
       
       void StrukturOpt(const ISCAS::Assignments& assignments, ROBDD& robdd);
       void inputsVariablesOpt(ROBDD& robdd, std::vector<std::string> Inputs);
       void operationenFuncOpt(ROBDD& robdd, const std::string& str, const ISCAS::Kind& art, const ISCAS::StrVec& Vec);
       
    void drawGraphDotOpt(ROBDD& robdd, const ROBDD::Func& crArg1, std::map<const ROBDD::Func*, int>& Funktionen, int& i, int& node_counter, std::ostream& os);
    void printDotOpt(ROBDD& robdd, ISCAS::StrVec str, std::ostream& os);
   
    void sizeBerechnen(ROBDD& robdd, const ROBDD::Func& crArg1, std::vector<const ROBDD::Func*>& Funktionen, int& node_counter);
    void switchup(ISCAS::StrVec &inputs);
    void finalGr(ROBDD &robdd, ISCAS::StrVec &inputs, ISCAS::StrVec &outputs, ISCAS::Assignments &assignments, std::ostream& os);
    int size(ROBDD& robdd, ISCAS::StrVec inputs, std::map<std::string, const ROBDD::Func&>* FOutputs);
       
       

 private: 
    std::map<std::string,const ROBDD::Func&>* m_Inputs;
    std::map<unsigned, std::string>* m_Variablen;

};

#endif 
