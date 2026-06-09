#define BOOST_TEST_MODULE HashTableTests
#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "hash_table.hpp"

using namespace aushev;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
    HashTable<std::string, int> ht;
    BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
    HashTable<std::string, int> ht(16);
    ht.add("a", 5);
    BOOST_CHECK_EQUAL(ht.size(), 1);

    ht.add("b", 7);
    BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
    HashTable<std::string, int> ht(16);
    ht.add("c", 10);
    ht.add("d", 20);

    BOOST_CHECK(ht.has("c"));
    BOOST_CHECK(ht.has("d"));
    BOOST_CHECK(!ht.has("e"));
}

BOOST_AUTO_TEST_CASE(test_get_and_getRef)
{
    HashTable<std::string, int> ht(16);
    ht.add("f", 15);
    ht.add("g", 25);

    BOOST_CHECK_EQUAL(ht.get("f"), 15);
    BOOST_CHECK_EQUAL(ht.get("g"), 25);

    ht.getRef("f") = 99;
    BOOST_CHECK_EQUAL(ht.get("f"), 99);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
    HashTable<std::string, int> ht(16);
    ht.add("first", 111);
    ht.add("second", 222);

    int val = ht.drop("first");
    BOOST_CHECK_EQUAL(val, 111);
    BOOST_CHECK_EQUAL(ht.size(), 1);
    BOOST_CHECK(!ht.has("first"));
    BOOST_CHECK(ht.has("second"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
    HashTable<std::string, int> ht(16);
    ht.add("x", 33);
    ht.add("x", 44);

    BOOST_CHECK_EQUAL(ht.size(), 1);
    BOOST_CHECK_EQUAL(ht.get("x"), 44);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
    HashTable<std::string, int> ht(16);
    ht.add("p", 1);
    ht.add("q", 2);
    ht.add("r", 3);

    ht.clear();
    BOOST_CHECK_EQUAL(ht.size(), 0);
    BOOST_CHECK(!ht.has("p"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
    HashTable<std::string, int> ht1(16);
    ht1.add("m", 7);
    ht1.add("n", 14);

    HashTable<std::string, int> ht2 = ht1;

    BOOST_CHECK_EQUAL(ht2.size(), 2);
    BOOST_CHECK(ht2.has("m"));
    BOOST_CHECK(ht2.has("n"));
    BOOST_CHECK_EQUAL(ht2.get("m"), 7);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
    HashTable<std::string, int> ht1(16);
    ht1.add("first", 50);
    ht1.add("second", 60);

    HashTable<std::string, int> ht2(16);
    ht2.add("third", 70);
    
    ht2 = ht1;

    BOOST_CHECK_EQUAL(ht2.size(), 2);
    BOOST_CHECK(ht2.has("first"));
    BOOST_CHECK(ht2.has("second"));
    BOOST_CHECK(!ht2.has("third"));
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
    HashTable<std::string, int> ht(11);
    ht.add("q", 1);
    ht.add("w", 2);
    ht.add("e", 3);
    ht.add("r", 4);
    ht.add("t", 5);
    ht.add("y", 6);

    ht.rehash(37);
    BOOST_CHECK_EQUAL(ht.size(), 6);
    BOOST_CHECK(ht.has("q"));
    BOOST_CHECK(ht.has("w"));
    BOOST_CHECK(ht.has("e"));
    BOOST_CHECK(ht.has("r"));
    BOOST_CHECK(ht.has("t"));
    BOOST_CHECK(ht.has("y"));
}

BOOST_AUTO_TEST_CASE(test_drop_missing_throws)
{
    HashTable<std::string, int> ht(16);
    ht.add("a", 1);
    
    BOOST_CHECK_THROW(ht.drop("missing_key"), std::out_of_range);
    BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_get_missing_throws)
{
    HashTable<std::string, int> ht(16);
    ht.add("a", 1);
    
    BOOST_CHECK_THROW(ht.get("missing_key"), std::out_of_range);
    BOOST_CHECK_THROW(ht.getRef("missing_key"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
    HashTable<std::string, int> ht(16);
    ht.add("b", 20);
    ht.add("a", 10);
    ht.add("c", 30);

    size_t count = 0;
    bool foundA = false;
    bool foundB = false;
    bool foundC = false;

    for (auto it = ht.begin(); it != ht.end(); ++it) {
        ++count;
        if ((*it).first == "a" && (*it).second == 10) foundA = true;
        if ((*it).first == "b" && (*it).second == 20) foundB = true;
        if ((*it).first == "c" && (*it).second == 30) foundC = true;
    }

    BOOST_CHECK_EQUAL(count, 3);
    BOOST_CHECK(foundA);
    BOOST_CHECK(foundB);
    BOOST_CHECK(foundC);
}

BOOST_AUTO_TEST_CASE(test_const_iterators)
{
    HashTable<std::string, int> ht(16);
    ht.add("key", 42);
    
    const HashTable<std::string, int>& constHt = ht;
    
    size_t count = 0;
    for (auto it = constHt.begin(); it != constHt.end(); ++it) {
        ++count;
        BOOST_CHECK_EQUAL((*it).first, "key");
        BOOST_CHECK_EQUAL((*it).second, 42);
    }
    BOOST_CHECK_EQUAL(count, 1);
}

BOOST_AUTO_TEST_SUITE_END()
