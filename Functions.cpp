//
//  Functions.cpp
//  numeric
//
//  Created by ZhuXiaodan on 1/5/16.
//  Copyright © 2016 ZhuXiaodan. All rights reserved.
//

#include "Functions.h"

Functions::Functions() {}
  
void Functions::defineFunction(std::string functionName, Expression * expr) {
  std::pair<std::string, Expression *> tmp;
  tmp.first = functionName;
  tmp.second = expr;
  formula.insert(tmp);
}

void Functions::definePara(std::string functionName,
                           std::vector<std::pair<std::string, double> > parameters) {
  std::pair<std::string, std::vector<std::pair<std::string, double> > > tmp;
  tmp.first = functionName;
  tmp.second = parameters;
  funcMap.insert(tmp);
}

std::vector<std::pair<std::string, double> > & Functions::getParaList(std::string name) {
  if (funcMap.find(name) != funcMap.end()) {
    return funcMap.find(name)->second;
  }
  else {
    std::cerr << "Do not find the function: " << name <<std::endl;
    exit(EXIT_FAILURE);
  }
}

void Functions::addName(std::string name) {
  nameList.push_back(name);
}

std::vector<std::string> & Functions::getFunctionList() {
  return nameList;
}

Expression * Functions::getExpression(std::string name) {
  return formula.find(name)->second;
}

void Functions::updateParaList(std::string &line, std::size_t &pos, std::string name) {
  std::vector<std::pair<std::string, double> > tempParas;
  std::vector<std::pair<std::string, double> > & paras = getParaList(name);
  tempParas = paras;
  for (std::vector<std::pair<std::string, double> >::iterator it = tempParas.begin();
       it != tempParas.end(); it++) {
    //e.g (g (f 2 3) 4), (g (f (g 2 3) 4) 5)
    skipSpace(line, pos);
    if (line[pos] == '(') {
      //may be (f 2 3)
      //evaluate first
      std::string subline = "(";
      int count = 1;
      pos++;
      while (count != 0 && line[pos] != '\0') {
        subline += line[pos];
        if (line[pos] == '(') {
          count++;
        }
        if (line[pos] == ')') {
          count--;
        }
        if (line[pos] == '\0' && count != 0) {
          std::cerr << "Evaluate expression has error. Expected )" << std::endl;
        }
        pos++;
      }
      std::size_t tmp_pos = 1;
      std::string subName = parseFunctionName(subline, tmp_pos);
      updateParaList(subline, tmp_pos, subName);
      it->second = getExpression(subName)->evaluate(subName);
      skipSpace(line, pos);
    }
    else if (line[pos] != ')' && line[pos] != '\0') {
      std::string strNum;
      while (line[pos] != ' ' && line[pos] != ')' && line[pos] != '\0') {
        strNum += line[pos];
        pos++;
      }
      //a number
      it->second = strtonum<double>(strNum);
      skipSpace(line, pos);
    }
  }
  for (std::size_t i = 0; i < tempParas.size(); i++) {
    paras[i] = tempParas[i];
  }
}

double Functions::evaluateExpression(std::string line, std::size_t &pos, std::string name) {
  updateParaList(line, pos, name);
  //printAll();
  return getExpression(name)->evaluate(name);
}
void Functions::printAll() {
  std::cout << "-----Function and paras-----" << std::endl;
  for (std::map<std::string, std::vector<std::pair<std::string, double> > >::iterator it = funcMap.begin(); it != funcMap.end(); it++) {
    std::cout << "Function name: " << it->first << std::endl;
    for (std::vector<std::pair<std::string, double> >::iterator ite = it->second.begin(); ite != it->second.end(); ite++) {
      std::cout << "para: " << ite->first << std::endl;
      std::cout << "value: " << ite->second << std::endl;
    }
  }
  std::cout << "-----End-----" << std::endl;
}

Functions::~Functions() {
  for (std::map<std::string, Expression *>::iterator it = formula.begin(); it != formula.end(); it++) {
    delete it->second;
  }
}

