#include "bad_strips.h"

bad_strips::bad_strips(std::string bad_strip_file)
{	
	//read the file with bad strips
	std::ifstream mask(bad_strip_file.c_str());
	if (!mask) std::cout << "No bad strip file " << bad_strip_file << " found, continuing wihtout!" << std::endl;
	while(!mask.eof())
	{
		std::string line;
		std::getline(mask, line);
		std::stringstream linestream(line);
		std::istream_iterator<int> begin(linestream), eos;
		int module;
		if (begin == eos) 
		{
			module = *begin;
			std::set<int> bad_strip_set;
			this->bad_strip_map[module] = bad_strip_set;
		}
		else
		{
			module = *begin;
			begin++;
			std::set<int> bad_strip_set (begin, eos);
			this->bad_strip_map[module] = bad_strip_set;
		}
	}
	
	int dut_n_masked_top_A = 0;
	int dut_n_masked_bot_A = 0;
	int dut_n_masked_A = 0;
	    
	int dut_n_masked_top_B = 0;
	int dut_n_masked_bot_B = 0;
	int dut_n_masked_B = 0;
	
	int fix_n_masked_top_A = 0;
	int fix_n_masked_bot_A = 0;
	int fix_n_masked_A = 0;
	
	int fix_n_masked_top_B = 0;
	int fix_n_masked_bot_B = 0;
	int fix_n_masked_B = 0;
	
	for (std::map< int, std::set<int> >::iterator map_it = bad_strip_map.begin(); map_it != bad_strip_map.end(); map_it++)
	{
		std::cout << "Reading bad strips for Sensor " << map_it->first << " : ";
		
		for (std::set<int>::iterator it = map_it->second.begin(); it != map_it->second.end(); it++)
		{
			std::cout << *it << " ";
			// DUT
			if (*it < 127 && map_it->first == 51001) dut_n_masked_top_A++, dut_n_masked_A++;// cbc A on top sensor
			if (*it >= 127 && map_it->first == 51001) dut_n_masked_top_B++, dut_n_masked_B++;// cbc B on top sensor
			
			if (*it < 127 && map_it->first == 51002) dut_n_masked_bot_A++, dut_n_masked_A++;// cbc A on top sensor
			if (*it >= 127 && map_it->first == 51002) dut_n_masked_bot_B++, dut_n_masked_B++;// cbc B on top sensor
			
			// FIX
			if (*it < 127 && map_it->first == 51011) fix_n_masked_top_A++, fix_n_masked_A++;// cbc A on top sensor
			if (*it >= 127 && map_it->first == 51011) fix_n_masked_top_B++, fix_n_masked_B++;// cbc B on top sensor
			
			if (*it < 127 && map_it->first == 51012) fix_n_masked_bot_A++, fix_n_masked_A++;// cbc A on top sensor
			if (*it >= 127 && map_it->first == 51012) fix_n_masked_bot_B++, fix_n_masked_B++;// cbc B on top sensor
		}
		std::cout << std::endl;
		
		// for (int i = 0; i < 127; i++)
// 		{
// 			if (map_it->second.find(i) != map_it->second.end()) // strip i is not a bad strip
// 			{
// 				this->good_strip_map["h_n_hits_dut_t_A"].insert(i);
// 				this->good_strip_map["h_n_hits_dut_t_B"].insert(i+127); // strips > 127 connected to CBC B
// 				this->good_strip_map["h_n_hits_dut_b_A"].insert(i);
// 				this->good_strip_map["h_n_hits_dut_b_B"].insert(i+127); // strips > 127 connected to CBC B
// 			}
// 		}
// 		
// 		for (int i = 0; i < 254; i++)
// 		{
// 			if (map_it->second.find(i) != map_it->second.end()) // strip i is not a bad strip
// 			{
// 				this->good_strip_map["h_n_hits_dut_A"]  
// 				this->good_strip_map["h_n_hits_dut_B"]  
// 			}
// 
// 		
// 		}
	}
	
	// fill the map string vs active strips
	this->active_strips["h_n_hits_dut_t_A"] = 127-dut_n_masked_top_A;
	this->active_strips["h_n_hits_dut_t_B"] = 127-dut_n_masked_top_B;
	this->active_strips["h_n_hits_dut_b_A"] = 127-dut_n_masked_bot_A;
	this->active_strips["h_n_hits_dut_b_B"] = 127-dut_n_masked_bot_B;
	this->active_strips["h_n_hits_dut_A"] = 254-dut_n_masked_A;
	this->active_strips["h_n_hits_dut_B"] = 254-dut_n_masked_B;
	
	this->active_strips["h_n_hits_fix_t_A"] = 127-fix_n_masked_top_A;
	this->active_strips["h_n_hits_fix_t_B"] = 127-fix_n_masked_top_B;
	this->active_strips["h_n_hits_fix_b_A"] = 127-fix_n_masked_bot_A;
	this->active_strips["h_n_hits_fix_b_B"] = 127-fix_n_masked_bot_B;
	this->active_strips["h_n_hits_fix_A"] = 254-fix_n_masked_A;
	this->active_strips["h_n_hits_fix_B"] = 254-fix_n_masked_B;
}

bad_strips::~bad_strips(){};

int bad_strips::n_active_strips(std::string histogram)
{
	return this->active_strips[histogram];
}

bool bad_strips::strip_bad(int sensor, int strip)
{
	if (this->bad_strip_map[sensor].find(strip) != this->bad_strip_map[sensor].end()) return true;
	else return false;
}
