#include "conditions_data.h"

conditions_data::conditions_data()
{
	run_number = 0;
	stubs_fix = 0;
	stubs_dut = 0;
	
	rotation_angle = 0;
	voltage_fix = 0;
	voltage_dut = 0;
	
	trigger_latency_dut = 0;
	trigger_latency_fix = 0;
	
	vcompth_dut = 0;
	vcompth_fix = 0;
	
	misc_param = 0;
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
	
	this->histonames.push_back("h_trg_latency_dut");
	this->histonames.push_back("h_trg_latency_fix");
	this->histonames.push_back("h_vcth_dut");
	this->histonames.push_back("h_vcth_fix");
	
	this->histonames.push_back("h_misc_parameter");
	
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
					this->n_events = temphisto->GetEntries(); // could be any histogram
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
				else if (*histos == histonames.at(4))
				{
					this->voltage_fix = temphisto->GetMean();
				}
				// TRG Latency; implement check for sigma different from 1 -> different settings for both CBCS -> Error
				else if (*histos == histonames.at(5))
				{
					if (temphisto->GetRMS() < 2)
					{
						this->trigger_latency_dut = temphisto->GetMean();
					}
					else std::cerr << "The CBCs on the DUT have different I2C values!" << std::endl;
				}
				else if (*histos == histonames.at(6))
				{
					if (temphisto->GetRMS() < 2)
					{
						this->trigger_latency_fix = temphisto->GetMean();
					}
					else std::cerr << "The CBCs on the DUT have different I2C values!" << std::endl;
				}
				
				// VCTH; implement check for sigma different from 1 -> different settings for both CBCS -> Error
				else if (*histos == histonames.at(7))
				{
					if (temphisto->GetRMS() < 2)
					{
						this->vcompth_dut = temphisto->GetMean();
					}
					else std::cerr << "The CBCs on the DUT have different I2C values!" << std::endl;
				}
				else if (*histos == histonames.at(8))
				{
					if (temphisto->GetRMS() < 2)
					{
						this->vcompth_fix = temphisto->GetMean();
					}
					else std::cerr << "The CBCs on the DUT have different I2C values!" << std::endl;
				}
				else if (*histos == histonames.at(9))
				{
					this->misc_param = temphisto->GetMean();
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

int conditions_data::nEvents()
{
	return this->n_events;
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

int conditions_data::trg_latency_dut()
{
	return this->trigger_latency_dut;
}

int conditions_data::trg_latency_fix()
{
	return this->trigger_latency_fix;
}

int conditions_data::vcth_dut()
{
	return this->vcompth_dut;
}

int conditions_data::vcth_fix()
{
	return this->vcompth_fix;
}

double conditions_data::vcth_dut_ke()
{
	// 1 vcth unit ^= 2.5mV
	// 45mV / fC
	// 1fC ^= 6241.5093 elementary charges
	return ((this->vcompth_dut * 2.5)/45) * 6.2415093; // in ke
}

double conditions_data::vcth_fix_ke()
{
	// 1 vcth unit ^= 2.5mV
	// 45mV / fC
	// 1fC ^= 6241.5093 elementary charges
	return ((this->vcompth_fix * 2.5)/45) * 6.2415093; // in ke
}

double conditions_data::misc_parameter()
{
	return this->misc_param;
}