#ifndef cmn_fit_h_
#define cmn_fit_h_

#include <TMath.h> 
#include <TH1D.h>
#include <TF1.h>
#include <iostream>

// Some global constants
const double nSamplingsCMN = 100;
const double sigmaRangeCMN = 6;
const double normGaus = 3.98942280401432703e-01;
 // 1/sqrt(2*PI()) for normalizing Area of Gaussian to 1

// find max of Histogram
double findMaximum(TH1D* histogram);

// probability of having a hit above threshold in normalized Gaus
double hitProbability(double threshold); 

// inverse of the above to extract threshold from probability
double inverse_hitProbability(double probability);

// binomial Pdf
double binomialPdf(int n, int k, double p); 

// Analytical function for the CMN affected nHit distribution
double hitProbFunction(double *xStrips, Double_t *par); 

// TF1 of the latter for Fit
TF1* fitDistribution(TH1D* hitCountHisto, int nActiveStrips = 127); 

// create analytical Distribution with any parameters
TH1D* createNoiseDistribution(double threshold, double cmnFraction=0, int nEvents=1, int nActiveStrips=127);

#endif