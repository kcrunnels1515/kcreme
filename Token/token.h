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
  static std::string pt(TokenType t) {
    std::string msg;
    switch(t){
      case NIL:
        msg = "NIL";
        break;
      case SEXPR:
        msg = "SEXPR";
        break;
      case ID:
        msg = "ID";
        break;
      case INT:
        msg = "INT";
        break;
      case FLOAT:
        msg = "FLOAT";
        break;
      case OPR:
        msg = "OPR";
        break;
      case LIST:
        msg = "LIST";
        break;
      case CHAR:
        msg = "CHAR";
        break;
      case STR:
        msg = "STR";
        break;
      case QUOTE:
        msg = "QUOTE";
        break;
      case OP:
        msg = "OP";
        break;
      case CP:
        msg = "CP";
        break;
      case ATOM:
        msg = "CP";
        break;
      case PGN:
        msg = "ROOT";
        break;
      default:
        msg = "strange occurrences";
        break;
    }
    return msg;
  }
  void print() {
    std::cout << "Token of type" << pt(token) << ": " << name << " @" << ind << std::endl;
  }
};

#endif // TOKEN_H_
