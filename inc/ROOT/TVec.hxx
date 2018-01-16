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

#include <ROOT/TSeq.hxx>
#include <TTreeReaderArray.h>

#include <numeric> // for inner_product
#include <sstream>
#include <type_traits>
#include <vector>

namespace ROOT {

namespace Internal {

namespace VecOps {

template <typename...>
struct TIsOneOf {
    static constexpr bool value = false;
};

template <typename F, typename S, typename... T>
struct TIsOneOf<F, S, T...> {
    static constexpr bool value =
        std::is_same<F, S>::value || TIsOneOf<F, T...>::value;
};

template<typename T>
struct TIsChar {
    static constexpr bool value = TIsOneOf<typename std::decay<T>::type, char, signed char, unsigned char, wchar_t, char16_t, char32_t> ::value;
};

void CheckSizes(size_t s0, size_t s1, const char *opName)
{
   if (s0 != s1) {
      std::stringstream err;
      err << "Cannot perform operation " << opName << ". The array sizes differ (" << s0 << " and " << s1 << ")";
      throw std::runtime_error(err.str());
   }
}
} // End of VecOps NS

} // End of Internal NS

namespace Experimental {

namespace VecOps {

template <typename T> class TVec;
template <typename T> TVec<double> sqrt(const TVec<T>&);

template <typename T>
class TVec {
   template <typename V>
   friend class TVec;
   template<typename V>
   friend TVec<double> sqrt(const TVec<V>&);

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
   TVec(TTreeReaderArray<T> &ttra) : fArray(static_cast<T*>(ttra.GetAddress())), fArraySize(ttra.GetSize()) {}
   /*! \endcond */

   /// Construct starting from a vector
   TVec(const std::vector<T> &vec) : fVector(vec), fArray(fVector.data()), fArraySize(fVector.size()){};
   TVec(std::vector<T> &&vec) : fVector(std::move(vec)), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Construct from initializer list
   TVec(std::initializer_list<T> init) : fVector(init), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Construct from a size
   TVec(size_type size) : fVector(size), fArray(fVector.data()), fArraySize(fVector.size()){};

   /// Default constructor
   TVec(){};

   /// Copy constructor. Cast of contained types is performed if needed.
   TVec(const TVec<T> &v) : fVector(v.fVector), fArray(fVector.data()), fArraySize(fVector.size()) {}

   template <typename V>
   TVec(const TVec<V> &v)
      : fVector(v.fVector.begin(), v.fVector.end()), fArray(fVector.data()), fArraySize(fVector.size()) {}

   /// Move constructor
   TVec(TVec<T> &&v) : fVector(std::move(v.fVector)), fArray(fVector.data()), fArraySize(fVector.size())
   {
      v.fArray = v.fVector.data();
      v.fArraySize = v.fVector.size();
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
      CheckOwnership();
      fVector.emplace_back(std::forward<Args>(args)...);
   }
   ///@}

   /** @name Math Operators with scalars
    *  Math operators involving TVec
   */
   ///@{
   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator+(const V &c) const
   {
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto &&e : newTVec.fVector) {
         e += c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator-(const V &c) const
   {
      return *this + (-c);
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator*(const V &c) const
   {
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto &&e : newTVec.fVector) {
         e *= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator/(const V &c) const
   {
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto &&e : newTVec.fVector) {
         e /= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] > c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>=(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] >= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] < c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<=(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] <= c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator==(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] == c;
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator!=(const V &c) const
   {
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] != c;
      }
      return newTVec;
   }

   ///@}

   /** @name Math Operators with TVecs
    *  Math operators involving TVecs
   */
   ///@{
   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator+(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "+");
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newTVec.fVector[i] += v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator-(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "-");
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newTVec.fVector[i] -= v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator*(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "*");
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newTVec.fVector[i] *= v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<typename std::common_type<T, V>::type> operator/(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "/");
      TVec<typename std::common_type<T, V>::type> newTVec(*this);
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newTVec.fVector[i] /= v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), ">");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] > v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator>=(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), ">=");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] >= v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "<");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] < v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator<=(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "<=");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] <= v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator==(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "==");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] == v.fArray[i];
      }
      return newTVec;
   }

   template <typename V>
   TVec<int> operator!=(const TVec<V> &v) const
   {
      ROOT::Internal::VecOps::CheckSizes(fArraySize, v.size(), "!=");
      TVec<int> newTVec(fArraySize);
      auto &newVec = newTVec.fVector;
      for (auto i : ROOT::TSeq<size_type>(fArraySize)) {
         newVec[i] = fArray[i] != v.fArray[i];
      }
      return newTVec;
   }
   ///@}
};

/** @name Math Functions
 *  Math functions on TVecs
*/
///@{
template<typename T>
TVec<double> sqrt(const TVec<T>& v)
{
   TVec<double> newTVec(v.size());
   auto &newVec = newTVec.fVector;
   for (const auto i : ROOT::TSeq<typename TVec<T>::size_type>(newTVec.size())) {
      newVec[i] = ::sqrt(v.fArray[i]);
   }
   return newTVec;
}
///@}

/// Inner product
template <typename T, typename V>
typename std::common_type<T, V>::type Dot (const TVec<T> v0, const TVec<V> v1)
{
   return std::inner_product(v0.data(), v0.data()+v0.size(), v1.data(), typename std::common_type<T, V>::type (0));
}

template<class T>
std::ostream& operator<<( std::ostream& os, const TVec<T>& v )
{
   // In order to print properly, convert to 64 bit int if this is a char
   using Print_t = typename std::conditional<ROOT::Internal::VecOps::TIsChar<T>::value, Long64_t, T>::type;
   os << "{ ";
   auto size = v.size();
   if (size) {
      auto data = v.data();
      for (auto i : ROOT::TSeq<typename TVec<T>::size_type>(size-1)) {
         os << (Print_t) data[i] << ", ";
      }
   os << (Print_t) data[size-1];
   }
   os << " }";
   return os;
}

} // End of VecOps NS

} // End of Experimental NS

} // End of ROOT NS

#include <string>

namespace cling {
template <typename T>
inline std::string printValue(ROOT::Experimental::VecOps::TVec<T> *tvec)
{
   std::stringstream ss;
   ss << *tvec;
   return ss.str();
}

} // namespace cling

#endif
