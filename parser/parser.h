#ifndef PARSER_H_
#define PARSER_H_
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>

#include "../Token/token.h"
#include "node.h"
#include "../Errors/errors.h"

struct Parser {
  size_t ind;
  Node* root;

  std::vector<Token> in_strm;
  std::vector<uint8_t> s_stk;
  std::vector<Node*> p_stk;
  enum {
    E = 0,
    S,
    R,
    A
  };
  // END,(,),CHR,FLT,IDN,INT,OPR,QUOTE,STR
  uint8_t a_tbl[26][10][2] = {
    {{E,0}, {S,2}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}},
    {{A,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}},
    {{E,0}, {S,6}, {S,7}, {S,8}, {S,9}, {S,10}, {S,11}, {S,12}, {S,13}, {S,14}},
    {{E,0}, {R,4}, {R,4}, {R,4}, {R,4}, {R,4}, {R,4}, {E,0}, {E,0}, {R,4}},
    {{E,0}, {S,6}, {S,17}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {E,0}, {S,14}},
    {{E,0}, {R,5}, {R,5}, {R,5}, {R,5}, {R,5}, {R,5}, {E,0}, {E,0}, {R,5}},
    {{E,0}, {S,6}, {S,19}, {S,8}, {S,9}, {S,10}, {S,11}, {S,20}, {S,13}, {S,14}},
    {{R,1}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}},
    {{E,0}, {R,12}, {R,12}, {R,12}, {R,12}, {R,12}, {R,12}, {E,0}, {E,0}, {R,12}},
    {{E,0}, {R,11}, {R,11}, {R,11}, {R,11}, {R,11}, {R,11}, {E,0}, {E,0}, {R,11}},
    {{E,0}, {R,14}, {R,14}, {R,14}, {R,14}, {R,14}, {R,14}, {E,0}, {E,0}, {R,14}},
    {{E,0}, {R,10}, {R,10}, {R,10}, {R,10}, {R,10}, {R,10}, {E,0}, {E,0}, {R,10}},
    {{E,0}, {S,6}, {E,0}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {S,13}, {S,14}},
    {{E,0}, {S,6}, {E,0}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {E,0}, {S,14}},
    {{E,0}, {R,13}, {R,13}, {R,13}, {R,13}, {R,13}, {R,13}, {E,0}, {E,0}, {R,13}},
    {{E,0}, {R,6}, {R,6}, {R,6}, {R,6}, {R,6}, {R,6}, {E,0}, {E,0}, {R,6}},
    {{E,0}, {R,7}, {R,7}, {R,7}, {R,7}, {R,7}, {R,7}, {E,0}, {E,0}, {R,7}},
    {{R,2}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}},
    {{E,0}, {S,6}, {S,22}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {E,0}, {S,14}},
    {{E,0}, {R,1}, {R,1}, {R,1}, {R,1}, {R,1}, {R,1}, {E,0}, {E,0}, {R,1}},
    {{E,0}, {S,6}, {E,0}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {S,13}, {S,14}},
    {{E,0}, {S,6}, {S,24}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {E,0}, {S,14}},
    {{E,0}, {R,2}, {R,2}, {R,2}, {R,2}, {R,2}, {R,2}, {E,0}, {E,0}, {R,2}},
    {{E,0}, {S,6}, {S,25}, {S,8}, {S,9}, {S,10}, {S,11}, {E,0}, {E,0}, {S,14}},
    {{R,3}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}, {E,0}},
    {{E,0}, {R,3}, {R,3}, {R,3}, {R,3}, {R,3}, {R,3}, {E,0}, {E,0}, {R,3}}
};
// | NIL | SEXPR | ID  | INT | FLOAT | OPR | LIST | CHAR | STR | QUOTE | (   | ) | program | atom | end |
  uint8_t g_tbl[26][3] = {
//  ATOM | LIST | SEXPR
    {0,0,1},
    {0,0,0},
    {3,4,5},
    {0,0,0},
    {15,0,16},
    {0,0,0},
    {3,18,5},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {3,21,5},
    {15,0,16},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {15,0,16},
    {0,0,0},
    {3,23,5},
    {15,0,16},
    {0,0,0},
    {15,0,16},
    {0,0,0},
    {0,0,0}
  };
  uint8_t r_tbl[15][2] = { // rule table
   // {15, 1}, // (0) $accept -> program $end (drop EOF token, then goto accept state)
    {PGN, 1}, // (1) program -> sexpr
    {SEXPR, 2}, // (2) sexpr -> ()
    {SEXPR, 3}, // (3) sexpr -> ( list )
    {SEXPR, 4}, // (4) sexpr -> ( OPR list )
    {LIST, 1}, // (5) list -> atom
    {LIST, 1}, // (6) list -> sexpr
    {LIST, 2}, // (7) list -> list atom
    {LIST, 2}, // (8) list -> list sexpr
    {LIST, 2}, // (9) list -> QUOTE atom
    {LIST, 2}, // (10) list -> QUOTE sexpr
    {ATOM, 1}, // (11) atom -> INT
    {ATOM, 1}, // (12) atom -> CHR
    {ATOM, 1}, // (13) atom -> STR
    {ATOM, 1}, // (14) atom -> FLT
    {ATOM, 1}, // (15) atom -> ID
  };

  Parser();
  void renew(std::vector<Token> input);
  void parse();
  void shift();
  void goto_s();
  void reduce(uint8_t c);
  void accept();
  void error();
  uint8_t ind_conv(TokenType t);
};

#endif // PARSER_H_
