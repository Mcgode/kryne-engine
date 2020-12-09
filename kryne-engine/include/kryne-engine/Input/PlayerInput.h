/**
 * @file
 * @author Max Godefroy
 * @date 01/12/2020.
 */

#ifndef INC_3D_DEMOS_PLAYERINPUT_H
#define INC_3D_DEMOS_PLAYERINPUT_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <boost/lexical_cast.hpp>
#include <glm/vec2.hpp>


using namespace std;


class PlayerInput {

public:

    /**
     * Retrieves the PlayerInput instance for the provided window. Will initialize one if none exist yet.
     * @param window    The window whose player input to retrieve.
     */
    static PlayerInput *getInput(GLFWwindow *window);

    /**
     * Clears the set the input changes for the current frame
     *
     * @warning Should not be called outside of the Renderer class, use at your own risk.
     */
    void clearFrameData();

protected:

    /**
     * Initializes a player input handler for the provided window.
     * @param window    The window receiving the player input.
     */
    explicit PlayerInput(GLFWwindow *window);

protected:

    /// GLFW window this player input is linked to
    GLFWwindow *window;

    /// Set of keys that were pressed during a time frame.
    unordered_set<int32_t> keysPressedThisFrame {};

    /// Set of keys that were released during a time frame.
    unordered_set<int32_t> keysReleasedThisFrame {};

    /// Set of keys that are pressed
    unordered_set<int32_t> keysDown {};

    /// The string of text that was input during the frame.
    string inputText {};

    /// The current position of the cursor
    glm::dvec2 cursorPosition {};


// Static part

protected:

    static void handleKeyInput(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

    static void handleTextInput(GLFWwindow *window, uint32_t unicodeChar);

    static void handleCursorPosition(GLFWwindow *window, double x, double y);

protected:

    static unordered_map<GLFWwindow *, PlayerInput *> inputMap;

};


#endif //INC_3D_DEMOS_PLAYERINPUT_H
