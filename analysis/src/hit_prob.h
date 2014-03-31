#ifndef hit_prob_h__
#define hit_prob_h__


#include "conditions_data.h"
#include "read_histograms.h"
#include "cmn_fit.h"
#include "bad_strips.h"
#include "plotstyle.h"


#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <TH1D.h>
#include <TProfile.h>
#include <TCanvas.h>



void hit_prob(std::string filename);

TH1D* get_hit_prob(std::string filename, std::string histoname);

#endif 