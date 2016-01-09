//  strtonum.h
//  numeric

#ifndef __STRTONUM_H__
#define __STRTONUM_H__

#include <sstream>

template <class Type>
Type strtonum(const std::string& str) {
  std::istringstream iss(str);
  Type num;
  iss >> num;
  return num;
}

#endif /* strtonum_h */
