// Author: Enrico Guiraud, Enric Tejedor, Danilo Piparo CERN  01/2018

/*************************************************************************
 * Copyright (C) 1995-2018, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVEC
#define ROOT_TVEC

#include <TTreeReaderArray.h>

#include <vector>

namespace ROOT {

namespace Experimental {

namespace VecOps {

template <typename T>
class TVec {
public:
   // The same types of the vector
   using value_type = typename std::vector<T>::value_type;
   using allocator_type = typename std::vector<T>::allocator_type;
   using size_type = typename std::vector<T>::size_type;
   using difference_type = typename std::vector<T>::difference_type;
   using reference = typename std::vector<T>::reference;
   using const_reference = typename std::vector<T>::const_reference;
   using pointer = typename std::vector<T>::pointer;
   using const_pointer = typename std::vector<T>::const_pointer;
   using iterator = typename std::vector<T>::iterator;
   using const_iterator = typename std::vector<T>::const_iterator;
   using reverse_iterator = typename std::vector<T>::reverse_iterator;
   using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

private:
   std::vector<T> fVector;
   const_pointer fArray = nullptr;
   size_t fArraySize = 0;
   void CheckOwnership() const
   {
      if (fVector.empty())
         throw std::runtime_error("This TVec instance cannot be modified");
   }
   TVec<int> GetTVecInt()
   {
      TVec<int> newTVec;
      auto &newVec = newTVec.fVector;
      const auto vsize = size();
      newVec.resize(vsize);
      newTVec.fArray = newVec.data();
      newTVec.fArraySize = vsize;
      return newTVec;
   }

public:
   // This constructor is not documented as it is to be considered "internal"
   /*! \cond PRIVATE */
   TVec(const TTreeReaderArray<T> &ttra) : fArray(ttra[0]), fArraySize(ttra.GetSize()){};
   /*! \endcond */

   /// Construct starting from an vector
   TVec(const std::vector<T> vec) : fVector(vec), fArray(fVector.data()), fArraySize(vec.size()){};

   /// Default constructor
   TVec(){};

   /// Copy constructor. Memcopy is used for efficiency
   TVec(const TVec<T> &v)
   {
      auto dataArray = v.data();

      fArraySize = v.size();
      fVector.resize(fArraySize);
      fArray = fVector.data();

      cout << fArraySize << " " << fVector.size() << endl;
      cout << dataArray[0] << " " << dataArray[1] << " " << dataArray[2];

      memcpy(&fVector[0], dataArray, fArraySize * sizeof(T));
   }

   // Here all the vector methods will be added

   /** @name Element access
   */
   ///@{
   reference operator[](size_type pos)
   {
      CheckOwnership();
      return fArray[pos];
   };
   const_reference operator[](size_type pos) const { return fArray[pos]; };

   reference front()
   {
      CheckOwnership();
      return fArray[0];
   };
   const_reference front() const { return fArray[0]; };

   reference back()
   {
      CheckOwnership();
      return fArray[fArraySize - 1];
   };
   const_reference back() const { return fArray[fArraySize - 1]; };

   pointer data() noexcept
   {
      CheckOwnership();
      return fArray;
   };
   const const_pointer data() const noexcept { return fArray; };
   ///@}

   /** @name Iterators
   */
   ///@{
   ///@}

   /** @name Capacity
   */
   ///@{
   size_type size() const noexcept { return fArraySize; };
   ///@}

   /** @name Modifiers
   */
   ///@{
   template <class... Args>
   void emplace_back(Args &&... args)
   {
      void CheckOwnership();
      fVector.emplace_back(std::move(args...));
   }
   ///@}

   /** @name Math Operators with scalars
    *  Math operators involving TVec
   */
   ///@{
   template <typename V>
   TVec<T> operator+(const V &c)
   {
      TVec<T> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector)
         e += c;
      return newTVec;
   }

   template <typename V>
   TVec<T> operator-(const V &c)
   {
      return *this + (-c);
   }

   template <typename V>
   TVec<T> operator*(const V &c)
   {
      TVec<T> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector)
         e *= c;
      return newTVec;
   }

   template <typename V>
   TVec<T> operator/(const V &c)
   {
      TVec<T> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector)
         e /= c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] > c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>=(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] >= c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] < c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<=(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] <= c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator==(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] == c;
      return newTVec;
   }

   template <typename V>
   TVec<int> operator!=(const V &c)
   {
      auto newTVec = GetTVecInt();
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size()))
         newVec[i] = fArray[i] != c;
      return newTVec;
   }

   ///@}

   /** @name Math Operators with TVecs
    *  Math operators involving TVec
   */
   ///@{
   ///@}
};

} // End of VecOps NS

} // End of Experimental NS

} // End of ROOT NS

namespace cling {
template <typename T>
inline std::string printValue(ROOT::Experimental::VecOps::TVec<T> *tvec)
{
   return printValue((std::vector<T> *)tvec);
}

} // namespace cling

#endif
