#define BOOST_TEST_MODULE StackQueueTests
#include <boost/test/included/unit_test.hpp>
#include "stack.h"
#include "queue.h"

BOOST_AUTO_TEST_CASE(stack_push_pop)
{
  aushev::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  BOOST_CHECK_EQUAL(stack.drop(), 3);
  BOOST_CHECK_EQUAL(stack.top(), 2);
  BOOST_CHECK_EQUAL(stack.drop(), 2);
  BOOST_CHECK_EQUAL(stack.drop(), 1);
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(stack_empty_operations)
{
  aushev::Stack<int> stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(stack_copy_construction)
{
  aushev::Stack<int> stack1;
  stack1.push(10);
  stack1.push(20);
  aushev::Stack<int> stack2(stack1);
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack2.top(), 20);
}

BOOST_AUTO_TEST_CASE(stack_move_construction)
{
  aushev::Stack<int> stack1;
  stack1.push(10);
  stack1.push(20);
  aushev::Stack<int> stack2(std::move(stack1));
  BOOST_CHECK(stack1.empty());
  BOOST_CHECK_EQUAL(stack2.size(), 2);
}

BOOST_AUTO_TEST_CASE(stack_copy_assignment)
{
  aushev::Stack<int> stack1;
  stack1.push(5);
  aushev::Stack<int> stack2;
  stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.size(), 1);
  BOOST_CHECK_EQUAL(stack2.top(), 5);
}

BOOST_AUTO_TEST_CASE(stack_move_assignment)
{
  aushev::Stack<int> stack1;
  stack1.push(5);
  aushev::Stack<int> stack2;
  stack2 = std::move(stack1);
  BOOST_CHECK(stack1.empty());
  BOOST_CHECK_EQUAL(stack2.size(), 1);
}

BOOST_AUTO_TEST_CASE(stack_clear)
{
  aushev::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.clear();
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(queue_push_drop)
{
  aushev::Queue<int> queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.front(), 1);
  BOOST_CHECK_EQUAL(queue.drop(), 1);
  BOOST_CHECK_EQUAL(queue.front(), 2);
  BOOST_CHECK_EQUAL(queue.drop(), 2);
  BOOST_CHECK_EQUAL(queue.drop(), 3);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(queue_empty_operations)
{
  aushev::Queue<int> queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(queue_copy_construction)
{
  aushev::Queue<int> queue1;
  queue1.push(10);
  queue1.push(20);
  aushev::Queue<int> queue2(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.front(), 10);
}

BOOST_AUTO_TEST_CASE(queue_move_construction)
{
  aushev::Queue<int> queue1;
  queue1.push(10);
  queue1.push(20);
  aushev::Queue<int> queue2(std::move(queue1));
  BOOST_CHECK(queue1.empty());
  BOOST_CHECK_EQUAL(queue2.size(), 2);
}

BOOST_AUTO_TEST_CASE(queue_copy_assignment)
{
  aushev::Queue<int> queue1;
  queue1.push(5);
  aushev::Queue<int> queue2;
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.size(), 1);
  BOOST_CHECK_EQUAL(queue2.front(), 5);
}

BOOST_AUTO_TEST_CASE(queue_move_assignment)
{
  aushev::Queue<int> queue1;
  queue1.push(5);
  aushev::Queue<int> queue2;
  queue2 = std::move(queue1);
  BOOST_CHECK(queue1.empty());
  BOOST_CHECK_EQUAL(queue2.size(), 1);
}

BOOST_AUTO_TEST_CASE(queue_clear)
{
  aushev::Queue<int> queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  queue.clear();
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(stack_lifo_order)
{
  aushev::Stack<int> stack;
  for (int i = 1; i <= 5; ++i) {
    stack.push(i);
  }
  for (int i = 5; i >= 1; --i) {
    BOOST_CHECK_EQUAL(stack.drop(), i);
  }
}

BOOST_AUTO_TEST_CASE(queue_fifo_order)
{
  aushev::Queue<int> queue;
  for (int i = 1; i <= 5; ++i) {
    queue.push(i);
  }
  for (int i = 1; i <= 5; ++i) {
    BOOST_CHECK_EQUAL(queue.drop(), i);
  }
}

BOOST_AUTO_TEST_CASE(stack_const_top)
{
  aushev::Stack<int> stack;
  stack.push(42);
  const aushev::Stack<int>& cstack = stack;
  BOOST_CHECK_EQUAL(cstack.top(), 42);
}

BOOST_AUTO_TEST_CASE(queue_const_front)
{
  aushev::Queue<int> queue;
  queue.push(42);
  const aushev::Queue<int>& cqueue = queue;
  BOOST_CHECK_EQUAL(cqueue.front(), 42);
}

BOOST_AUTO_TEST_CASE(stack_exception_on_empty_drop)
{
  aushev::Stack<int> stack;
  BOOST_CHECK_THROW(stack.drop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_exception_on_empty_drop)
{
  aushev::Queue<int> queue;
  BOOST_CHECK_THROW(queue.drop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_exception_on_empty_top)
{
  aushev::Stack<int> stack;
  BOOST_CHECK_THROW(stack.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_exception_on_empty_front)
{
  aushev::Queue<int> queue;
  BOOST_CHECK_THROW(queue.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_large_operations)
{
  aushev::Stack<int> stack;
  const int size = 1000;
  for (int i = 0; i < size; ++i) {
    stack.push(i);
  }
  BOOST_CHECK_EQUAL(stack.size(), size);
  for (int i = size - 1; i >= 0; --i) {
    BOOST_CHECK_EQUAL(stack.drop(), i);
  }
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(queue_large_operations)
{
  aushev::Queue<int> queue;
  const int size = 1000;
  for (int i = 0; i < size; ++i) {
    queue.push(i);
  }
  BOOST_CHECK_EQUAL(queue.size(), size);
  for (int i = 0; i < size; ++i) {
    BOOST_CHECK_EQUAL(queue.drop(), i);
  }
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(stack_self_assignment)
{
  aushev::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack = stack;
  BOOST_CHECK_EQUAL(stack.size(), 2);
  BOOST_CHECK_EQUAL(stack.top(), 2);
}

BOOST_AUTO_TEST_CASE(queue_self_assignment)
{
  aushev::Queue<int> queue;
  queue.push(1);
  queue.push(2);
  queue = queue;
  BOOST_CHECK_EQUAL(queue.size(), 2);
  BOOST_CHECK_EQUAL(queue.front(), 1);
}

BOOST_AUTO_TEST_CASE(stack_mixed_operations)
{
  aushev::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.drop();
  stack.push(3);
  stack.push(4);
  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top(), 4);
}

BOOST_AUTO_TEST_CASE(queue_mixed_operations)
{
  aushev::Queue<int> queue;
  queue.push(1);
  queue.push(2);
  queue.drop();
  queue.push(3);
  queue.push(4);
  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.front(), 2);
}

BOOST_AUTO_TEST_CASE(no_memory_leaks)
{
  {
    aushev::Stack<int> stack;
    for (int i = 0; i < 100; ++i) {
      stack.push(i);
    }
    aushev::Stack<int> stack2;
    stack2 = stack;
    stack.clear();
  }
  {
    aushev::Queue<int> queue;
    for (int i = 0; i < 100; ++i) {
      queue.push(i);
    }
    aushev::Queue<int> queue2;
    queue2 = queue;
    queue.clear();
  }
}
