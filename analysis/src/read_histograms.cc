#include "read_histograms.h"


TH1D* get_histogram(std::string filename, std::string foldername, std::string histoname)
{
	TFile* fileIn = TFile::Open(filename.c_str(), "READ");
	if (fileIn) 
	{
		fileIn->cd(foldername.c_str());
		TH1D* temphisto = (TH1D*)gDirectory->Get(histoname.c_str());
		if (!temphisto) 
		{
			std::cerr << "Failed to read histogram " << histoname << std::endl;
			return NULL;
		}
		else
		{
			return temphisto;
			fileIn->Close();
		}
	} 
	else 
	{
		std::cerr << "Could not open File " << filename << std::endl;
		return NULL;
	}
	return NULL;
}

