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
    void insert(std::string word, std::unique_ptr<TrieNode>& cur_node);
    void print(std::unique_ptr<TrieNode>& node, std::string prefix = "", bool add_line = false);
    void print_minimum_prefixes(std::unique_ptr<TrieNode> &node, std::string prefix = "");    
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
  new_node.get()->label = word;
  new_node.get()->is_end_of_word = true;
  return std::move(new_node);
}

std::unique_ptr<TrieNode>& Trie::get_child_by_string(std::unique_ptr<TrieNode>& node, const std::string& word)
{
  for(auto &item : node.get()->childs)
  {
    if(item.get()->label.rfind(word.front(), 0) == 0)
    {
      return item;
    }
  } 

  return node;
}

bool Trie::has_childs(std::unique_ptr<TrieNode>& node)
{
  return node.get()->childs.size() != 0;
}

uint32_t Trie::count_childs(std::unique_ptr<TrieNode>& node)
{
  return node.get()->childs.size();
}

void Trie::move_childs(std::unique_ptr<TrieNode>& from_node, std::unique_ptr<TrieNode>& to_node)
{
  for(auto &item : from_node.get()->childs) 
  {
    to_node.get()->childs.push_back(std::move(item));    
  }
  from_node.get()->childs.clear();
}

void Trie::insert(std::string word)
{
  if(root.get()->label.empty() && !has_childs(root)) 
  {    
    root.get()->label = word;
    root.get()->is_end_of_word = true;
  }
  else
  {
    insert(word, root);
  }
}

void Trie::insert(std::string word, std::unique_ptr<TrieNode> &cur_node)
{
  auto mismatch_pair = std::mismatch(word.begin(), word.end(), cur_node.get()->label.begin());
  auto word_iterator = mismatch_pair.first;
  auto label_iterator = mismatch_pair.second;

  std::string common_prefix(word.begin(), word_iterator);
  std::string word_suffix(word_iterator, word.end());
  std::string label_suffix(label_iterator, cur_node.get()->label.end());
  std::string current_label = cur_node.get()->label;

  std::vector<std::string> suffixes_vector {label_suffix, word_suffix};

  if (!current_label.empty() && (common_prefix == word))
  {
    cur_node.get()->is_end_of_word = true;
  }
  else
  {
    cur_node.get()->is_end_of_word = false;
  }
  cur_node.get()->label = common_prefix;

  for(const auto& suffix : suffixes_vector)
  {
    if (!suffix.empty())
    {
      std::unique_ptr<TrieNode> &child = get_child_by_string(cur_node, suffix);
      if (child == cur_node)      //no childs with such word
      {
        std::unique_ptr<TrieNode> new_node = create_new_trie_node(suffix);

        if (suffix == label_suffix && current_label.compare(0, common_prefix.length(), common_prefix) == 0)
        {
          move_childs(cur_node, new_node);
        }

        cur_node.get()->childs.push_back(std::move(new_node));
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
  std::cout << root.get()->label << (root.get()->is_end_of_word ? "$" : "") << std::endl;
  print(root);
}

void Trie::print(std::unique_ptr<TrieNode> &node, std::string prefix, bool add_line)
{
  if(node != root) 
  {      
    std::cout << prefix <<  "+ " << node.get()->label << (node.get()->is_end_of_word ? "$" : "") << std::endl;    
    prefix += add_line ? "| " : "  ";
  }

  for (auto &item : node.get()->childs)
  {      
    print(item, prefix, count_childs(node) > 1 && item != node.get()->childs.back());
  }
}

void Trie::print_minimum_prefixes()
{
  print_minimum_prefixes(root);
}

void Trie::print_minimum_prefixes(std::unique_ptr<TrieNode> &node, std::string prefix)
{
  if(count_childs(node) == 0)
  {
    std::cout << prefix << node.get()->label << " " << prefix << node.get()->label.front() << std::endl;
    return;
  }

  for(auto &item : node.get()->childs)
  {
    print_minimum_prefixes(item, prefix + node.get()->label);
  }

  if(node.get()->is_end_of_word)
  {
    std::cout << prefix << node.get()->label << " " << prefix << node.get()->label << std::endl;
  }
}