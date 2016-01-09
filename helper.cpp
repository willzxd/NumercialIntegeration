//
//  helper.cpp
//  numeric
//
//  Created by ZhuXiaodan on 1/6/16.
//  Copyright © 2016 ZhuXiaodan. All rights reserved.
//
#include <string>
#include <cstring>
#include <iostream>
#include <limits>
#include <random>
#include <chrono>
#include "Functions.h"
#include "Expr.h"
#include "strtonum.h"

#include "helper.h"

/*skip space*/
void skipSpace(std::string &line, std::size_t &pos) {
  while (line[pos] == ' ') {
    pos++;
  }
}

/* parse the function name */
std::string parseFunctionName(std::string &line, std::size_t &pos) {
  std::string name;
  while (line[pos] != ' ' && line[pos] != ')') {
    name += line[pos];
    pos++;
  }
  return name;
}

bool isValidOp(std::string op) {
  return strstr("+-*/", op.c_str()) || op == "pow" || op == "sqrt";
}

bool isValidId(std::string line) {
  for (std::size_t i = 0; i < line.length(); i++) {
    if (!(line[i]>='0' && line[i] <= '9') && line[i] != ' ') {
      continue;
    }
    else {
      return false;
    }
  }
  return true;
}

std::vector<std::pair<std::string, double> > parseParameter(std::string &line, std::size_t &pos) {
  
  std::vector<std::pair<std::string, double> > paraList;
  std::pair<std::string, double> paraPair;
  while (line[pos] != ')' && line[pos] != '\0') {
    std::string para;
    while (line[pos] != ' ' && line[pos] != ')') {
      para += line[pos];
      pos++;
    }
    paraPair.first = para;
    //std::cout<< "parameter: " << para << std::endl;
    paraPair.second = 0.0;
    paraList.push_back(paraPair);
    skipSpace(line, pos);
  }
  if (line[pos] == '\0') {
    std::cerr << "Format of defination is wrong" << std::endl;
    exit(EXIT_FAILURE);
  }
  return paraList;
}

bool isValidFunc(std::string op, Functions * funcs) {
  std::vector<std::string> list = funcs->getFunctionList();
  for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); it++) {
    if (*it == op) {
      return true;
    }
  }
  return false;
}

Expression * makeExpr(std::string op, Expression * lhs, Expression * rhs) {
  if (op == "+") {
    return new PlusExpression(lhs, rhs);
  }
  else if (op == "-") {
    return new MinusExpression(lhs, rhs);
  }
  else if (op == "*") {
    return new TimesExpression(lhs, rhs);
  }
  else if (op == "/") {
    return new DivExpression(lhs, rhs);
  }
  else if (op == "sqrt") {
    return new SqrtExpression(lhs);
  }
  else {
    //pow
    return new PowExpression(lhs, rhs);
  }
  return NULL;
}

/* make expression with ordinary operators */
Expression * parseOp(std::string &line, std::size_t &pos, std::string name,
                     Functions *funcs) {
  std::string op;
  while (line[pos] != ' ' && line[pos] != ')') {
    op += line[pos];
    pos++;
  }
  if (!isValidOp(op)) {
    std::cerr << "Find invalid operator." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (op == "sqrt") {
    Expression * lhs = parseExpression(line, pos, name, funcs);
    skipSpace(line, pos);
    if (line[pos] == ')') {
      pos++;
      return makeExpr(op, lhs, NULL);
    }
    else {
      std::cerr << "Expected ) but found " << line[pos] << std::endl;
      delete lhs;
      return NULL;
    }
  }
  else {
    //+ - * / pow
    Expression * lhs = parseExpression(line, pos, name, funcs);
    if (lhs == NULL) {
      return NULL;
    }
    Expression * rhs = parseExpression(line, pos, name, funcs);
    if (rhs == NULL) {
      delete lhs;
      return NULL;
    }
    skipSpace(line, pos);
    if (line[pos] == ')') {
      pos++;
      return makeExpr(op, lhs, rhs);
    }
    else {
      std::cerr << "Expected ) but found " << line[pos] << std::endl;
      delete lhs;
      delete rhs;
      return NULL;
    }
  }
  return NULL;
}

Expression * parseFunc(std::string &line, std::size_t &pos, Functions *funcs) {
  //strip the existing function
  int count = 1;
  std::string func = "(";
  while (count != 0 && line[pos] != '\0') {
    func += line[pos];
    if (line[pos] == '(') {
      count++;
    }
    else if (line[pos] == ')') {
      count--;
    }
    //std::cout << func << std::endl;
    pos++;
  }
  //std::cout << "Construct a new StringExpr: " << func << std::endl;
  return new StringExpression(func, funcs);
}
/*
 * Construct the whole expression
 * line: user input
 * pos: the current position in the line
 * name: the current function name
 * funcs: all functions info
 */
Expression * parseExpression(std::string &line, std::size_t &pos, std::string name,
                             Functions * funcs) {
  skipSpace(line, pos);
  
  if (line[pos] == '\0') {
    std::cerr << "End of line found mid expression" << std::endl;
    exit(EXIT_FAILURE);
  }
  else if (line[pos] == '(') {
    // (op E E) or (identifier p1 p2 ...)
    pos++;
    std::string op;
    std::size_t tmp_pos = pos;
    skipSpace(line, pos);
    while (line[tmp_pos] != ' ' && line[tmp_pos] != ')') {
      op += line[tmp_pos];
      tmp_pos++;
    }
    if (isValidOp(op)) {
      // op can be + ,- ,* ,/ ,pow ,sqrt
      return parseOp(line, pos, name, funcs);
    }
    else if (isValidFunc(op, funcs)) {
      //a function name
      return parseFunc(line, pos, funcs);
    }
    else {
      std::cerr << "Find invalid operator:" << op << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else if ((line[pos]>='0' && line[pos] <= '9') || line[pos] == '.'|| line[pos] == '-') {
    //number
    //convert to double constant value
    double num;
    std::string constant = "";
    while (line[pos] != ' ' && line[pos] != '\0'
           && line[pos] != ')' && line[pos] != '(') {
      constant +=line[pos];
      pos++;
    }
    num = strtonum<double>(constant);
    //std::cout << "Find a constant: " << num << std::endl;
    //return NumExpression
    return new NumExpression(num);
  }
  else {
    //identifier (parameter of the function)
    std::string para;
    while (line[pos] != ' ' && line[pos] != ')' && line[pos] != '(' && !(line[pos] >= '0' && line[pos] <= '9') && line[pos] != '\0') {
      para += line[pos];
      pos++;
    }
    //check the whether it is a parameter
    bool check = false;
    for (std::vector<std::pair<std::string, double> >::iterator it = funcs->getParaList(name).begin();
         it != funcs->getParaList(name).end(); it++) {
      if (para.compare(it->first) == 0) {
        check = true;
        break;
      }
    }
    if (check) {
      return new NumExpression(para, funcs);
    }
    else {
      std::cerr << "Expected Parameters: " << para
      << ", but not found in paraList." << std::endl;
      return NULL;
    }
  }
}

/* parse functions as a function name, map of parameters and an expression */
void parse(std::string &line, Functions * functions){
  std::size_t pos = line.find("(");
  std::string name;
  if (pos != std::string::npos) {
    skipSpace(line, pos);
    pos++;
    name = parseFunctionName(line, pos);
    //std::cout << "function name is: " << name << std::endl;
  }
  else {
    std::cerr << "Format of defination is wrong" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  //parse a list of function parameters
  skipSpace(line, pos);
  std::vector<std::pair<std::string, double> > paras = parseParameter(line, pos);
  functions->definePara(name, paras);
  
  //parse a Expression for this function
  pos = line.find("=");
  Expression * expr;
  if (pos != std::string::npos) {
    pos++;
    expr = parseExpression(line, pos, name, functions);
    if (expr == NULL) {
      std::cerr << "Parse failed!\nInput: " << line << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else {
    std::cerr << "Format of defination is wrong" << std::endl;
    exit(EXIT_FAILURE);
  }
  functions->addName(name);
  //std::cout << expr->toString() << std::endl;
  functions->defineFunction(name, expr);
  printDef(name, paras);
}

void printDef(std::string name, std::vector<std::pair<std::string, double> > & paras) {
  std::string info = "defined (";
  info += name;
  for (std::size_t i = 0; i < paras.size(); i++) {
    info += " ";
    info += paras[i].first;
  }
  info += ")";
  std::cout << info << std::endl;
}

double evaluate(std::string &line, Functions *functions) {
  std::size_t pos = line.find("(");
  std::string name;
  if (pos != std::string::npos) {
    skipSpace(line, pos);
    pos++;
    name = parseFunctionName(line, pos);
    //std::cout << "function name is: " << name << std::endl;
  }
  else {
    std::cerr << "Format of defination is wrong" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  //parse a list of function parameters
  skipSpace(line, pos);
  return functions->evaluateExpression(line, pos, name);
}

void printEval(std::string line, double res) {
  std::string info = "(";
  std::size_t pos = line.find("(");
  pos++;
  int count = 1;
  while (count != 0 && line[pos] != '\0') {
    if (line[pos] != ' ') {
      info += line[pos];
      if (line[pos] == '(') {
        count++;
      }
      if (line[pos] == ')') {
        count--;
      }
      pos++;
    }
    else {
      info += " ";
      skipSpace(line, pos);
    }
  }
  std::cout << info << " = " << res << std::endl;
}

void printInte(std::string line, double res) {
  std::cout << "Integration result is: " << res << std::endl;
}

/* compute integeration */
double integeral(std::string &line, Functions * funcs) {
  //find function name
  std::size_t pos = 0;
  std::string name = "";
  std::string prec = "";
  std::vector<std::pair<double, double> > rangeList;
  double precision = 0.0;
  while (line[pos] != ' ' && !(line[pos] >= '0' && line[pos] <= '9')) {
    name += line[pos];
    pos++;
  }
  skipSpace(line, pos);
  //check if the function exsists.
  if (!isValidFunc(name, funcs)) {
    std::cerr << "Function: " << name << " has not been defined!" << std::endl;
    exit(EXIT_FAILURE);
  }
  //std::cout  << "Integeration function name: " << name << std::endl;
  //find precision
  while (line[pos] != ' ' && ((line[pos] == '.') || (line[pos] >= '0' && line[pos] <= '9'))) {
    prec += line[pos];
    pos++;
  }
  precision = strtonum<double>(prec);
  skipSpace(line, pos);
  
  //put range of parameters to a list
  std::vector<double> tmp;
  while (line[pos] != '\0') {
    std::string tmpstr = "";
    while (line[pos] != ' ' && line[pos] != '\0') {
      tmpstr += line[pos];
      pos++;
    }
    tmp.push_back(strtonum<double>(tmpstr));
    skipSpace(line, pos);
  }
  for (size_t i = 0; i < tmp.size(); i += 2) {
    std::pair<double, double> range;
    range.first = tmp[i];
    range.second = tmp[i + 1];
    rangeList.push_back(range);
  }
  //check size of rangeList is the same as the size of paraList
  if (rangeList.size() != funcs->getParaList(name).size()) {
    std::cerr << "Integeration: number of parameters does not match!" << std::endl;
    exit(EXIT_FAILURE);
  }
  //init paras
  std::vector<std::pair<std::string, double> > & paras = funcs->getParaList(name);
  for (size_t i = 0; i < rangeList.size(); i++) {
    paras[i].second = rangeList[i].first;
  }
  //call compute function
  double res = 0.0;
  computeInteg (0, &res, precision, rangeList, name, funcs);
  return pow(precision, rangeList.size()) * res;
}

void computeInteg (std::size_t start, double *res, double precision,
                     std::vector<std::pair<double, double> >rangeList,
                     std::string name, Functions * funcs) {
  if (start == rangeList.size()) {
    //funcs->printAll();
    double cur = funcs->getExpression(name)->evaluate(name);
    //std::cout << "-----result: " << cur << std::endl;
    *res += cur;
    return;
  }
  
  for (double i = rangeList[start].first + precision / 2; i < rangeList[start].second; i += precision) {
    std::vector<std::pair<std::string, double> > & paras = funcs->getParaList(name);
    paras[start].second = i;
    computeInteg(start + 1, res, precision, rangeList, name, funcs);
    //std::cout << "Current integeration is: " << *res << std::endl;
  }
}

double mcint(std::string &line, Functions * funcs) {
  //find function name
  std::size_t pos = 0;
  std::string name = "";
  std::string prec = "";
  std::vector<std::pair<double, double> > rangeList;
  int numPt = 0;
  while (line[pos] != ' ' && !(line[pos] >= '0' && line[pos] <= '9')) {
    name += line[pos];
    pos++;
  }
  skipSpace(line, pos);
  //check if the function exsists.
  if (!isValidFunc(name, funcs)) {
    std::cerr << "Function: " << name << " has not been defined!" << std::endl;
    exit(EXIT_FAILURE);
  }
  //std::cout  << "Integeration function name: " << name << std::endl;
  //find precision
  while (line[pos] != ' ' && (line[pos] >= '0' && line[pos] <= '9')) {
    prec += line[pos];
    pos++;
  }
  numPt = strtonum<int>(prec);
  skipSpace(line, pos);
  
  //put range of parameters to a list
  std::vector<double> tmp;
  while (line[pos] != '\0') {
    std::string tmpstr = "";
    while (line[pos] != ' ' && line[pos] != '\0') {
      tmpstr += line[pos];
      pos++;
    }
    tmp.push_back(strtonum<double>(tmpstr));
    skipSpace(line, pos);
  }
  double area = 1;
  for (size_t i = 0; i < tmp.size(); i += 2) {
    std::pair<double, double> range;
    range.first = tmp[i];
    range.second = tmp[i + 1];
    area *= (tmp[i+1] - tmp[i]);
    rangeList.push_back(range);
  }
  //check size of rangeList is the same as the size of paraList
  if (rangeList.size() != funcs->getParaList(name).size()) {
    std::cerr << "Integeration: number of parameters does not match!" << std::endl;
    exit(EXIT_FAILURE);
  }
  //init paras
  std::vector<std::pair<std::string, double> > & paras = funcs->getParaList(name);
  for (size_t i = 0; i < rangeList.size(); i++) {
    paras[i].second = rangeList[i].first;
  }
  
  //update max, min and area
  double max = std::numeric_limits<double>::lowest();
  double min = std::numeric_limits<double>::max();
  double positive = 0;
  double negative = 0;
  std::size_t start = 0;
  updateMaxMin(start, &max, &min, rangeList, name, funcs);
  if (max >0 && min > 0) {
    min = 0;
  }
  else if (max < 0 && min < 0) {
    max = 0;
  }
  area *= (max - min);
  
  //generate random points
  unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  for (int i = 0; i < numPt; i++) {
    for (size_t j = 0; j < rangeList.size(); j++) {
      std::uniform_real_distribution<double> distribution (rangeList[j].first, rangeList[j].second);
      double p = distribution(generator);
      paras[j].second = p;
    }
    std::uniform_real_distribution<double> distribution (min, max);
    double val = distribution(generator);
    double rval = funcs->getExpression(name)->evaluate(name);
    if (rval > 0 && val > 0 && rval > val) {
      positive++;
    }
    else if (rval < 0 && val < 0 && val > rval) {
      negative++;
    }
  }
  double res = (positive - negative) / numPt * area;
  return res;
}

void updateMaxMin(std::size_t start, double * max, double * min,
                  std::vector<std::pair<double, double> > rangeList,
                  std::string name, Functions * funcs) {
  if (start == rangeList.size()) {
    //update max min
    double res = funcs->getExpression(name)->evaluate(name);
    if (res > *max) {
      *max = res;
    }
    if (res < *min) {
      *min = res;
    }
    return;
  }
  std::vector<std::pair<std::string, double> > & paras = funcs->getParaList(name);
  for (double i = rangeList[start].first + 0.01 / 2; i < rangeList[start].second; i += 0.01) {
    paras[start].second = i;
    updateMaxMin(start + 1, max, min, rangeList, name, funcs);
  }
}

void printMcint(std::string line, double res) {
  std::cout << "Monte Carlo Integeration result is: " << res << std::endl;
}

double gradAsc(std::string &line, Functions * funcs, int flag) {
  //function name, gamma, convergedDistance, several numbers
  std::size_t pos = 0;
  std::string name = "";
  std::string g = "";
  std::vector<std::pair<double, double> > rangeList;
  double gamma = 0;
  double conDist = 0;
  while (line[pos] != ' ' && !(line[pos] >= '0' && line[pos] <= '9')) {
    name += line[pos];
    pos++;
  }
  skipSpace(line, pos);
  
  //check if the function exsists.
  if (!isValidFunc(name, funcs)) {
    std::cerr << "Function: " << name << " has not been defined!" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout  << "Function name: " << name;
  
  //find gamma
  while (line[pos] != ' ' && ((line[pos] >= '0' && line[pos] <= '9') || line[pos] == '.')) {
    g += line[pos];
    pos++;
  }
  gamma = strtonum<double>(g);
  skipSpace(line, pos);
  std::cout << " Gamma value: " << gamma;
  
  //find convergedDistance
  std::string d = "";
  while (line[pos] != ' ' && ((line[pos] >= '0' && line[pos] <= '9') || line[pos] == '.')) {
    d += line[pos];
    pos++;
  }
  conDist = strtonum<double>(d);
  skipSpace(line, pos);
  std::cout << " ConvergedDistance: " << conDist;
  
  //put start point parameters to a list
  std::vector<double> point;
  std::cout << " Start point: ";
  while (line[pos] != '\0') {
    std::string tmpstr = "";
    while (line[pos] != ' ' && line[pos] != '\0') {
      tmpstr += line[pos];
      pos++;
    }
    double tmp = strtonum<double>(tmpstr);
    point.push_back(tmp);
    std::cout << tmp << " ";
    skipSpace(line, pos);
  }
  std::cout << std::endl;
  
  //check size of point is the same as the size of paraList
  if (point.size() != funcs->getParaList(name).size()) {
    std::cerr << "Integeration: number of parameters does not match!" << std::endl;
    exit(EXIT_FAILURE);
  }
  //init paras
  std::vector<std::pair<std::string, double> > & paras = funcs->getParaList(name);
  for (size_t i = 0; i < point.size(); i++) {
    paras[i].second = point[i];
  }
  
  //begin compute max/min
  double dist = std::numeric_limits<double>::max();
  while (dist > conDist) {
    dist = 0;
    std::vector<double> gradients;
    double precision = 0.1e-9;
    
    //compute gradients
    double origin = funcs->getExpression(name)->evaluate(name);
    for (std::size_t i = 0; i < point.size(); i++) {
      paras[i].second = point[i] + precision;
      double cur = funcs->getExpression(name)->evaluate(name);
      gradients.push_back((cur-origin) / precision);
      paras[i].second = point[i];
    }
    
    //compute dist
    for (std::size_t i = 0; i < point.size(); i++) {
      double delta = gamma * gradients[i];
      dist += dist + pow(delta, 2);
      //update parameters of function
      if (flag == 0) {
        point[i] = point[i] + delta;
      }
      else {
        point[i] = point[i] - delta;
      }
      paras[i].second = point[i];
    }
    dist = sqrt(dist);
  }
  return funcs->getExpression(name)->evaluate(name);
}

void printGrad(std::string line, double res, int flag) {
  if (flag == 0) {
    //max
    std::cout << "The max value is " << res << std::endl;
  }
  else {
    //min
    std::cout << "The min value is " << res << std::endl;
  }
}
