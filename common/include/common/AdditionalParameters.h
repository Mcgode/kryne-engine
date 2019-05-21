//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_ADDITIONALPARAMETERS_H
#define INC_3D_DEMOS_ADDITIONALPARAMETERS_H

#include <map>
#include <string>
#include <any>

using namespace std;


class AdditionalParameters {

public:

    AdditionalParameters() = default;

    bool contains(const string& key);

    void cleanupLoopParameters();

    void insertProgramLongParameter(const string& key, const any& value);

    void insertLoopLongParameter(const string& key, const any& value);

    any get(const string& key);

private:

    map<string, any> programAgedParameters{};
    map<string, any> loopAgedParameters{};

};


#endif //INC_3D_DEMOS_ADDITIONALPARAMETERS_H
