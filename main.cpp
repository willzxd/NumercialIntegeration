#include <string>
#include <iostream>
#include <fstream>
#include "Functions.h"
#include "Expr.h"
#include "strtonum.h"
#include "helper.h"

int main (int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./numerics inputFileName" << std::endl;
    return EXIT_SUCCESS;
  }
  
  std::string line;
  Functions * functions = new Functions();
  std::fstream f(argv[1]);
  while (std::getline(f, line)) {
    std::size_t pos = 0;
    if ((pos = line.find("define")) != std::string::npos) {
      parse(line, functions);
    }
    else if (((pos = line.find("evaluate")) != std::string::npos)) {
      //compute the value of a function
      double res = evaluate(line, functions);
      printEval(line, res);
    }
    else if (((pos = line.find("numint")) != std::string::npos)) {
      pos += 6;
      skipSpace(line, pos);
      std::string s = line.substr(pos);
      double res = integeral(s, functions);
      printInte(line, res);
    }
    else if (((pos = line.find("mcint")) != std::string::npos)) {
      pos += 5;
      skipSpace(line, pos);
      std::string s = line.substr(pos);
      double res = mcint(s, functions);
      printMcint(line, res);
    }
    else if (((pos = line.find("max")) != std::string::npos)) {
      pos += 3;
      skipSpace(line, pos);
      std::string s = line.substr(pos);
      double res = gradAsc(s, functions, 0);
      printGrad(line, res, 0);
    }
    else if (((pos = line.find("min")) != std::string::npos)) {
      //std::cout << "This is a Gradient Descent" << std::endl;
      pos += 3;
      skipSpace(line, pos);
      std::string s = line.substr(pos);
      double res = gradAsc(s, functions, 1);
      printGrad(line, res, 1);
    }
    else if (((pos = line.find("exit")) != std::string::npos)) {
      delete functions;
      f.close();
      return EXIT_SUCCESS;
    }
  }
  delete functions;
  f.close();
  return EXIT_SUCCESS;
}
