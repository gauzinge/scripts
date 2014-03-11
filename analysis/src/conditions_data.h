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
	
	int trg_latency_dut();
	int trg_latency_fix();
	
	int vcth_dut();
	int vcth_fix();
	
	double vcth_dut_ke();
	double vcth_fix_ke();
	
private:
	std::string foldername;
	std::vector<std::string> histonames;
	int run_number;
	int stubs_fix;
	int stubs_dut;
	
	double rotation_angle;
	double voltage_fix;
	double voltage_dut;

	int trigger_latency_dut;
	int trigger_latency_fix;
	
	int vcompth_dut;
	int vcompth_fix;
};

#endif