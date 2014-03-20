#ifdef ROOT_VERSION

#include "conditions_data.cc"
#include "read_histograms.cc"
#include "cmn_fit.cc"
#include "bad_strips.cc"
#include "plotstyle.cc"
 
#else

#include "conditions_data.h"
#include "read_histograms.h"
#include "cmn_fit.h"
#include "bad_strips.h"
#include "plotstyle.h"

#endif

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
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

// for use in CINT as root script!

// back to per cbc version!
int fit_cmdata(std::string filename, std::string cbc)
{
	// decode conditions data
	conditions_data mycondata(filename);
	
	// read the number of inactive strips from the bad_strip file and fill a 
	bad_strips mybadstrips("/afs/cern.ch/user/g/gauzinge/tb_data/bad_strips.txt");
	
	std::cout << "Running CM Analysis for Run " << mycondata.runnumber() << " on CBC " << cbc << std::endl;
	
	// TApp fuffaround
	gROOT->Reset();
	set_plotstyle();
	gStyle->SetOptStat(1111111);
	
	// hardcoded!!! folder and histogramnames
	std::string foldername = "lowlevel";
	std::vector<std::string> histonames;
	
	if (cbc == "A")
	{
		histonames.push_back("h_n_hits_fix_t_A");
		histonames.push_back("h_n_hits_fix_b_A");
		histonames.push_back("h_n_hits_fix_A");
	}
	else
	{
		histonames.push_back("h_n_hits_fix_t_B");
		histonames.push_back("h_n_hits_fix_b_B");
		histonames.push_back("h_n_hits_fix_B");
	}
	
	// Canvas
	TCanvas* stackcanvas = new TCanvas("stackcanvas","Simulation vs. Measurement");
	stackcanvas->SetWindowSize(900,700);
	stackcanvas->SetCanvasSize(800,600);
	stackcanvas->Divide(2,2);
	
	// Legend
	TLegend* aLegend = new TLegend(0.0156, 0.396, 0.436, 0.691, "");
	aLegend->SetBorderSize(0);
	aLegend->SetFillColor(kWhite);
	aLegend->SetTextSize(0.07);
	
	// Save
	std::string resultfilename = "/afs/cern.ch/user/g/gauzinge/tb_data/results/cm_analysis/run" + mycondata.runstring() + "_" + cbc + "_cmAnalyis.root";
	TFile* resultfile =new TFile(resultfilename.c_str(),"RECREATE");
	
	unsigned int padcounter = 1;
	
	// histogram loop
	for (std::vector<std::string>::iterator histos =  histonames.begin(); histos != histonames.end(); histos++)
	{
		// get the Data histogram
		TH1D* datahisto = get_histogram(filename, foldername, *histos);
		datahisto->SetLineColor(LC(1));
		datahisto->SetLineWidth(2);
		
		TF1* fit = fitDistribution(datahisto, mybadstrips.n_active_strips(*histos));
		double threshold = fit->GetParameter(0);
		double cm_fraction = fit->GetParameter(1);
		
		TH1D* no_cm_histo = createNoiseDistribution(threshold, cm_fraction, datahisto->GetEntries(), mybadstrips.n_active_strips(*histos));
		
		std::string title;
		
		if(*histos == "h_n_hits_fix_t_A") title = "CM Noise: Data vs. Simulation top A";
		if(*histos == "h_n_hits_fix_b_A") title = "CM Noise: Data vs. Simulation bot A";
		if(*histos == "h_n_hits_fix_A") title = "CM Noise: Data vs. Simulation A";
		
		if(*histos == "h_n_hits_fix_t_B") title ="CM Noise: Data vs. Simulation top B";
		if(*histos == "h_n_hits_fix_b_B") title = "CM Noise: Data vs. Simulation bot B";
		if(*histos == "h_n_hits_fix_B") title = "CM Noise: Data vs. Simulation B";
		
		std::string histoname = *histos;
		
		resultfile->cd();
		
		stackcanvas->cd(padcounter);
		THStack* hs = new THStack(histoname.c_str(),title.c_str());
		hs->Add(datahisto,"AH");
		hs->Add(no_cm_histo,"AH");
		hs->Draw("nostack");
		fit->Draw("same");
		hs->GetXaxis()->SetTitle("# of Hits");
			
		if (padcounter == 1)
		{
			aLegend->AddEntry(datahisto,"Data","f");
			aLegend->AddEntry(no_cm_histo,"no CM noise","f");
		}
		if (padcounter < 4)
		{
			aLegend->AddEntry(fit,Form("Fit CM fraction %.2f", cm_fraction),"f");
		}
		stackcanvas->Update();
		padcounter++;
			
		// hsa->Write("",TObject::kOverwrite);
	}
	
	stackcanvas->cd(4);
	aLegend->Draw();
	stackcanvas->Update();
	
	
	stackcanvas->Write("",TObject::kOverwrite);
	resultfile->Close();
	
	std::string pdffilename =  "/afs/cern.ch/user/g/gauzinge/tb_data/results/cm_analysis/run" + mycondata.runstring() + "_" + cbc + "_cmAnalyis.pdf";
	stackcanvas->SaveAs(pdffilename.c_str());
	
	return 0;
}


void syntax(char* progname) 
{
     std::cout << "Syntax: " << progname << " Datafile CBC ('A' or 'B')" << std::endl;
}

//Standalone APP
// #ifndef ROOT_VERSION
#if !defined( __CINT__) && !defined (__MAKECINT__)
int main(int argc, char** argv)
{
	// syntax snippet, very useful
	if (argc!=3) 
	{
		syntax(argv[0]);
	}
	
	// some filename acrobatics for results etc.
	std::string filename=(std::string)argv[1];
	char cbc = *argv[2];
	
	// decode conditions data
	conditions_data mycondata(filename);
	
	// read the number of inactive strips from the bad_strip file and fill a 
	bad_strips mybadstrips("/afs/cern.ch/user/g/gauzinge/tb_data/bad_strips.txt");
	
	std::cout << "Running CM Analysis for Run " << mycondata.runnumber() << " on CBC " << cbc << std::endl;
	
	// TApp fuffaround
	gROOT->Reset();
	set_plotstyle();
	TApplication app("Root Application", &argc, argv);
	gStyle->SetOptStat(1111111);
	
	// hardcoded!!! folder and histogramnames
	std::string foldername = "lowlevel";
	std::vector<std::string> histonames;
	
	if (cbc == 'A')
	{
		histonames.push_back("h_n_hits_fix_t_A");
		histonames.push_back("h_n_hits_fix_b_A");
		histonames.push_back("h_n_hits_fix_A");
	}
	else
	{
		histonames.push_back("h_n_hits_fix_t_B");
		histonames.push_back("h_n_hits_fix_b_B");
		histonames.push_back("h_n_hits_fix_B");
	}
	
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
	aLegend->SetTextSize(0.07);
	
	// // Save
	// std::string resultfilename = "/afs/cern.ch/user/g/gauzinge/tb_data/results/cm_analysis/run" + mycondata.runstring() + "_" + cbc + "_cmAnalyis.root";
	// TFile* resultfile =new TFile(resultfilename.c_str(),"RECREATE");
	// 
	int padcounter = 1;
	
	// histogram loop
	for (std::vector<std::string>::iterator histos =  histonames.begin(); histos != histonames.end(); histos++)
	{
		// get the Data histogram
		TH1D* datahisto = get_histogram(filename, foldername, *histos);
		datahisto->SetLineColor(LC(1));
		datahisto->SetLineWidth(2);
		
		std::string title;
		
		if(*histos == "h_n_hits_fix_t_A") title = "CM Noise: Data vs. Simulation top A";
		if(*histos == "h_n_hits_fix_b_A") title = "CM Noise: Data vs. Simulation bot A";
		if(*histos == "h_n_hits_fix_A") title = "CM Noise: Data vs. Simulation A";
		
		if(*histos == "h_n_hits_fix_t_B") title ="CM Noise: Data vs. Simulation top B";
		if(*histos == "h_n_hits_fix_b_B") title = "CM Noise: Data vs. Simulation bot B";
		if(*histos == "h_n_hits_fix_B") title = "CM Noise: Data vs. Simulation B";
		
		std::string histoname = *histos;
		
		// resultfile->cd();
		
		stackcanvas->cd(padcounter);
		THStack* hs = new THStack(histoname.c_str(),title.c_str());
		hs->Add(datahisto,"AH");
		
		TF1* fit = fitDistribution(datahisto, mybadstrips.n_active_strips(*histos));
		fit->SetLineColor(LC(padcounter+7));
		fit->SetLineWidth(2);
		double threshold = fit->GetParameter(0);
		double cm_fraction = fit->GetParameter(1);
		
		TH1D* no_cm_histo = createNoiseDistribution(threshold, 0, datahisto->GetEntries(), mybadstrips.n_active_strips(*histos));
		no_cm_histo->SetLineColor(LC(11));
		
		hs->Add(no_cm_histo,"AH");
		hs->Draw("nostack");
		fit->Draw("same");
		hs->GetXaxis()->SetTitle("# of Hits");
			
		if (padcounter == 1)
		{
			aLegend->AddEntry(datahisto,"Data","f");
			aLegend->AddEntry(no_cm_histo,"no CM noise","f");
		}
		if (padcounter < 4)
		{
			aLegend->AddEntry(fit,Form("Fit CM fraction %.2f", fabs(cm_fraction)),"l");
		}
		stackcanvas->Update();
		padcounter++;
	}
	
	stackcanvas->cd(4);
	aLegend->Draw();
	stackcanvas->Update();
	
	
	// stackcanvas->Write("",TObject::kOverwrite);
	// resultfile->Close();
	
	std::string pdffilename =  "/afs/cern.ch/user/g/gauzinge/tb_data/results/cm_analysis/run" + mycondata.runstring() + "_" + cbc + "_cmAnalyis.pdf";
	stackcanvas->SaveAs(pdffilename.c_str());
	
	app.Run();
		
	return 0;
}

#endif