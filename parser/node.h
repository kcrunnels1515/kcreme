#ifndef NODE_H_
#define NODE_H_
#include <string>
#include <iostream>
#include "parser.h"
#include "../ttmath/ttmath.h"
#include "../ttmath/ttmathint.h"
#include "../Token/token.h"
#include <vector>

typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(128)> Float;
typedef ttmath::Int<TTMATH_BITS(64)> Int;
typedef ttmath::UInt<TTMATH_BITS(64)> Uint;

union Value;
struct Node;

union Value {
  std::string str = "";
  char chr;
  Float flt;
  Int sint;
  Uint uint;
  void* data;
  ~Value(){}
};

struct Node {
  Value val;
  TokenType token;
  std::vector<Node*> kids;
  Node* parent;

  Node(std::string s, TokenType t) {
    switch (t) {
        case NIL:
          val.str = "NIL";
          break;
        case SEXPR:
          val.str = "SEXPR";
          break;
        case ID:
          val.str = s;
          break;
        case INT:
          if (s[0] == '-' || s[0] == '+') {
            val.sint = s;
          } else {
            val.uint = s;
          }
          break;
        case FLOAT:
          val.flt = s;
          break;
        case CHAR:
          val.chr = s[0];
          break;
        case STR:
          val.str = s;
          break;
        case QUOTE:
          val.str = "QUOTE";
          break;
        case LIST:
          val.str = "LIST";
        default:
          val.str = "weird shit has happened: " + s;
          break;
      }
    token = t;
  }

};

#endif // NODE_H_
