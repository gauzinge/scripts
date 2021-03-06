#ifdef ROOT_VERSION

#include "conditions_data.cc"
#include "read_histograms.cc"
#include "cmn_fit.cc"
#include "bad_strips.cc"
#include "plotstyle.cc"

#else

#include "hit_prob.h"

#endif

#ifndef hit_prob_cc_
#define hit_prob_cc_

// void hit_prob(std::string filename)
// {
// 	// decode conditions data
// 	conditions_data mycondata(filename);
// 	
// 	// read the number of inactive strips from the bad_strip file and fill a 
// 	bad_strips mybadstrips("/afs/cern.ch/user/g/gauzinge/tb_data/bad_strips.txt");
// 	
// 	gROOT->Reset();
// 	set_plotstyle();
// 	gStyle->SetOptStat(1111111);
// 	
// 	// hardcoded!!! folder and histogramnames
// 	std::string foldername = "lowlevel";
// 	std::vector<std::string> histonames;
// 	
// 	histonames.push_back("h_hits_FIX_b");
// 	histonames.push_back("h_hits_FIX_t");
// 	
// 	TCanvas* profilecanvas = new TCanvas("profilecanvas","Normalized Hit Probability");
// 	profilecanvas->Divide(1,histonames.size());
// 	
// 	TCanvas* thresholdcanvas = new TCanvas("thresholdcanvas","Normalized Threshold");
// 	thresholdcanvas->Divide(1,histonames.size());
// 	int padcounter = 1;
// 	
// 	for(std::vector<std::string>::iterator histos = histonames.begin(); histos != histonames.end(); histos++)
// 	{
// 		TH1D* hithisto = get_histogram(filename, foldername, *histos);
// 		hithisto->SetMarkerStyle(2);
// 		hithisto->SetMarkerSize(.6);
// 		
// 		std::string name = *histos + "_threshold";
// 		
// 		TH1D* thresholdhisto = new TH1D(name.c_str(),"Normalized Threshold (wrt. Noise)",hithisto->GetNbinsX()+1,-0.5,254.5);
// 		thresholdhisto->SetMarkerColor(LC(2));
// 		thresholdhisto->SetMarkerStyle(2);
// 		thresholdhisto->SetMarkerSize(.6);
// 		
// 		
// 		if (mycondata.nEvents())
// 		{
// 			hithisto->Scale(1/static_cast<double>(mycondata.nEvents()));
// 			profilecanvas->cd(padcounter);
// 			hithisto->Draw("P");
// 			
// 			for (int bins = 0; bins <= hithisto->GetNbinsX()+1; bins++)
// 			{
// 				if (hithisto->GetBinContent(bins) > 0)
// 				{
// 					thresholdhisto->SetBinContent(bins, inverse_hitProbability(hithisto->GetBinContent(bins)));
// 				}
// 			}
// 			thresholdcanvas->cd(padcounter);
// 			thresholdhisto->Draw("P");
// 		} 
// 		else std::cerr << "Histogram Empty!" << std::endl;
// 		
// 		padcounter++;
// 	}
// }

TH1D* get_hit_prob(std::string filename, std::string histoname)
{
	// decode conditions data
	conditions_data mycondata(filename);
	
	// read the number of inactive strips from the bad_strip file and fill a 
	bad_strips mybadstrips("/afs/cern.ch/user/g/gauzinge/tb_data/bad_strips.txt");
	
	// hardcoded!!! folder and histogramnames
	std::string foldername = "lowlevel";
	
	TH1D* hithisto = get_histogram(filename, foldername, histoname);
		
	std::string name = histoname + "_threshold";
		
	TH1D* thresholdhisto = new TH1D(name.c_str(),"Normalized Threshold (wrt. Noise)",hithisto->GetNbinsX()+1,-0.5,254.5);
		
	if (mycondata.nEvents())
	{
		hithisto->Scale(1/static_cast<double>(mycondata.nEvents()));
			
		for (int bins = 0; bins <= hithisto->GetNbinsX()+1; bins++)
		{
			if (hithisto->GetBinContent(bins) > 0)
			{
				thresholdhisto->SetBinContent(bins, inverse_hitProbability(hithisto->GetBinContent(bins)));
			}
		}
		return thresholdhisto;
	} 
	else 
	{
		std::cerr << "Histogram Empty!" << std::endl;
		return NULL;
	}
}

#endif