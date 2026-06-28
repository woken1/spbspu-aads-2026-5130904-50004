#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.h"

BOOST_AUTO_TEST_CASE(push_front_basic)
{
  aushev::List<int> lst;
  lst.push_front(5);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_front(15);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 15);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(push_back_basic)
{
  aushev::List<int> lst;
  lst.push_back(100);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  lst.push_back(200);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 100);
  ++it;
  BOOST_CHECK_EQUAL(*it, 200);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  aushev::List<int> lst;
  lst.push_back(7);
  lst.push_back(14);
  lst.push_back(21);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 14);
  ++it;
  BOOST_CHECK_EQUAL(*it, 21);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.size(), 1);
  BOOST_CHECK_EQUAL(*lst.begin(), 21);
  lst.pop_front();
  BOOST_CHECK(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  aushev::List<int> lst;
  lst.push_back(5);
  lst.push_back(10);
  lst.push_back(15);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 1);
  BOOST_CHECK_EQUAL(*lst.begin(), 5);
  lst.pop_back();
  BOOST_CHECK(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_operation)
{
  aushev::List<int> lst;
  lst.push_back(3);
  lst.push_back(6);
  lst.push_back(9);
  lst.clear();
  BOOST_CHECK(lst.size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(empty_check)
{
  aushev::List<int> lst;
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 0);
  lst.push_back(42);
  BOOST_CHECK(!lst.empty());
}

BOOST_AUTO_TEST_CASE(front_back_values)
{
  aushev::List<int> lst;
  lst.push_back(11);
  lst.push_back(22);
  lst.push_back(33);
  BOOST_CHECK_EQUAL(lst.front(), 11);
  BOOST_CHECK_EQUAL(lst.back(), 33);
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  aushev::List<int> lst1;
  lst1.push_back(8);
  lst1.push_back(16);
  aushev::List<int> lst2(lst1);
  BOOST_CHECK_EQUAL(lst2.front(), 8);
  BOOST_CHECK_EQUAL(lst2.back(), 16);
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  aushev::List<int> lst1;
  lst1.push_back(8);
  lst1.push_back(16);
  aushev::List<int> lst2(std::move(lst1));
  BOOST_CHECK(lst1.empty());
  BOOST_CHECK_EQUAL(lst2.front(), 8);
}

BOOST_AUTO_TEST_CASE(iterator_traversal)
{
  aushev::List<int> lst;
  lst.push_back(2);
  lst.push_back(4);
  lst.push_back(6);
  auto it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK(it == lst.end());
}

BOOST_AUTO_TEST_CASE(const_iterator)
{
  aushev::List<int> lst;
  lst.push_back(100);
  lst.push_back(200);
  const aushev::List<int>& clst = lst;
  auto it = clst.cbegin();
  BOOST_CHECK_EQUAL(*it, 100);
  ++it;
  BOOST_CHECK_EQUAL(*it, 200);
  ++it;
  BOOST_CHECK(it == clst.cend());
}

BOOST_AUTO_TEST_CASE(bidirectional_iterator)
{
  aushev::List<int> lst;
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

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  aushev::List<int> lst1;
  lst1.push_back(1);
  lst1.push_back(2);
  aushev::List<int> lst2;
  lst2 = lst1;
  BOOST_CHECK_EQUAL(lst2.size(), 2);
  BOOST_CHECK_EQUAL(lst2.front(), 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  aushev::List<int> lst1;
  lst1.push_back(1);
  lst1.push_back(2);
  aushev::List<int> lst2;
  lst2 = std::move(lst1);
  BOOST_CHECK(lst1.empty());
  BOOST_CHECK_EQUAL(lst2.size(), 2);
}

BOOST_AUTO_TEST_CASE(empty_list_operations)
{
  aushev::List<int> lst;
  BOOST_CHECK(lst.empty());
  lst.pop_front();
  BOOST_CHECK(lst.empty());
  lst.pop_back();
  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(single_element_list)
{
  aushev::List<int> lst;
  lst.push_back(42);
  BOOST_CHECK_EQUAL(lst.size(), 1);
  BOOST_CHECK_EQUAL(lst.front(), 42);
  BOOST_CHECK_EQUAL(lst.back(), 42);
  lst.pop_front();
  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(mixed_push_operations)
{
  aushev::List<int> lst;
  lst.push_back(1);
  lst.push_front(0);
  lst.push_back(2);
  lst.push_front(-1);
  BOOST_CHECK_EQUAL(lst.size(), 4);
  BOOST_CHECK_EQUAL(lst.front(), -1);
  BOOST_CHECK_EQUAL(lst.back(), 2);
}

BOOST_AUTO_TEST_CASE(self_assignment)
{
  aushev::List<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst = lst;
  BOOST_CHECK_EQUAL(lst.size(), 3);
  BOOST_CHECK_EQUAL(lst.front(), 1);
  BOOST_CHECK_EQUAL(lst.back(), 3);
}

BOOST_AUTO_TEST_CASE(iterator_boundaries)
{
  aushev::List<int> lst;
  lst.push_back(42);
  auto it = lst.begin();
  BOOST_CHECK(it != lst.end());
  BOOST_CHECK_EQUAL(*it, 42);
  ++it;
  BOOST_CHECK(it == lst.end());
  --it;
  BOOST_CHECK(it == lst.begin());
  BOOST_CHECK_EQUAL(*it, 42);
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  aushev::List<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  auto it1 = lst.begin();
  auto it2 = lst.begin();
  auto it3 = lst.begin();
  ++it3;
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(it3 == ++lst.begin());
}

BOOST_AUTO_TEST_CASE(empty_list_iteration)
{
  aushev::List<int> lst;
  BOOST_CHECK(lst.begin() == lst.end());
  BOOST_CHECK(lst.cbegin() == lst.cend());
}

BOOST_AUTO_TEST_CASE(large_list_operations)
{
  aushev::List<int> lst;
  const int size = 1000;
  for (int i = 0; i < size; ++i) {
    lst.push_back(i);
  }
  BOOST_CHECK_EQUAL(lst.size(), size);
  BOOST_CHECK_EQUAL(lst.front(), 0);
  BOOST_CHECK_EQUAL(lst.back(), size - 1);
  int expected = 0;
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected++);
  }
}

BOOST_AUTO_TEST_CASE(mixed_push_pop_operations)
{
  aushev::List<int> lst;
  lst.push_back(1);
  lst.push_front(0);
  lst.push_back(2);
  lst.push_back(3);
  lst.pop_front();
  lst.push_front(-1);
  lst.pop_back();
  BOOST_CHECK_EQUAL(lst.size(), 3);
  BOOST_CHECK_EQUAL(lst.front(), -1);
  BOOST_CHECK_EQUAL(lst.back(), 2);
}

BOOST_AUTO_TEST_CASE(const_list_access)
{
  aushev::List<int> lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);
  const aushev::List<int>& clst = lst;
  BOOST_CHECK_EQUAL(clst.front(), 10);
  BOOST_CHECK_EQUAL(clst.back(), 30);
  BOOST_CHECK_EQUAL(clst.size(), 3);
}

BOOST_AUTO_TEST_CASE(move_semantics_preserve_state)
{
  aushev::List<int> lst1;
  lst1.push_back(1);
  lst1.push_back(2);
  aushev::List<int> lst2(std::move(lst1));
  BOOST_CHECK(lst1.empty());
  BOOST_CHECK_EQUAL(lst1.size(), 0);
  BOOST_CHECK_EQUAL(lst2.size(), 2);
  BOOST_CHECK_EQUAL(lst2.front(), 1);
  BOOST_CHECK_EQUAL(lst2.back(), 2);
  aushev::List<int> lst3;
  lst3.push_back(3);
  lst3 = std::move(lst2);
  BOOST_CHECK(lst2.empty());
  BOOST_CHECK_EQUAL(lst3.size(), 2);
  BOOST_CHECK_EQUAL(lst3.front(), 1);
}

BOOST_AUTO_TEST_CASE(bidirectional_full_traversal)
{
  aushev::List<int> lst;
  for (int i = 1; i <= 5; ++i) {
    lst.push_back(i);
  }
  auto it = lst.end();
  int expected = 5;
  for (int i = 0; i < 5; ++i) {
    --it;
    BOOST_CHECK_EQUAL(*it, expected);
    --expected;
  }
}

BOOST_AUTO_TEST_CASE(clear_and_reuse)
{
  aushev::List<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.clear();
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 0);
  lst.push_back(3);
  lst.push_back(4);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  BOOST_CHECK_EQUAL(lst.front(), 3);
  BOOST_CHECK_EQUAL(lst.back(), 4);
}

BOOST_AUTO_TEST_CASE(copy_preserves_order)
{
  aushev::List<int> lst1;
  for (int i = 0; i < 10; ++i) {
    lst1.push_back(i);
  }
  aushev::List<int> lst2(lst1);
  BOOST_CHECK_EQUAL(lst1.size(), lst2.size());
  auto it1 = lst1.begin();
  auto it2 = lst2.begin();
  while (it1 != lst1.end() && it2 != lst2.end()) {
    BOOST_CHECK_EQUAL(*it1, *it2);
    ++it1;
    ++it2;
  }
}

BOOST_AUTO_TEST_CASE(assignment_chain)
{
  aushev::List<int> lst1;
  aushev::List<int> lst2;
  aushev::List<int> lst3;
  lst1.push_back(1);
  lst2.push_back(2);
  lst3.push_back(3);
  lst1 = lst2;
  lst2 = lst3;
  BOOST_CHECK_EQUAL(lst1.size(), 1);
  BOOST_CHECK_EQUAL(lst2.size(), 1);
  BOOST_CHECK_EQUAL(lst3.size(), 1);
  BOOST_CHECK_EQUAL(lst1.front(), 2);
  BOOST_CHECK_EQUAL(lst2.front(), 3);
  BOOST_CHECK_EQUAL(lst3.front(), 3);
}

BOOST_AUTO_TEST_CASE(no_memory_leaks)
{
  {
    aushev::List<int> lst;
    for (int i = 0; i < 100; ++i) {
      lst.push_back(i);
    }
    aushev::List<int> lst2;
    lst2 = lst;
    lst.clear();
  }
}

BOOST_AUTO_TEST_CASE(sequence_transposition)
{
  struct Sequence {
    std::string name;
    aushev::List<int> numbers;
  };
  aushev::List<Sequence> sequences;
  Sequence s1;
  s1.name = "first";
  s1.numbers.push_back(1);
  s1.numbers.push_back(1);
  s1.numbers.push_back(1);
  sequences.push_back(s1);
  Sequence s2;
  s2.name = "second";
  s2.numbers.push_back(2);
  s2.numbers.push_back(2);
  s2.numbers.push_back(2);
  s2.numbers.push_back(2);
  sequences.push_back(s2);
  Sequence s3;
  s3.name = "third";
  sequences.push_back(s3);
  Sequence s4;
  s4.name = "fourth";
  s4.numbers.push_back(4);
  s4.numbers.push_back(4);
  sequences.push_back(s4);
  BOOST_CHECK_EQUAL(sequences.size(), 4);
  auto it = sequences.begin();
  BOOST_CHECK_EQUAL((*it).name, "first");
  ++it;
  BOOST_CHECK_EQUAL((*it).name, "second");
  ++it;
  BOOST_CHECK_EQUAL((*it).name, "third");
  ++it;
  BOOST_CHECK_EQUAL((*it).name, "fourth");
}

BOOST_AUTO_TEST_CASE(const_iterator_traversal)
{
  aushev::List<int> lst;
  for (int i = 0; i < 5; ++i) {
    lst.push_back(i * 10);
  }
  const aushev::List<int>& clst = lst;
  int expected = 0;
  for (auto it = clst.cbegin(); it != clst.cend(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected);
    expected += 10;
  }
}

BOOST_AUTO_TEST_CASE(pop_on_empty_list)
{
  aushev::List<int> lst;
  lst.pop_front();
  BOOST_CHECK(lst.empty());
  lst.pop_back();
  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(front_back_after_operations)
{
  aushev::List<int> lst;
  lst.push_back(100);
  BOOST_CHECK_EQUAL(lst.front(), 100);
  BOOST_CHECK_EQUAL(lst.back(), 100);
  lst.push_front(50);
  BOOST_CHECK_EQUAL(lst.front(), 50);
  BOOST_CHECK_EQUAL(lst.back(), 100);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.front(), 100);
  BOOST_CHECK_EQUAL(lst.back(), 100);
  lst.push_back(150);
  BOOST_CHECK_EQUAL(lst.front(), 100);
  BOOST_CHECK_EQUAL(lst.back(), 150);
}
