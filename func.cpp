#include <iostream>
#include "../include/Func.h"
#include "../include/ISCAS.h"
#include "../include/ROBDD.h"
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <map>

using namespace std;
//using LabelMap = std::unordered_map<unsigned, std::string>;

//and Function
const ROBDD::Func& andFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
  return rROBDD.ite(crArg1,crArg2,rROBDD.genFalse());
}

//not Func
const ROBDD::Func& notFunc(ROBDD& rROBDD, const ROBDD::Func& input){
  return rROBDD.ite(rROBDD.genFalse(), rROBDD.genTrue(), input);

}

//or Func
const ROBDD::Func& orFunc(ROBDD& rROBDD,const ROBDD::Func& crArg1, const ROBDD::Func& crArg2){
  return rROBDD.ite(crArg1, rROBDD.genTrue(), crArg2);
}
//nandFunc= not func + and func 
const ROBDD::Func& nandFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
  const ROBDD::Func& andResult = andFunc(rROBDD, crArg1, crArg2);
  const ROBDD::Func& nandResult = notFunc(rROBDD, andResult);
  return nandResult;
}

const ROBDD::Func& xorFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
  const ROBDD::Func& andResult = andFunc(rROBDD,crArg1,crArg2);
  const ROBDD::Func& notAndResult = notFunc(rROBDD, andResult);
  const ROBDD::Func& notArg1 = notFunc(rROBDD,crArg1);
  const ROBDD::Func& notArg2 = notFunc(rROBDD,crArg2);
  const ROBDD::Func& orResult = orFunc(rROBDD,notArg1,notArg2);
  const ROBDD::Func& xorResult = andFunc(rROBDD,notAndResult,orResult);
  return xorResult;

}

const ROBDD::Func& impliesFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2) {
  return rROBDD.ite(crArg1,crArg2,rROBDD.genFalse());
}

//nor Func = not and OR
const ROBDD::Func& norFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2){
  const ROBDD::Func& orResult = orFunc(rROBDD, crArg1, crArg2);
  return notFunc(rROBDD, orResult);
}

const ROBDD::Func& equivalentFunc(ROBDD& rROBDD, const ROBDD::Func& crArg1, const ROBDD::Func& crArg2){
  const ROBDD::Func& xorResult = xorFunc(rROBDD, crArg1, crArg2);
  return notFunc(rROBDD, xorResult);
}

void readFunc(ROBDD& robdd, ISCAS& iscas, std::unordered_map<std::string, const ROBDD::Func*>& func_map, 
    std::unordered_map<const ROBDD::Func*, std::string> func_names){ 
    
  for(auto &input : iscas.inputs()) {
    const auto &func = robdd.genVar(func_map.size() + 1);
    func_map[input] = &func;
    func_names[&func] = input; //speicher Namen
    std::cout << func_names[&func] << std::endl; 
   // LabelMap[func.getVar()] = input;
  //  labelMap.insert({robdd.getVar(func), input});
    
  }
  // create ROBDD functions for each
  for (auto &assignment : iscas.assignments()) {
  auto &name = std::get<0>(assignment);
  auto &kind = std::get<1>(assignment);
  auto &variables = std::get<2>(assignment);

  
  //Initialize with the first var in 
  
  const ROBDD::Func* func = func_map[variables[0]];

 // const ROBDD::Func *newFunc = func_map.find(*variables.begin())->second;

  //start at the second function
 // for(auto iterator = variables.begin() + 1; iterator != variables.end(); ++iterator) {
 for(size_t i = 1; i <variables.size(); ++i) {
   const ROBDD::Func* operand = func_map[variables[i]];
   // const ROBDD::Func *result = func_map.find(*iterator)->second;

    //
    if(kind == ISCAS::AND || kind == ISCAS::NAND) {
    func = &andFunc(robdd, *func, *operand);
    }
    if(kind == ISCAS::OR || kind == ISCAS::NOR){
     func = &orFunc(robdd, *func, *operand);
    }
    if(kind == ISCAS::XOR) {
      func = &xorFunc(robdd, *func, *operand);
    }
    //kommt noch mehrere
  }
  if(kind == ISCAS::NAND || kind == ISCAS::NOR || kind == ISCAS::NOT) {
  func = &notFunc(robdd, *func);
  }
  //add result
  func_map[name] = func;
  func_names[func] = name;//speicher name für Ergebnisfunktion
  }
 
}

/*void dot(ISCAS& iscas, const ROBDD::Func& func, ROBDD& robdd, std::set<const ROBDD::Func*>& unique_map) {
  
  //
  //std::unordered_map <std::string, const ROBDD::Func*> map;
  
  if(unique_map.find(&func) != unique_map.end()) {

    return; //Funktion bereits gezeichnet
  }
  unique_map.insert(&func);

  if(robdd.isConstant(func)) {
    std::cout <<&func << "[shape=box, label=\"" << (robdd.isTrue(func) ? "TRUE" : "FALSE") << "\"];" << std::endl;
  }else {
    //std::cout <<&func << "[label=\"G " << robdd.getVar(func) << "\", shape=circle];" << std::endl;
    const ROBDD::Func& thenFunc = robdd.getThen(func);
    const ROBDD::Func& elseFunc = robdd.getElse(func);

    //std::cout<<robdd.getVar(func) <<"[label=\" " <<robdd.getVar(func) 
      //<<"\"];" 
      //<< robdd.getVar(robdd.getElse(func)) <<" -> "<< "\"" << &thenFunc << "[label=\"1\"];" <<std::endl;

   // std::cout << &func << "->" <<&elseFunc << "[label=\"0\", color=red];" <<std::endl;
    dot(thenFunc, robdd, unique_map);
    dot(elseFunc, robdd, unique_map);
  //std::unordered_map<std::string, const ROBDD::Func*> func_map;
 //for( const auto& aout : func_map){
  //const std::string& name = out.first;
  //std::cout << name << std::endl;
  //if(!robdd.isConstant(func)) {

      //
      std::cout << robdd.getVar(func) 
        <<" " 
        << robdd.getVar(robdd.getElse(func)) << " "  << "->"
        << "\"" << robdd.getVar(robdd.getThen(func)) << "\""
        << " [label =\"1\"];" << std::endl;

     // const ROBDD::Func& f_then = robdd.getThen(func);

  //    std::cout << robdd.getThen(func)
    //    << " " << "->" 
     //   <<robdd.getVar(robdd.getThen(func)) << " " << "->"
      //  <<"\" " << robdd.getVar(robdd.getThen(func))<< " [label =\"0\", color=red];"  << std::endl;
      
      dot(robdd.getThen(func), robdd, unique_map);
     // dot(robdd.getElse(func), robdd, unique_map);
    //}
    //std::cout << "\t\""<< &robdd << "\"[label=\"" << robdd.getVar() << "\"];" << std::endl;
    //const ROBDD::Func& thenFunc = robdd.getThen(func);
    //const ROBDD::Func& elseFunc = robdd.getElse(func);
    //std::cout << "\t\"" << &func << "\"-> \"Func" << &thenFunc << "\" [label=\"1\"];" << std::endl;
    //std::cout << "\t\"" << &func << "\" -> \"" << &elseFunc <<"\" [label=\"0\"];" <<std::endl;
    //dot(thenFunc,robdd,iscas, unique_map);
    //dot(elseFunc,robdd,iscas, unique_map);
  }/* else 
    if(robdd.isFalse(func)) {
    cout << "\t\t\"" << &robdd << "\" [shape=box, label = 0]" << endl;
  } if(robdd.isTrue(func)) {
      cout << "\t\t\"" << &robdd << "\" [shape=box, label = 1]" <<endl;

  } */
//}
//}
//}
void resolveNode(const ROBDD::Func* currentNode, std::unordered_map<std::string, const ROBDD::Func*>& func_map,
                 std::unordered_map<const ROBDD::Func*, std::string>& func_names, ISCAS& iscas, std::vector<std::string>& resolvedInputs) {
    for (const auto& input : iscas.inputs()) {
        if (func_map[input] == currentNode) {
            resolvedInputs.push_back(input);
            return;
        }
    }

    std::string currentNodeName = func_names[currentNode];
    for (const auto& assignment : iscas.assignments()) {
        if (std::get<0>(assignment) == currentNodeName) {
            auto& variables = std::get<2>(assignment);
            for (const auto& var : variables) {
                const ROBDD::Func* varNode = func_map[var];
                resolveNode(varNode, func_map, func_names, iscas, resolvedInputs);
            }
            break;
        }
    }
}

void dot(ROBDD& robdd, ISCAS& iscas, std::unordered_map<std::string, const ROBDD::Func*>& func_map,
         std::unordered_map<const ROBDD::Func*, std::string>& func_names) {
    
    std::unordered_set<const ROBDD::Func*> visited;
    std::vector<const ROBDD::Func*> stack;

    std::cout << "digraph ROBDD {" << std::endl;
    std::cout << "subgraph cluster_0 {" << std::endl;
    std::cout << "style = invis;" << std::endl;

    // Ausgabe der Outputs als Knoten
   /* for (const auto& output : iscas.outputs()) {
        std::cout << output << " [shape=plaintext];" << std::endl;
    }
    std::cout << "}" << std::endl;

    // Verarbeitung der Outputs
    for (const auto& output : iscas.outputs()) {
        const auto it = func_map.find(output);
        if (it == func_map.end()) continue;
        const ROBDD::Func* node = it->second;

        stack.push_back(node);

        while (!stack.empty()) {
            node = stack.back();
            stack.pop_back();

            if (visited.find(node) != visited.end()) {
                continue;
            }
            visited.insert(node);

            if (robdd.isConstant(*node)) {
                // Zeichne konstante Knoten (1 oder 0)
                //std::cout << "\"" << node << "\" [shape=box, label=\"" << (robdd.isTrue(*node) ? "1" : "0") << "\"];" << std::endl;
            } else {
                // Auflösen der Inputs
               // const std::string& nodeName = func_names[node];
                //std::vector<std::string> resolvedInputs;

        //        resolveNode(&robdd.getThen(*node), func_map, func_names, iscas, resolvedInputs);
        /*        for (const auto& inputName : resolvedInputs) {
                    std::cout << "\"" << node << "\" -> \"" << func_map[inputName] << "\" [label=\"1\"];" << std::endl;
                }*/
                //resolvedInputs.clear(); // Leeren der Inputs für die nächste Verwendung

                //resolveNode(&robdd.getElse(*node), func_map, func_names, iscas, resolvedInputs);
    /*            for (const auto& inputName : resolvedInputs) {
                    std::cout << "\"" << node << "\" -> \"" << func_map[inputName] << "\" [label=\"0\", color=red];" << std::endl;
                }*/
              //  resolvedInputs.clear(); // Leeren der Inputs für die nächste Verwendung

               // std::cout << "\"" << node << "\" [label=\"" << nodeName << "\", shape=circle];" << std::endl;

         //       stack.push_back(&robdd.getThen(*node));
           //     stack.push_back(&robdd.getElse(*node));
      //      }
    //    }
        //visited.clear();
    //}
//}

    std::cout << "}" << std::endl;
}
