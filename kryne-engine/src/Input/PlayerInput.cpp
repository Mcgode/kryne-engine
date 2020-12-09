/**
 * @file
 * @author Max Godefroy
 * @date 01/12/2020.
 */

#include "kryne-engine/Input/PlayerInput.h"

PlayerInput *PlayerInput::getInput(GLFWwindow *window)
{
    if (PlayerInput::inputMap.find(window) != PlayerInput::inputMap.end()) {
        cerr << "Unable to initialize player input";
    }
    return nullptr;
}


PlayerInput::PlayerInput(GLFWwindow *window) : window(window)
{
    PlayerInput::inputMap.emplace(window, this);

    glfwSetKeyCallback(window, PlayerInput::handleKeyInput);
    glfwSetCharCallback(window, PlayerInput::handleTextInput);
    glfwSetCursorPosCallback(window, PlayerInput::handleCursorPosition);
}


void PlayerInput::clearFrameData()
{
    this->keysPressedThisFrame.clear();
    this->keysReleasedThisFrame.clear();
    this->inputText = "";
}


void PlayerInput::handleKeyInput(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    const auto pair = PlayerInput::inputMap.find(window);

    if (pair != PlayerInput::inputMap.end()) {

        const auto playerInput = pair->second;
        KeyData keyData { key, mods };

        if (action == GLFW_PRESS) {
            playerInput->keysPressedThisFrame.emplace(keyData);
            playerInput->keysDown.emplace(keyData);
        } else if (action == GLFW_RELEASE) {
            playerInput->keysReleasedThisFrame.emplace(keyData);
            playerInput->keysDown.erase(keyData);
        }

    }
}


void PlayerInput::handleTextInput(GLFWwindow *window, uint32_t unicodeChar)
{
    const auto pair = PlayerInput::inputMap.find(window);

    if (pair != PlayerInput::inputMap.end()) {

        const auto playerInput = pair->second;

        playerInput->inputText += boost::lexical_cast<string>(unicodeChar);

    }
}


void PlayerInput::handleCursorPosition(GLFWwindow *window, double x, double y)
{
    const auto pair = PlayerInput::inputMap.find(window);

    if (pair != PlayerInput::inputMap.end()) {

        const auto playerInput = pair->second;

        playerInput->cursorPosition = glm::dvec2(x, y);

    }
}
