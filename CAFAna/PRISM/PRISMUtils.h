#pragma once

#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/OscCalcFwdDeclare.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/IOscCalc.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <vector>

#define PRISMDEBUG

#ifdef PRISMDEBUG
#define PRISMOUT(a) std::cout << a << std::endl;
#else
#define PRISMOUT(a)
#endif

namespace ana {

class FVMassCorrection {
  size_t fOverflow;

public:
  TH1D *fvmasscor;

  FVMassCorrection();
  double GetWeight(double vtx_x_cm);
};

inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

ana::Weight NDSliceCorrection(double reference_width_cm, std::vector<double> const &Edges);

extern const ana::Weight kMassCorrection;

template <typename T>
inline void FillWithNulls(std::vector<std::unique_ptr<T>> &v, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    v.emplace_back(nullptr);
  }
}

template <typename T>
inline void FillWithNulls(std::vector<std::shared_ptr<T>> &v, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    v.emplace_back(nullptr);
  }
}

template <typename T>
inline void FillWithNulls(std::vector<std::vector<std::shared_ptr<T>>> &v, 
                          size_t n1, size_t n2) {
  for (size_t i = 0; i < n1; ++i) {
    std::vector<std::shared_ptr<T>> tmp;
    for (size_t j = 0; j < n2; ++j) {
      tmp.emplace_back(nullptr);
    }
    v.emplace_back(tmp);
  }
}

struct PRISMStateBlob {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> SelPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixPredInterps; //unique_ptr
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixPredInterps; //unique_ptr
  // For MC Eff Correction
  std::vector<std::unique_ptr<PredictionInterp>> NDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDSelTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDSelTruePredInterps;
  //----------------------
  std::vector<std::unique_ptr<PredictionInterp>> FarDetPredInterps;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nonswap;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nueswap;
  std::vector<std::unique_ptr<DataPredictionNoExtrap>> FarDetDataPreds;

  std::unique_ptr<PredictionInterp> NDFluxPred_293kA_nu;
  std::unique_ptr<PredictionInterp> NDFluxPred_293kA_nub;
  std::unique_ptr<PredictionInterp> NDFluxPred_280kA_nu;
  std::unique_ptr<PredictionInterp> NDFluxPred_280kA_nub;
  std::unique_ptr<PredictionInterp> FDFluxPred_293kA_nu;
  std::unique_ptr<PredictionInterp> FDFluxPred_293kA_nub;

  bool Have(size_t pc) {
    bool IsNu = PRISM::IsNuConfig(pc);
    bool IsND = PRISM::IsNDConfig(pc);
    size_t fd_pc = 0;
    size_t IsNue = PRISM::IsNueConfig(pc);
    if (!IsND) {
      fd_pc = PRISM::GetFDConfig(pc);
    }

    // Don't need MatchPredInterps for Nue (they aren't made/used)
    return PRISM && (IsNue || bool(MatchPredInterps[pc])) &&
           bool(SelPredInterps[pc]) &&
           (IsND || (bool(FarDetPredInterps[fd_pc]) &&
                     bool(FarDetData_nonswap[fd_pc]) &&
                     bool(FarDetData_nueswap[fd_pc])));
  }

  void Init() {
    FillWithNulls(MatchPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(SelPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDMatrixPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDMatrixPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(NDUnselTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDSelTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDUnselTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FDSelTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nonswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nueswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetDataPreds, PRISM::kNPRISMFDConfigs);

    NDFluxPred_293kA_nu = nullptr;
    NDFluxPred_293kA_nub = nullptr;
    NDFluxPred_280kA_nu = nullptr;
    NDFluxPred_280kA_nub = nullptr;
    FDFluxPred_293kA_nu = nullptr;
    FDFluxPred_293kA_nub = nullptr;
  }
};

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname);

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname, T *ty) {
  PRISMOUT("Saving " << ty << "to " << dirname);
  if (!ty) {
    abort();
  }

  f.cd();
  TDirectory *dir = gDirectory;
  
  ty->SaveTo(dir, dirname.c_str());

}

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname,
                   std::unique_ptr<T> &ty) {
  PRISMOUT("Saving " << ty.get() << "to " << dirname);
  if (!ty) {
    abort();
  }

  f.cd();
  TDirectory *dir = gDirectory;

  ty->SaveTo(dir, dirname.c_str());
}

osc::IOscCalcAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalcAdjustable *icalc = nullptr);

double GetCalcValue(osc::IOscCalcAdjustable *icalc = nullptr,
                    std::string paramname = "");

std::vector<const ana::IFitVar *>
GetOscVars(std::vector<std::string> const &osc_vars, int hie = 1, int oct = 0);
void ScrubOscVars(std::vector<const IFitVar *> &oscvars,
                  std::vector<std::string> const &names_to_scrub);

SystShifts GetSystShifts(fhicl::ParameterSet const &ps);
std::vector<ana::ISyst const *>
GetListOfSysts(std::vector<std::string> const &);

// Function to take a HistAxis with potentially multiple binnings
// and convert e.g. 2 binnings into 1 binning. Useful for the smearing matrix
// axes and covariance matrix axes, where each axis could be for multiple variables.
HistAxis GetMatrixAxis(const std::vector<HistAxis> &axisvec);

HistAxis GetTwoDAxis(const HistAxis &axis1, const HistAxis &axis2);

inline ReweightableSpectrum ToReweightableSpectrum(Spectrum const &spec, 
                                                   double POT, HistAxis const &axis) {
  Eigen::MatrixXd spec_mat;

  if (spec.NDimensions() == 2) {
    int NRows = spec.GetBinnings().at(1).NBins();
    int NCols = spec.GetBinnings().at(0).NBins();
    spec_mat.resize(NRows, NCols);
    Eigen::ArrayXd spec_arr = spec.GetEigen();
    std::cout << "arr size = " << spec_arr.size() <<
      ", mat size = " << NRows * NCols << std::endl;
    for (int row = 0; row < NRows; row++ ) {
      for (int col = 0; col < NCols; col++ ) {
        //if ((col + row * NCols == 0) || (col + row * NCols == (spec_arr.size() - 1))) continue;
        spec_mat(row, col) = spec_arr(col + row * NCols);     
      }
    }
  } /*else if (spec.NDimensions() == 3) {
    TH3 *spec3d_h = spec.ToTH3(POT);
    // Reweighting axis binning
    const Binning rwbins = Binning::FromTAxis(spec3d_h->GetZaxis());
    // analysis axis put on to 1D
    Binning xbins = axis.GetBinnings()[0]; 
    int n = 1;
    for (const Binning &b : axis.GetBinnings()) {
    //for (const Binning &b : spec.GetBinnings()) {
      n *= b.NBins();
      xbins = Binning::Simple(n, 0, n);
    }

    spec_h = HistCache::NewTH2D("", xbins, rwbins);
    for (int xit = 1; xit <= spec3d_h->GetYaxis()->GetNbins(); xit++) { // EHad

      for (int zit = 1; zit <= spec3d_h->GetZaxis()->GetNbins(); zit++) { // RWvar
        // Get projection of ELep axis
        TH1D *projY = spec3d_h->ProjectionY("", xit, xit, zit, zit);
        // fill 2D hist with ELep*EHad x-axis
        int NbinsY = projY->GetXaxis()->GetNbins();
        for (int yit = 1; yit <= NbinsY; yit++) {
          spec_h->SetBinContent(yit + ((xit - 1) * NbinsY), 
                                zit, 
                                projY->GetBinContent(yit));
        }
        HistCache::Delete(projY);
      }
    }
  } else {
    std::cout << "[ERROR] Not 2D or 3D, check input dimensions" << std::endl;
    abort();
  }*/ 

  LabelsAndBins anaAxis = LabelsAndBins(spec.GetLabels().at(0), spec.GetBinnings().at(0));
  LabelsAndBins weightAxis = LabelsAndBins(spec.GetLabels().at(1), spec.GetBinnings().at(1));

  ReweightableSpectrum rwspec(std::move(spec_mat), anaAxis, weightAxis, POT, 0);

  return rwspec;
}

inline Spectrum ToSpectrum(ReweightableSpectrum const &rwspec, double pot) {
  Eigen::MatrixXd mat = rwspec.GetEigen(pot);
  int NCols = mat.cols();
  int NRows = mat.rows();

  Eigen::ArrayXd arr(NCols * NRows);
  for (int row = 0; row < NRows; row++ ) {
    for (int col = 0; col < NCols; col++ ) {
      arr(col + row * NCols) = mat(row, col);
    }
  }

  std::vector<std::string> labels = rwspec.GetLabels();
  std::vector<Binning> bins = rwspec.GetBinnings();
  for (auto const &tb : rwspec.GetTrueBinnings()) {
    bins.push_back(tb);
    labels.push_back("");
  } 

  LabelsAndBins spec_LBs(labels, bins);

  Spectrum ret(std::move(arr), spec_LBs, pot, 0);
  return ret;  
}

} // namespace ana
