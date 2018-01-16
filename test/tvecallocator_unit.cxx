#include <ROOT/TVecAllocator.hxx>

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
using namespace ROOT::Detail::VecOps;

TEST(TVecAllocator, ReusePointer)
{
   std::vector<double> vmodel{1, 2, 3};
   TVecAllocator<double> alloc0(vmodel.data(), vmodel.size());

   EXPECT_EQ(vmodel.data(), alloc0.allocate(123));

   TVecAllocator<double> alloc1(vmodel.data(), vmodel.size());

   std::vector<double, TVecAllocator<double>> v(vmodel.size(), double(), alloc1);

   EXPECT_EQ(vmodel.data(), v.data());

   for (auto i : {0, 1, 2}) {
      EXPECT_EQ(vmodel[i], v.at(i));
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
