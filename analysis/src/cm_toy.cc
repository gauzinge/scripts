#include "cm_toy.h"

TH1D* cmnTest(int nEvents, double noiseSigma, double cmnSigma, std::string histoname, TH1D* thresholds)
{
	TRandom3 myDice;
	// const int nStrips=127;
	TH1D* hitCountHisto;
	 
	int hitCount;
	// hitCountHisto = new TH1D("hitCountHisto","Hit count", nStrips+1, -0.5, nStrips+0.5);
	hitCountHisto = new TH1D("hitCountHisto","Hit count", 127, 0, 127);
	
	double cmn;
	double stripValue;
	
	for (int i = 0; i < nEvents; ++i) 
	{
		cmn = myDice.Gaus(0,cmnSigma);
		hitCount = 0;
		
		//this loop has to go over the right strips of the right sensor
		int first_strip;
		int last_strip;
		
		if (histoname == "h_n_hits_dut_t_A" || histoname == "h_n_hits_fix_t_A") first_strip = 0, last_strip = 127;
		if (histoname == "h_n_hits_dut_t_B" || histoname == "h_n_hits_fix_t_B") first_strip = 127, last_strip = 254;
		if (histoname == "h_n_hits_dut_b_A" || histoname == "h_n_hits_fix_b_A") first_strip = 0, last_strip = 127;
		if (histoname == "h_n_hits_dut_b_B" || histoname == "h_n_hits_fix_b_B") first_strip = 127, last_strip = 254;
		
		//TODO: something wrong here!
		if (i == 0) std::cout << first_strip << " " << last_strip << std::endl;
		
		for (int j = first_strip; j < last_strip ; ++j) 
		{
			if (i == 0) std::cout << j << " " << thresholds->GetBinContent(j) << std::endl;
			if(thresholds->GetBinContent(j) > 0) // strip is not a bad strip
			// this ensures that i only loop over the active strips
			{
				stripValue = cmn + myDice.Gaus(0,noiseSigma);
				if (stripValue > thresholds->GetBinContent(j)) 
				{
					hitCount++;
				}
			}
		}
		hitCountHisto->Fill(hitCount);
	}
	return hitCountHisto;
}