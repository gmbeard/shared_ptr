#ifndef GMB_MEMORY_TESTS_SHARED_PTR_TESTS_HPP_INCLUDED
#define GMB_MEMORY_TESTS_SHARED_PTR_TESTS_HPP_INCLUDED 1

#include <catch.hpp>
#include "shared_ptr/shared_ptr.hpp"

TEST_CASE("shared_ptr tests", "[shared_ptr]")
{
  using gmb::memory::shared_ptr;
  testutils::logged_deleter<int>::delete_count = 0;

  SECTION("going out of scope calls delete")
  {
    {
      shared_ptr<int, testutils::logged_deleter<int> > p(new int(42));
      REQUIRE(*p == 42);
    }

    REQUIRE(testutils::logged_deleter<int>::delete_count == 1);
  }

  SECTION("Copying point only deletes once")
  {
    {
      shared_ptr<int, testutils::logged_deleter<int> > p1(new int(42)), p2, p3;
      REQUIRE(*p1 == 42);

      p2 = p1;
      REQUIRE(*p2 == 42);

      p3 = p2;
      REQUIRE(*p3 == 42);
    }

    REQUIRE(testutils::logged_deleter<int>::delete_count == 1);
  }

  SECTION("Creating with char array allows chars at indexes to be dereferenced")
  {
    shared_ptr<char[]> p(new char[sizeof("Hello, shared_ptr") + 1]);
    std::strcpy(p.get(), "Hello, shared_ptr");

    REQUIRE(p[0] == 'H');
    REQUIRE(p[4] == 'o');
    REQUIRE(0 == std::strcmp(p.get(), "Hello, shared_ptr"));
  }

  SECTION("Two pointers pointing to same object compare equal")
  {
    shared_ptr<int, testutils::logged_deleter<int> > p1(new int(42)), p2;
    p2 = p1;
    REQUIRE(p1 == p2);
  }
}

#endif //GMB_MEMORY_TESTS_SHARED_PTR_TESTS_HPP_INCLUDED
