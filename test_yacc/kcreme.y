%{
  #include <stdio.h>
  #include <string>
  #include "Node.hpp"
  //#include "Token.hpp"
  int flag=0;
  struct Node* root;
  extern int yylex();
  void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
  struct Node* sexpr;
  struct Node* quote;
  struct Node* vec;
  struct Node* atom;
  struct Node* list;
  std::string* string;
  int token;
}

%token <string> IDN STR CHR INT FLT OPR
%token <token> NIL LIST VEC QUOTE SEXPR

%type <sexpr> sexpr
//%type <quote> quote
//%type <vec> vec
%type <list> list
%type <atom> atom


%start program

%%

program: list { root = $1;}

sexpr: '(' ')' { $$ = new struct Node("", NIL); }
     | '(' list ')' { $$ = new struct Node("", SEXPR); $$->kids.push_back($2); }
     | '(' OPR list ')' { $$ = new struct Node("", SEXPR); $$->kids.push_back(new struct Node(*$2, OPR)); $$->kids[0]->kids.push_back($3); }

list: atom { $$ = new struct Node("", LIST); $$->kids.push_back($1); }
    | sexpr { $$ = new struct Node("", LIST); $$->kids.push_back($1); }
    | list atom { $$ = new struct Node("", LIST); $$->kids.push_back($1); $$->kids.push_back($2); }
    | list sexpr { $$ = new struct Node("", LIST); $$->kids.push_back($1); $$->kids.push_back($2); }
    | QUOTE atom { $$ = new struct Node("", LIST); $$->kids.push_back($1); }
    | QUOTE sexpr { $$ = new struct Node("", LIST); $$->kids.push_back($1); }

atom: INT { $$ = new struct Node(*$1, INT); }
    | CHR { $$ = new struct Node(*$1, CHR); }
    | STR { $$ = new struct Node(*$1, STR); }
    | FLT { $$ = new struct Node(*$1, STR); }
    | IDN { $$ = new struct Node(*$1, IDN); }

%%
