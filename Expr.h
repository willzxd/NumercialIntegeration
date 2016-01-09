#ifndef __EXPR_H___
#define __EXPR_H___

#include <assert.h>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>

#include "helper.h"

class Functions;

class Expression {
public:
  virtual std::string toString() const = 0;
  virtual double evaluate(std::string superName) = 0;
  virtual ~Expression(){}
};

class NumExpression : public Expression {
private:
  double num;
  Functions * f;
  std::string var;
public:
  /* constants */
  NumExpression(double _num);
  
  /* var */
  NumExpression(std::string _var, Functions * _f);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~NumExpression() {};
};

class PlusExpression : public Expression {
private:
  Expression * lhs;
  Expression * rhs;
public:
  PlusExpression(Expression * _lhs, Expression * _rhs);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~PlusExpression();
};

class MinusExpression : public Expression {
private:
  Expression * lhs;
  Expression * rhs;
public:
  MinusExpression(Expression * _lhs, Expression * _rhs);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~MinusExpression();
};

class TimesExpression : public Expression {
private:
  Expression * lhs;
  Expression * rhs;
public:
  TimesExpression(Expression * _lhs, Expression * _rhs);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~TimesExpression();
};

class DivExpression : public Expression {
private:
  Expression * lhs;
  Expression * rhs;
public:
  DivExpression(Expression * _lhs, Expression * _rhs);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~DivExpression();
};

class PowExpression: public Expression {
private:
  Expression * lhs;
  Expression * rhs;
public:
  PowExpression(Expression * _lhs, Expression * _rhs);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~PowExpression();
};

class SqrtExpression: public Expression {
private:
  Expression * e;
public:
  SqrtExpression(Expression * _e);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~SqrtExpression();
};

class StringExpression: public Expression {
private:
  /* store nestFunction as a string */
  std::string str;
  Functions * f;
public:
  StringExpression(std::string _str, Functions * _f);
  virtual std::string toString() const;
  virtual double evaluate(std::string superName);
  ~StringExpression();
};

#endif
