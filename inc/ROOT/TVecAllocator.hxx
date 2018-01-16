// Author: Enrico Guiraud, Enric Tejedor, Danilo Piparo CERN  01/2018

/*************************************************************************
 * Copyright (C) 1995-2018, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVECALLOCATOR
#define ROOT_TVECALLOCATOR

#include <iostream>
#include <memory>

namespace ROOT {
namespace Detail {
namespace VecOps {

template <typename T>
class TVecAllocator // : public std::allocator<T>
{
public:
   using StdAlloc_t = std::allocator<T>;
   using value_type = typename StdAlloc_t::value_type;
   using pointer = typename StdAlloc_t::pointer;
   using const_pointer = typename StdAlloc_t::const_pointer;
   using reference = typename StdAlloc_t::reference;
   using const_reference = typename StdAlloc_t::const_reference;
   using size_type = typename StdAlloc_t::size_type;
   using difference_type = typename StdAlloc_t::difference_type;

private:
   using StdAllocTraits_t = std::allocator_traits<StdAlloc_t>;
   pointer fInitialAddress = nullptr;
   size_type fInitialSize = 0;
   bool fIsFirstAllocation = false;
   StdAlloc_t fStdAllocator;

public:
   TVecAllocator(pointer p, size_type n) : fInitialAddress(p), fInitialSize(n), fIsFirstAllocation(true){};
   TVecAllocator() = default;

   pointer allocate(std::size_t n)
   {
      if (n > std::size_t(-1) / sizeof(T))
         throw std::bad_alloc();
      if (fIsFirstAllocation) {
         fIsFirstAllocation = false;
         return fInitialAddress;
      }
      return StdAllocTraits_t::allocate(fStdAllocator, n);
   }

   void deallocate(pointer p, std::size_t n)
   {
      if (p != fInitialAddress)
         StdAllocTraits_t::deallocate(fStdAllocator, p, n);
   }
};

} // End NS VecOps
} // End NS Internal
} // End NS ROOT

#endif
