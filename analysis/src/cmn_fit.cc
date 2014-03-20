#include "cmn_fit.h"

double findMaximum(TH1D* histogram)
{
	int maxbin = histogram->GetMaximumBin();
	return histogram->GetXaxis()->GetBinCenter(maxbin);
}

double hitProbability(double threshold) 
{
	return 0.5-(TMath::Erf(threshold/sqrt(2))/2); 
	// area above threshold under the gaussian curve. 
	// The Factors are to only treat the positive half
	// 1-erf(x/sqrt(2)/2 + .5)
}

double inverse_hitProbability(double probability)
{
	// the inverse of the above function!
	return sqrt(2)*TMath::ErfInverse(1-2*probability);
}

double binomialPdf(int n, int k, double p) 
{
	return TMath::Binomial(n,k)*pow(p,k)*pow((1-p), n-k);
}

double hitProbFunction(double *xStrips, Double_t *par) 
{
	const double samplingHalfStep = sigmaRangeCMN/nSamplingsCMN;
	
	double& threshold     = par[0];
	double& cmnFraction   = par[1];
	double& nEvents       = par[2];
	double& nActiveStrips = par[3];
	
	double result = 0;
	double hitProb;
	double sampleProbability, x;
	
	int iStrips = ceil(*xStrips-0.5); // round to nearest integer
	if ((iStrips < 0)||(iStrips > nActiveStrips)) return 0; // only defined in range
	
	for (int j = 0; j < nSamplingsCMN; ++j) 
	{
		// loop over all x values
		x = -sigmaRangeCMN+j*2*samplingHalfStep;
		
		// approximate probability at sampling point by interpolating
		sampleProbability = hitProbability(x-samplingHalfStep);
		sampleProbability -= hitProbability(x+samplingHalfStep);
		
		// probability of hit taking cmn into account
		hitProb = hitProbability(threshold+x*cmnFraction);
		// distribution function scaled to nevents 
		result += binomialPdf(nActiveStrips, iStrips, hitProb)*sampleProbability*nEvents;
	}
	return result;  
}


TF1* fitDistribution(TH1D* hitCountHisto, int nActiveStrips) 
{
	// First-order approximation
	double prob = findMaximum(hitCountHisto)/ hitCountHisto->GetNbinsX();
	
	// retrieve the threshold from the maximum of the actual nhit distribution
	double threshold = inverse_hitProbability(prob);
	std::cout << "Threshold " << threshold << std::endl;
	// initialize cmnFraction to 0 anc later extract from fit
	double cmnFraction=0;
	
	// define TF1* for the fit
	// TF1(name, function, minimum, maximum, n_parameters)
	TF1 *fitProbability = new TF1("fitProbability", hitProbFunction, 0, nActiveStrips,4);
	
	// Name Parameters
	fitProbability->SetParName(0, "threshold");
	fitProbability->SetParName(1, "cmnFraction");
	fitProbability->SetParName(2, "nEvents");
	fitProbability->SetParName(3, "nActiveStrips");
	
	// Set Parameters
	fitProbability->SetParameter(0, threshold);
	fitProbability->SetParameter(1, cmnFraction);
	
	// Fix Parameters nEvents & nActiveStrips as these I know
	fitProbability->FixParameter(2, hitCountHisto->GetEntries());
	fitProbability->FixParameter(3, nActiveStrips);
	
	// std::cout << fitProbability->GetParameter(0) << " " << fitProbability->GetParameter(1) << " " << fitProbability->GetParameter(2) << " " << fitProbability->GetParameter(3) << " " << std::endl;
	
	// Fit and return
	hitCountHisto->Fit(fitProbability,"RQMN");
	std::cout << fitProbability->GetParameter(0) << " " << fitProbability->GetParameter(1) << " " << fitProbability->GetParameter(2) << " " << fitProbability->GetParameter(3) << " " << std::endl;
	
	return fitProbability;
}

TH1D* createNoiseDistribution(double threshold, double cmnFraction, int nEvents, int nActiveStrips)
{
	// create an analytical noise distribution from the hit probability function with any parameters
	TH1D* noiseDistribution = new TH1D("noiseDistribution", "Noise hit distributtion", nActiveStrips+1, -0.5, nActiveStrips+0.5);
	
	double par[4];
	par[0] = threshold;
	par[1] = cmnFraction;
	par[2] = nEvents;
	par[3] = nActiveStrips;
	double x;
	
	for (int i = 0; i <= nActiveStrips; ++i) 
	{
		x=i;
		noiseDistribution->Fill(x,hitProbFunction(&x, par));
	}
	return noiseDistribution;
}
