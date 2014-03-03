#include <TColor.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TKey.h>
#include <TList.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TSystemDirectory.h>
#include <TPad.h>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <TRandom3.h>
#include <TH1I.h>
#include <vector>
#include <iostream>
#include <TLegend.h>
#include <THStack.h>
#include <TPaveStats.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#include "read_histograms.cc"

struct solution
{
	double threshold;
	double cm_fraction;
	TH1D* simhisto;
	TH1D* nocmhisto;
};


TH1D* cmnTest(int nStrips,
int nEvents,
double noiseSigma, // noise in ENC set 1	
double cmnSigma, // CM noise in ENC set relative to 1
double threshold, // in ENC, very to reproduce mean
bool doDraw=true) 
{
	TRandom3 myDice;
	// const int nStrips=127;
	TH1D* hitCountHisto;
  
	int hitCount;
	// hitCountHisto = new TH1D("hitCountHisto","Hit count", nStrips+1, -0.5, nStrips+0.5);
	hitCountHisto = new TH1D("hitCountHisto","Hit count", nStrips, 0, nStrips);
	double cmn;
	double stripValue;
	for (int i=0; i<nEvents; ++i) {
		cmn = myDice.Gaus(0,cmnSigma);
		hitCount=0;
		for (int j=0; j<nStrips; ++j) {
			stripValue=cmn+myDice.Gaus(0,noiseSigma);
			if (stripValue>threshold) {
				hitCount++;
			}
		}
		hitCountHisto->Fill(hitCount);
	}
	if (doDraw) {
		gStyle->SetOptStat(1111111);
		hitCountHisto->Draw();
	}
	return hitCountHisto;
	delete hitCountHisto;
}

solution fit_nhit_histo(std::string filename, std::string histoname)
{
	int nstrips;
	if (histoname == "h_n_hits_dut_A" || histoname == "h_n_hits_dut_B" || histoname == "h_n_hits_fix_A" || histoname == "h_n_hits_fix_B") nstrips = 254;
	else nstrips = 127;
	
	TH1D* datahist = get_histogram(filename, "low_level", histoname);
	double mean = datahist->GetMean();
	double sigma = datahist->GetRMS();
	int entries = datahist->GetEntries();
	
	TH1D* simhisto = NULL;
	double threshold = 3;
	double cm_fraction = 0.01;
	double simulated_mean = 0;
	double simulated_sigma = 0;
	
	while (!((fabs(mean - simulated_mean) < 0.2) && (fabs(sigma - simulated_sigma) < 0.1)))
	{
		simhisto = cmnTest(nstrips, entries,1,cm_fraction,threshold,false);
		simulated_mean = simhisto->GetMean();
		simulated_sigma = simhisto->GetRMS();
		std::cout << "threshold " <<threshold << " cm_fraction " << cm_fraction << " diff mean " << mean-simulated_mean << " diff sigma " << sigma-simulated_sigma << std::endl;
		if (simulated_mean < mean) threshold -= 0.01;
		else threshold += 0.01;
		if (simulated_sigma < sigma) cm_fraction += 0.01;
		else cm_fraction -= 0.01;
		// if (simulated_mean - mean > 1) break;
	}
	std::cout << "Found threshold value that reproduces the datahisto (thrshold/cm_fraction): " << threshold << "/" << cm_fraction << std::endl;
	
	simhisto->SetLineColor(2);
	TH1D* nocmhisto = cmnTest(nstrips, entries,1,0,threshold,false);
	nocmhisto->SetLineColor(3);
	
	solution aSolution;
	aSolution.threshold =  threshold;
	aSolution.cm_fraction = cm_fraction;
	aSolution.simhisto = simhisto;
	aSolution.nocmhisto = nocmhisto;
	
	return aSolution;
	
	// THStack* histo_comp = new THStack("histo_comp","Comon Mode Noise");
// 	histo_comp->Add(simhisto);
// 	histo_comp->Add(nocmhisto);
// 	return histo_comp;
}

void test() // think about options; i.e: sensor, chip, sensor/chip
{
	gStyle->SetOptStat(1111111);
	
	std::string filename = "data/run429_analysis.root";
	std::string histoname = "h_n_hits_fix_b_B";
	
	TH1D* datahisto = get_histogram(filename, histoname);
	solution thesolution = fit_nhit_histo(filename, histoname);
	
	TCanvas* stackcanvas = new TCanvas("stackcanvas","Simulation vs. Measurement");
    TLegend* aLegend = new TLegend(0.54, 0.58, 0.88, 0.875, "");
    aLegend->SetFillColor(kWhite);
	aLegend->AddEntry(datahisto,"Data","l");
	// TH1D* sim = (TH1D*)stack->GetStack()->FindObject("hitCountHisto");
	aLegend->AddEntry(thesolution.simhisto,Form("Simulation CM fraction %.2f", thesolution.cm_fraction),"l");
	aLegend->AddEntry(thesolution.nocmhisto,"no CM noise","l");
	
	stackcanvas->cd();
	// stack->Draw("nostack");
	THStack* hs = new THStack("hs","CM Noise: Data vs. Simulation");
	hs->Add(datahisto);
	hs->Add(thesolution.simhisto);
	hs->Add(thesolution.nocmhisto);
	hs->Draw("nostack");

	// stack.simhisto->Draw("same");
// 	stack.nocmhisto->Draw("same");
	aLegend->Draw("same");
	

}

void create_map(double max_trh) //FIXME, eventually tprofile?
{
	TCanvas* graphcanvas = new TCanvas("Graphs","Curves for fixed CM/Trsh");
	TMultiGraph* multi = new TMultiGraph();
	multi->SetTitle(";mean #mu; sigma #sigma");
	
	for(double i=0; i < 2; i+=0.1) // CM fraction
	{
		TGraph* tmpgr = new TGraph();
		for(double j=0; j < max_trh; j+=0.1) // threshold
		{
			TH1D* temp = cmnTest(1E5,1,j,i,false);
			tmpgr->SetPoint(tmpgr->GetN(),temp->GetMean(),temp->GetRMS());
			std::cout << i << " " << j << " mean " <<  temp->GetMean() << " rms " << temp->GetRMS() << std::endl;
		}
		multi->Add(tmpgr,"l");
	}
	
	graphcanvas->cd();
	multi->Draw("apl");
	graphcanvas->SaveAs("test.pdf");
}