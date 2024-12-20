#include "lex.h"

void Lexer::renew(std::string input) {
  if (chars)
    delete chars;
  chars = new CharStream(input);
}

bool Lexer::peek(std::vector<std::string> patterns) {
  for (int i = 0; i < patterns.size(); ++i) {
    std::regex r(patterns[i]);
    if (!chars->in_strm(i) || !std::regex_match(std::string(1, chars->get(i)), r)) return false;
  }
  return true;
}

bool Lexer::match(std::vector<std::string> patterns) {
  bool peeked = peek(patterns);
  if (peeked) {
    for (int i = 0; i < patterns.size(); ++i) {
      chars->advance();
    }
  }
  return peeked;
}

void Lexer::consume(std::string pattern, std::string msg) {
  if (!match({pattern})) {
    throw LexError(chars->ind, msg.c_str());
  }
}

void Lexer::lex(std::vector<Token>& tokens) {
  while(chars->in_strm(0)) {
    if(match({ "[ \\b\\n\\r\\t]" }))
      {
        chars->skip();
      } else
      {
        tokens.push_back(lex_tok());
      }
  }
}

Token Lexer::lex_tok() {
  if (peek({"[A-Za-z_]"})) {
    return lex_idn();
  } else if (peek({"[0-9]"})) { // unsigned
    return lex_num();
  } else if (peek({"[+-]", "0", "\\.", "[0-9]"})) { // signed less than one
     return lex_num();
  } else if (peek({"[+-]", "[1-9]"})) { //signed greater than or equal to one
    return lex_num();
  } else if (peek({"`"})) {
    return lex_chr();
  } else if (peek({"\""})) {
    return lex_str();
  } else if (peek({"."})) {
    return lex_opr();
  } else {
    throw LexError(chars->ind, "Symbol not recognised for this grammar");
  }
}
Token Lexer::lex_idn() {
  chars->advance();
  while(match({"[A-Za-z0-9_-]"}));
  return chars->emit(ID);
}

Token Lexer::lex_num() {
  if (match({"[+-]"})) { //signed
    if (match({"[1-9]"})) {
      while(match({"[0-9]"}));
      if (match({"\\.", "[0-9]"})) {
        while(match({"[0-9]"}));
        return chars->emit(FLOAT);
      } else {
        return chars->emit(INT);
      }
    } else if (match({"0", "\\.", "[0-9]"})) {
       while(match({"[0-9]"}));
       return chars->emit(FLOAT);
    } else { // emit +/- operator
      return chars->emit(OPR);
    }
  } else if (match({"[1-9]"})) { //unsigned
    while(match({"[0-9]"}));
    if (match({"\\.", "[0-9]"})) {
      while(match({"[0-9]"}));
      return chars->emit(FLOAT);
    } else {
      return chars->emit(INT);
    }
  } else if (match({"0"})) { //unsigned
    if (match({"\\.", "[0-9]"})) {
      while(match({"[0-9]"}));
      return chars->emit(FLOAT);
    } else {
      return chars->emit(INT);
    }
  } else {
    throw LexError(chars->ind, "Unexpection character");
  }

}

Token Lexer::lex_chr() {
  consume("`", "Expected grave");
  if (match({"\\\\"})) {
    consume("[bnrt\'\"`\\\\]", "Expected valid escape character");
  } else {
    consume("[^`\\\\]", "Invalid character inside graves");
  }
  consume("`", "Expected closing grave");
  return chars->emit(CHAR);
}
Token Lexer::lex_str() {
  bool done = false;
  consume("\"", "Expected opening quote");
  while(!done) {
    if (match({"[^\"\\\\\n\t]"})) {}
    else if (match({"\\\\"})) {
      consume("[bnrt\'\"`\\\\]", "Expected valid escape character");
    } else if (match({"\""})) {
      done = true;
    } else {
      throw LexError(chars->ind, "Expected closing quote");
    }
  }
  return chars->emit(STR);
}
Token Lexer::lex_opr() {
  if (match({"<"}) || match({">"}) || match({"!"}) || match({"="})) {
    match({"="});
    return chars->emit(OPR);
  } else if (match({"\'"})) {
    return chars->emit(QUOTE);
  } else if (match({"\\("})) {
    return chars->emit(OP);
  } else if (match({"\\)"})) {
    return chars->emit(CP);
  } else {
    chars->advance();
    return chars->emit(OPR);
  }
}
