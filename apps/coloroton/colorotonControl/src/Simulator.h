#pragma once
#include "ofMain.h"
#include "VisSettings.h"

class Simulator
{
public:
    Simulator();
    ~Simulator();
    
    void setup(VisSettings * settings_);
    void draw();
    
private:
    
    
    int wTexture;
    int hTexture;
    
    ofImage containerTex; // the OF logo
    ofLight light; // creates a light and enables lighting
    ofEasyCam cam; // add mouse controls for camera movement
    
    ofPlanePrimitive borteR, borteL, borteF, borteB, deckel;
    ofBoxPrimitive container;
    
    float wContainer{ 500.0f };
    float dContainer{ 250.0f };
    float hContainer{ 300.0f };
    float hBorte{ 25.0f };
    
    VisSettings* settings;
};

