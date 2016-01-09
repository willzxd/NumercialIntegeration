#ifndef __FUNCTIONS_H___
#define __FUNCTIONS_H___

#include <map>
#include <vector>
#include <string>

#include "strtonum.h"
#include "helper.h"
#include "Expr.h"

class Functions {
private:
  std::map<std::string, std::vector<std::pair<std::string, double> > > funcMap;
  std::map<std::string, Expression *> formula;
  std::vector<std::string> nameList;
public:
  Functions();
  
  /* store function name -> expr */
  void defineFunction(std::string functionName, Expression * expr);
  
  /* store functionName -> parameterList */
  void definePara(std::string functionName, std::vector<std::pair<std::string, double> > parameters);
  
  /* store functionName as a list */
  void addName(std::string name);

  std::vector<std::pair<std::string, double> > & getParaList(std::string name);
  
  std::vector<std::string> & getFunctionList();
  
  Expression * getExpression(std::string name);
  
  void updateParaList(std::string &line, std::size_t &pos, std::string name);
  
  double evaluateExpression(std::string line, std::size_t &pos, std::string name);
  
  /* only for debug help */
  void printAll();
  
  ~Functions();
};

#endif
