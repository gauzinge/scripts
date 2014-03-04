#ifndef read_histograms_h_
#define read_histograms_h_

#include <TROOT.h>
#include <TKey.h>
#include <TList.h>
#include <TFile.h>
#include <TH1D.h>
#include <TSystemDirectory.h>
#include <vector>
#include <iostream>


TH1D* get_histogram(std::string filename, std::string foldername, std::string histoname);

#endif