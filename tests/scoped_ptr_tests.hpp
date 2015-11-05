#ifndef GMB_MEMORY_TESTS_SCOPED_PTR_TESTS_HPP_INCLUDED
#define GMB_MEMORY_TESTS_SCOPED_PTR_TESTS_HPP_INCLUDED 1

#include <catch.hpp>
#include "scoped_ptr/scoped_ptr.hpp"

TEST_CASE("scoped_ptr manages lifetime", "[scoped_ptr]")
{
  using gmb::memory::scoped_ptr;
  testutils::logged_deleter<int>::delete_count = 0;

  SECTION("going out of scope calls delete")
  {

    {
      //  Forced scope so we can observe
      //  any deletions...
      scoped_ptr<int, testutils::logged_deleter<int> > 
        p(new int(42));
    }

    REQUIRE(testutils::logged_deleter<int>::delete_count == 1);
  }

  SECTION("Tranferring via reset call delete correct amount of times")
  {
    {
      scoped_ptr<int, testutils::logged_deleter<int> > 
        p(new int(42)), p2;
      REQUIRE(p);
      REQUIRE(!p2);

      p2.reset(p.release());

      REQUIRE(!p);
      REQUIRE(p2);
    }

    REQUIRE(testutils::logged_deleter<int>::delete_count == 1);
  }

  SECTION("Reset to a new value calls delete twice")
  {
    {
      scoped_ptr<float, testutils::logged_deleter<float> > 
        p(new float(0.1f));
      p.reset(new float(0.2f));
    }

    REQUIRE(testutils::logged_deleter<float>::delete_count == 2);
  }

  SECTION("Ptr to char array dereferences correct chars at indexed")
  {
    scoped_ptr<char[]> p(new char[sizeof("Hello, scoped_ptr") + 1]);
    std::strcpy(p.get(), "Hello, scoped_ptr");

    REQUIRE(p[0] == 'H');
    REQUIRE(p[2] == 'l');
    REQUIRE(std::strlen(p.get()) == strlen("Hello, scoped_ptr"));
  }
}

#endif //GMB_MEMORY_TESTS_SCOPED_PTR_TESTS_HPP_INCLUDED
