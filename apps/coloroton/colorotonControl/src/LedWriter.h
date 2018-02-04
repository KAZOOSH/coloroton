#pragma once
#include "ofMain.h"
#include "VisSettings.h"

#include "ofxTeensyOcto.h"

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
    void updateLeds();
    
    void draw();
    
private:
    vector <vector<ledIndex>> mapping;
    
    VisSettings* settings;
    
    int nLedsPerChannel = 0;
    int nChannels;
    int wChannel;
    
    
    ofImage iTemp;
    ofFbo fbo;
    
    // LED - Teensy stuff
    //-----------------------------
    ofxTeensyOcto teensy;
    int stripWidth;
    int stripHeight;
    int stripsPerPort;
    int numPorts;
    int brightness;
    
};

