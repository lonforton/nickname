#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

struct TrieNode
{
  std::string label;
  bool is_end_of_word;
  std::vector<std::unique_ptr<TrieNode>> childs;
};

class Trie 
{
  public:
    Trie();
    void insert(std::string word);        
    void print();
    void print_minimum_prefixes();

  private:
    std::unique_ptr<TrieNode> create_new_trie_node(const std::string& word);
    void insert(const std::string &word, std::unique_ptr<TrieNode>& cur_node);
    void print(std::unique_ptr<TrieNode>& node, std::string prefix = "", bool add_line = false);
    void print_minimum_prefixes(std::unique_ptr<TrieNode> &node, const std::string &prefix = "");    
    std::unique_ptr<TrieNode>& get_child_by_string(std::unique_ptr<TrieNode>& node, const std::string& word);
    bool has_childs(std::unique_ptr<TrieNode>& node);
    uint32_t count_childs(std::unique_ptr<TrieNode>& node);
    void move_childs(std::unique_ptr<TrieNode>& from_node, std::unique_ptr<TrieNode>& to_node);

    std::unique_ptr<TrieNode> root;
};

Trie::Trie()
{  
  root = std::make_unique<TrieNode>();
}

std::unique_ptr<TrieNode> Trie::create_new_trie_node(const std::string& word)
{
  std::unique_ptr<TrieNode> new_node = std::make_unique<TrieNode>();
  new_node->label = word;
  new_node->is_end_of_word = true;
  return new_node;
}

std::unique_ptr<TrieNode>& Trie::get_child_by_string(std::unique_ptr<TrieNode>& node, const std::string& word)
{
  for(auto &item : node->childs)
  {
    if(item->label[0] == word[0])
      return item;
  } 

  return node;
}

bool Trie::has_childs(std::unique_ptr<TrieNode>& node)
{
  return node->childs.size() != 0;
}

uint32_t Trie::count_childs(std::unique_ptr<TrieNode>& node)
{
  return node->childs.size();
}

void Trie::insert(std::string word)
{
  if(root->label.empty() && !has_childs(root)) 
  {    
    root->label = word;
    root->is_end_of_word = true;
  }
  else
  {
    insert(word, root);
  }
}

void Trie::insert(const std::string &word, std::unique_ptr<TrieNode> &cur_node)
{
  auto mismatch_pair = std::mismatch(word.begin(), word.end(), cur_node->label.begin(), cur_node->label.end());
  auto word_iterator = mismatch_pair.first;
  auto label_iterator = mismatch_pair.second;

  std::string common_prefix(word.begin(), word_iterator);
  std::string word_suffix(word_iterator, word.end());
  std::string label_suffix(label_iterator, cur_node->label.end());
  std::string current_label = cur_node->label;

  std::vector<std::string> suffixes_vector {label_suffix, word_suffix};

  if (common_prefix != cur_node->label && common_prefix != word)
  {
    cur_node->is_end_of_word = false;
  }

  cur_node->label = common_prefix;

  for(const auto &suffix : suffixes_vector)
  {
    if (!suffix.empty())
    {
      std::unique_ptr<TrieNode> &child = get_child_by_string(cur_node, suffix);
      if (child == cur_node)      //no childs with such start of suffix
      {
        std::unique_ptr<TrieNode> new_node = create_new_trie_node(suffix);

        if (suffix == label_suffix && current_label.compare(0, common_prefix.length(), common_prefix) == 0)
        {
          cur_node->childs.swap(new_node->childs);
        }

        cur_node->childs.push_back(std::move(new_node));
      }
      else
      {
        insert(suffix, child);
      }
    }
  }
}

void Trie::print()
{
  std::cout << root->label << (root->is_end_of_word ? "$" : "") << std::endl;
  print(root);
}

void Trie::print(std::unique_ptr<TrieNode> &node, std::string prefix, bool add_line)
{
  if(node != root) 
  {      
    std::cout << prefix <<  "+ " << node->label << (node->is_end_of_word ? "$" : "") << std::endl;    
    prefix += add_line ? "| " : "  ";
  }

  for (auto &item : node->childs)
  {      
    print(item, prefix, count_childs(node) > 1 && item != node->childs.back());
  }
}

void Trie::print_minimum_prefixes()
{
  print_minimum_prefixes(root);
}

void Trie::print_minimum_prefixes(std::unique_ptr<TrieNode> &node, const std::string &prefix)
{
  if(count_childs(node) == 0)
  {
    std::cout << prefix << node->label << " " << prefix << node->label.front() << std::endl;
    return;
  }

  for(auto &item : node->childs)
  {
    print_minimum_prefixes(item, prefix + node->label);
  }

  if(node->is_end_of_word)
  {
    std::cout << prefix << node->label << " " << prefix << node->label << std::endl;
  }
}