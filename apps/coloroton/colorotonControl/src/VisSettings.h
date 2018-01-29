#pragma once
#include "ofMain.h"



class VisSettings
{
public:
	VisSettings();
	~VisSettings();

	void loadSettings(string settingsFile);
	


	int width;
	int height = 2;
	ofJson settings;
	


private:
	
};

