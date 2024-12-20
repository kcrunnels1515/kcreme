#ifndef DEFS_H_
#define DEFS_H_

enum TokenType {
  NIL,
  SEXPR,
  ID,
  INT,
  FLOAT,
  CHAR,
  STR,
  VEC,
  QUOTE
};
  // NIL,    // nothing value
  // OP,     // open parens
  // CP,     // close parens
  // SQ,     // shorthand QUOTE using '
  // QUOTE,  // QUOTE
  // DELIM,  // type deliminator
  // STR,    // string of chars
  // INT,    // integer
  // FLOAT,  // float
  // ATOM,   // generic atom
  // BOOL,   // boolean
  // STRING, // string data type
  // OPER,   // generic operator
  // SEXPR,  // s-expression
  // TDECL,  // type declaration
  // START,  // Grammar start symbol
  // END,    // End symbol
  // EXPR,   // symbol or sexpr
  // SYMBOL, // symbol/variable
  // ARG,    // arguments to a function, purely a parser-intermediate node
  //         // arguments are transferred to the function during parsing
  // PLUS,
  // MINUS,
  // DIV,
  // MULT,
  // MOD,
  // QUOTE_O,
  // ATOM_O,
  // EQ,
  // CONS,
  // CAR,
  // CDR,
  // COND,
  // LAMBDA,
  // DEFINE,
  // LET,
  // SETS


enum Action {
  ERROR,
  REDUCE,
  SHIFT,
  ACCEPT,
  GOTO
};

#endif // DEFS_H_
