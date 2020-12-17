#include "CAFAna/Atmos/AtmosOscSpect.h"

#include "CAFAna/Core/Binning.h" // kTrueEnergyBins
#include "CAFAna/Core/OscCurve.h"

namespace ana
{
  const double kRefBaseline = 1000;

  //----------------------------------------------------------------------
  AtmosOscSpect::AtmosOscSpect(const LabelsAndBins& recoAxis)
    : ReweightableSpectrum(recoAxis, LabelsAndBins("1000 # times True E / L", kTrueEnergyBins))
  {
    fMat.resize(kTrueEnergyBins.NBins()+2, recoAxis.GetBins1D().NBins()+2);
    fMat.setZero();
  }

  // TODO copy constructors et al handling fCache

  //----------------------------------------------------------------------
  Spectrum AtmosOscSpect::Oscillated(osc::IOscCalc* calc, int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && fCache->hash && *hash == *fCache->hash){
      delete hash;
      return fCache->spect;
    }

    ((osc::IOscCalcAdjustable*)calc)->SetL(kRefBaseline);

    const OscCurve curve(calc, from, to);
    const Spectrum ret = WeightedBy(curve);
    if(hash){
      fCache->spect = ret;
      fCache->hash.reset(hash);
    }

    return ret;
  }
}
