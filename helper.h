//  helper.h
//  numeric

#ifndef helper_h
#define helper_h

class Functions;
class Expression;

/*skip space*/
void skipSpace(std::string &line, std::size_t &pos);

/* parse the function name */
std::string parseFunctionName(std::string &line, std::size_t &pos);

/* check if it is a valid operator */
bool isValidOp(std::string op);

/* check if it is a valid identifier */
bool isValidId(std::string str);


Expression * parseExpression(std::string &line, std::size_t &pos, std::string name,
                             Functions * funcs);
/* parse parameters */
std::vector<std::pair<std::string, double> > parseParameter(std::string &line, std::size_t &pos);

/* check whether the function has been difined */
bool isValidFunc(std::string op, Functions * funcs);

/* make an expr */
Expression * makeExpr(std::string op, Expression * lhs, Expression * rhs);


/* find a operator, parse the rest part */
Expression * parseOp(std::string &line, std::size_t &pos, std::string name,
                     Functions *funcs);

/* parse a sub function */
Expression * parseFunc(std::string &line, std::size_t &pos, Functions *funcs);

/* definition parse from this function */
void parse(std::string &line, Functions * functions);

/* evaluate parse from this function */
double evaluate(std::string &line, Functions *functions);

/* compute integeration */
double integeral(std::string &line, Functions * funcs);

void computeInteg (std::size_t start, double * res, double precision,
                     std::vector<std::pair<double, double> > rangeList,
                     std::string name, Functions * funcs);

/* compute mc integeration */
double mcint(std::string &line, Functions * funcs);

void updateMaxMin(std::size_t start, double * max, double * min,
                  std::vector<std::pair<double, double> > rangeList,
                  std::string name, Functions * funcs);

/* max and min */
double gradAsc(std::string &line, Functions * funcs, int flag);

std::vector<double> computeGrad(std::vector<double> point, Functions * funcs);

/* print out the info of definition */
void printDef(std::string name, std::vector<std::pair<std::string, double> > & paras);

/* print out the result of evaluation */
void printEval(std::string line, double res);

/* print out the result of integration */
void printInte(std::string line, double res);

/* print out the res of mcint */
void printMcint(std::string line, double res);

/* print out res of max or min */
void printGrad(std::string line, double res, int flag);

#endif /* helper_h */
