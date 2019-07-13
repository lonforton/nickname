#include <iostream>
#include <memory>

#include "radix_tree.h"

int main() 
{  
  auto root = std::make_unique<TrieNode>();

  std::string input_line;
  while (std::getline(std::cin, input_line))
  {
    insert(root.get(), input_line);
    print_trie(root.get());
  }

  print_trie(root.get());

  print_minimum_prefixes(root.get());
  
  return 0; 
} 

