#include <gtest/gtest.h>
#include <ROOT/TVec.hxx>
#include <vector>

TEST(VecOps, DefaultCtor)
{
   ROOT::Experimental::VecOps::TVec<int> v;
   EXPECT_EQ(v.size(), 0u);
}

TEST(VecOps, InitListCtor)
{
   ROOT::Experimental::VecOps::TVec<int> v{1,2,3};
   EXPECT_EQ(v.size(), 3u);
   // FIXME uncomment when operator[] is fixed
   //EXPECT_EQ(v[0], 1);
   //EXPECT_EQ(v[1], 2);
   //EXPECT_EQ(v[2], 3);
}

TEST(VecOps, CopyCtor)
{
   ROOT::Experimental::VecOps::TVec<int> v1{1,2,3};
   ROOT::Experimental::VecOps::TVec<int> v2(v1);
   EXPECT_EQ(v1.size(), 3u);
   EXPECT_EQ(v2.size(), 3u);
   // FIXME uncomment when operator[] is fixed
   //EXPECT_EQ(v2[0], 1);
   //EXPECT_EQ(v2[1], 2);
   //EXPECT_EQ(v2[2], 3);
}

TEST(VecOps, MoveCtor)
{
   ROOT::Experimental::VecOps::TVec<int> v1{1,2,3};
   ROOT::Experimental::VecOps::TVec<int> v2(std::move(v1));
   EXPECT_EQ(v1.size(), 0u);
   EXPECT_EQ(v2.size(), 3u);
}

TEST(VecOps, CopyStdVector)
{
   std::vector<double> v{1., 2., 3.};
   ROOT::Experimental::VecOps::TVec<double> vec(v);
   EXPECT_EQ(vec.size(), 3u);
   // FIXME uncomment when operator[] is fixed
   //EXPECT_DOUBLE_EQ(vec[0], 1.);
   //EXPECT_DOUBLE_EQ(vec[1], 2.);
   //EXPECT_DOUBLE_EQ(vec[2], 3.);
}

TEST(VecOps, MoveStdVector)
{
   std::vector<double> stdv{1., 2., 3.};
   ROOT::Experimental::VecOps::TVec<double> tv(std::move(stdv));
   EXPECT_EQ(stdv.size(), 0u);
   EXPECT_EQ(tv.size(), 3u);
}
