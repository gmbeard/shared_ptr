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

#ifndef GMB_MEMORY_SHARED_PTR_HPP_INCLUDED
#define GMB_MEMORY_SHARED_PTR_HPP_INCLUDED 1

#include "detail/traits.hpp"
#include "detail/shared_ptr_handle.hpp"
#include "detail/shared_ptr_base.hpp"

namespace gmb { namespace memory
{

  template<typename T, typename Deleter = 
    detail::default_deleter<T> >
  class shared_ptr : public detail::shared_ptr_base<T>
  {
    using detail::shared_ptr_base<T>::handle_;

  public:
    typedef typename detail::shared_ptr_base<T>::pointer_type          
      pointer_type;
    typedef typename detail::shared_ptr_base<T>::const_pointer_type    
      const_pointer_type;
    typedef typename detail::shared_ptr_base<T>::reference_type        
      reference_type;
    typedef typename detail::shared_ptr_base<T>::const_reference_type  
      const_reference_type;
    typedef typename detail::shared_ptr_base<T>::item_type            
      item_type;

    typedef Deleter deleter_type;

  public:
    explicit shared_ptr(Deleter d = Deleter())
      : detail::shared_ptr_base<T>(detail::create_handle<item_type>(0, d))
    { }

    template<typename U>
    explicit shared_ptr(U *p)
      : detail::shared_ptr_base<T>(detail::create_handle<item_type>(
          static_cast<pointer_type>(p), Deleter()))
    { }

    explicit shared_ptr(detail::shared_ptr_handle *h, 
      detail::created_from_this_tag, Deleter d = Deleter())
      : detail::shared_ptr_base<T>(h)
    {
      handle_->inc_ref();
    }

    template<typename U, typename UDeleter>
    explicit shared_ptr(U *p, UDeleter d)
      : detail::shared_ptr_base<T>(detail::create_handle<item_type>(
          static_cast<pointer_type>(p), d))
    { }

    shared_ptr(shared_ptr const &other)
      : detail::shared_ptr_base<T>(other.handle_)
    {
      handle_->inc_ref();
    }

    ~shared_ptr()
    {
      if(!handle_->dec_ref()) {
        detail::destroy_handle(handle_);
      }
    }

    friend void swap(shared_ptr &lhs, shared_ptr &rhs)
    {
      using std::swap;
      swap(lhs.handle_, rhs.handle_);
    }

    shared_ptr & operator=(shared_ptr const &rhs)
    {
      shared_ptr tmp(rhs);
      swap(*this, tmp);
      return *this;
    }

    operator bool() const
    {
      return (0 != handle_->ptr());
    }

    reference_type operator*()
    {
      return *reinterpret_cast<pointer_type>(handle_->ptr());
    }

    pointer_type operator->()
    {
      return reinterpret_cast<pointer_type>(handle_->ptr());
    }

    const_pointer_type operator->() const
    {
      return reinterpret_cast<const_pointer_type>(handle_->ptr());
    }

    const_reference_type operator*() const
    {
      return *reinterpret_cast<const_pointer_type>(handle_->ptr());
    }

    pointer_type get()
    {
     return reinterpret_cast<pointer_type>(handle_->ptr());
    }

    const_pointer_type get() const
    {
      return reinterpret_cast<const_pointer_type>(handle_->ptr());
    }
  };

  template<typename T>
  bool operator==(shared_ptr<T> const &lhs, shared_ptr<T> const &rhs)
  {
    return lhs.get() == rhs.get();
  }

  template<typename T>
  bool operator!=(shared_ptr<T> const &lhs, shared_ptr<T> const &rhs)
  {
    return !(lhs == rhs);
  }
}}

#endif //GMB_MEMORY_SHARED_PTR_HPP_INCLUDED
