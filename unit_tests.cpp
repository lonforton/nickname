#define BOOST_TEST_MODULE nickname_test_module

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

#include "radix_tree.h"

struct TrieFixtureTest
{
  TrieFixtureTest() 
  {
    root = std::make_unique<TrieNode>();
  }

  std::unique_ptr<TrieNode> root;
};

struct cout_redirect
{
  cout_redirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer))
  {
  }

  ~cout_redirect()
  {
    std::cout.rdbuf(old);
  }

private:
  std::streambuf *old;
};

BOOST_AUTO_TEST_SUITE(nickname_test_suite)

BOOST_FIXTURE_TEST_CASE(nickname_input_test_1, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    print_trie(root.get());    
  }
  BOOST_CHECK(output.is_equal("aleksey$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_2, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    print_trie(root.get());    
  }
BOOST_CHECK(output.is_equal("\n\
+ aleksey$\n\
+ sasha$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_3, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    insert(root.get(), std::string("aleks"));
    print_trie(root.get());
  }
BOOST_CHECK(output.is_equal("\n\
+ aleks$\n\
| + ey$\n\
+ sasha$\n"));
  
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_4, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    insert(root.get(), std::string("aleks"));
    insert(root.get(), std::string("alek"));
    print_trie(root.get());    
  }
BOOST_CHECK(output.is_equal("\n\
+ alek$\n\
| + s$\n\
|   + ey$\n\
+ sasha$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_5, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    insert(root.get(), std::string("aleks"));
    insert(root.get(), std::string("alek"));
    insert(root.get(), std::string("alesha"));
    print_trie(root.get());    
  }
BOOST_CHECK(output.is_equal("\n\
+ ale\n\
| + k$\n\
| | + s$\n\
| |   + ey$\n\
| + sha$\n\
+ sasha$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_6, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    insert(root.get(), std::string("aleks"));
    insert(root.get(), std::string("alek"));
    insert(root.get(), std::string("alesha"));
    insert(root.get(), std::string("maksim"));
    print_trie(root.get());    
  }
BOOST_CHECK(output.is_equal("\n\
+ ale\n\
| + k$\n\
| | + s$\n\
| |   + ey$\n\
| + sha$\n\
+ maksim$\n\
+ sasha$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_repeat, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleks"));
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("aleks"));
    print_trie(root.get());    
  }
BOOST_CHECK(output.is_equal("aleks$\n\
+ ey$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_min_prefix_test_1, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    insert(root.get(), std::string("aleksey"));
    insert(root.get(), std::string("sasha"));
    insert(root.get(), std::string("aleks"));
    insert(root.get(), std::string("alek"));
    insert(root.get(), std::string("alesha"));
    insert(root.get(), std::string("maksim"));
    print_minimum_prefixes(root.get());    
  }
  BOOST_CHECK(output.is_equal("aleksey alekse\n\
aleks aleks\n\
alek alek\n\
alesha ales\n\
maksim m\n\
sasha s\n"));
}

BOOST_AUTO_TEST_SUITE_END()
