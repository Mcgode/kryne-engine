/**
 * @file
 * @author Max Godefroy
 * @date 01/12/2020.
 */

#ifndef INC_KRYNE_ENGINE_PLAYERINPUT_H
#define INC_KRYNE_ENGINE_PLAYERINPUT_H

#include <set>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <boost/lexical_cast.hpp>
#include <glm/vec2.hpp>

#define KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW


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

        inline bool operator ==(const KeyData &other) const { return key == other.key && mod == other.mod; }

        /// Hash struct for the PlayerInput::KeyData struct
        struct Hasher {

            inline uint64_t operator() (const KeyData &keyData) const
            {
                return ((uint64_t) keyData.key << 32) | ((uint64_t) keyData.mod);
            }

        };

    };


public:

    /**
     * Retrieves the PlayerInput instance for the provided window. Will initialize one if none exist yet.
     * @param window    The window whose player input to retrieve.
     */
    static shared_ptr<PlayerInput> tryMakeInput(GLFWwindow *window);

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
    [[nodiscard]] inline glm::dvec2 getCursorMovement() const { return this->cursorPosition - this->previousCursorPosition; }

    /**
     * Makes a non-fuzzy check on whether the provided key is pressed or not.
     * @param key       The GLFW key value.
     * @param modifier  The GLFW key modifiers value. None (0) by default.
     * @return `true` if the precise key is pressed, `false` otherwise.
     */
    [[nodiscard]] inline bool isKeyDown(int32_t key, int32_t modifier = 0) const {
        KeyData kd(key, modifier);
        return this->keysDown.find(kd) != this->keysDown.end();
    }

    /**
     * Returns the scroll input for this frame
     */
    [[nodiscard]] inline const glm::vec2 &getScrollInput() const { return scrollInput; }

    ~PlayerInput();


protected:

    /**
     * Initializes a player input handler for the provided window.
     * @param window    The window receiving the player input.
     */
    explicit PlayerInput(GLFWwindow *window);

    /// The map of GLFW windows to their associated PlayerInput instances.
#ifdef KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW
    static unordered_map<GLFWwindow *, PlayerInput *> &inputMap();
#else
    static unordered_map<GLFWwindow *, weak_ptr<PlayerInput>> &inputMap();
#endif


protected:

    /// GLFW window this player input is linked to
    GLFWwindow *window;

    /// Set of keys that are pressed
    unordered_set<KeyData, KeyData::Hasher> keysDown {};

    /// The string of text that was input during the frame.
    string inputText {};

    /// The current position of the cursor
    glm::dvec2 cursorPosition {};

    /// The previous cursor position
    glm::dvec2 previousCursorPosition {};

    /// The total scroll input for this frame
    glm::vec2 scrollInput {};


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

        inline bool operator ==(const KeyMapItem &other) const {
            if (associatedKey == other.associatedKey)
                return name == other.name;
            else
                return false;
        }

        /// Hash struct for the PlayerInput::KeyData struct
        struct Hasher {

            inline std::uint64_t operator()(const PlayerInput::KeyMapItem &item) const
            {
                // We use the associated key hash, since there shouldn't be different KeyMap items with the same KeyData
                // but with different names
                return KeyData::Hasher()(item.associatedKey);
            }

        };

    };

    /// The key callback function pointer type
    typedef weak_ptr<function<void ()>> CallbackPointer;

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
    inline void onKeyPress(const string &keyName, const shared_ptr<function<void()>> &callback) {
        this->addCallback(keyName, CallbackPointer(callback), this->keyPressCallbacks);
    }

    /**
     * Calls the provided callback every time this input key is released.
     * @param keyName   The input key name.
     * @param object    The object from which the method callback will be called.
     * @param callback  The callback to call on event, a method of the provided object.
     */
    inline void onKeyRelease(const string &keyName, const shared_ptr<function<void()>> &callback) {
        this->addCallback(keyName, CallbackPointer(callback), this->keyReleaseCallbacks);
    }

protected:

    /// The callbacks list type
    typedef vector<CallbackPointer> CallbackList;

    /**
     * Adds a callback for an input key event
     * @param keyName       The input key name
     * @param object        The object from which the method callback will be called.
     * @param callback      The callback to call on event, a method of the provided object.
     * @param callbacksMap  The collection of callbacks to add to.
     */
    void addCallback(const string &keyName, CallbackPointer callback, unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> &callbacksMap);

    /**
     * Calls all the callbacks corresponding to the provided input.
     * @param data       The input key data.
     * @param callbacks  The collection of callbacks to call.
     */
    void callCallbacks(const KeyData &data,
                       const unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> &callbacks) const;

protected:

    /// The keymap for this input handler.
    unordered_map<string, KeyMapItem> keyMap {};

    /// A rapid access hashmap to get all the PlayerInput::keyMap elements corresponding to a key value.
    unordered_map<int32_t, vector<KeyMapItem>> keyToKeyMapItems {};

    /// All the callbacks associated to the pressing of PlayerInput::keyMap items.
    unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> keyPressCallbacks {};

    /// All the callbacks associated to the releasing of PlayerInput::keyMap items.
    unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> keyReleaseCallbacks {};


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

    /**
     * GLFW callback function for handling mouse scroll input.
     */
    static void handleScrollInput(GLFWwindow *window, double xScroll, double yScroll);

};


#endif //INC_KRYNE_ENGINE_PLAYERINPUT_H
