#include "conditions_data.h"

conditions_data::conditions_data()
{
	run_number = 0;
	stubs_fix = 0;
	stubs_dut = 0;
	
	rotation_angle = 0;
	voltage_fix = 0;
	voltage_dut = 0;
}

conditions_data::conditions_data(std::string filename)
{
	// initialize
	this->foldername = "conditions";
	this->histonames.push_back("h_stub_dut");
	this->histonames.push_back("h_stub_fix");
	this->histonames.push_back("h_angle_dut");
	this->histonames.push_back("h_hv_dut");
	this->histonames.push_back("h_hv_fix");
	
	TFile* fileIn = TFile::Open(filename.c_str(), "READ");
	if (fileIn) 
	{
		fileIn->cd(this->foldername.c_str());
		for (std::vector<std::string>::iterator histos = this->histonames.begin(); histos != this->histonames.end(); histos++)
		{
			TH1D* temphisto = (TH1D*)gDirectory->Get((*histos).c_str());
			if (!temphisto) 
			{
				std::cerr << "Failed to read histogram!" << *histos << std::endl;
			}
			else if (temphisto->GetEntries() == 0)
			{
				std::cerr << "The histogram " << *histos << " is empty!" << std::endl;
			}
			else // sanity checks done. Now retrieve value from histogram according to name
			{
				if (*histos == histonames.at(0)) // #stubs dut
				{
					this->stubs_dut = temphisto->GetBinContent(2); // no info on how many stubs per event but on #of events w. stubs
				}
				else if (*histos == histonames.at(1)) // #stubs fix
				{
					this->stubs_fix = temphisto->GetBinContent(2); // no info on how many stubs per event but on #of events w. stubs
				}
				else if (*histos == histonames.at(2))
				{
					this->rotation_angle = temphisto->GetMean();
				}
				else if (*histos == histonames.at(3))
				{
					this->voltage_dut = temphisto->GetMean();
				}
				else if (*histos == histonames.at(3))
				{
					this->voltage_fix = temphisto->GetMean();
				}
			}
		}
		// some filename acrobatics for results etc.
		unsigned help = filename.find_last_of("_");
		std::string run_no_string = filename.substr(help-5,5); 
		std::stringstream ss (run_no_string);
		ss >> this->run_number;
	}
	else std::cerr << "Could not open file " << filename << std::endl; 
}

conditions_data::~conditions_data()
{
}

int conditions_data::runnumber()
{
	return this->run_number;
}

std::string conditions_data::runstring()
{
	std::stringstream ss;
	ss << this->run_number;
	return ss.str();
}

int conditions_data::n_stubs_fix()
{
	return this->stubs_fix;
}

int conditions_data::n_stubs_dut()
{
	return this->stubs_dut;
}

double conditions_data::angle()
{
	return this->rotation_angle;
}

double conditions_data::hv_fix()
{
	return this->voltage_fix;
}

double conditions_data::hv_dut()
{
	return this->voltage_dut;
}