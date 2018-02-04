#include "Simulator.h"



Simulator::Simulator()
{
}


Simulator::~Simulator()
{
}

void Simulator::setup(VisSettings * settings_)
{
    settings = settings_;
    wTexture = settings->width;
    hTexture = settings->height;
    
    containerTex.load("container.png");
    
    // draw the ofBox outlines with some weight
    ofSetLineWidth(10);
    
    //container
    container.set(wContainer - 0.1, hContainer, dContainer - 0.1);
    //container.setOrientation(ofQuaternion(90, ofVec3f(1, 0, 0)));
    container.setPosition(0, -hContainer / 2 - 30, -dContainer / 2);
    container.mapTexCoordsFromTexture(containerTex.getTexture());
    
    //borte
    //textureFbo.allocate(wTexture, hTexture);
    
    borteL.set(dContainer, hBorte);
    borteR.set(dContainer, hBorte);
    borteF.set(wContainer, hBorte);
    borteB.set(wContainer, hBorte);
    
    deckel.set(wContainer, dContainer);
    
    borteL.setPosition(ofVec3f(-wContainer / 2, 0, -dContainer / 2));
    borteL.setOrientation(ofQuaternion(90, ofVec3f(0, 1, 0)));
    
    borteR.setPosition(ofVec3f(wContainer / 2, 0, -dContainer / 2));
    borteR.setOrientation(ofQuaternion(270, ofVec3f(0, 1, 0)));
    
    borteB.setPosition(ofVec3f(0, 0, -dContainer));
    
    deckel.setPosition(ofVec3f(0, hBorte / 2, -dContainer / 2));
    deckel.setOrientation(ofQuaternion(90, ofVec3f(1, 0, 0)));
    
    float lGes = 2 * dContainer + 2 * wContainer;
    int px0 = 0;
    int pxW = wContainer*wTexture / lGes;
    int pxD = dContainer*wTexture / lGes;
    
    borteF.mapTexCoords(px0, hTexture, px0 + pxW, 0);
    px0 += pxW;
    borteR.mapTexCoords(px0 + pxD, hTexture, px0, 0);
    px0 += pxD;
    borteB.mapTexCoords(px0 + pxW, hTexture, px0, 0);
    px0 += pxW;
    borteL.mapTexCoords(px0, hTexture, px0 + pxD, 0);
}

void Simulator::draw()
{
    ofEnableDepthTest();
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
    settings->texture.bind();
    ofFill();
    ofSetColor(255);
    borteL.draw();
    borteF.draw();
    borteB.draw();
    borteR.draw();
    settings->texture.unbind();
    
    ofPopMatrix();
    
    cam.end();
    
    
    settings->texture.draw(0, 0, settings->texture.getWidth(), 20);
    
    ofDisableDepthTest();
    
}
