//
//  weatherManager.hpp
//  superShape
//
//  Created by cafe on 08/02/2018.
//

#ifndef weatherManager_hpp
#define weatherManager_hpp

#include "ofMain.h"
#include "ofxJSON.h"

class weatherManager {
public:
    weatherManager();
    
    void updateWeather();
    void updateWeatherWithInterval();
    
    int lastUpdateTime;
    int updateInterval;
    
    string url;
    
    ofxJSONElement json;
    
    int sunrise;
    int sunset;
private:
};

#endif /* weatherManager_hpp */
