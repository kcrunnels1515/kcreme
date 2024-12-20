#include <iostream>
#include <vector>
#include <string.h>
//#include "MemoryManager/MemoryManager.hpp"
//#include "ValueInterface/ValueInterface.hpp"
//#include "Parser/Parser.hpp"
#include "Lexer/lex.h"
#include "parser/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

// extern "C" {
//   void* copy_res(void* data, size_t len) {
//     void* ptr = malloc(len);
//     memcpy(ptr, data, len);
//     return ptr;
//   }
// }

// void fill_operators(std::map<std::string,unsigned>& _oper_int, std::map<unsigned,std::string>& _int_asm) {
//   auto insert_op = [&](std::string name, std::string asm_code, unsigned ind) -> bool {
//     if (_oper_int.find(name) != _oper_int.end())  {
//       return false;
//     }
//     _oper_int[name] = ind;
//     _int_asm[ind] = asm_code;
//     return true;
//   };

  // variadic addition of numbers
  // expects stack structure:
  // int1
  // int2
  // int3
  // ...
  // numberOfInts
  //
  //
  // mov [rsp], rdi     ; move number of ints to rdi
  // pop                ; remove number of ints from stack
  // mov [rsp], rsi     ; move first int to rsi
  // pop                ; remove first int from stack
  // loop:              ; loop label
  //     sub 1, rdi     ; adjust number of remaining ints
  //     je end         ; have all ints been added
  //     mov [rsp], rdx ; move top int to rdx
  //     pop            ; remove top int from stack
  //     add rdx, rsi   ; add ints
  //     jmp loop
  //
  // end:
  //     push rsi
  //
//  insert_op("+", "mov [rsp], rdi;\npop;\nmov [rsp], rsi;\npop;\nloop: sub 1, rdi;\nje end;\nmov [rsp], rdx;\npop;\nadd rdx,rsi;\njmp loop;\nend: push rsi;\n", 0);

  // variadic subtraction of numbers
  // expects stack structure:
  // int1
  // int2
  // int3
  // ...
  // numberOfIntsToSubtract = # ints - 1
  //
  //
  // mov [rsp], rdi     ; move number of ints to rdi
  // pop                ; remove number of ints from stack
  // mov [rsp], rsi     ; move first int to rsi
  // pop                ; remove first int from stack
  // loop:              ; loop label
  //     sub 1, rdi     ; adjust number of remaining ints
  //     je end         ; have all ints been added
  //     mov [rsp], rdx ; move top int to rdx
  //     pop            ; remove top int from stack
  //     add rdx, rsi   ; add ints
  //     jmp loop       ; jump back to loop beginning
  //
  // end:
  //     mov [rsp], rdx ; copy first int to register rdx
  //     sub rsi, rdx   ; rdx = rdx - rsi
  //     push rdx       ; push answer to stack
  //
  //insert_op("-", "mov [rsp], rdi;\npop;\nmov [rsp], rsi;\nloop:sub 1, rdi;\nsub 1, rdi;\nje end;\nmov [rsp], rdx;\npop;\nadd rdx, rsi;\njmp loop;\nend: mov [rsp], rdx;\nsub rsi, rdx;\npush rdx;\n", 1);
//}

int main(int argc, char *argv[]) {
  static char *line_read = (char*)NULL;
  //std::map<std::string,unsigned> oper_int;
  //std::map<unsigned,std::string> int_asm;
  //fill_operators(oper_int, int_asm);
  Lexer l;
  Parser p;
  std::string line = "";
  rl_bind_key ('\t', rl_insert);
  //Parser p(oper_int, int_asm);

  do {
    if (line_read) {
      free(line_read);
      line_read = (char*)NULL;
    }

    line_read = readline("> ");

    if (line_read && *line_read) {
      add_history (line_read);
      line = std::string(line_read);
      l.renew(line);
      std::vector<Token> toks;
      try {
        l.lex(toks);
      } catch (LexError e) {
        std::cout << e.what() << std::endl;
      }

      p.renew(toks);

      try {
        p.parse();
      } catch (ParseError e) {
        std::cout << e.what() << std::endl;
      }

      for (Token& t : toks) {
        t.print();
      }
      //p.parse(line);
      //p.print_parse_tree();
      //p.emit_code();
      //p.print_code();
      //p.clear();
    }
  } while (true);

  return 0;
}
