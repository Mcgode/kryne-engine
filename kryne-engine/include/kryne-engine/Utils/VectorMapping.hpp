/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_KRYNE_ENGINE_VECTORMAPPING_H
#define INC_KRYNE_ENGINE_VECTORMAPPING_H

#pragma once

#include <vector>
#include <glm/detail/qualifier.hpp>


using namespace glm;
using namespace std;


namespace Utils {

    /**
     * Flat-maps a list of float vectors into a list of floats.
     * @tparam L    The amount of components per individual vector
     * @param data  The list of vectors to flat-map.
     * @return A vector of floats, containing the flat-mapped input data.
     */
    template<int L>
    vector<float> flatMapFloatVectors(const vector<vec<L, float, defaultp>> &data)
    {
        vector<float> flatMap;

        for (const auto &v : data)
        {
            for (int i = 0; i < L; i++)
                flatMap.push_back(v[i]);
        }

        return flatMap;
    }

}

#endif //INC_KRYNE_ENGINE_VECTORFLATTENING_H
