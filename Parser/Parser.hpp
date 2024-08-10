#ifndef PARSER_H_
#define PARSER_H_
#include <map>
#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include <string>
#include <cstdlib>

enum TokenType {
  NIL, // nothing value
  OP, // open parens
  CP, // close parens
  SQ, // shorthand QUOTE using '
  QUOTE,  // QUOTE
  DELIM,  // type deliminator
  STR,    // string
  INT,    // integer
  FLOAT,  // float
  ATOM,   // generic atom
  OPER,   // generic operator
  SEXPR,  // s-expression
  TDECL   // type declaration
};


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
  std::map<std::string,unsigned>& oper_names;
  unsigned node_num;

  // code generation data structures
  std::string code;

  // functions
  void lex(std::string& text);
  void parse_tok(Node* tk, std::string s);
  void reduce(int pos, int len, bool delim);
  void recursive_print(Node* nd, std::vector<bool> flag, int depth, bool is_last);
public:
  Parser(std::map<std::string,unsigned>& _oper_names);
  void parse(std::string& text);
  void print_parse_tree();
  void clear();
};

inline bool is_int(const std::string & s);
inline bool is_float(const std::string & s);
void ptr_to_str(std::string& str, void* ptr);

#endif // PARSER_H_
