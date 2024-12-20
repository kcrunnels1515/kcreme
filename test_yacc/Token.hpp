#ifndef TOKEN_H_
#define TOKEN_H_

enum TokenType {
  NIL,
  SEXPR,
  ID,
  INT,
  FLOAT,
  OP,
  LIST,
  CHAR,
  STR,
  VEC,
  QUOTE
};

#endif // TOKEN_H_
