#pragma once
#include "ofMain.h"



class VisSettings
{
public:
    VisSettings();
    ~VisSettings();
    
    void loadSettings(string settingsFile);
    
    ofEvent<bool> newSettingsLoaded;
    
    int width;
    int height = 2;
    ofJson settings;
    
    ofTexture texture;
    ofPixels pixel;
    
    vector<int> channelW;
    
private:
    
};

