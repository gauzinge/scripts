// little root script to produce a plot showing the efficiency (#stubs/#total triggers) over different latency settings
// now, the latency<->run number conversion is done with a lookup map and the affected runs are manually pushed in a vector of strings
// in Lab operation, the FE1 is the FIX module with module ID 51010(51011, 51012)
// G. Auzinger, 03/2014
#ifdef ROOT_VERSION

#include "src/conditions_data.cc"
#include "src/read_histograms.cc"
#include "src/plotstyle.cc"
 
#else

#include "src/conditions_data.h"
#include "src/read_histograms.h"
#include "src/plotstyle.h"

#endif

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include <TGraph.h>
#include <TH1D.h>
#include <TCanvas.h>



// these are ugly global variables but need to find an automated way to associate run nr and scan setting, histogram name and histogram directory
std::string path = "/afs/cern.ch/user/g/gauzinge/tb_data/results/";
std::string histoname = "n_stubs_fix";
std::string directoryname = "stubs";

void latency_analysis ()
{
	set_plotstyle();
	// map and vector of filenames, run numbes and latency settings

	std::map<int,int> run_vs_latency;
	std::vector<std::string> filenames;

	run_vs_latency[603]=12;
	run_vs_latency[605]=13;
	run_vs_latency[606]=14;
	run_vs_latency[607]=15;
	run_vs_latency[608]=16;
	run_vs_latency[609]=17;

	filenames.push_back("run00603_results.root");
	filenames.push_back("run00605_results.root");
	filenames.push_back("run00606_results.root");
	filenames.push_back("run00607_results.root");
	filenames.push_back("run00608_results.root");
	filenames.push_back("run00609_results.root");
	
	// Fill a TGraph with the number of Stubs vs. Latency (nothin fancy yet!)
	TGraph* latencygraph = new TGraph();
		
	for (std::vector<std::string>::iterator files =  filenames.begin(); files != filenames.end(); files++)
	{
		// get from n_stub histo the total number of events + the number of found stubs (bin > 1)
		std::string filename = path + *files;
		
		// read conditions data
		conditions_data mycondata(filename);
		
		TH1D* histo = get_histogram(filename,directoryname, histoname);
		// histo->ClearUnderflowAndOverflow(); // to avoid confusion
		
		int n_events = histo->GetEntries(); // total # od events
		int n_events_wo_stubs = histo->GetBinContent(1); // events w. 0 stubs
		int n_stubs = n_events - n_events_wo_stubs;
		double efficiency = n_stubs / (double)n_events;
		
		std::cout << "Run: " << mycondata.runnumber() << " Latency: " << run_vs_latency[mycondata.runnumber()] << " Efficiency: " << efficiency*100 << "%" << std::endl;
		latencygraph->SetPoint(latencygraph->GetN(),run_vs_latency[mycondata.runnumber()],efficiency);
	}
	TCanvas* latencycanvas = new TCanvas("latencycanvas","N_stubs vs. Latency");
	latencycanvas->cd();
	
	latencygraph->SetTitle("Efficiency vs. Latency");
	latencygraph->SetMarkerColor(2);
	latencygraph->SetMarkerStyle(2);
	latencygraph->SetMarkerSize(2);
	latencygraph->GetXaxis()->SetTitle("Latency [BX]");
	latencygraph->GetYaxis()->SetTitle("N_stubs/N_events");
	latencygraph->GetYaxis()->SetTitleOffset(1.2);
	
	latencygraph->Draw("AP");
	
	std::string resultfile = path + "latencyscan.root";
	std::string resultfile_pdf = path + "latencyscan.pdf";
	latencycanvas->SaveAs(resultfile.c_str());
	latencycanvas->SaveAs(resultfile_pdf.c_str());
}