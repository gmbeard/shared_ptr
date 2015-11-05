/*
  The MIT License (MIT)

  Copyright (c) 2015 Greg Beard

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED
#define GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED 1

#include <cstdio>
#include <cstdint>
#include <new>

namespace gmb { namespace memory { namespace detail
{
  struct shared_ptr_handle
  {
    typedef void(*deleter)(shared_ptr_handle *, void *);

    shared_ptr_handle(void *p, deleter d)
      : deleter_(d)
    {
      *(uintptr_t *)&blob_[0] = (uintptr_t)p;
      *(reinterpret_cast<size_t *>(&blob_[0] + sizeof(uintptr_t))) = 0;
    }

    ~shared_ptr_handle()
    {
      deleter_(this, ptr());
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

  template<typename Deleter>
  struct shared_ptr_handle_impl : shared_ptr_handle
  {
    shared_ptr_handle_impl(void *p, Deleter d)
      : shared_ptr_handle(p, &shared_ptr_handle_impl::delete_impl),
        d_(d)
    { }

  private:
    static void delete_impl(shared_ptr_handle *parent, void *p)
    {
      (*static_cast<shared_ptr_handle_impl *>(parent)).d_(p);
    }

    Deleter d_;
  };

  template<typename T>
  struct default_deleter
  {
    void operator()(void *p)
    {
      if(p) {
        reinterpret_cast<T *>(p)->~T();
      }
      ::operator delete(p);
    }
  };

  template<typename T>
  struct default_deleter<T[]>
  { 
    void operator()(void *p)
    {
      delete [] static_cast<T *>(p);
    }
  };

  template<typename T, typename Deleter>
  inline shared_ptr_handle * create_handle(T *p, Deleter d)
  {
    void *bytes = ::operator new(sizeof(shared_ptr_handle_impl<Deleter>));
    //  TODO(gmbeard): 
    //    We won't always be using allocation that throws on failure.
    //    We should throw if bytes == NULL

    try {
      new (bytes) shared_ptr_handle_impl<Deleter>(p, d);
      static_cast<shared_ptr_handle_impl<Deleter> *>(bytes)->inc_ref();
    }
    catch(...) {
      ::operator delete(bytes);
      throw;
    }

    return static_cast<shared_ptr_handle *>(bytes);
  }

  inline void destroy_handle(shared_ptr_handle *p)
  {
    p->~shared_ptr_handle();
    ::operator delete(p);
  }

}}}

#endif //GMB_MEMORY_SHARED_PTR_HANDLE_HPP_INCLUDED
