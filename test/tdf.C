#include "ROOT/TVec.hxx"

#include "TCanvas.h"
#include "TRandom.h"
#include <ROOT/TDataFrame.hxx>

using namespace ROOT::Experimental;
using namespace ROOT::Experimental::VecOps;
using namespace ROOT::Detail::VecOps;

void tdf()
{

   auto rndmVector = []() {
      std::vector<double> v(8);
      for (auto &&e : v) {
         e = gRandom->Gaus();
      }
      return v;
   };

   TDataFrame tdf(8);
   auto df = tdf.Define("px", rndmVector).Define("py", rndmVector);

   auto calcQuant = [](const std::vector<double>& v0, const std::vector<double>& v1) {
      // Boilerplate to absorb in TDF proper
      TVecAllocator<double> alloc0(v0.data(), v0.size());
      TVec<double> tvec0(v0.size(), 0, alloc0);
      TVecAllocator<double> alloc1(v1.data(), v1.size());
      TVec<double> tvec1(v1.size(), 0, alloc1);

      // That's what will stay
      return sqrt(tvec0 * tvec1);
   };

   auto h = df.Define("quant", calcQuant, {"px","py"}).Histo1D<TVec<double>>("quant");

   TCanvas c;
   h->Draw();
   c.Print("myHist.png");

}

int main() {tdf();return 0;}
