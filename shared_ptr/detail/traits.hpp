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

#ifndef GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED
#define GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED 1

namespace gmb { namespace memory { namespace detail
{
//  template<typename T>
//  struct void_handler
//  {
//    typedef T&        reference_type;
//    typedef T const&  const_reference_type;
//  };
//
//  template<>
//  struct void_handler<void>
//  {
//    typedef void    reference_type;
//    typedef void    const_reference_type;
//  };

  template<typename T>
  struct ptr_traits
  {
    typedef T &       reference_type;
    typedef T const & const_reference_type;
    typedef T *       pointer_type;
    typedef T const * const_pointer_type;
    typedef T         item_type;
  };

  template<typename T>
  struct ptr_traits<T[]>
  {
    typedef T &       reference_type;
    typedef T const & const_reference_type;
    typedef T *       pointer_type;
    typedef T const * const_pointer_type;
    typedef T         item_type;
  };

  template<>
  struct ptr_traits<void>
  {
    typedef void          reference_type;
    typedef void          const_reference_type;
    typedef void *        pointer_type;
    typedef void const *  const_pointer_type;
    typedef void          item_type;
  };

  struct false_type {};
  struct true_type {};

  struct shared_ptr_handle;
  struct shared_from_this_base
  {
    shared_from_this_base()
      : handle_(0)
    { }

    shared_ptr_handle *handle_;
  };
  
  template<typename T>
  struct is_shared_from_this {
    typedef false_type value;
  };

  template<>
  struct is_shared_from_this<shared_from_this_base>
  {
    typedef true_type value;
  };

  template<typename T>
  struct enable_if_array_type
  { }; 

  template<typename T>
  struct enable_if_array_type<T[]>
  {
    typedef T &       reference_type;
    typedef T const & const_reference_type;
  };

}}}
#endif //GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED
