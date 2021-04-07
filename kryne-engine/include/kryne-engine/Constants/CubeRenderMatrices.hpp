/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#pragma once


#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Constants {

    /// The projection matrix used for cube texture rendering.
    const glm::mat4 cubeRenderProjectionMatrix = glm::perspective(glm::half_pi<float>(), 1, 0.1, 10);

    /// The view matrices array used for cube texture rendering.
    const glm::mat4 cubeRenderViewMatrices[] =
            {
                glm::lookAt(glm::vec3(0), glm::vec3( 1, 0, 0), glm::vec3(0, -1, 0)),
                glm::lookAt(glm::vec3(0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
                glm::lookAt(glm::vec3(0), glm::vec3(0,  1, 0), glm::vec3(0, 0,  1)),
                glm::lookAt(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
                glm::lookAt(glm::vec3(0), glm::vec3(0, 0,  1), glm::vec3(0, -1, 0)),
                glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
            };

}