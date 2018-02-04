#include "VisSettings.h"



VisSettings::VisSettings()
{
}


VisSettings::~VisSettings()
{
}

void VisSettings::loadSettings(string settingsFile){
    ofFile file(settingsFile);
    file >> settings;
    bool out = true;
    ofNotifyEvent(newSettingsLoaded,out);
}
