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
     * A structure for holding keyboard key and mouse button data
     */
    struct KeyData {

        /// The keyboard key / mouse button value
        int32_t key;

        /// The modifier keys for this input
        int32_t mod;

        explicit KeyData(int32_t key): key(key), mod(0) {};

        explicit KeyData(int32_t key, int32_t mod): key(key), mod(mod) {};

    };


public:

    /**
     * Retrieves the PlayerInput instance for the provided window. Will initialize one if none exist yet.
     * @param window    The window whose player input to retrieve.
     */
    static PlayerInput *getInput(GLFWwindow *window);


protected:

    /**
     * Initializes a player input handler for the provided window.
     * @param window    The window receiving the player input.
     */
    explicit PlayerInput(GLFWwindow *window);


protected:

    /// GLFW window this player input is linked to
    GLFWwindow *window;

    /// Set of keys that are pressed
    unordered_set<KeyData> keysDown {};

    /// The string of text that was input during the frame.
    string inputText {};

    /// The current position of the cursor
    glm::dvec2 cursorPosition {};


// === Key mapping ===

public:

    struct KeyMapItem {

        string name;
        KeyData associatedKey;

    };

    class CallbackObject {};

    typedef void (CallbackObject::*KeyCallback) ();

public:

    void registerKey(const string &name, int32_t key, int32_t mods = 0);

    inline void onKeyPress(const string &keyName, CallbackObject *object, KeyCallback callback) {
        this->addCallback(keyName, object, callback, this->keyPressCallbacks);
    }

    inline void onKeyRelease(const string &keyName, CallbackObject *object, KeyCallback callback) {
        this->addCallback(keyName, object, callback, this->keyReleaseCallbacks);
    }

protected:

    typedef unordered_set<pair<CallbackObject *, KeyCallback>> CallbackSet;

    void addCallback(const string &keyName, CallbackObject *object, KeyCallback callback,
                     unordered_map<KeyMapItem, CallbackSet> &callbacksMap);

    void callCallbacks(const KeyData &data, const unordered_map<KeyMapItem, CallbackSet> &callbacks) const;

protected:

    unordered_set<KeyMapItem> keyMap {};

    unordered_map<int32_t, vector<KeyMapItem>> keyToKeyMapItems {};

    unordered_map<KeyMapItem, CallbackSet> keyPressCallbacks;

    unordered_map<KeyMapItem, CallbackSet> keyReleaseCallbacks;


// === GLFW input callbacks handling ===

protected:

    /**
     * GLFW callback function for handling keyboard input.
     */
    static void handleKeyInput(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

    /**
     * GLFW callback function for handling text input.
     */
    static void handleTextInput(GLFWwindow *window, uint32_t unicodeChar);

    /**
     * GLFW callback function for handling cursor position input.
     */
    static void handleCursorPosition(GLFWwindow *window, double x, double y);

    /**
     * GLFW callback function for handling mouse button input.
     */
    static void handleMouseButtonInput(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);


protected:

    /// The map of GLFW windows to their associated PlayerInput instances.
    static unordered_map<GLFWwindow *, PlayerInput *> inputMap;

};


#endif //INC_3D_DEMOS_PLAYERINPUT_H
