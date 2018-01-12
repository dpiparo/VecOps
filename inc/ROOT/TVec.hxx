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
   template<typename TT> friend class TVec;
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

public:
   // This constructor is not documented as it is to be considered "internal"
   /*! \cond PRIVATE */
   TVec(const TTreeReaderArray<T> &ttra) : fArray(ttra[0]), fArraySize(ttra.GetSize()){};
   /*! \endcond */

   /// Construct starting from an vector
   TVec(const std::vector<T> vec) : fVector(vec), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Construct from initialiser list
   TVec(std::initializer_list<T> init) : fVector(init), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Construct from a size
   TVec(size_type size) : fVector(size), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Default constructor
   TVec(){};

   /// Copy constructor. Cast of contained types is performed if needed.
   template<typename V>
   TVec(const TVec<V> &v)
   {
      auto vData = v.data();
      fArraySize = v.size();
      fVector.resize(fArraySize);
      fArray = fVector.data();
      // This works only with pods
      //auto dataArray = v.data();
      //memcpy((T*)fArray, dataArray, fArraySize * sizeof(T));
      std::copy(vData, vData + fArraySize, fVector.begin());
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
   TVec<typename std::common_type<T,V>::type> operator+(const V &c)
   {
      TVec<typename std::common_type<T,V>::type> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector) {
         e += c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T,V>::type> operator-(const V &c)
   {
      return *this + (-c);
   }

   template <typename V>
   TVec<typename std::common_type<T,V>::type> operator*(const V &c)
   {
      TVec<typename std::common_type<T,V>::type> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector) {
         e *= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T,V>::type> operator/(const V &c)
   {
      TVec<typename std::common_type<T,V>::type> newTVec(*this);
      // polish once iterators are there!
      for (auto &&e : newTVec.fVector) {
         e /= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] > c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>=(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] >= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] < c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<=(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] <= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator==(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] == c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator!=(const V &c)
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (const auto i : ROOT::TSeq<size_type>(newTVec.size())) {
         newVec[i] = fArray[i] != c;
      }
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
