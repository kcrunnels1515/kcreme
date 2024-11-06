#ifndef GRAMMAR_H_
#define GRAMMAR_H_
#include "../Defs/Defs.hpp"
#include "../Parser/Parser.hpp"
#include <map>
#include <list>
#include <vector>

struct DotProd {
  std::list<TokenType>& item;
  std::list<TokenType>::iterator dot;
  DotProd(std::list<TokenType>& _item);
  DotProd(const DotProd& d);
  DotProd& operator=(const DotProd& d);
};

struct State {
  std::map<TokenType, DotProd> I;
  Action a;
};

class Grammar {
  private:
  std::map<TokenType, std::vector<std::list<TokenType>>> productions;
  std::vector<State*> states;
  void closure();
  void update_states(TokenType tt);

  bool is_terminal(TokenType tt);
  void rotate_list(std::list<TokenType>& t_list);
  public:
  Grammar();
  void add_rule(TokenType tt, std::list<TokenType> terms);
  void check_valid(std::vector<Node*>& stack, Node*& lookahead);
};

#endif // GRAMMAR_H_
