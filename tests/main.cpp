#include <iostream>
#include <fstream>
#include <cassert>
#include <string.h>

#include "shared_ptr/shared_ptr.hpp"

template<typename Sig>
struct test_t
{
  test_t(Sig f, void *p)
    : fn_(f),
      p_(p)
  { }

  ~test_t()
  {
    fn_(p_);
  }
private:
  Sig   fn_;
  void *p_;
};

typedef test_t<void(*)(void *)> function_t;

template<typename T>
void inc_delete(void *p)
{
  (*reinterpret_cast<T *>(p))++;
}

gmb::memory::shared_ptr<function_t> do_something(gmb::memory::shared_ptr<function_t> p)
{
  gmb::memory::shared_ptr<function_t> tmp(p);
  return tmp;
}

void write_to_output(char const *val, gmb::memory::shared_ptr<std::ostream> p)
{
  (*p) << val << std::endl;
}

int main(int, char const *[])
{
  using gmb::memory::shared_ptr;

  int delcount = 0;

  {
    typedef shared_ptr<void, void(*)(void *)> char_buffer_t;
    char *p = (char *)std::malloc(sizeof("Hello, World!") + 1);
    std::strcpy(p, "Hello, World!");
    char_buffer_t buff(p, std::free);
  }

//  {
//    shared_ptr<std::ostream> file_ptr(new std::ofstream("C:\\temp\\tmp.txt", std::ios::beg));
//    write_to_output("Hello, World!", file_ptr);
//  }

  {
    shared_ptr<function_t> p(new function_t(inc_delete<int>, &delcount));
    shared_ptr<function_t> p2 = p; 

    shared_ptr<function_t> p3 = do_something(p);
  }
  std::cout << "Deleted: " << delcount << std::endl;

  assert(delcount == 1);
}
