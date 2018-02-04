#include "VisCreator.h"



VisCreator::VisCreator()
{
}


VisCreator::~VisCreator()
{
}

void VisCreator::setup(VisSettings* settings_){
    settings = settings_;
    bool t = true;
    blobColor.push_back(ofColor(255));
    blobColor.push_back(ofColor(255));
    
    onUpdateSettings(t);
    
    
    
    renderFbo.allocate(settings->width, settings->height);
}

void VisCreator::onUpdateSettings(bool& b){
    ofJson j = settings->settings;
    
    
    //tex
    fadeInTime = j["texture"]["fadeInTime"].get<int>();
    fadeOutTime = j["texture"]["fadeOutTime"].get<int>();
    minIntensity = j["texture"]["minIntensity"].get<float>();
    maxIntensity = j["texture"]["maxIntensity"].get<float>();
    
    //noise
    color1 = getColor(j["tNoise"]["color1"].get<string>());
    color2 = getColor(j["tNoise"]["color2"].get<string>());
    scale = j["tNoise"]["scale"].get<float>();
    speed = j["tNoise"]["speed"].get<float>();
    
    //wave
    blobColor[0] = getColor(j["tWave"]["color"][0].get<string>());
    blobColor[1] = getColor(j["tWave"]["color"][1].get<string>());
    blobCreationPos[0] = j["tWave"]["position"][0].get<float>();
    blobCreationPos[1] = j["tWave"]["position"][1].get<float>();
    wBlob = j["tWave"]["wBlob"].get<float>();
    creationOffset = j["tWave"]["creationOffset"].get<int>();
    blobLifetime = j["tWave"]["blobLifetime"].get<int>();
    blobSpeed = j["tWave"]["blobSpeed"].get<float>();
}

ofTexture * VisCreator::getTexture()
{
    return &tex;
}

void VisCreator::createTexture(int mode){
    if (mode == 0){
        
        renderFbo.begin();
        ofClear(0, 0);
        createNoise();
        createWave();
        renderFbo.end();
        
    }
    else if (mode == 1){
        renderFbo.begin();
        ofClear(0, 0);
        createTestModules();
        renderFbo.end();
    }
    
    settings->texture = renderFbo.getTexture();
    if (!settings->pixel.isAllocated())settings->pixel.allocate(renderFbo.getWidth(), renderFbo.getHeight(), 4);
    renderFbo.readToPixels(settings->pixel);
    
}

void VisCreator::setTouch(int idTouch, bool isActive){
    //check if fadeIn
    
    if(!(touch[idTouch] == isActive)){
        int tTouch = 0;
        for (auto t:touch){
            if (t) ++tTouch;
        }
        
        if (tTouch == 0 && isActive) {
            tFade = ofGetElapsedTimeMillis();
            fadeIn = false;
        }else if (!isActive && tTouch == 1){
            tFade = ofGetElapsedTimeMillis();
            fadeIn = true;
        }
        
        touch[idTouch] = isActive;
    }
}

ofColor VisCreator::getColor(string c){
    auto co = ofSplitString(c,",");
    return ofColor(ofToFloat(co[0]),ofToFloat(co[1]),ofToFloat(co[2]));
}

void VisCreator::createNoise()
{
    //set intensity for fadeIn/out
    long now = ofGetElapsedTimeMillis();
    float intensity = maxIntensity;
    if(fadeIn && now - tFade < fadeInTime){
        intensity = ofxeasing::map(now, tFade, tFade+ fadeInTime, minIntensity, maxIntensity, ofxeasing::exp::easeInOut);
        // if(intensity > lastIntensity) intensity = lastIntensity;
    }
    else if(!fadeIn && now - tFade < fadeOutTime){
        intensity = ofxeasing::map(now, tFade, tFade+ fadeOutTime, maxIntensity, minIntensity, ofxeasing::exp::easeInOut);
        if(intensity > lastIntensity) intensity = lastIntensity;
    }
    else if (touch[0] || touch[1]) intensity = minIntensity;
    
    lastIntensity = intensity;
    
    //draw noise
    for (int x = 0; x < settings->width; x++)
    {
        for (int y = 0; y < settings->height; y++)
        {
            float value = noise.noise((float)x / (float)settings->width*scale, (float)y / (float)settings->height*scale, ofGetElapsedTimef()*.1*speed);
            
            //fade border
            int fade = 20;
            if(x > settings->width - fade){
                float value2 = noise.noise((float)(x-settings->width) / (float)settings->width*scale, (float)y / (float)settings->height*scale, ofGetElapsedTimef()*.1*speed);
                float value = ofMap(x, settings->width -x, settings->width, value, value2);
                
            }
            
            
            //normalize to 0-1
            value = 0.5*(value + 1);
            
            value = ofxeasing::map(value, 0, 1.0, 0.0, 1.0, ofxeasing::exp::easeInOut);
            
            ofColor c1 = color1;
            ofColor c = c1.lerp(color2, value);
            c *= intensity;
            
            
            ofSetColor(c);
            ofDrawRectangle(x, y, 1, 1);
        }
    }
}

void VisCreator::createWave(){
    //create blobs
    long now = ofGetElapsedTimeMillis();
    for (int i=0; i<touch.size(); ++i) {
        if (touch[i] && now - lastCreation[i] > creationOffset) {
            blobs.push_back(Blob(blobCreationPos[i],blobSpeed,wBlob,now,i));
            blobs.push_back(Blob(blobCreationPos[i],-blobSpeed,wBlob,now,i));
            lastCreation[i] = now;
        }
    }
    //erase blobs
    std::vector<Blob>::iterator it = blobs.begin();
    while (it != blobs.end()) {
        // Remove elements while iterating
        if (now - (*it).creationTime > blobLifetime) {
            // erase() makes the passed iterator invalid
            // But returns the iterator to the next of deleted element
            it = blobs.erase(it);
        } else
            it++;
    }
    
    //update blobs
    for (auto& b:blobs){
        b.position += b.speed;
        if(b.position < 0) b.position += settings->width;
        else if (b.position > settings->width) b.position -= settings->width;
    }
    
    //draw bobs
    for (auto& b:blobs){
        float intensity = ofxeasing::map(now, b.creationTime, b.creationTime + blobLifetime, 100, 0, ofxeasing::exp::easeInOut);
        ofSetColor(ofColor(blobColor[b.id].r,blobColor[b.id].g,blobColor[b.id].b, intensity));
        ofDrawRectangle(b.position,0,b.width,2);
    }
    
}

void VisCreator::createTestModules(){
    ofPushMatrix();
    for (int i=0; i<settings->channelW.size(); ++i)
    {
        ofColor c;
        c.setHsb((i*40)%256,255,255);
        ofSetColor(c);
        ofDrawRectangle(0,0,settings->channelW[i],2);
        ofTranslate(settings->channelW[i],0);
    }
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawRectangle((int)(ofGetElapsedTimeMillis()/40)%(int)renderFbo.getWidth(),0,1,2);
    
    ofSetColor(255);
}