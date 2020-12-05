#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long elapsed_seconds) { 
    int hours = elapsed_seconds / 60 / 60;
    int minutes = (elapsed_seconds / 60) % 60;
    int seconds = elapsed_seconds % 60;
    string hours_string = (hours >= 10) ? std::to_string(hours) :"0" + std::to_string(hours);
    string minutes_string = (minutes >= 10) ? std::to_string(minutes) : "0" + std::to_string(minutes);
    string seconds_string = (seconds >= 10) ? std::to_string(seconds) : "0" + std::to_string(seconds);
    return hours_string + ":" + minutes_string + ":" + seconds_string;
 }