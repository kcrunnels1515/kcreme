#include "Parser.hpp"

// check stack boundaries
#define CSB (size, ind, code)  \
  if ((size - 1 - ind) >= 0) { \
    code                       \
  }                            \

inline bool is_int(const std::string & s) // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

inline bool is_float(const std::string & s) // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtod(s.c_str(), &p);

   return (*p == 0);
}

void ptr_to_str(std::string& str, void* ptr) {
  char *buf;
  size_t sz;
  sz = snprintf(NULL, 0, "0x%x", ptr);
  buf = (char *)malloc(sz + 1); /* make sure you check for != NULL in real code */
  if (buf != NULL){
    snprintf(buf, sz+1, "0x%x", ptr);
    str = buf;
    free(buf);
  }
}

//Node::Node(TokenType _tt, void* _value, size_t _size) : ttype(_tt), value(_value), size(_size) {}

Node::~Node() {
  free(value);
  for (Node*& i : this->chldrn) {
    delete i;
  }
}

Parser::Parser(std::map<std::string,unsigned>& _oper_names) : oper_names(_oper_names), node_num(0) {}

void Parser::lex(std::string& text){
  std::string accum = "";
  for (char i : text) {
    switch (i) {
      case ' ':
        if (!accum.empty()) {
          Node* tmp = new Node;
          this->parse_tok(tmp, accum);
          this->in_strm.push(tmp);
          accum.clear();
        }
        break;
      case '(':
        if (!accum.empty()) {
          Node* tmp = new Node;
          this->parse_tok(tmp, accum);
          this->in_strm.push(tmp);
          accum.clear();
        }
        {
          Node* tmp = new Node;
          tmp->ttype = OP;
          tmp->size = 0;
          tmp->value = nullptr;
          this->in_strm.push(tmp);
        }
        break;
      case ')':
        if (!accum.empty()) {
          Node* tmp = new Node;
          this->parse_tok(tmp, accum);
          this->in_strm.push(tmp);
          accum.clear();
        }
        {
          Node* tmp = new Node;
          tmp->ttype = CP;
          tmp->size = 0;
          tmp->value = nullptr;
          this->in_strm.push(tmp);
        }
        break;
      case ':':
        if (!accum.empty()) {
          Node* tmp = new Node;
          this->parse_tok(tmp, accum);
          this->in_strm.push(tmp);
          accum.clear();
        }
        {
          Node* tmp = new Node;
          tmp->ttype = DELIM;
          tmp->size = 0;
          tmp->value = nullptr;
          this->in_strm.push(tmp);
        }
        break;
      case '\'':
        if (!accum.empty()) {
          Node* tmp = new Node;
          this->parse_tok(tmp, accum);
          this->in_strm.push(tmp);
          accum.clear();
        }
        {
          Node* tmp = new Node;
          tmp->ttype = SQ;
          tmp->size = 0;
          tmp->value = nullptr;
          this->in_strm.push(tmp);
        }
        break;
      // case ',':
      //   if (!accum.empty()) {
      //     Token* tmp = new Token(NIL);
      //     this->parse_tok(tmp, accum);
      //     this->tokens.push_back(tmp);
      //     accum.clear();
      //   } else {
      //     Token* tmp = new Token(COMMA);
      //     this->tokens.push_back(tmp);
      //   }
      //   break;
      default:
        accum.push_back(i);
        break;
    }
  }
  if (!accum.empty()) {
    Node* tmp = new Node;
    this->parse_tok(tmp, accum);
    this->in_strm.push(tmp);
    accum.clear();
  }
}

void Parser::parse_tok(Node* tk, std::string s) {
  // INT
  if (is_int(s)){
    tk->ttype = INT;
    tk->size = sizeof(long);
    char* p{};
    const long i = strtol(s.c_str(), &p, 10);
    tk->value = malloc(sizeof(long));
    memcpy(tk->value, &i, sizeof(long));
    // FLOAT
  } else if (is_float(s)) {
    tk->ttype = FLOAT;
    tk->size = sizeof(double);
    char* p{};
    double d = strtod(s.c_str(), &p);
    tk->value = malloc(sizeof(double));
    memcpy(tk->value, &d, sizeof(double));
  // OPERATOR NAME
  } else if (this->oper_names.find(s) != this->oper_names.end()) {
    tk->ttype = OPER;
    tk->size = sizeof(unsigned);
    tk->value = malloc(sizeof(unsigned));
    memcpy(tk->value, &this->oper_names[s], sizeof(unsigned));
  } else {
    tk->ttype = STR;
    tk->size = s.size() + 1;
    tk->value = malloc(tk->size);
    memset(tk->value, 0, tk->size);
    memcpy(tk->value, s.c_str(), s.size());
  }
}

void Parser::reduce(int pos, int len, bool delim) {
  Node* new_node = new Node;
  if (this->stack[pos]->ttype == OP) {
    new_node->ttype = SEXPR;

    // identify functional node
    Node* fn = this->stack[pos+1];
    if (fn->ttype == QUOTE) {
      new_node = fn;
    } else {
      new_node->chldrn.push_back(fn);
    }

    // dont include CP in tree
    for (int i = pos+2; i < pos+len-1; ++i)
      // move all following nodes below functional node
      fn->chldrn.push_back(this->stack[i]);

  // if is a squote
  } else if (this->stack[pos]->ttype == SQ) {
    new_node->ttype = QUOTE;
    new_node->chldrn.push_back(this->stack[pos+1]);

  // if a typedecl
  } else if (delim) {
    new_node->ttype = TDECL;
    new_node->chldrn.push_back(this->stack[pos]);
    new_node->chldrn.push_back(this->stack[pos+2]);

  // if an atom value
  } else {
    new_node->ttype = ATOM;
    new_node->chldrn.push_back(this->stack[pos]);
  }
  // remove old nodes
  this->stack.erase(this->stack.begin() + pos, this->stack.begin() + pos + len);
  // insert new node
  this->stack.push_back(new_node);
  this->node_num++;
}

void Parser::parse(std::string& text) {
  // turn input text into tokens
  this->lex(text);
  std::vector<unsigned> paren_locs = {};
  // stupid hacky fix so that positional checking doesn't pass a -1 into reduce
  int sq_loc = -5;

  while (!in_strm.empty()) {
    // copy first token in queue to reduction stack
    this->stack.push_back(in_strm.front());
    // if the new token is not a closing paren, increment the number of nodes
    if (this->stack.back()->ttype != CP) this->node_num++;
    // remove the first node from the queue
    this->in_strm.pop();

    // on token type of newest node on stack
    switch (this->stack.back()->ttype) {
      // if node is an open paren, put location of it in paren tracking vector
      case OP:
        paren_locs.push_back(this->stack.size()-1);
        break;
      case CP:
        // call reduce function on nodes from last open paren over number of nodes between
        // open paren and last node
        // not a typedecl reduction
        this->reduce(paren_locs.back(), this->stack.size() - paren_locs.back(), false);
        paren_locs.pop_back();

        // if last squote is the previous node, reduce to quote
        if (sq_loc == this->stack.size() - 2) this->reduce(this->stack.size() - 2, 2, false);
        // stupid hacky fix so that positional checking doesn't pass a -1 into reduce
        sq_loc = -5;

        // if previous node is a delim, reduce with delim
        if (this->stack[this->stack.size() - 2]->ttype == DELIM) {
          this->reduce(this->stack.size() - 3, 3, true);
        }
        break;
      case SQ:
        sq_loc = this->stack.size() - 1;
        break;
      case QUOTE:
        break;
      case DELIM:
        break;
      case STR:
      case INT:
      case FLOAT:
        this->reduce(this->stack.size() - 1, 1, false);
        // if last squote is the previous node, reduce to quote
        if (sq_loc == this->stack.size() - 2) this->reduce(this->stack.size() - 2, 2, false);
        // stupid hacky fix so that positional checking doesn't pass a -1 into reduce
        sq_loc = -5;
      case ATOM:
      case OPER:
        break;
      case SEXPR:
        break;
      case NIL:
        break;
      case TDECL:
        break;
    }
  }
}

void Parser::recursive_print(Node* nd, std::vector<bool> flag, int depth = 0, bool is_last = false) {
  auto node_text = [](Node* n) -> std::string {
    std::string ans = "";
    switch(n->ttype){
      case NIL:
        ans = "()";
        break;
      case SQ:
        ans = "'";
        break;
      case DELIM:
        ans = ":";
        break;
      case STR:
        ans = "STR: ";
        for (int i = 0; i < n->size; ++i){
          ans.push_back(*((char*)n->value + i));
        }
        break;
      case INT:
        ans = "INT: " + std::to_string(*((int*)n->value));
        break;
      case FLOAT:
        ans = "FLOAT: " + std::to_string(*(double*)n->value);
        break;
      case ATOM:
        ans = "ATOM";
        break;
      case OPER:
        ans = "OPER: " + std::to_string(*(unsigned*)n->value);
        break;
      case SEXPR:
        ans = "SEXPR";
        break;
      case TDECL:
        ans = "???";
        break;
      default:
        ans = "ayo what fam";
    }
    return ans;
  };

  // Condition when node is None
    if (nd == NULL)
        return;

    // Loop to print the depths of the
    // current node
    for (int i = 1; i < depth; ++i) {

        // Condition when the depth
        // is exploring
        if (flag[i] == true) {
            std::cout << "| "
                << " "
                << " "
                << " ";
        }

        // Otherwise print
        // the blank spaces
        else {
            std::cout << " "
                << " "
                << " "
                << " ";
        }
    }

    // Condition when the current
    // node is the root node
    if (depth == 0)
        std::cout << node_text(nd) << '\n';

    // Condition when the node is
    // the last node of
    // the exploring depth
    else if (is_last) {
        std::cout << "+--- " << node_text(nd) << '\n';

        // No more childrens turn it
        // to the non-exploring depth
        flag[depth] = false;
    }
    else {
        std::cout << "+--- " << node_text(nd) << '\n';
    }

    int it = 0;
    for (auto i = nd->chldrn.begin();
    i != nd->chldrn.end(); ++i, ++it)

        // Recursive call for the
        // children nodes
        recursive_print(*i, flag, depth + 1,
            it == (nd->chldrn.size()) - 1);
    flag[depth] = true;
}

void Parser::print_parse_tree(){
  std::vector<bool> flag(this->node_num, true);
  recursive_print(this->stack[0], flag);
  std::cout << "node number: " << this->node_num << std::endl;
}

void Parser::clear() {
  stack.clear();
  node_num = 0;
}
