//
//  weatherManager.cpp
//  superShape
//
//  Created by cafe on 08/02/2018.
//

#include "weatherManager.hpp"

weatherManager::weatherManager() {
    url = "http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=e69f027f111c4b9f3d7c5cef5da1c8a7";
    updateInterval = 20;
    lastUpdateTime = 0;
}

void weatherManager::updateWeather() {
    bool parseSuccessful = json.open(url);
    
//    if(parseSuccessful)
//        ofLogNotice("weatherManager::updateWeather") << json.getRawString(true);
    
    sunrise = json["sys"]["sunrise"].asInt();
    sunset = json["sys"]["sunset"].asInt();
    
//    cout<<sunrise<<endl;
//    cout<<sunset<<endl;
}

void weatherManager::updateWeatherWithInterval() {
    if(ofGetUnixTime() - lastUpdateTime > updateInterval) {
        updateWeather();
        lastUpdateTime = ofGetUnixTime();
    }
}
