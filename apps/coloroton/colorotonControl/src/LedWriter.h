#pragma once
#include "ofMain.h"
#include "VisSettings.h"

struct ledIndex {
	int channel;
	int index;
};

class LedWriter
{
public:
	LedWriter();
	~LedWriter();

	void setup(VisSettings* settings);
	void loadMapping(string mappingFile);
private:
	vector <vector<ledIndex>> mapping;

	VisSettings* settings;

	int nLedsPerChannel = 0;
	int nChannels;
	int wChannel;
};

