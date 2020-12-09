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

        if (action == GLFW_PRESS) {
            playerInput->keysPressedThisFrame.emplace(key);
            playerInput->keysDown.emplace(key);
        } else if (action == GLFW_RELEASE) {
            playerInput->keysReleasedThisFrame.emplace(key);
            playerInput->keysDown.erase(key);
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
