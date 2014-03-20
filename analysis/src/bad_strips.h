#ifndef bad_strips_h_
#define bad_strips_h_

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

class bad_strips{
public:
	bad_strips(std::string bad_strip_file);
	~bad_strips();
	int n_active_strips(std::string histogram);
	bool strip_bad(int sensor, int strip);
private:
	std::map<int, std::set<int> > bad_strip_map;
	std::map<std::string, int> active_strips;
};

#endif