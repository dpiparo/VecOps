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

#include <ROOT/TVecAllocator.hxx>

#include <algorithm>
#include <cmath>   // for sqrt
#include <numeric> // for inner_product
#include <vector>

namespace ROOT {

// Other helpers
namespace Internal {

namespace VecOps {

template <typename...>
struct TIsOneOf {
   static constexpr bool value = false;
};

template <typename F, typename S, typename... T>
struct TIsOneOf<F, S, T...> {
   static constexpr bool value = std::is_same<F, S>::value || TIsOneOf<F, T...>::value;
};

template <typename T>
struct TIsChar {
   static constexpr bool value =
      TIsOneOf<typename std::decay<T>::type, char, signed char, unsigned char, wchar_t, char16_t, char32_t>::value;
};

} // End of VecOps NS

} // End of Internal NS

namespace Experimental {

namespace VecOps {

template <typename T>
using TVec = std::vector<T, ROOT::Detail::VecOps::TVecAllocator<T>>;

/** @name Math Operators with scalars
 *  Math operators involving TVec
*/
///@{
template <typename T, typename V>
auto operator+(const TVec<T> &v, const V &c) -> TVec<decltype(v[0] + c)>
{
   TVec<decltype(v[0] + c)> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t + c; });
   return w;
}

template <typename T, typename V>
auto operator-(const TVec<T> &v, const V &c) -> TVec<decltype(v[0] - c)>
{
   TVec<decltype(v[0] - c)> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t - c; });
   return w;
}

template <typename T, typename V>
auto operator*(const TVec<T> &v, const V &c) -> TVec<decltype(v[0] * c)>
{
   TVec<decltype(v[0] * c)> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t * c; });
   return w;
}

template <typename T, typename V>
auto operator/(const TVec<T> &v, const V &c) -> TVec<decltype(v[0] / c)>
{
   TVec<decltype(v[0] / c)> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t / c; });
   return w;
}

template <typename T, typename V>
TVec<char> operator>(const TVec<T> &v, const V &c)
{
   TVec<char> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t > c; });
   return w;
}

template <typename T, typename V>
TVec<char> operator>=(const TVec<T> &v, const V &c)
{
   TVec<char> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t >= c; });
   return w;
}

template <typename T, typename V>
TVec<char> operator<(const TVec<T> &v, const V &c)
{
   TVec<char> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t < c; });
   return w;
}

template <typename T, typename V>
TVec<char> operator<=(const TVec<T> &v, const V &c)
{
   TVec<char> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t <= c; });
   return w;
}

template <typename T, typename V>
TVec<char> operator==(const TVec<T> &v, const V &c)
{
   TVec<char> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [&c](const T &t) { return t == c; });
   return w;
}

///@}

/** @name Math Operators with TVecs
 *  Math operators involving TVecs
*/
///@{
template <typename T, typename V>
auto operator+(const TVec<T> &v0, const TVec<V> &v1) -> TVec<decltype(v0[0] + v1[0])>
{
   TVec<decltype(v0[0] + v1[0])> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t + v; });
   return w;
}

template <typename T, typename V>
auto operator-(const TVec<T> &v0, const TVec<V> &v1) -> TVec<decltype(v0[0] - v1[0])>
{
   TVec<decltype(v0[0] - v1[0])> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t - v; });
   return w;
}

template <typename T, typename V>
auto operator*(const TVec<T> &v0, const TVec<V> &v1) -> TVec<decltype(v0[0] - v1[0])>
{
   TVec<decltype(v0[0] * v1[0])> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t * v; });
   return w;
}

template <typename T, typename V>
auto operator/(const TVec<T> &v0, const TVec<V> &v1) -> TVec<decltype(v0[0] - v1[0])>
{
   TVec<decltype(v0[0] / v1[0])> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t / v; });
   return w;
}

template <typename T, typename V>
TVec<char> operator>(const TVec<T> &v0, const TVec<V> &v1)
{
   TVec<char> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t > v; });
   return w;
}

template <typename T, typename V>
TVec<char> operator>=(const TVec<T> &v0, const TVec<V> &v1)
{
   TVec<char> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t >= v; });
   return w;
}

template <typename T, typename V>
TVec<char> operator<(const TVec<T> &v0, const TVec<V> &v1)
{
   TVec<char> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t <= v; });
   return w;
}

template <typename T, typename V>
TVec<char> operator==(const TVec<T> &v0, const TVec<V> &v1)
{
   TVec<char> w;
   w.resize(v0.size());
   std::transform(v0.begin(), v0.end(), v1.begin(), w.begin(), [](const T &t, const V &v) { return t == v; });
   return w;
}

///@}

/** @name Math Functions
 *  Math functions on TVecs
*/
///@{
template <typename T>
auto sqrt(const TVec<T> &v) -> TVec<decltype(std::sqrt(v[0]))>
{
   TVec<decltype(std::sqrt(v[0]))> w(v.size());
   std::transform(v.begin(), v.end(), w.begin(), [](const T &t) { return std::sqrt(t); });
   return w;
}
///@}

/// Inner product
template <typename T, typename V>
auto Dot(const TVec<T> v0, const TVec<V> v1) -> decltype(v0[0] * v1[0])
{
   return std::inner_product(v0.begin(), v0.end(), v1.begin(), decltype(v0[0] * v1[0])(0));
}

template <class T>
std::ostream &operator<<(std::ostream &os, const TVec<T> &v)
{
   // In order to print properly, convert to 64 bit int if this is a char
   using Print_t = typename std::conditional<ROOT::Internal::VecOps::TIsChar<T>::value, long long int, T>::type;
   os << "{ ";
   auto size = v.size();
   if (size) {
      for (size_t i = 0; i < size - 1; ++i) {
         os << (Print_t)v[i] << ", ";
      }
      os << (Print_t)v[size - 1];
   }
   os << " }";
   return os;
}

} // End of VecOps NS

} // End of Experimental NS

} // End of ROOT NS

#include <sstream>

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
