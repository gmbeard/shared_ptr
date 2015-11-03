#ifndef GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED
#define GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED 1

namespace gmb { namespace memory { namespace detail
{

  template<typename T>
  struct void_handler
  {
    typedef T&        reference_type;
    typedef T const&  const_reference_type;
  };

  template<>
  struct void_handler<void>
  {
    typedef void    reference_type;
    typedef void    const_reference_type;
  };

}}}
#endif //GMB_MEMORY_DETAIL_TRAITS_HPP_INCLUDED
