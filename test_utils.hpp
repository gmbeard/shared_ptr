#ifndef TEST_UTILS_HPP_INCLUDED
#define TEST_UTILS_HPP_INCLUDED 1

namespace testutils
{
  template<typename T>
  struct logged_deleter
  {
    void operator()(void *p) {
      delete reinterpret_cast<T *>(p);
      if(p) {
        delete_count++;
      }
    }

    static int delete_count;
  };
}

template<typename T>
int testutils::logged_deleter<T>::delete_count(0);

#endif //TEST_UTILS_HPP_INCLUDED
