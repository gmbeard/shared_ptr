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

#ifndef GMB_MEMORY_SCOPED_PTR_HPP_INCLUDED
#define GMB_MEMORY_SCOPED_PTR_HPP_INCLUDED 1

#include "../shared_ptr/detail/traits.hpp"
#include "../shared_ptr/detail/shared_ptr_handle.hpp"
#include "detail/scoped_ptr_base.hpp"

namespace gmb { namespace memory {

  template<typename T, typename Deleter = detail::default_deleter<T> >
  class scoped_ptr : public detail::scoped_ptr_base<T>
  {
    using detail::scoped_ptr_base<T>::ptr_;

  public:

    typedef typename detail::scoped_ptr_base<T>::pointer_type          pointer_type;
    typedef typename detail::scoped_ptr_base<T>::const_pointer_type    const_pointer_type;
    typedef typename detail::scoped_ptr_base<T>::reference_type        reference_type;
    typedef typename detail::scoped_ptr_base<T>::const_reference_type  const_reference_type;
    typedef Deleter  deleter_type;

    explicit scoped_ptr(deleter_type d = deleter_type())
      : detail::scoped_ptr_base<T>(),
        deleter_(d)
    { }

    template<typename U>
    explicit scoped_ptr(U *p)
      : detail::scoped_ptr_base<T>(static_cast<pointer_type>(p)),
        deleter_(deleter_type())
    { }

    template<typename U, typename UDeleter>
    scoped_ptr(U *p, UDeleter d)
      : detail::scoped_ptr_base<T>(static_cast<pointer_type>(p)),
        deleter_(d)
    { }

    ~scoped_ptr()
    {
      deleter_(ptr_);
    }

    friend void swap(scoped_ptr &lhs, scoped_ptr &rhs)
    {
      using std::swap;
      swap(lhs.ptr_, rhs.ptr_);
      swap(lhs.deleter_, rhs.deleter_);
    }

    template<typename U>
    void reset(U *p)
    {
      scoped_ptr tmp(p, deleter_);
      swap(*this, tmp);
    }

    pointer_type release()
    {
      pointer_type tmp = ptr_;
      ptr_ = 0;
      return tmp;
    }

    pointer_type operator->()
    {
      return ptr_;
    }

    const_pointer_type operator->() const
    {
      return ptr_;
    }

    reference_type operator*()
    {
      return *ptr_;
    }

    const_reference_type operator*() const
    {
      return *ptr_;
    }

    operator bool() const
    {
      return ptr_ != 0;
    }

    pointer_type get()
    {
      return ptr_;
    }

    const_pointer_type get() const
    {
      return ptr_;
    }
  
  private:
    scoped_ptr(scoped_ptr const &) /* = delete */;
    scoped_ptr & operator=(scoped_ptr const &) /* = delete */;

    Deleter       deleter_;
  };
}}

#endif //GMB_MEMORY_SCOPED_PTR_HPP_INCLUDED
