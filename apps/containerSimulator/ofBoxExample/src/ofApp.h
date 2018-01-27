#pragma once

#include "ofMain.h"
#include "FractalNoise.h"

#include "ofxGui.h"
#include "ofxEasing.h"



class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void generateTexture();
    void genTex0();
    void genTex1();
    void genTex2();
    
    ofImage containerTex; // the OF logo
    ofLight light; // creates a light and enables lighting
    ofEasyCam cam; // add mouse controls for camera movement
    
    ofFbo textureFbo;
    int wTexture{ (int) (21.5 * 30) };
    int hTexture{ 2 };
    
    
    ofPlanePrimitive borteR, borteL, borteF, borteB, deckel;
    ofBoxPrimitive container;
    FractalNoise noise;
    
    float wContainer{ 500.0f };
    float dContainer{ 250.0f };
    float hContainer{ 300.0f };
    float hBorte{ 25.0f };
    
    ofxPanel gui;
    ofParameter<ofColor> color1;
    ofParameter<ofColor> color2;
    ofParameter<float> scale;
    ofParameter<float> speed;
    ofParameter<int> mode;
};
