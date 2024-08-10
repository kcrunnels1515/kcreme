#include <iostream>
#include <vector>
#include <string.h>
//#include "MemoryManager/MemoryManager.hpp"
//#include "ValueInterface/ValueInterface.hpp"
#include "Parser/Parser.hpp"
#include <readline/readline.h>
#include <readline/history.h>

extern "C" {
  void* copy_res(void* data, size_t len) {
    void* ptr = malloc(len);
    memcpy(ptr, data, len);
    return ptr;
  }
}

int main(int argc, char *argv[]) {
  static char *line_read = (char*)NULL;
  std::map<std::string,unsigned> oper_names = {{"+", 0}, {"-", 1}};
  std::string line = "";
  rl_bind_key ('\t', rl_insert);
  Parser p(oper_names);

  do {
    if (line_read) {
      free(line_read);
      line_read = (char*)NULL;
    }

    line_read = readline("> ");

    if (line_read && *line_read) {
      add_history (line_read);
      line = std::string(line_read);
      p.parse(line);
      p.print_parse_tree();
      p.clear();
    }
  } while (true);

  return 0;
}
