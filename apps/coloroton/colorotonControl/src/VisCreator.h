#pragma once
#include "ofMain.h"
#include "FractalNoise.h"
#include "VisSettings.h"
#include "ofxEasing.h"


class Blob{
public:
    Blob(float p,
         float s,
         float w,
         long c,
         int id_){
        position = p;
        speed = s;
        width = w;
        creationTime = c;
        id = id_;
    }
    float position;
    float speed;
    float width;
    long creationTime;
    int id;
};

class VisCreator
{
public:
    VisCreator();
    ~VisCreator();
    
    void setup(VisSettings* settings);
    
    ofTexture* getTexture();
    
    void createTexture(int mode = 0);
    
    void onUpdateSettings(bool& b);
    
    void setTouch(int idTouch, bool isActive);
    
protected:
    ofColor getColor(string c);
    
private:
    ofFbo renderFbo;
    ofTexture tex;
    
    
    VisSettings* settings;
    
    bool fadeIn;
    long tFade = 0;
    int fadeInTime = 3000;
    int fadeOutTime = 300;
    float minIntensity = 0.2;
    float maxIntensity = 1.0;
    
    
    //noise
    void createNoise();
    FractalNoise noise;
    ofColor color1;
    ofColor color2;
    float scale;
    float speed;
    
    //special
    void createWave();
    float blobSpeed = 2.0f;
    vector<ofColor> blobColor;
    vector<float> blobCreationPos = {0,0};
    float wBlob = 3.0f;
    int creationOffset = 400;
    int blobLifetime = 10000;
    vector<bool> touch = {false, false};
    vector<long> lastCreation = {0,0};
    vector<Blob> blobs;
    
    
    float lastIntensity;
    
    //test
    void createTestModules();
};

