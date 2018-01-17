#include <ROOT/TVecAllocator.hxx>

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
using namespace ROOT::Detail::VecOps;

TEST(TVecAllocator, ReusePointer)
{
   std::vector<double> vreference{1, 2, 3,4,5,6,7,8,9,10};

   std::vector<double> vmodel (vreference);
   TVecAllocator<double> alloc0(vmodel.data(), vmodel.size());

   EXPECT_EQ(vmodel.data(), alloc0.allocate(123));

   TVecAllocator<double> alloc1(vmodel.data(), vmodel.size());

   std::vector<double, TVecAllocator<double>> v(alloc1);
   v.resize(vmodel.size());

   EXPECT_EQ(vmodel.data(), v.data());
   EXPECT_EQ(vmodel.size(), v.size());

   for (size_t i=0; i < vmodel.size(); ++i) {
      EXPECT_EQ(vmodel[i],vreference[i]);
      EXPECT_EQ(v[i],vreference[i]);
   }

   v.emplace_back(3.);
   EXPECT_NE(vmodel.data(), v.data());

   auto res = 0;
   try {
      v.resize(-1);
   } catch (...) {
      res = 1;
   }
   EXPECT_EQ(1, res);
}
