#include "parser.h"

Parser::Parser() {
  ind = 0;
  root = nullptr;
}

uint8_t Parser::ind_conv(TokenType t) {
  switch(t) {
    case SEXPR:
      return 2;
    case ID:
      return 5;
    case INT:
      return 6;
    case FLOAT:
      return 4;
    case OPR:
      return 7;
    case LIST:
      return 1;
    case CHAR:
      return 3;
    case STR:
      return 9;
    case QUOTE:
      return 8;
    case OP:
      return 1;
    case CP:
      return 2;
    case ATOM:
      return 0;
    case END:
      return 0;
    default:
      return 255;
  }
}

void Parser::renew(std::vector<Token> input) {
  // set token stream, add EOF token, and reverse
  // so that we can pop instead of removing from the front
  in_strm = input;
  in_strm.push_back(Token("", 0, END));
  std::reverse(in_strm.begin(), in_strm.end());
  p_stk.clear();
  s_stk.clear();
  root = nullptr;
}

void Parser::parse() {
  s_stk.push_back(0);
  while (!root) {
    uint8_t* act = a_tbl[s_stk.back()][ind_conv(in_strm.back().token)];
    if (act[0] == S) {
      s_stk.push_back(act[1]);
      shift();
    } else if (act[0] == R) {
      reduce(act[1]);
      s_stk.push_back(g_tbl[s_stk.back()][ind_conv(p_stk.back()->token)]);
    } else if (act[0] == A) {
      root = p_stk.back();
      std::cout << "Accepted" << std::endl;
    } else {
      throw ParseError(in_strm.back().token, s_stk.back());
    }
  }
}

void Parser::shift() {
  p_stk.push_back(new Node(in_strm.back().name,in_strm.back().token));
  in_strm.pop_back();
}

void Parser::reduce(uint8_t c) {
  Node* tmp;
  uint8_t* r = r_tbl[c];
  for (int i = 0; i < r[1]; ++i)
    s_stk.pop_back();

  if (r[0] < 0) {
    p_stk.pop_back();
  } else {
    tmp = new Node("", (TokenType)r[0]);
    for (int i = 0; i < r[1]; ++i) {
      if (p_stk.back()->token != OP && p_stk.back()->token != CP) {
        tmp->kids.push_back(p_stk.back());
        p_stk.back()->parent = tmp;
      }
      p_stk.pop_back();
    }
    std::reverse(tmp->kids.begin(), tmp->kids.end());
    p_stk.push_back(tmp);
  }
}

void Parser::trim(Node* cur) {
  std::vector<Node*>& kids = cur->kids;
  switch (cur->token) {
  case NIL:
    break;
  case SEXPR:
    {
      if (kids.size() == 0) {
        cur->token = NIL;
      } else {
        // if first element is an operator,
        // move all children of sexpr to operator
        // and resize the sexpr child vector
        // to only contain operator
        if (kids[0]->token == OPR) {
          for (int j = 1; j < kids.size(); ++j){
            kids[0]->kids.push_back(kids[j]);
          }
          kids.resize(1);
        } else if (kids[0]->token == SEXPR) {
          // first element is lambda function

        } else if (kids[0]->token == ID) {
          // first element is a named function

        } else if (kids[0]->token == QUOTE) {
          // quote expression

        } else {
          // error
        }
      }
    }
    break;
  case ID:
    break;
  case INT:
    break;
  case FLOAT:
    break;
  case OPR:
    msg = "OPR";
    break;
  case LIST:
    {
      collapse_list(cur->kids, cur);
    }
    break;
  case CHAR:
    msg = "CHAR";
    break;
  case STR:
    msg = "STR";
    break;
  case QUOTE:
    msg = "QUOTE";
    break;
  case OP:
    msg = "OP";
    break;
  case CP:
    msg = "CP";
    break;
  case ATOM:
    msg = "CP";
    break;
  case PGN:
    msg = "ROOT";
    break;
  default:
    msg = "strange occurrences";
    break;

  }  
}

void Parser::print(){
  print_node("", root, true);
}

void Parser::print_node(std::string indent, Node* cur, bool is_last) {
  if( cur != nullptr )
    {
        std::cout << indent;

        std::cout << (is_last ? "└──" : "├──" );

        // print the value of the node
        std::cout << Token::pt(cur->token) << std::endl;

        // enter the next tree level - left and right branch
        for (int i = 0; i < cur->kids.size(); ++i) {
          if (i == cur->kids.size() - 1){
            print_node(indent + (is_last ? "    " : "│   " ), cur->kids[i], true);
          } else {
            print_node(indent + (is_last ? "    " : "│   " ), cur->kids[i], true);
          }
        }
    }
}

void Parser::collapse_list(std::vector<Node*>& cons, Node* cur) {
  for (Node* i : cur->kids) {
    if (i->token == LIST){
      collapse_list(cons,i);
    } else {
      cons.push_back(i);
    }
  }
}
