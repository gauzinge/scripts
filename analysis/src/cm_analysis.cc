
#ifndef __CINT__
//Standard includes
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#include <TApplication.h>
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
#include <TRandom3.h>
#include <TH1D.h>
#include <TLegend.h>
#include <THStack.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#endif

#include "read_histograms.h"

// std::string filename = "/afs/cern.ch/user/g/gauzinge/tb_data/results/run429_results.root";


struct solution
{
	double threshold;
	double cm_fraction;
	TH1D* simhisto;
	TH1D* nocmhisto;
};


TH1D* cmnTest(int nStrips, // Function to simulate noisy N_hit histo
int nEvents,
double noiseSigma, // noise in ENC set 1	
double cmnSigma, // CM noise in ENC set relative to 1
double threshold) // in ENC, very to reproduce mean
{		
	TRandom3 myDice;
	// const int nStrips=127;
	TH1D* hitCountHisto;
  
	int hitCount;
	// hitCountHisto = new TH1D("hitCountHisto","Hit count", nStrips+1, -0.5, nStrips+0.5);
	hitCountHisto = new TH1D("hitCountHisto","Hit count", nStrips, 0, nStrips);
	double cmn;
	double stripValue;
	for (int i=0; i<nEvents; ++i) 
	{
		cmn = myDice.Gaus(0,cmnSigma);
		hitCount=0;
		for (int j=0; j<nStrips; ++j) 
		{
			stripValue=cmn+myDice.Gaus(0,noiseSigma);
			if (stripValue>threshold) 
			{
				hitCount++;
			}
		}
		hitCountHisto->Fill(hitCount);
	}
	return hitCountHisto;
}

solution fit_nhit_histo(TH1D* datahist) // Match Sim to Data
{
	int nstrips = datahist->GetNbinsX();
	// if (histoname == "h_n_hits_dut_A" || histoname == "h_n_hits_dut_B" || histoname == "h_n_hits_fix_A" || histoname == "h_n_hits_fix_B") nstrips = 254;
	// else nstrips = 127;
	// TH1D* datahist = get_histogram(filename, foldername, histoname);
	
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
		simhisto = cmnTest(nstrips, entries,1,cm_fraction,threshold);
		simulated_mean = simhisto->GetMean();
		simulated_sigma = simhisto->GetRMS();
		std::cout << "THRESHOLD " <<threshold << " | diff mean " << mean-simulated_mean <<  " | CM FRACTION " << cm_fraction  << " | diff sigma " << sigma-simulated_sigma << std::endl;
		
		double mean_step;
		double sigma_step;
		
		if (fabs(mean-simulated_mean) > 2) mean_step = 0.1;
		else mean_step = 0.005;
		if (fabs(sigma-simulated_sigma) > 2) sigma_step = 0.1;
		else sigma_step = 0.005;
		
		if (simulated_mean < mean) threshold -= mean_step;
		else threshold += mean_step;
		if (simulated_sigma < sigma) cm_fraction += sigma_step;
		else cm_fraction -= sigma_step;
		
		delete gDirectory->FindObject("hitCountHisto");
	}
	
	std::cout << std::endl;
	std::cout << "***************************************************************************************************************" << std::endl;
	std::cout << "Found threshold value that reproduces the datahisto (thrshold/cm_fraction): " << threshold << "/" << cm_fraction << std::endl;
	std::cout << "***************************************************************************************************************" << std::endl;
	std::cout << std::endl;
	
	TH1D* foundhisto = cmnTest(nstrips, entries,1,cm_fraction,threshold);
	TH1D* nocmhisto = cmnTest(nstrips, entries,1,0,threshold);
	
	solution aSolution;
	aSolution.threshold =  threshold;
	aSolution.cm_fraction = cm_fraction;
	aSolution.simhisto = foundhisto;
	aSolution.nocmhisto = nocmhisto;
		
	return aSolution;
}



void syntax(char* progname) {
     std::cout << "Syntax: " << progname << " Datafile" << std::endl;
}

//Standalone APP
#ifndef __CINT__


int main(int argc, char** argv)
{
	// syntax snippet, very useful
    if (argc!=2) {
        syntax(argv[0]);
    }
	
	// some filename acrobatics for results etc.
    std::string filename=(std::string)argv[1];
	unsigned help = filename.find_last_of("_");
	std::string run_no_string = filename.substr(help-3,3);
	
	std::cout << "Running CM Analysis for Run " << run_no_string << std::endl;
	
	// TApp fuffaround
    gROOT->Reset();
    TApplication app("Root Application", &argc, argv);
	gStyle->SetOptStat(1111111);
	
	// hardcoded!!! folder and histogramnames
	std::string foldername = "lowlevel";
	std::vector<std::string> histonames;
	
	histonames.push_back("h_n_hits_fix_t_B");
	histonames.push_back("h_n_hits_fix_b_B");
	histonames.push_back("h_n_hits_fix_B");
	
	// Canvas
	TCanvas* stackcanvas = new TCanvas("stackcanvas","Simulation vs. Measurement");
	stackcanvas->SetWindowSize(900,700);
	stackcanvas->SetCanvasSize(800,600);
	stackcanvas->Connect("Closed()","TApplication", &app,"Terminate()");
	stackcanvas->Divide(2,2);
	
	// Legend
	TLegend* aLegend = new TLegend(0.0156, 0.396, 0.436, 0.691, "");
	aLegend->SetBorderSize(0);
	aLegend->SetFillColor(kWhite);
	
	int padcounter = 1;
	
	// histogram loop
	for (std::vector<std::string>::iterator histos =  histonames.begin(); histos != histonames.end(); histos++)
	{
		TH1D* datahisto = get_histogram(filename, foldername, *histos);
		solution thesolution = fit_nhit_histo(datahisto);

		datahisto->SetLineColor(2);
		thesolution.simhisto->SetLineColor(padcounter+8);
		thesolution.nocmhisto->SetLineColor(3);
		stackcanvas->cd(padcounter);
		std::string title;
		
		if(*histos == "h_n_hits_fix_t_B") title ="CM Noise: Data vs. Simulation top B";
		if(*histos == "h_n_hits_fix_b_B") title = "CM Noise: Data vs. Simulation bot B";
		if(*histos == "h_n_hits_fix_B") title = "CM Noise: Data vs. Simulation B";
		
		if(*histos == "h_n_hits_fix_t_A") title = "CM Noise: Data vs. Simulation top A";
		if(*histos == "h_n_hits_fix_b_A") title = "CM Noise: Data vs. Simulation bot A";
		if(*histos == "h_n_hits_fix_A") title = "CM Noise: Data vs. Simulation A";
		
		THStack* hs = new THStack("hs",title.c_str());
		hs->Add(datahisto,"AH");
		hs->Add(thesolution.simhisto,"AH");
		hs->Add(thesolution.nocmhisto,"AH");
		hs->Draw("nostack");
		hs->GetXaxis()->SetTitle("# of Hits");

		if (padcounter == 1)
		{
			aLegend->AddEntry(datahisto,"Data","f");
			aLegend->AddEntry(thesolution.nocmhisto,"no CM noise","f");
		}
		if (padcounter < 4)
		{
			thesolution.simhisto->SetLineColor(padcounter+3);
			aLegend->AddEntry(thesolution.simhisto,Form("Simulation CM fraction %.2f", thesolution.cm_fraction),"f");
		}
		stackcanvas->Update();
		padcounter++;
	}
	stackcanvas->cd(4);
	aLegend->Draw();
	stackcanvas->Update();
	
	// Save
	std::string resultfilename = "/afs/cern.ch/user/g/gauzinge/tb_data/results/run" + run_no_string + "_cmAnalyis.root";
	std::string pdffilename =  "/afs/cern.ch/user/g/gauzinge/tb_data/results/run" + run_no_string + "_cmAnalyis.pdf";
	stackcanvas->SaveAs(resultfilename.c_str());
	stackcanvas->SaveAs(pdffilename.c_str());
	
	app.Run();
	
	return 0;
}
#endif


// void cbc_cm(std::string filename) // think about options; i.e: sensor, chip, sensor/chip
// {
// 	gStyle->SetOptStat(1111111);
// 	
// 	std::string foldername = "lowlevel";
// 	std::vector<std::string> histonames;
// 	
// 	histonames.push_back("h_n_hits_fix_t_B");
// 	histonames.push_back("h_n_hits_fix_b_B");
// 	histonames.push_back("h_n_hits_fix_B");
// 	
// 	TCanvas* stackcanvas = new TCanvas("stackcanvas","Simulation vs. Measurement");
// 	stackcanvas->Connect("Closed()","TApplication", &app,"Terminate()");
// 	stackcanvas->Divide(1,3);
// 	int padcounter = 1;
// 	
// 	for (std::vector<std::string>::iterator histos =  histonames.begin(); histos != histonames.end(); histos++)
// 	{
// 		TH1D* datahisto = get_histogram(filename, foldername, *histos);
// 		solution thesolution = fit_nhit_histo(datahisto);
// 
// 		stackcanvas->cd(padcounter);
// 		THStack* hs = new THStack("hs","CM Noise: Data vs. Simulation");
// 		hs->Add(datahisto);
// 		hs->Add(thesolution.simhisto);
// 		hs->Add(thesolution.nocmhisto);
// 		hs->Draw("nostack");
// 
// 		// stack.simhisto->Draw("same");
// 		// 	stack.nocmhisto->Draw("same");
// 		TLegend* aLegend = new TLegend(0.54, 0.58, 0.88, 0.875, "");
// 		aLegend->SetFillColor(kWhite);
// 		aLegend->AddEntry(datahisto,"Data","l");
// 		aLegend->AddEntry(thesolution.simhisto,Form("Simulation CM fraction %.2f", thesolution.cm_fraction),"l");
// 		aLegend->AddEntry(thesolution.nocmhisto,"no CM noise","l");
// 		aLegend->Draw("same");
// 		padcounter++;
// 	}
// }

// void create_map(double max_trh) //FIXME, eventually tprofile?
// {
// 	TCanvas* graphcanvas = new TCanvas("Graphs","Curves for fixed CM/Trsh");
// 	TMultiGraph* multi = new TMultiGraph();
// 	multi->SetTitle(";mean #mu; sigma #sigma");
// 	
// 	for(double i=0; i < 2; i+=0.1) // CM fraction
// 	{
// 		TGraph* tmpgr = new TGraph();
// 		for(double j=0; j < max_trh; j+=0.1) // threshold
// 		{
// 			TH1D* temp = cmnTest(1E5,1,j,i,false);
// 			tmpgr->SetPoint(tmpgr->GetN(),temp->GetMean(),temp->GetRMS());
// 			std::cout << i << " " << j << " mean " <<  temp->GetMean() << " rms " << temp->GetRMS() << std::endl;
// 		}
// 		multi->Add(tmpgr,"l");
// 	}
// 	
// 	graphcanvas->cd();
// 	multi->Draw("apl");
// 	graphcanvas->SaveAs("test.pdf");
// }