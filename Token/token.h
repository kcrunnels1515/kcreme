#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>
#include <iostream>

enum TokenType {
  NIL = 0,
  SEXPR = 1,
  ID = 2,
  INT = 3,
  FLOAT = 4,
  OPR = 5,
  LIST = 6,
  CHAR = 7,
  STR = 8,
  QUOTE = 9,
  OP = 10,
  CP = 11,
  PGN = 12,
  ATOM = 13,
  END = 14
};

struct Token {
  std::string name;
  size_t ind;
  TokenType token;
  Token(std::string s, size_t i, TokenType t) : name(s), ind(i), token(t) {}
  void print() {
    std::cout << "Token of type ";
    switch(token){
      case NIL:
        std::cout << "NIL";
        break;
      case SEXPR:
        std::cout << "SEXPR";
        break;
      case ID:
        std::cout << "ID";
        break;
      case INT:
        std::cout << "INT";
        break;
      case FLOAT:
        std::cout << "FLOAT";
        break;
      case OPR:
        std::cout << "OPR";
        break;
      case LIST:
        std::cout << "LIST";
        break;
      case CHAR:
        std::cout << "CHAR";
        break;
      case STR:
        std::cout << "STR";
        break;
      case QUOTE:
        std::cout << "QUOTE";
        break;
      default:
        std::cout << "strange occurrences";
        break;
    }
    std::cout << ": " << name << " @" << ind << std::endl;
  }
};

#endif // TOKEN_H_
