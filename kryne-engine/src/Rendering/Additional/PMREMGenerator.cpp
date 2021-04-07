/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#include "kryne-engine/Rendering/Additional/PMREMGenerator.hpp"

bool PMREMGenerator::isProcessing(const shared_ptr<EnvironmentMap> &map)
{
    scoped_lock<mutex> l(this->dequeMutex);
    return find(this->mapsToProcess.begin(), this->mapsToProcess.end(), map) != this->mapsToProcess.end();
}


void PMREMGenerator::processMap(const shared_ptr<EnvironmentMap> &map)
{
    if (!this->isProcessing(map) && (**map)->getId() != 0)
    {
        scoped_lock<mutex> l(this->dequeMutex);
        this->mapsToProcess.push_back(map);
    }
}
