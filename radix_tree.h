#include <memory>
#include <vector>
#include <iostream>

const int ALPHABET_SIZE = 26;

struct TrieNode
{
  TrieNode *childs[ALPHABET_SIZE];
  std::string label;
  bool is_end_of_word;
};

TrieNode * getNode(void)
{
  TrieNode *node = new TrieNode;

  node->is_end_of_word = false;

  for (int i = 0; i < ALPHABET_SIZE; i++)
    node->childs[i] = nullptr;

  return node;
}

struct TrieNode* createNewTrieNode(std::string label, bool is_end_of_word)
{
  struct TrieNode *new_node = getNode();
  new_node->label = label;
  new_node->is_end_of_word = is_end_of_word;

  return new_node;
}

uint32_t getLetterIndex(char letter)
{
  return std::tolower(letter) - 'a';
}

uint32_t countChilds(TrieNode *node)
{
  uint32_t childs_counter =  0;
  for (size_t i = 0; i < ALPHABET_SIZE; ++i)
  {
    if(node->childs[i])
      ++childs_counter;
  }

  return childs_counter;
}

void moveChildsToNode(TrieNode *from_node, TrieNode *to_node)
{
  for (size_t i = 0; i < ALPHABET_SIZE; ++i)
  {
    if (from_node->childs[i])
    {
      to_node->childs[i] = from_node->childs[i];
      from_node->childs[i] = nullptr;
    }
  }
}

void insert(TrieNode* current_node, std::string word, TrieNode *parent = nullptr)
{
  auto mismatch_pair = std::mismatch(word.begin(), word.end(), current_node->label.begin());

  auto word_iterator = mismatch_pair.first;
  auto label_iterator = mismatch_pair.second;
  std::string word_suffix(word_iterator, word.end());
  std::string label_suffix(label_iterator, current_node->label.end());
  std::string label_prefix(current_node->label.begin(), label_iterator);

  uint32_t word_letter_index = getLetterIndex(*word_iterator);
  uint32_t label_letter_index = getLetterIndex(*label_iterator);

  if (word_suffix == word)         //mismatch from start of word
  {
    if(parent) 
    {
      parent->childs[word_letter_index] = createNewTrieNode(word_suffix, true);
    }
    else 
    {
      if(current_node->label.empty())
      {
        if(countChilds(current_node) == 0)
        {
          current_node->label = word;
          current_node->is_end_of_word = true;
        }
        else
        {
          if (current_node->childs[word_letter_index])   //childs exists, inserting there
          {
            insert(current_node->childs[word_letter_index], word_suffix, current_node);
          }
          else
          {
            current_node->childs[word_letter_index] = createNewTrieNode(word_suffix, true);
          }
        }
      }
      else
      {
        TrieNode *new_node = createNewTrieNode(label_suffix, true);
        moveChildsToNode(current_node, new_node);
        current_node->childs[label_letter_index] = new_node;
        current_node->childs[word_letter_index] = createNewTrieNode(word_suffix, true);
        current_node->label.clear();
        current_node->is_end_of_word = false;
      }
    }
  }
  else if (word_suffix.empty())       //full match or word has less letters than label
  {
    if(label_suffix.empty()) return;
    current_node->label = word;
    current_node->is_end_of_word = true;
    if (current_node->childs[label_letter_index])   
    {
      insert(current_node->childs[label_letter_index], label_suffix, current_node);
    }
    else         //creating new child and moving current nodes there
    {
      TrieNode *new_node = createNewTrieNode(label_suffix, true);
      moveChildsToNode(current_node, new_node);
      current_node->childs[label_letter_index] = new_node;
      current_node->is_end_of_word = true;
    }
  }
  else    //word has more letters than label 
  {
    if (!label_suffix.empty())        //label suffix moves to child
    {
      TrieNode *new_node = createNewTrieNode(label_suffix, current_node->is_end_of_word);
      moveChildsToNode(current_node, new_node);
      current_node->childs[label_letter_index] = new_node;
      current_node->label = label_prefix;
      current_node->is_end_of_word = false;
    }
    if (current_node->childs[word_letter_index])     
    {
      insert(current_node->childs[word_letter_index], word_suffix, current_node);
    }
    else
    {
      current_node->childs[word_letter_index] = createNewTrieNode(word_suffix, true);      
    }      
  }  
}

void print_trie(TrieNode *current_node, std::string prefix = "")
{
  std::cout << current_node->label;
  if (current_node->is_end_of_word)
  {
    std::cout << "$";
  }
  std::cout << std::endl;

  for (size_t i = 0; i < 26; ++i)
  {     
    if (current_node->childs[i])
    {            
      std::cout << prefix;      
      std::cout << "+ ";
      if(countChilds(current_node) > 1)
      {
        prefix += "| ";
      }
      else 
      {
        prefix += "  ";
      }

      print_trie(current_node->childs[i], prefix);
      prefix.erase(prefix.end() - 2, prefix.end());
    }
  }
}

void print_minimum_prefixes(TrieNode *current_node, std::string prefix = "")
{
  if (countChilds(current_node) == 0)
  {
    std::cout << prefix << current_node->label << " " << prefix << current_node->label.front() << std::endl;
    return;
  }

  for (size_t i = 0; i < ALPHABET_SIZE; ++i)
  {
    if (current_node->childs[i])
    {
      print_minimum_prefixes(current_node->childs[i], prefix + current_node->label);
      if (current_node->is_end_of_word)
      {
        std::cout << prefix << current_node->label << " " << prefix << current_node->label.front() << std::endl;        
      }
    }
  }
}
