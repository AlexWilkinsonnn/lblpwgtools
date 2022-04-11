#pragma once

#include "CAFAnaCore/CAFAna/Core/Binning.h"

namespace ana
{
  /// Default true-energy bin edges
  Binning TrueEnergyBins();
  /// No entries for underflow or overflow
  std::vector<double> TrueEnergyBinCenters();

  /// Default true-energy bin edges
  const Binning kTrueEnergyBins = TrueEnergyBins();
  /// No entries for underflow or overflow
  const std::vector<double> kTrueEnergyBinCenters = TrueEnergyBinCenters();
  
  /// LBL and ND analyses binnings
  const std::vector<double> kBinEdges = {0.,  0.5,  1.,  1.25, 1.5, 1.75,
                                   2.,  2.25, 2.5, 2.75, 3.,  3.25,
                                   3.5, 3.75, 4.,  5.,   6.,  10.};
  const std::vector<double> kYBinEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 1.0};
  const std::vector<double> kV3BinEdges = {0.,  0.75, 1.,  1.25, 1.5, 1.75, 2., 2.25,
                                      2.5, 2.75, 3.,  3.25, 3.5, 3.75, 4., 4.25,
                                      4.5, 5.,   5.5, 6.,   7.,  8.,   10.};
  const std::vector<double> kYV3BinEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0};

  const std::vector<double> kPRISMBinEdges = {0.,  0.5,  1.,  1.25, 1.5, 1.75,
                                              2.,  2.25, 2.5, 2.75, 3.,  3.25,
                                              3.5, 3.75, 4.,  5.,   6.,  10., 15.};
  const std::vector<double> kHadBinEdges = {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 
                                            0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 
                                            1.125, 1.25, 1.5, 2., 6.};
  /*const std::vector<double> kHadBinEdges = {0., 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07,
                                            0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15,
                                            0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23,
                                            0.24, 0.25, 0.26, 0.27, 0.28, 0.29, 0.3, 0.31, 
                                            0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39, 
                                            0.4, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 
                                            0.48, 0.49, 0.5, 0.52, 0.54, 0.56, 0.58, 0.6,
                                            0.62, 0.64, 0.66, 0.68, 0.7, 0.72, 0.74, 0.76,
                                            0.78, 0.8, 0.82, 0.84, 0.86, 0.88, 0.9, 0.92,
                                            0.94, 0.96, 0.98, 1.0, 1.05, 1.1, 1.15, 1.2,
                                            1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6, 
                                            1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 
                                            2.25, 2.5, 2.75, 3.0, 3.5, 4., 6.};*/

  const std::vector<double> kLepBinEdges = {0., 0.5, 0.75, 1., 1.25, 1.5, 1.625, 1.75,  
                                            1.875, 2., 2.125, 2.25, 2.375, 2.5, 2.75, 3.,
                                            3.25, 3.5, 4., 6., 10.};

  const std::vector<double> kLinearCombBinEdges = {0., 0.25, 0.5, 0.75, 1., 1.25, 1.5,
                                                   1.75, 2., 2.25, 2.5, 2.75, 3., 3.25,
                                                   3.5, 3.75, 4., 4.25, 4.5, 4.75, 5.,
                                                   5.25, 5.5, 5.75, 6., 6.5, 7., 7.5,
                                                   8., 8.5, 9., 9.5, 10., 11., 12., 13.,
                                                   14., 15., 17.5, 20., 60., 100.};

  /// Both Nue and Numu use the same binning at the moment
  /// Eventually define as k{ND,FD}{Nue,Numu}Binning
  const Binning kFDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kNDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kPRISMRecoBinning = Binning::Custom(kPRISMBinEdges);
  const Binning kHadRecoBinning = Binning::Custom(kHadBinEdges);
  const Binning kLepRecoBinning = Binning::Custom(kLepBinEdges);
  const Binning kLinearCombBinning = Binning::Custom(kLinearCombBinEdges);
  const Binning kFDRecoV3Binning = Binning::Custom(kV3BinEdges);
  const Binning kNDRecoV3Binning = Binning::Custom(kV3BinEdges);
  const Binning kNDRecoOABinning = Binning::Simple(20, 0, 4);
  const Binning kYBinning = Binning::Custom(kYBinEdges);
  const Binning kYV3Binning = Binning::Custom(kYV3BinEdges);
  const Binning kTrueBinning = Binning::Simple(100, 0, 10);
  const Binning kTrueCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoVeryCoarseBinning = Binning::Simple(5, 0, 10);
  const Binning kOneBinBinning = Binning::Simple(1, 0, 10);
}
