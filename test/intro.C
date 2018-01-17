
#ifdef __CLING__
#include "../inc/ROOT/TVec.hxx"
#else
#include "ROOT/TVec.hxx"
#endif

#include "TRandom.h"
#include <ROOT/TDataFrame.hxx>

using namespace ROOT::Experimental;
using namespace ROOT::Experimental::VecOps;

template <typename T>
void Print(const TVec<T> &v)
{
   std::cout << "Size: " << v.size() << " " << v << std::endl;
}

void intro()
{

   std::cout << "Many operations on TVec<T> equal to {0,1,2,3}" << std::endl;

   std::cout << "\nWe start from some constructors" << std::endl;
   {
      std::cout << "Initialiser list ctor:" << std::endl;
      TVec<float> v0{0, 1, 2, 3};
      Print(v0);

      std::cout << "Size ctor:" << std::endl;
      TVec<int> v1(4);
      Print(v1);
   }

   std::cout << "\nNow some ops" << std::endl;
   {
      std::cout << "Sum with scalar (3):" << std::endl;
      TVec<float> v0{0, 1, 2, 3};
      auto res0 = v0 + 3;
      Print(res0);

      std::cout << "Division by scalar (3.):" << std::endl;
      TVec<int> v1{0, 1, 2, 3};
      auto res1 = v1 / 3.;
      Print(res1);

      std::cout << "Greater than a scalar (2, note the return type, TVec<int>):" << std::endl;
      TVec<double> v2{0, 1, 2, 3};
      auto res2 = v2 > 2.;
      Print(res2);
   }

   std::cout << "\nNow some vecops!" << std::endl;
   {
      std::cout << "Sum with TVec ({4,5,6,7}):" << std::endl;
      TVec<float> v00{0, 1, 2, 3};
      TVec<float> v01{4, 5, 6, 7};
      auto res0 = v00 + v01;
      Print(res0);

      std::cout << "Multiplication by TVec ({4,5,6,7}):" << std::endl;
      auto res00 = v00 * v01;
      Print(res00);

      std::cout << "Division by TVec ({4,5,6,7}):" << std::endl;
      TVec<int> v10{0, 1, 2, 3};
      auto res1 = v10 / v01;
      Print(res1);

      std::cout << "Greater than a TVec ({1,1,2,2}, note the return type, TVec<int>):" << std::endl;
      TVec<float> v20{1, 1, 2, 2};
      auto res2 = v00 > v20;
      Print(res2);

      std::cout << "Dot of 2 TVecs of different type:" << std::endl;
      TVec<int> v30{0, 1, 2, 3};
      TVec<float> v31{0, 1, 2, 3};
      auto res3 = Dot(v30, v31);
      std::cout << res3 << std::endl;

      std::cout << "Square root of a TVec:" << std::endl;
      TVec<float> v40{0, 1, 2, 3};
      auto res4 = sqrt(v40);
      std::cout << res4 << std::endl;
   }

   {
      std::cout << "Something intriguing:" << std::endl;
      TVec<float> v0{1, 2, 3};
      TVec<char> v1{7, 8, 9};
      TVec<int> v2{3, 3, 4};
      std::cout << "v0 = " << v0 << std::endl;
      std::cout << "v1 = " << v1 << std::endl;
      std::cout << "v2 = " << v2 << std::endl;
      std::cout << "v0 + 1 = " << v0 + 1 << std::endl;
      std::cout << "v1 - v2 = " << v1 - v2 << std::endl;
      std::cout << "(v1 - v2) / 3. = " << (v1 - v2) / 3. << std::endl;
      std::cout << "v0 + 1 + (v1 - v2) / 3. = " << v0 + 1 + (v1 - v2) / 3. << std::endl;
      std::cout << "(v0 + 1 + (v1 - v2) / 3.) > 4  " << ((v0 + 1 + (v1 - v2) / 3.) > 4) << std::endl;
   }

   {
      std::cout << "Now using a TTreeReaderArray to initialise, mimic what would happen in a TDF" << std::endl;
      // Ideally: tdf.Define("pt", "sqrt(px*px + py*py)").Histo1D("pt"); or, now not possible in TDF,
      // tdf.Histo1D("sqrt(px*px + py*py)")

      auto rndmVector = []() {
         std::vector<double> v(8);
         for (auto &&e : v) {
            e = gRandom->Gaus();
         }
         return v;
      };
      TDataFrame tdf(8);
      tdf.Define("px", rndmVector)
         .Define("py", rndmVector)
         .Snapshot<std::vector<double>,std::vector<double>>("t", "dataset.root", {"px", "py"});

      auto f = TFile::Open("dataset.root");
      TTreeReader myReader("t", f);
      TTreeReaderArray<double> px(myReader, "px");
      TTreeReaderArray<double> py(myReader, "py");

      // So far so good. Now the serious stuff
      TH1F h("myhisto", "The Histo", 64, 0, 2);
      while (myReader.Next()) {
         auto pxpp = (double**)px.GetAddress();
         auto pypp = (double**)py.GetAddress();
         ROOT::Detail::VecOps::TVecAllocator<double> allpx(*pxpp, px.GetSize());
         ROOT::Detail::VecOps::TVecAllocator<double> allpy(*pypp, py.GetSize());
         const TVec<double> pxv(px.GetSize(), double(), allpx);
         const TVec<double> pyv(py.GetSize(), double(), allpy);
         std::cout << pxv << " " << pyv << std::endl;
         std::cout << pxv*pyv << std::endl;
      }
   }
}

int main() {intro();return 0;}
