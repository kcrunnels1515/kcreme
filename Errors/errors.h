#ifndef ERRORS_H_
#define ERRORS_H_

#include "../Token/token.h"
#include <exception>
#include <string>
#include <iostream>

class LexError : public std::exception {
private:
  size_t ind;
  std::string msg;
public:
  LexError(size_t loc, const char* m) : ind(loc) {
    msg = "LexError: " + std::string(m) + "; @ind = " + std::to_string(loc);
  }
  const char* what() const throw() {
    return msg.c_str();
  }
};

class ParseError : public std::exception {
  private:
  TokenType recv;
  int state;
  std::string msg;
  public:
  ParseError(TokenType _recv, char _s) : recv(_recv), state(_s) {
    msg = "In state " + std::to_string(state) + ", received token of type ";

    switch (_recv) {
        case NIL:
          msg += "NIL\n";
          break;
        case SEXPR:
          msg += "SEXPR\n";
          break;
        case ID:
          msg += "ID\n";
          break;
        case INT:
          msg += "INT\n";
          break;
        case FLOAT:
          msg += "FLOAT\n";
          break;
        case OPR:
          msg += "OPR\n";
          break;
        case LIST:
          msg += "LIST\n";
          break;
        case CHAR:
          msg += "CHAR\n";
          break;
        case STR:
          msg += "STR\n";
          break;
        case QUOTE:
          msg += "QUOTE\n";
          break;
        case OP:
          msg += "OP\n";
          break;
        case CP:
          msg += "CP\n";
          break;
        case PGN:
          msg += "PGN\n";
          break;
        case ATOM:
          msg += "ATOM\n";
          break;
        case END:
          msg += "END\n";
          break;
    }

  }
  const char* what() const throw() {
    return msg.c_str();
  }

};

#endif // ERRORS_H_
