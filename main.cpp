#include <iostream>
#include <memory>

#include "trie.h"

int main()
{
  Trie trie;

  std::string input_line;
  while (std::getline(std::cin, input_line))
  {
    trie.insert(input_line);
    trie.print();
  }

  trie.print();
  trie.print_minimum_prefixes();

  return 0;
}
