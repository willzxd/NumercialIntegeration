//
//  Expr.cpp
//  numeric
//
//  Created by ZhuXiaodan on 1/5/16.
//  Copyright © 2016 ZhuXiaodan. All rights reserved.
//

#include "Expr.h"
#include "Functions.h"

NumExpression::NumExpression(double _num){
  num = _num;
  var = "";
  f = NULL;
}

NumExpression::NumExpression(std::string _var, Functions * _f) {
  num = 0.0;
  var = _var;
  f = _f;
}

std::string NumExpression::toString() const {
  if (var == "") {
    std::ostringstream oss;
    oss << num;
    std::string res = oss.str();
    return res;
  }
  else {
    return var;
  }
}

double NumExpression::evaluate(std::string superName) {
  if (var != "") {
    bool change = false;
    std::vector<std::pair<std::string, double> > & paras = f->getParaList(superName);
    for (std::size_t i = 0; i < paras.size(); i++) {
      if (paras[i].first == var) {
        num = paras[i].second;
        change = true;
        break;
      }
    }
    if (change == false) {
      std::cerr << "Parameter " << var << " value is lost!" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  return num;
}


PlusExpression::PlusExpression(Expression * _lhs, Expression * _rhs) {
  lhs = _lhs;
  rhs = _rhs;
}

std::string PlusExpression::toString() const {
  std::ostringstream oss;
  oss << "(" << lhs->toString() << " + " << rhs->toString() << ")";
  std::string res = oss.str();
  return res;
}

double PlusExpression::evaluate(std::string superName) {
  return lhs->evaluate(superName) + rhs->evaluate(superName);
}

PlusExpression::~PlusExpression() {
  delete lhs;
  delete rhs;
}


MinusExpression::MinusExpression(Expression * _lhs, Expression * _rhs) {
  lhs = _lhs;
  rhs = _rhs;
}
std::string MinusExpression::toString() const {
  std::ostringstream oss;
  oss << "(" << lhs->toString() << " - " << rhs->toString() << ")";
  std::string res = oss.str();
  return res;
}

double MinusExpression::evaluate(std::string superName) {
  return lhs->evaluate(superName) - rhs->evaluate(superName);
}

MinusExpression::~MinusExpression() {
  delete lhs;
  delete rhs;
}


TimesExpression::TimesExpression(Expression * _lhs, Expression * _rhs) {
  lhs = _lhs;
  rhs = _rhs;
}

std::string TimesExpression::toString() const {
  std::ostringstream oss;
  oss << "(" << lhs->toString() << " * " << rhs->toString() << ")";
  std::string res = oss.str();
  return res;
}

double TimesExpression::evaluate(std::string superName) {
  return (lhs->evaluate(superName) * rhs->evaluate(superName));
}

TimesExpression::~TimesExpression() {
  delete lhs;
  delete rhs;
}


DivExpression::DivExpression(Expression * _lhs, Expression * _rhs) {
  lhs = _lhs;
  rhs = _rhs;
}

std::string DivExpression::toString() const {
  std::ostringstream oss;
  oss << "(" << lhs->toString() << " / " << rhs->toString() << ")";
  std::string res = oss.str();
  return res;
}

double DivExpression::evaluate(std::string superName) {
  return lhs->evaluate(superName) / rhs->evaluate(superName);
}


DivExpression::~DivExpression() {
  delete lhs;
  delete rhs;
}

PowExpression::PowExpression(Expression * _lhs, Expression * _rhs) {
  lhs = _lhs;
  rhs = _rhs;
}

std::string PowExpression::toString() const {
  std::string res;
  res = "(pow (" + lhs->toString() + ", " + rhs->toString() + "))";
  return res;
}

double PowExpression::evaluate(std::string superName) {
  return pow(lhs->evaluate(superName), rhs->evaluate(superName));
}

PowExpression::~PowExpression(){
  delete lhs;
  delete rhs;
}


SqrtExpression::SqrtExpression(Expression * _e) {
  e = _e;
}

std::string SqrtExpression::toString() const {
  std::string res;
  res = "(sqrt(" + e->toString() + "))";
  return res;
}
double SqrtExpression::evaluate(std::string superName) {
  assert(e->evaluate(superName) >= 0);
  return sqrt(e->evaluate(superName));
}

SqrtExpression::~SqrtExpression(){
  delete e;
}

StringExpression::StringExpression(std::string _str, Functions * _f) {
  str = _str;
  f = _f;
}
  
std::string StringExpression::toString() const {
  return str;
}
  
double StringExpression::evaluate(std::string superName) {
  //find function name
  std::size_t pos = str.find("(");
  std::string name;
  std::vector<std::pair<std::string, double> > & paras = f->getParaList(superName);
  //std::vector<std::pair<std::string, double> > tmpParas;
  //tmpParas = paras;
  if (pos != std::string::npos) {
    skipSpace(str, pos);
    pos++;
    name = parseFunctionName(str, pos);
    //std::cout << "function name is: " << name << std::endl;
  }
  else {
    std::cerr << "Format of defination is wrong" << std::endl;
    exit(EXIT_FAILURE);
  }
  //std::cout << "Find a StringExpresion name: " << name << std::endl;
  std::vector<std::pair<std::string, double> > & subparas = f->getParaList(name);
  //update new paraList for the function
  //if a para is another stringexpr or expr, parse it, then evaluate
  //use the function expr to evaluate
  for (std::vector<std::pair<std::string, double> >::iterator it = subparas.begin();
       it != subparas.end(); it++) {
    std::string variable;
    double value = 0.0;
    skipSpace(str, pos);
    //parse (f a b c) as the function name and parameters
    //maybe (f a b c) (f 3 3 4) (f 3 a b) (f (* a 3) b c) (f (* a b) c 3) (f a b (g c))
    if (str[pos] != '(' && str[pos] != ')') {
      // a number or an id
      while (str[pos] != ' ' && str[pos] != ')') {
        variable += str[pos];
        pos++;
      }
      if (isValidId(variable)) {
        //an identifier
        for (std::vector<std::pair<std::string, double> >::iterator i = paras.begin();
             i != paras.end(); i++) {
          if (i->first == variable) {
            it->second = i->second;
            //std::cout << "update the parameter: " << it->first << " value: " << it->second <<std::endl;
            break;
          }
        }
      }
      else {
        //a number
        it->second = strtonum<double>(variable);
        //std::cout << "update the parameter: " << it->first << " value: " << it->second <<std::endl;
      }
    }
    else if (str[pos] == '(') {
      //a regular expression or another function
      std::string substr = "(";
      pos++;
      int count = 1;
      while (count != 0 && str[pos] != '\0') {
        substr += str[pos];
        if (str[pos] == '(') {
          count++;
        }
        if (str[pos] == ')') {
          count--;
        }
        pos++;
        if (str[pos] == '\0' && count != 0) {
          std::cerr << "Parse StringExpression failed: Expected )" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      //(f a b (* 3 c)) substr = "(* 3 c))"
      std::size_t subpos = 0;
      Expression * subExpr;
      subExpr = parseExpression(substr, subpos, superName, f);
      value = subExpr->evaluate(superName);
      delete subExpr;
      it->second = value;
    }
    else if (str[pos] == ')') {
      //check
      //compute
    }
  }
  double res = f->getExpression(name)->evaluate(name);
  return res;
}

StringExpression::~StringExpression(){}


