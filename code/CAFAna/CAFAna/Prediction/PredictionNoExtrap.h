#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana
{
  class Loaders;

  class DUNERunPOTSpectrumLoader;

  /// Prediction that just uses FD MC, with no extrapolation
  class PredictionNoExtrap: public PredictionExtrap
  {
  public:
    PredictionNoExtrap(PredictionExtrap* pred);

    // This is the DUNE constructor
    PredictionNoExtrap(DUNERunPOTSpectrumLoader& loaderBeam,
                       DUNERunPOTSpectrumLoader& loaderNue,
                       DUNERunPOTSpectrumLoader& loaderNuTau,
                       DUNERunPOTSpectrumLoader& loaderNC,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,                       
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(DUNERunPOTSpectrumLoader& loaderBeam,
                       DUNERunPOTSpectrumLoader& loaderNue,
                       DUNERunPOTSpectrumLoader& loaderNuTau,
                       DUNERunPOTSpectrumLoader& loaderNC,
		       const HistAxis& axis,
		       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);


    PredictionNoExtrap(Loaders& loaders,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(Loaders& loaders,
                       const HistAxis& axis,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loader,
                       SpectrumLoaderBase& loaderSwap,
                       SpectrumLoaderBase& loaderTau,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loader,
                       SpectrumLoaderBase& loaderSwap,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    virtual ~PredictionNoExtrap();

    static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const override;

  };

  class DUNENoExtrapPredictionGenerator: public IPredictionGenerator
  {
  public:
    DUNENoExtrapPredictionGenerator(DUNERunPOTSpectrumLoader& loaderBeam, 
                                    DUNERunPOTSpectrumLoader& loaderNue,
                                    DUNERunPOTSpectrumLoader& loaderNuTau,
                                    DUNERunPOTSpectrumLoader& loaderNC,
                                    HistAxis axis,
                                    Cut cut)
      : fLoaderBeam(loaderBeam), fLoaderNue(loaderNue),
        fLoaderNuTau(loaderNuTau), fLoaderNC(loaderNC),
        fAxis(axis), fCut(cut)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift) const override
    {
      return std::unique_ptr<IPrediction>(
        new PredictionNoExtrap(fLoaderBeam, fLoaderNue, fLoaderNuTau, fLoaderNC,
                               fAxis, fCut, shiftMC));
    }
  protected:
    DUNERunPOTSpectrumLoader& fLoaderBeam;
    DUNERunPOTSpectrumLoader& fLoaderNue;
    DUNERunPOTSpectrumLoader& fLoaderNuTau;
    DUNERunPOTSpectrumLoader& fLoaderNC;
    HistAxis fAxis;
    Cut fCut;
  };
}