#ifndef GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED
#define GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED 1

#include <cstdio>
#include <cstdint>
#include <new>

namespace gmb { namespace memory { namespace detail
{
  struct shared_ptr_handle
  {
    typedef void(*deleter)(void *);

    shared_ptr_handle(void *p, deleter d)
      : deleter_(d)
    {
      *(uintptr_t *)&blob_[0] = (uintptr_t)p;
      *(reinterpret_cast<size_t *>(&blob_[0] + sizeof(uintptr_t))) = 0;
    }

    ~shared_ptr_handle()
    {
      deleter_(ptr());
    }

    size_t inc_ref()
    {
      return ++ref();
    }

    size_t dec_ref()
    {
      return --ref();
    }

    void * ptr()
    {
      return (void *)*(uintptr_t *)&blob_[0];
    }

  private:

    shared_ptr_handle(shared_ptr_handle const &) /* = delete */;
    shared_ptr_handle & operator=(shared_ptr_handle const &) /* = delete */;

    size_t & ref()
    {
      return *(reinterpret_cast<size_t *>(&blob_[0] + sizeof(uintptr_t)));
    }

    char blob_[sizeof(uintptr_t) + sizeof(size_t)];
    deleter deleter_;
  };

  template<typename T>
  inline void default_delete(void *p)
  {
    if(p) {
      reinterpret_cast<T *>(p)->~T();
    }

    ::operator delete(p);
  }

  template<typename T>
  inline shared_ptr_handle * create_handle(T *p, shared_ptr_handle::deleter d)
  {
    void *bytes = ::operator new(sizeof(shared_ptr_handle));
    //  TODO(gmbeard): 
    //    We should throw if allocation fails. We won't
    //    always be using allocation that throws itself

    try {
      new (bytes) shared_ptr_handle(p, d);
      static_cast<shared_ptr_handle *>(bytes)->inc_ref();
    }
    catch(...) {
      ::operator delete(bytes);
      throw;
    }

    return static_cast<shared_ptr_handle *>(bytes);
  }

  void destroy_handle(shared_ptr_handle *p)
  {
    p->~shared_ptr_handle();
    ::operator delete(p);
    //std::free(p);
  }

}}}

#endif //GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED
