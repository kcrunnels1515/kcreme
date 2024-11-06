#include "Grammar.hpp"

DotProd::DotProd(std::list<TokenType>& _item) : item(_item) {
  this->dot = item.begin();
}

DotProd::DotProd(const DotProd& d) : item(d.item) {
  dot = d.dot;
  dot++;
}

DotProd& DotProd::operator=(const DotProd& d){
  if (this != &d){
    item = d.item;
    dot = d.dot;
  }
  return *this;
}

Grammar::Grammar() {
  // Start -> Expression
  productions = {
    {START, {
        {EXPR, END}
      }},
    {EXPR, {
        {SEXPR},
        {QUOTE},
        {ATOM}
      }},
    {SEXPR, {
        {OP, OPER, ARG, CP},
        {OP, SEXPR, ARG, CP}
      }},
    {ARG, {
        {ATOM},
        {SEXPR},
        {QUOTE},
        {TDECL},
        {ARG, ARG}
      }},
    {ATOM, {
        {INT},
        {FLOAT},
        {BOOL}
      }},
    {TDECL, {
        {ATOM, DELIM, ATOM}
      }},
    {OPER, {
        {PLUS},
        {MINUS},
        {DIV},
        {MULT},
        {MOD},
        {QUOTE_O},
        {ATOM_O},
        {EQ},
        {CONS},
        {CAR},
        {CDR},
        {COND},
        {LAMBDA},
        {DEFINE},
        {LET},
        {SETS},
      }},
    {QUOTE, {
        {OP, QUOTE_O, EXPR},
        {SQ, EXPR}
      }},
  };
}

void Grammar::closure(){
  states.push_back(new State);
  DotProd cur = DotProd(this->productions[START][0]);
  states[0]->I[START] = cur;
  while (!is_terminal())
}

bool Grammar::is_terminal(TokenType tt) {
  switch(tt) {
    case SEXPR:
    case TDECL:
    case START:
    case QUOTE:
    case ATOM:
    case OPER:
    case SYMBOL:
    case EXPR:
    case ARG:
      return false;
    default:
      return true;
  }
}

void Grammar::rotate_list(std::list<TokenType>& t_list){
  t_list.push_back(t_list.front());
  t_list.pop_front();
}

void Grammar::add_rule(TokenType tt, std::list<TokenType> terms){
  productions[tt].push_back(terms);
}

void Grammar::check_valid(std::vector<Node*>& stack, Node*& lookahead){

}
