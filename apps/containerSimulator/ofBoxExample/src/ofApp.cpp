#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    
    gui.setup("panel");
    gui.add(color1.set("farbe1", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(color2.set("farbe2", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(speed.set("geschwindigkeit", 1.0, 0.1, 3.0));
    gui.add(scale.set("scale", 1.0, 0.01, 10.0));
    gui.add(mode.set("mode", 0, 0, 10));
    
    gui.loadFromFile("settings.xml");
    
    //ofEnableArbTex();
    
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();
    
    // ofBox uses texture coordinates from 0-1, so you can load whatever
    // sized images you want and still use them to texture your box
    // but we have to explicitly normalize our tex coords here
    //ofEnableNormalizedTexCoords();
    
    // loads the OF logo from disk
    containerTex.load("container.png");
    
    // draw the ofBox outlines with some weight
    ofSetLineWidth(10);
    
    //container
    container.set(wContainer - 0.1, hContainer, dContainer - 0.1);
    //container.setOrientation(ofQuaternion(90, ofVec3f(1, 0, 0)));
    container.setPosition(0,  -hContainer/2 - 30, -dContainer/2);
    container.mapTexCoordsFromTexture(containerTex.getTexture());
    
    //borte
    textureFbo.allocate(wTexture, hTexture);
    
    borteL.set(dContainer, hBorte);
    borteR.set(dContainer, hBorte);
    borteF.set(wContainer, hBorte);
    borteB.set(wContainer, hBorte);
    
    deckel.set(wContainer, dContainer);
    
    borteL.setPosition(ofVec3f(-wContainer/2, 0, -dContainer/2));
    borteL.setOrientation(ofQuaternion(90, ofVec3f(0, 1, 0)));
    
    borteR.setPosition(ofVec3f(wContainer / 2, 0, -dContainer / 2));
    borteR.setOrientation(ofQuaternion(270, ofVec3f(0, 1, 0)));
    
    borteB.setPosition(ofVec3f(0, 0, -dContainer));
    
    deckel.setPosition(ofVec3f(0, hBorte/2, -dContainer / 2));
    deckel.setOrientation(ofQuaternion(90, ofVec3f(1, 0, 0)));
    
    float lGes = 2 * dContainer + 2 * wContainer;
    int px0 = 0;
    int pxW = wContainer*wTexture / lGes;
    int pxD = dContainer*wTexture / lGes;
    
    borteF.mapTexCoords(px0, hTexture,px0 + pxW, 0);
    px0 += pxW;
    borteR.mapTexCoords(px0 +pxD, hTexture, px0, 0);
    px0 += pxD;
    borteB.mapTexCoords(px0 + pxW, hTexture, px0, 0);
    px0 += pxW;
    borteL.mapTexCoords(px0,hTexture, px0 + pxD, 0);
    
    noise.setBaseFrequency(2.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    generateTexture();
    
    ofBackground(0, 0, 0);
    
    
    
    cam.begin();
    
    ofPushMatrix();
    //container
    
    containerTex.bind();
    ofFill();
    ofSetColor(255);
    container.draw();
    containerTex.unbind();
    
    //deckel
    ofSetColor(0);
    deckel.draw();
    
    //borte
    textureFbo.getTexture().bind();
    ofFill();
    ofSetColor(255);
    borteL.draw();
    borteF.draw();
    borteB.draw();
    borteR.draw();
    textureFbo.getTexture().unbind();
    
    ofPopMatrix();
    
    cam.end();
    
    
    textureFbo.draw(0, 0, textureFbo.getWidth(), 50);
    
    ofDisableDepthTest();
    gui.draw();
    ofEnableDepthTest();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        gui.saveToFile("settings.xml");
    }
    if (key == 'l') {
        gui.loadFromFile("settings.xml");
    }
    if (key == 'c') {
        cam.enableMouseInput();
    }
    if (key == 'v') {
        cam.disableMouseInput();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::generateTexture()
{
    if (mode == 0){
        genTex0();
    }
    else if (mode == 1){
        genTex1();
    }
    
}

void ofApp::genTex0(){
    textureFbo.begin();
    ofClear(0, 0);
    for (int x = 0; x < wTexture; x++)
    {
        for (int y = 0; y < hTexture; y++)
        {
            float value = noise.noise((float)x / (float)wTexture*scale, (float)y / (float)hTexture*scale, ofGetElapsedTimef()*.1*speed);
            
            value = 0.5*(value + 1);
            
            value = ofxeasing::map(value, 0, 1.0, 0.0, 1.0, ofxeasing::exp::easeInOut);
            
            ofColor c1 = color1;
            ofColor c = c1.lerp(color2, value);
            ofSetColor(c);
            ofDrawRectangle(x, y, 1, 1);
        }
    }
    //ofSetColor(255, 0, 0);
    //ofDrawRectangle(0, 0, 1000, 500);
    
    textureFbo.end();
}

void ofApp::genTex1(){
    textureFbo.begin();
    ofClear(0, 0);
    for (int x = 0; x < wTexture; x++)
    {
        for (int y = 0; y < hTexture; y++)
        {
            
            float value = noise.noise((float)x / (float)wTexture*scale, 0, ofGetElapsedTimef()*.1*speed);
            
            value = 0.5*(value + 1);
            
            value = ofxeasing::map(value, 0, 1.0, 0.0, 1.0, ofxeasing::exp::easeInOut);
            
            ofColor c1 = color1;
            ofColor c = c1.lerp(color2, value);
            ofSetColor(c);
            ofDrawRectangle(x, y, 1, 1);
        }
    }
    //ofSetColor(255, 0, 0);
    //ofDrawRectangle(0, 0, 1000, 500);
    
    textureFbo.end();
}

void ofApp::genTex2(){
    textureFbo.begin();
    ofClear(0, 0);
    for (int x = 0; x < wTexture; x++)
    {
        for (int y = 0; y < hTexture; y++)
        {
           /* float position =
            
            float value = noise.noise((float)x / (float)wTexture*scale, 0, ofGetElapsedTimef()*.1*speed);
            
            value = 0.5*(value + 1);
            
            value = ofxeasing::map(value, 0, 1.0, 0.0, 1.0, ofxeasing::exp::easeInOut);
            
            ofColor c1 = color1;
            ofColor c = c1.lerp(color2, value);
            ofSetColor(c);
            ofDrawRectangle(x, y, 1, 1);*/
        }
    }
    //ofSetColor(255, 0, 0);
    //ofDrawRectangle(0, 0, 1000, 500);
    
    textureFbo.end();
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
