//
// Created by max on 21/05/19.
//

#include "kryne-engine/Core/AdditionalParameters.h"

bool AdditionalParameters::contains(const string& key)
{
    return loopAgedParameters.find(key) != loopAgedParameters.end() ||
           programAgedParameters.find(key) != programAgedParameters.end();
}


void AdditionalParameters::cleanupLoopParameters()
{
    loopAgedParameters.clear();
}


void AdditionalParameters::insertLoopLongParameter(const string& key, const any& value)
{
    loopAgedParameters[key] = value;
}


void AdditionalParameters::insertProgramLongParameter(const string& key, const any& value)
{
    programAgedParameters.insert(pair<string, any>(key, value));
}


any AdditionalParameters::get(const string& key) {
    if (programAgedParameters.find(key) != programAgedParameters.end())
        return programAgedParameters.find(key)->second;

    else if (loopAgedParameters.find(key) != loopAgedParameters.end())
        return loopAgedParameters.find(key)->second;

    else
        return any();
}




