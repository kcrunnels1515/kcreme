#ifndef PARSER_H_
#define PARSER_H_
#include <map>
#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include <string>
#include <cstdlib>
#include "../ValueInterface/ValueInterface.hpp"

// enum TokenType {
//   NIL, // nothing value
//   OP, // open parens
//   CP, // close parens
//   SQ, // shorthand QUOTE using '
//   QUOTE,  // QUOTE
//   DELIM,  // type deliminator
//   STR,    // string
//   INT,    // integer
//   FLOAT,  // float
//   ATOM,   // generic atom
//   OPER,   // generic operator
//   SEXPR,  // s-expression
//   TDECL   // type declaration
// };


struct Node {
  TokenType ttype;
  void* value;
  size_t size;
  std::vector<Node*> chldrn;

  ~Node();
};

class Parser {
private:
  // parsing data structures
  std::queue<Node*> in_strm;
  std::vector<Node*> stack;
  // unsigned int is index in jmp table for function
  std::map<std::string,unsigned>& oper_to_int;
  std::map<unsigned,std::string>& int_to_asm;
  unsigned node_num;

  // code generation data structures
  std::string code;
  ValuePool vp;

  // meaningful functions
  void lex(std::string& text);
  void parse_tok(Node* tk, std::string s);
  void reduce(int pos, int len, bool delim);
  void recursive_print(Node* nd, std::vector<bool> flag, int depth, bool is_last);

  // helper functions
  bool sc_type(int offset, TokenType t);
public:
  Parser(std::map<std::string,unsigned>& _oper_int, std::map<unsigned,std::string>& _oper_asm);
  void insert_symbol(std::string n, std::string asm_code);
  void build_operators();
  void parse(std::string& text);
  void print_parse_tree();
  void clear();
  void emit_code();
  void print_code();
};

inline bool is_int(const std::string & s);
inline bool is_float(const std::string & s);
void ptr_to_str(std::string& str, void* ptr);

#endif // PARSER_H_
