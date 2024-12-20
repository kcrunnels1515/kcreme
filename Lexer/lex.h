#ifndef LEX_H_
#define LEX_H_
#include <string>
#include <regex>
#include <vector>
#include "../Errors/errors.h"
#include <bits/stdc++.h>
#include "../Token/token.h"

struct CharStream {
  std::string input;
  size_t ind;
  size_t len;

  CharStream(std::string _input) : input(_input), ind(0), len(0) {}
  bool in_strm(size_t offset) { return input.size() > (ind + offset); }
  char get(size_t offset) { return input[ind+offset]; }
  void advance() { ind++; len++; }
  void skip() { len = 0; }
  Token emit(TokenType t) { size_t start = ind - len; size_t tmp = len; skip(); return Token(input.substr(start, tmp), start, t); }
};

class Lexer {
  CharStream *chars;
public:
  Lexer() : chars(nullptr) {};
  void renew(std::string input);
  bool peek(std::vector<std::string> patterns);
  bool match(std::vector<std::string> patterns);
  void consume(std::string pattern, std::string msg);
  void lex(std::vector<Token>& tokens);
  Token lex_tok();
  Token lex_idn();
  Token lex_num();
  Token lex_chr();
  Token lex_str();
  Token lex_opr();
};

#endif // LEX_H_
