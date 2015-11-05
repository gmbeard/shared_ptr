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

#ifndef GMB_MEMORY_DETAIL_SCOPED_PTR_BASE_HPP_INCLUDED
#define GMB_MEMORY_DETAIL_SCOPED_PTR_BASE_HPP_INCLUDED 1

namespace gmb { namespace memory { namespace detail
{
  template<typename T>
  struct scoped_ptr_base
  {
    typedef typename detail::ptr_traits<T>::pointer_type          pointer_type;
    typedef typename detail::ptr_traits<T>::const_pointer_type    const_pointer_type;
    typedef typename detail::ptr_traits<T>::reference_type        reference_type;
    typedef typename detail::ptr_traits<T>::const_reference_type  const_reference_type;

    scoped_ptr_base()
      : ptr_(0)
    { }

    scoped_ptr_base(pointer_type p)
      : ptr_(p)
    { }

    pointer_type ptr_;
  };

  template<typename T>
  struct scoped_ptr_base<T[]>
  {
    typedef typename detail::ptr_traits<T>::pointer_type          pointer_type;
    typedef typename detail::ptr_traits<T>::const_pointer_type    const_pointer_type;
    typedef typename detail::ptr_traits<T>::reference_type        reference_type;
    typedef typename detail::ptr_traits<T>::const_reference_type  const_reference_type;

    scoped_ptr_base()
      : ptr_(0)
    { }

    scoped_ptr_base(pointer_type p)
      : ptr_(p)
    { }

    reference_type operator[](size_t n)
    {
      return ptr_[n];
    }

    const_reference_type operator[](size_t n) const
    {
      return ptr_[n];
    }

    pointer_type ptr_;
  };

}}}
#endif //GMB_MEMORY_DETAIL_SCOPED_PTR_BASE_HPP_INCLUDED
