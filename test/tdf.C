#include "ROOT/TVec.hxx"

#include "TCanvas.h"
#include "TRandom.h"
#include <ROOT/TDataFrame.hxx>

using namespace ROOT::Experimental;
using namespace ROOT::Experimental::VecOps;
using namespace ROOT::Detail::VecOps;

template<typename T>
TVec<T> ToTVec(const std::vector<T> &v)
{
   TVecAllocator<T> alloc(v.data(), v.size());
   TVec<T> tvec(v.size(), 0, alloc);
   return std::move(tvec);
}

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
   auto df = tdf.Define("Muon_pt", rndmVector).Define("Muon_eta", rndmVector);

   auto calcQuant = [](const std::vector<double>& v0, const std::vector<double>& v1) {
      // Boilerplate to absorb in TDF proper
      auto tvec0 = ToTVec(v0);
      auto tvec1 = ToTVec(v1);
      // That's what will stay
      return sqrt(tvec0 * tvec1);
   };

   auto h = df.Define("quant", calcQuant, {"Muon_pt","Muon_eta"}).Histo1D<TVec<double>>("quant");

   TCanvas c;
   h->Draw();
   c.Print("myHist.png");

/*
   ROOT-8865
   Draw("Muon_pt","Muon_eta[0] > 1")
   Draw("Muon_pt[0]","Muon_eta[0] > 1")
   Draw("Muon_pt[1]","Muon_eta[0] > 1")
   Draw("Muon_pt[0]","Sum$(Muon_pt*(Muon_eta > 1)) > 30")
   Draw("Muon_pt","Sum$(Muon_pt*(Muon_eta > 1)) > 30")
   Draw("Muon_pt","Muon_eta> 1")
*/
   using doubles = std::vector<double>;
   // ROOT-8865
   // Will evolve in the jitted version (VecOps are not known to the interpreter for now)
   // Draw("Muon_pt","Muon_eta[0] > 1")
   auto h0 = df.Define("q0", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), w[0] > 1);}, {"Muon_pt","Muon_eta"}).Histo1D<TVec<double>>("q0");
   // Draw("Muon_pt[0]","Muon_eta[0] > 1")
   auto h1 = df.Define("q1", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), w[0] > 1)[0];}, {"Muon_pt","Muon_eta"}).Histo1D<double>("q1");
   // Draw("Muon_pt[1]","Muon_eta[0] > 1")
   auto h2 = df.Define("q2", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), w[0] > 1)[1];}, {"Muon_pt","Muon_eta"}).Histo1D<double>("q2");
   // Draw("Muon_pt[0]","Sum$(Muon_pt*(Muon_eta > 1)) > 30")
   auto h3 = df.Define("q3", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), Sum(ToTVec(v)*(ToTVec(w)>1)) > 30)[0];}, {"Muon_pt","Muon_eta"}).Histo1D<double>("q3");
   // Draw("Muon_pt","Sum$(Muon_pt*(Muon_eta > 1)) > 30")
   auto h4 = df.Define("q4", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), Sum(ToTVec(v)*(ToTVec(w)>1)) > 30);}, {"Muon_pt","Muon_eta"}).Histo1D<TVec<double>>("q4");
   // Draw("Muon_pt","Muon_eta> 1")
   auto h5 = df.Define("q5", [](const doubles &v,const doubles &w){ return Filter(ToTVec(v), ToTVec(w) > 1);}, {"Muon_pt","Muon_eta"}).Histo1D<TVec<double>>("q5");
}

int main() {tdf();return 0;}
