#define BOOST_TEST_MODULE nickname_test_module

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

#include "trie.h"

struct TrieFixtureTest
{
  TrieFixtureTest() 
  {
    
  }  

  Trie trie;
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
    trie.insert("aleksey");
    trie.print();  
  }
  BOOST_CHECK(output.is_equal("aleksey$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_2, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.print();  
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
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.insert("aleks");
    trie.print();  
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
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.insert("aleks");
    trie.insert("alek");
    trie.print();  
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
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.insert("aleks");
    trie.insert("alek");
    trie.insert("alesha");
    trie.print();  
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
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.insert("aleks");
    trie.insert("alek");
    trie.insert("alesha");
    trie.insert("maksim");
    trie.print();   
  }
BOOST_CHECK(output.is_equal("\n\
+ ale\n\
| + k$\n\
| | + s$\n\
| |   + ey$\n\
| + sha$\n\
+ sasha$\n\
+ maksim$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_input_test_repeat, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    trie.insert("aleks");
    trie.insert("aleksey");
    trie.insert("aleks");
    trie.print();     
  }
BOOST_CHECK(output.is_equal("aleks$\n\
+ ey$\n"));
}

BOOST_FIXTURE_TEST_CASE(nickname_min_prefix_test_1, TrieFixtureTest)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());
    trie.insert("aleksey");
    trie.insert("sasha");
    trie.insert("aleks");
    trie.insert("alek");
    trie.insert("alesha");
    trie.insert("maksim");
    trie.print_minimum_prefixes();  
  }
  BOOST_CHECK(output.is_equal("aleksey alekse\n\
aleks aleks\n\
alek alek\n\
alesha ales\n\
sasha s\n\
maksim m\n"));
}

BOOST_AUTO_TEST_SUITE_END()
