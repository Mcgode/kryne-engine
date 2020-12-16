/**
 * @file
 * @author Max Godefroy
 * @date 26/07/2019
 */

#ifndef INC_KRYNE_ENGINE_FIRST_PERSON_CAMERA_H
#define INC_KRYNE_ENGINE_FIRST_PERSON_CAMERA_H

#include <chrono>
#include <algorithm>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "OldCamera.h"

class FirstPersonCamera : public OldCamera {

public:

    FirstPersonCamera(float speed, float drag);

    glm::vec3 getCurrentPosition() override;

    glm::mat4 getViewMatrix() override;

    void onMouseMovementInput(Window *window, double xInput, double yInput) override;

    void onKeyInput(Window *window, GLint key) override {};

    void onScrollInput(Window *window, float value) override {};

    void frameUpdate(Window *window) override;

private:
    glm::vec3 position{};
    double pitch;
    double yaw;
    double speed;
    double drag;

    double lastMouseXPosition = NAN; ///< The x position of the mouse cursor during the last update
    double lastMouseYPosition = NAN; ///< The y position of the mouse cursor during the last update

    std::chrono::time_point<std::chrono::system_clock> lastFrameTime;
};


#endif //INC_KRYNE_ENGINE_FIRST_PERSON_CAMERA_H
