#ifndef DEFAULTSETTINGS_H
#define DEFAULTSETTINGS_H

#include <string>
#include "world.h"

struct DefaultSettings {

    static const unsigned long worldRows;
    static const unsigned long worldCols;
    static const bool isSaveLastStep;
    static const World::NeighborCountingPolicy neighborCountingPolicy;



    static const int timerIntervaMs;
    static const int fontSize;
    static const std::string fileExtension;

};

const unsigned long DefaultSettings::worldRows{40};
const unsigned long DefaultSettings::worldCols{80};
const bool DefaultSettings::isSaveLastStep{ false };
const World::NeighborCountingPolicy DefaultSettings::neighborCountingPolicy{ World::WITHOUT_BORDER };

const int DefaultSettings::timerIntervaMs{ 100 };
const int DefaultSettings::fontSize{ 11 };
const std::string DefaultSettings::fileExtension{".wrld"};


#endif // DEFAULTSETTINGS_H
