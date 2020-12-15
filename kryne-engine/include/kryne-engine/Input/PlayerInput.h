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

    /**
     * Prepares the instance for the incoming input events.
     *
     * @warning Should be called before the window events are polled.
     *          Don't call this unless you know what you are doing.
     */
    void willPollEvents();

    /**
     * Returns the current cursor position.
     */
    [[nodiscard]] inline const glm::dvec2 &getCursorPosition() const { return this->cursorPosition; }

    /**
     * Returns the cursor movement relative to its previous position.
     */
    [[nodiscard]] inline glm::dvec2 getCursorMovement() const;


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

    /// The previous cursor position
    glm::dvec2 previousCursorPosition {};


// === Key mapping ===

public:

    /**
     * Structure for storing a mapped key/button data
     */
    struct KeyMapItem {

        /// The name of the input
        string name;

        /// The associated key data
        KeyData associatedKey;

    };

    /// An interface for classes which will use input callbacks
    class CallbackObject {};

    /// The key callback function type
    typedef void (CallbackObject::*InputCallback) ();

public:

    /**
     * Registers an input key to the key map, in the form of a PlayerInput::KeyMapItem
     * @param name  The name for this key
     * @param key   The key/button value
     * @param mods  The modifiers for this input.
     */
    void registerKey(const string &name, int32_t key, int32_t mods = 0);

    /**
     * Calls the provided callback every time this input key is pressed.
     * @param keyName   The input key name.
     * @param object    The object from which the method callback will be called.
     * @param callback  The callback to call on event, a method of the provided object.
     */
    inline void onKeyPress(const string &keyName, CallbackObject *object, InputCallback callback) {
        this->addCallback(keyName, object, callback, this->keyPressCallbacks);
    }

    /**
     * Calls the provided callback every time this input key is released.
     * @param keyName   The input key name.
     * @param object    The object from which the method callback will be called.
     * @param callback  The callback to call on event, a method of the provided object.
     */
    inline void onKeyRelease(const string &keyName, CallbackObject *object, InputCallback callback) {
        this->addCallback(keyName, object, callback, this->keyReleaseCallbacks);
    }

protected:

    /// The callbacks set type
    typedef unordered_set<pair<CallbackObject *, InputCallback>> CallbackSet;

    /**
     * Adds a callback for an input key event
     * @param keyName       The input key name
     * @param object        The object from which the method callback will be called.
     * @param callback      The callback to call on event, a method of the provided object.
     * @param callbacksMap  The collection of callbacks to add to.
     */
    void addCallback(const string &keyName, CallbackObject *object, InputCallback callback,
                     unordered_map<KeyMapItem, CallbackSet> &callbacksMap);

    /**
     * Calls all the callbacks corresponding to the provided input.
     * @param data       The input key data.
     * @param callbacks  The collection of callbacks to call.
     */
    void callCallbacks(const KeyData &data, const unordered_map<KeyMapItem, CallbackSet> &callbacks) const;

protected:

    /// The keymap for this input handler.
    unordered_map<string, KeyMapItem> keyMap {};

    /// A rapid access hashmap to get all the PlayerInput::keyMap elements corresponding to a key value.
    unordered_map<int32_t, vector<KeyMapItem>> keyToKeyMapItems {};

    /// All the callbacks associated to the pressing of PlayerInput::keyMap items.
    unordered_map<KeyMapItem, CallbackSet> keyPressCallbacks;

    /// All the callbacks associated to the releasing of PlayerInput::keyMap items.
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
