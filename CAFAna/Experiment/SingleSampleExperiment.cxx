#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Experiment/CovMxChiSq.h"
#include "CAFAna/Experiment/CovMxChiSqPreInvert.h"
#include "CAFAna/Experiment/CovMxLL.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH2.h"

namespace ana
{
  //----------------------------------------------------------------------
  TMatrixD* SingleSampleExperiment::GetCov(const std::string& fname,
                                           const std::string& matname)
  {
    TDirectory* thisDir = gDirectory->CurrentDirectory();

    TFile f(fname.c_str());
    TMatrixD* ret = (TMatrixD*)f.Get(matname.c_str());

    if(!ret){
      std::cout << "Could not obtain covariance matrix named "
                << matname << " from " << fname << std::endl;
      abort();
    }

    thisDir->cd();

    return ret;
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data)
    : fMC(pred), fData(data), fCov(0)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TMatrixD* cov,
                                                 ETestStatistic stat)
    : fMC(pred), fData(data)
  {
    switch(stat){
    case kCovMxChiSq:
      fCov = new CovMxChiSq(EigenMatrixXdFromTMatrixD(cov));
      break;

    case kCovMxChiSqPreInvert:
      fCov = new CovMxChiSqPreInvert(EigenMatrixXdFromTMatrixD(cov),
                                     pred->Predict((osc::IOscCalc*)0).GetEigen(data.POT()));
      break;

    case kCovMxLogLikelihood:
      fCov = new CovMxLL(EigenMatrixXdFromTMatrixD(cov));
      break;

    default:
      std::cout << "Unknown test statistic: " << stat << std::endl;
      abort();
    }
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const std::string& covMatFilename,
                                                 const std::string& covMatName,
                                                 ETestStatistic stat)

    : SingleSampleExperiment(pred, data, GetCov(covMatFilename, covMatName), stat)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::~SingleSampleExperiment()
  {
    delete fCov;
  }

  //----------------------------------------------------------------------
  double SingleSampleExperiment::ChiSq(osc::IOscCalcAdjustable* calc,
                                       const SystShifts& syst) const
  {
    Eigen::ArrayXd apred = fMC->PredictSyst(calc, syst).GetEigen(fData.POT());
    Eigen::ArrayXd adata = fData.GetEigen(fData.POT());

    if(fCov){
      return fCov->ChiSq(apred, adata);
    }
    else{
      // No covariance matrix - use standard LL
      ApplyMask(apred, adata);

      // full namespace qualification to avoid degeneracy with method inherited from IExperiment
      return ana::LogLikelihood(apred, adata);
    }
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::ApplyMask(Eigen::ArrayXd& a,
                                         Eigen::ArrayXd& b) const
  {
    if(fMaskA.size() == 0) return;

    assert(a.size() == fMaskA.size());
    assert(b.size() == fMaskA.size());

    // Arrays mean we get bin-by-bin operations
    a *= fMaskA;
    b *= fMaskA;
  }

  //----------------------------------------------------------------------
  stan::math::var SingleSampleExperiment::LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                                        const SystShifts &syst) const
  {
    if(fCov){
      std::cout << "SingleSampleExperiment doesn't yet support the combination of covariance matrix and OscCalcStan" << std::endl;
      abort();
    }

    const Spectrum pred = fMC->PredictSyst(osc, syst);

    const Eigen::ArrayXd data = fData.GetEigen(fData.POT());

    // It's possible to have a non-stan prediction. e.g. from a NoOsc
    // prediction with no systs.
    if(pred.HasStan()){
      // fully-qualified so that we get the one in StanUtils.h
      //
      // LogLikelihood(), confusingly, returns chi2=-2*LL
      return ana::LogLikelihood(pred.GetEigenStan(fData.POT()), data) / -2.;
    }
    else{
      return ana::LogLikelihood(pred.GetEigen(fData.POT()), data) / -2.;
    }
  }


  //----------------------------------------------------------------------
  void SingleSampleExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("SingleSampleExperiment").Write("type");

    fMC->SaveTo(dir, "mc");
    fData.SaveTo(dir, "data");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<SingleSampleExperiment> SingleSampleExperiment::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "SingleSampleExperiment");
    delete ptag;

    const IPrediction* mc = ana::LoadFrom<IPrediction>(dir, "mc").release();
    const std::unique_ptr<Spectrum> data = Spectrum::LoadFrom(dir, "data");

    delete dir;

    auto ret = std::make_unique<SingleSampleExperiment>(mc, *data);
    return ret;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::SetMaskHist(double xmin, double xmax, double ymin, double ymax)
  {
    fMaskA = GetMaskArray(fData, xmin, xmax, ymin, ymax);
    if(fCov) fCov->SetMask(fMaskA);
  }
}
