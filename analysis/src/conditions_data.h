#ifndef conditions_data_h_
#define conditions_data_h_

#include <TROOT.h>
#include <TKey.h>
#include <TList.h>
#include <TFile.h>
#include <TH1D.h>
#include <TSystemDirectory.h>
#include <vector>
#include <iostream>
#include <sstream>

class conditions_data{
public: 
	conditions_data();
	conditions_data(std::string filename);
	~conditions_data();
	
	int runnumber();
	std::string runstring();
	int n_stubs_fix();
	int n_stubs_dut();
	double angle();
	double hv_fix();
	double hv_dut();
private:
	std::string foldername = "conditions";
	std::vector<std::string> histonames {"h_stub_dut","h_stub_fix","h_angle_dut","h_hv_dut","h_hv_fix"};
	int run_number;
	int stubs_fix;
	int stubs_dut;
	
	double rotation_angle;
	double voltage_fix;
	double voltage_dut;
};

#endif