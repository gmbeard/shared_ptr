#ifndef GMB_MEMORY_SHARED_PTR_HPP_INCLUDED
#define GMB_MEMORY_SHARED_PTR_HPP_INCLUDED 1

#include "detail/shared_ptr_handle.hpp"

namespace gmb { namespace memory
{

  template<typename T>
  class shared_ptr
  {
  public:
    typedef T*        pointer_type;
    typedef T const*  const_pointer_type;
    typedef T         item_type;
    typedef T&        reference_type;
    typedef T const&  const_reference_type;

  public:
    shared_ptr()
      : handle_(detail::create_handle<T>(0, &detail::default_delete<T>))
    { }

    template<typename U>
    explicit shared_ptr(U *p)
      : handle_(detail::create_handle<T>(p, &detail::default_delete<U>))
    { }

    shared_ptr(shared_ptr const &other)
      : handle_(other.handle_)
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
      return (0 != handle);
    }

    reference_type operator*()
    {
      return *reinterpret_cast<pointer_type>(handle_->ptr());
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

  private:
    detail::shared_ptr_handle *handle_;
  };
}}

#endif //GMB_MEMORY_SHARED_PTR_HPP_INCLUDED
