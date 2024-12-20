#include <iostream>
#include "Node.hpp"
extern Node* root;
extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();
    std::cout << root << std::endl;
    return 0;
}
