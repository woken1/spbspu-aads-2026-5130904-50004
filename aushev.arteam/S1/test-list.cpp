#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.h"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(copy_and_move)
{
  aushev::List< int > lst1;
  lst1.push_front(4);
  lst1.push_front(5);
  lst1.push_front(6);

  aushev::List< int > lst2(lst1);
  BOOST_CHECK_EQUAL(lst2.front(), 6);

  aushev::List< int > lst3(std::move(lst1));
  BOOST_CHECK_EQUAL(lst3.front(), 6);
  BOOST_CHECK(lst1.empty());
}

BOOST_AUTO_TEST_CASE(empty_check)
{
  aushev::List< int > lst;
  BOOST_CHECK(lst.empty());
  lst.push_back(1);
  BOOST_CHECK(!lst.empty());
}

BOOST_AUTO_TEST_CASE(size_check)
{
  aushev::List< int > lst;
  BOOST_CHECK_EQUAL(lst.size(), 0);
  lst.push_back(1);
  lst.push_back(2);
  BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(front_access)
{
  aushev::List< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  BOOST_CHECK_EQUAL(lst.front(), 10);
  lst.front() = 100;
  BOOST_CHECK_EQUAL(lst.front(), 100);
}

BOOST_AUTO_TEST_CASE(back_access)
{
  aushev::List< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  BOOST_CHECK_EQUAL(lst.back(), 20);
  lst.back() = 200;
  BOOST_CHECK_EQUAL(lst.back(), 200);
}

BOOST_AUTO_TEST_CASE(push_front_check)
{
  aushev::List< int > lst;
  lst.push_front(5);
  lst.push_front(15);
  BOOST_CHECK_EQUAL(lst.front(), 15);
  BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(push_back_check)
{
  aushev::List< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  BOOST_CHECK_EQUAL(lst.back(), 20);
  BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(pop_front_check)
{
  aushev::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.front(), 2);
  BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_back_check)
{
  aushev::List< int > lst;
  lst.push_back(11); 
  lst.push_back(2);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.back(), 11);
  BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(clear_check)
{
  aushev::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.clear();
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 0);
}

BOOST_AUTO_TEST_CASE(bidirectional_traversal)
{
  aushev::List< int > lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);

  auto it = lst.end();
  --it;
  BOOST_CHECK_EQUAL(*it, 30);
  --it;
  BOOST_CHECK_EQUAL(*it, 20);
  --it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(no_memory_leaks)
{
  {
    aushev::List< int > lst;
    for (int i = 0; i < 100; ++i) {
      lst.push_back(i);
    }
    aushev::List< int > lst2;
    lst2 = lst;
    lst.clear();
  }
}

BOOST_AUTO_TEST_SUITE_END()
