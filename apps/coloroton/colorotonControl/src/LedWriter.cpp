#include "LedWriter.h"



LedWriter::LedWriter()
{
    
}

LedWriter::~LedWriter()
{
}


void LedWriter::setup(VisSettings * settings_)
{
    settings = settings_;
    
    loadMapping(settings->settings["ledWriter"]["mappingFile"].get<string>());
    
    /*std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
     
     ofLogNotice("ofApp::setup") << "Connected Devices: ";
     
     for (std::size_t i = 0; i < devicesInfo.size(); ++i)
     {
     ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i].description();
     }
     
     
     if (!devicesInfo.empty())
     {
     bool success = false;
     for (auto& d:devicesInfo) {
     if (d.description() == settings->settings["ledWriter"]["serial"].get<string>()) {
     octoWs.setup(d, 115200);
     success = true;
     ofLogNotice("ofApp::setup") << "Successfully setup " << d;
     }
     }
     if(!success)
     {
     ofLogError("ofApp::setup") << "Setup OctoWS failed";
     }
     }*/
    
    
    // SYSTEM SETTINGS
    //--------------------------------------
    stripWidth = 240;                            // pixel width of strip
    stripHeight = 8;                            // pixel height of strip
    stripsPerPort = 8;                          // total number of strips per port
    numPorts = 1;                               // total number of teensy ports?
    brightness = 200;                             // LED brightness
    
    teensy.setup(stripWidth, stripHeight, stripsPerPort, numPorts);
    
    /* Configure our teensy boards (portName, xOffset, yOffset, width%, height%, direction) */
    teensy.serialConfigure(settings->settings["ledWriter"]["serial"].get<string>(), 0, 0, 100, 100, 0);
    
    
}

void LedWriter::updateLeds(){
    //write bytes to leds
    
    //sync stuff, not needed but present in video example
   /* message[0] = '*';  // first Teensy is the frame sync master
    int usec = (int)((1000000.0 / 30) * 0.75);
    message[1] = 168;//(char)(usec);   // request the frame sync pulse
    message[2] = 97;//(char)(usec >> 8); // at 75% of the frame time*/
    
    //create message using mapping
    for(int x = 0; x < mapping.size(); ++x){
        for(int y = 0; y < mapping[0].size(); ++y){
            /*int index = (mapping[x][y].index + mapping[x][y].channel*nLedsPerChannel)*3;
             ofColor c = settings->pixel.getColor(x,y);
             
             message[index] = c.g;
             message[index+1] = c.g;
             message[index+2] = c.g;*/
            iTemp.setColor(mapping[x][y].index, mapping[x][y].channel, settings->pixel.getColor(x,y));
        }
    }
    
    
    teensy.pixels1 = iTemp.getPixels();
    teensy.update();
    
}

void LedWriter::draw(){
    teensy.draw(0,0);
}

void LedWriter::loadMapping(string mappingFile)
{
    vector<bool> isMaster;
    vector<int> nLeds;
    
    // read file
    ofBuffer buffer = ofBufferFromFile(mappingFile);
    
    if (buffer.size()) {
        for (auto& line : buffer.getLines()) {
            string code = ofSplitString(line, "#")[0];
            if (code[1] == 'S') isMaster.push_back(false);
            else isMaster.push_back(true);
            int val = ofToInt(code.substr(2));
            nLeds.push_back(val);
        }
    }
    
    //init mapping
    mapping.clear();
    settings->width = 0;
    nChannels = nLeds.size() / 2;
    
    for (auto& v : nLeds)
    {
        settings->width += v;
    }
    
    for (int i = 0; i < settings->width; i++)
    {
        ledIndex v;
        v.channel = -1;
        v.index = -1;
        vector<ledIndex> row = { v,v };
        mapping.push_back(row);
    }
    
    //create mapping
    int offset = 0;
    int channel = 0;
    for (int i = 0; i < nLeds.size(); i += 2)
    {
        if ((nLeds[i] + nLeds[i + 1])*2 > nLedsPerChannel) nLedsPerChannel = (nLeds[i] + nLeds[i + 1])*2;
    }
    for (int i = 0; i < nLeds.size(); i += 2)
    {
        if (isMaster[i]) //left master
        {
            int index = 0;
            //left bottom
            for (int x = 0; x < nLeds[i]; x++)
            {
                mapping[offset + nLeds[i] - x - 1][1].channel = channel;
                mapping[offset + nLeds[i] - x - 1][1].index = index;
                ++index;
            }
            //top
            for (int x = 0; x < nLeds[i + 1] + nLeds[i]; x++)
            {
                mapping[offset + x][0].channel = channel;
                mapping[offset + x][0].index = index;
                ++index;
            }
            
            //rightbottom
            for (int x = 0; x < nLeds[i + 1]; x++)
            {
                mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].channel = channel;
                mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].index = index;
                ++index;
            }
        } else //right master
        {
            //rightup
            int index = 0;
            for (int x = 0; x < nLeds[i + 1]; x++)
            {
                mapping[x + offset + nLeds[i]][0].channel = channel;
                mapping[x + offset + nLeds[i]][0].index = index;
                ++index;
            }
            //bottom
            for (int x = 0; x < nLeds[i + 1] + nLeds[i]; x++)
            {
                mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].channel = channel;
                mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].index = index;
                ++index;
            }
            //left top
            for (int x = 0; x < nLeds[i]; x++)
            {
                mapping[x + offset][0].channel = channel;
                mapping[x + offset][0].index = index;
                ++index;
            }
        }
        
        //show mapping
        cout << "channel " << channel << endl << endl;
        
        for (int j = offset; j < nLeds[i] + nLeds[i + 1] + offset; j++)
        {
            cout << mapping[j][0].index << " ";
        }
        cout << endl;
        for (int j = offset; j < nLeds[i] + nLeds[i + 1] + offset; j++)
        {
            cout << mapping[j][1].index << " ";
        }
        cout << endl << endl << endl;
        
        //set offset
        channel++;
        offset += nLeds[i] + nLeds[i + 1];
        //offset+=nLedsPerChannel;
    }
    
    iTemp.allocate(nLedsPerChannel,8,OF_IMAGE_COLOR);
    fbo.allocate(nLedsPerChannel,8);
    
    //init byte buffer
    //message = ofx::IO::ByteBuffer(nLedsPerChannel*8*3 +3, 0);
    settings->channelW = nLeds;
    
}
