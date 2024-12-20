%{
#include <string>
#include "parser.hpp"
#include "Node.hpp"
//#include "../Defs/Defs.h"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

[ \n\t]                     ;

[A-Za-z_][A-Za-z0-9_-]*     SAVE_TOKEN; return IDN;

[+-]?([0-9]|[1-9][0-9]*)    SAVE_TOKEN; return INT;

[+-]?(0[.][0-9][0-9]*|[1-9][0-9]*[.][0-9][0-9]*) SAVE_TOKEN; return FLT;

\'([^\'\\]|\\[bnrt\'\"\\])\'  SAVE_TOKEN; return CHR;

\"([^\'\\]|\\[bnrt\'\"\\])*\" SAVE_TOKEN; return STR;

([<>!=]'='?|.)           SAVE_TOKEN; return OPR;

%%
