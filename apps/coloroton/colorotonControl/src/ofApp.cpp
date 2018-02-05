#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(90);
    ofSetDataPathRoot("../Resources/");
    settings.loadSettings("settings.json");
    
    //create osc
    host = settings.settings["osc"]["host"].get<string>();
    port = settings.settings["osc"]["port"].get<int>();
    //sender.setup(host, port);
    
    //touch
    touchState.push_back(false);
    touchState.push_back(false);
#if defined(TARGET_RASPBERRY_PI)
    touch.push_back(new GPIO(settings.settings["gpio"]["pin"][0].get<string>()));
    touch.push_back(new GPIO(settings.settings["gpio"]["pin"][1].get<string>()));
    for(auto& t:touch){
        t->export_gpio();
        t->setdir_gpio("in");
    }
    lastStateChange.push_back(0);
    lastStateChange.push_back(0);
#endif
    
    
    //init modules
    ledWriter.setup(&settings);
    visCreator.setup(&settings);
    
#if !defined(TARGET_RASPBERRY_PI)
    simulator.setup(&settings);
#endif
    ofAddListener(settings.newSettingsLoaded, &visCreator, &VisCreator::onUpdateSettings);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    visCreator.createTexture(1);
    ledWriter.updateLeds();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
#if !defined(TARGET_RASPBERRY_PI)
    simulator.draw();
    
    ofPushMatrix();
    ofTranslate (0, 50);
    ledWriter.draw();
    ofPopMatrix();
#endif
}

void ofApp::checkGpio(){
#if defined(TARGET_RASPBERRY_PI)
    for(int i=0; i<touch.size(); ++i){
        string temp;
        touch[i]->getval_gpio(temp);
        touchUpdated(i,ofToInt(temp));
    }
#endif
}

void ofApp::touchUpdated(int id, bool isActive){
    if(touchState[id] != isActive){
        touchState[id] = isActive;
        visCreator.setTouch(id,isActive);
        
        //send to puredata
        ofxOscMessage m;
        m.setAddress("/" + ofToString(id));
        m.addIntArg(isActive);
        sender.sendMessage(m,false);
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') settings.loadSettings("settings.json");
    if (key == '1') touchUpdated(0,true);
    if (key == '2') touchUpdated(1,true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') touchUpdated(0,false);
    if (key == '2') touchUpdated(1,false);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
