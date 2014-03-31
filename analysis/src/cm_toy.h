#ifndef cm_toy_h__
#define cm_toy_h__

#include <TH1D.h>
#include <TRandom3.h>

#include "bad_strips.h"

#define FIX_T 51011
#define FIX_B 51012
#define DUT_T 51001
#define DUT_B 51002

TH1D* cmnTest(int nEvents, double noiseSigma, double cmnSigma, std::string histoname, TH1D* tresholds);

#endif