#pragma once

#include "ofMain.h"
#include "VisSettings.h"
#include "LedWriter.h"
#include "VisCreator.h"
#include "Simulator.h"
#include "ofxOsc.h"

#if defined(TARGET_RASPBERRY_PI)
#include "ofxGPIO.h""
#endif

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void touchUpdated(int id, bool isActive);
    
    void checkGpio();
    
private:
    VisSettings settings;
    LedWriter ledWriter;
    VisCreator visCreator;
    Simulator simulator;
    
    ofxOscSender sender;
    string host;
    string port;
    
    vector<bool> touchState;
    
#if defined(TARGET_RASPBERRY_PI)
    vector<GPIO*> touch;
    vector<long> lastStateChange;
#endif
    
};
