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
    glfwSetMouseButtonCallback(window, PlayerInput::handleMouseButtonInput);
}


void PlayerInput::handleKeyInput(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    const auto pair = PlayerInput::inputMap.find(window);

    if (pair != PlayerInput::inputMap.end()) {

        const auto playerInput = pair->second;
        KeyData keyData { key, mods };

        if (action == GLFW_PRESS) {
            playerInput->callCallbacks(keyData, playerInput->keyPressCallbacks);
            playerInput->keysDown.emplace(keyData);
        } else if (action == GLFW_RELEASE) {
            playerInput->callCallbacks(keyData, playerInput->keyReleaseCallbacks);
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


void PlayerInput::handleMouseButtonInput(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
{
    const auto pair = PlayerInput::inputMap.find(window);

    if (pair != PlayerInput::inputMap.end()) {

        const auto playerInput = pair->second;
        KeyData keyData { button, mods };

        if (action == GLFW_PRESS) {
            playerInput->callCallbacks(keyData, playerInput->keyPressCallbacks);
            playerInput->keysDown.emplace(keyData);
        } else if (action == GLFW_RELEASE) {
            playerInput->callCallbacks(keyData, playerInput->keyReleaseCallbacks);
            playerInput->keysDown.erase(keyData);
        }
    }
}


void PlayerInput::registerKey(const string &name, int32_t key, int32_t mods)
{
    KeyMapItem item { name, KeyData(key, mods) };
    this->keyMap.emplace(item);

    const auto pair = this->keyToKeyMapItems.find(key);
    if (pair == this->keyToKeyMapItems.end()) {
        vector<KeyMapItem> v = { item };
        this->keyToKeyMapItems.emplace(key, v);
    } else
        pair->second.emplace_back(item);
}


void PlayerInput::addCallback(const string &keyName,
                              PlayerInput::CallbackObject *object,
                              PlayerInput::InputCallback callback,
                              unordered_map<KeyMapItem, CallbackSet> &callbacksMap)
{
    for (const auto &item : this->keyMap) {
        if (item.name == keyName) {

            const auto it = callbacksMap.find(item);

            if (it == callbacksMap.end()) {
                CallbackSet set;
                set.emplace(make_pair(object, callback));

                callbacksMap.emplace(item, set);
            } else
                it->second.emplace(make_pair(object, callback));

            return;
        }
    }
    cerr << "Unable to find key named " << keyName << endl;
}


void PlayerInput::callCallbacks(const KeyData &data, const unordered_map<KeyMapItem, CallbackSet> &callbacks) const
{
    const auto ktkm = this->keyToKeyMapItems.find(data.key);

    if (ktkm != this->keyToKeyMapItems.end()) {

        const auto set = ktkm->second;

        auto it = set.begin();
        auto currentItem = *it;

        for (it++; it != set.end(); it++) {
            if ((it->associatedKey.mod & data.mod) == it->associatedKey.mod && it->associatedKey.mod > currentItem.associatedKey.mod)
                currentItem = *it;
        }

        if ((currentItem.associatedKey.mod & data.mod) == currentItem.associatedKey.mod) {
            const auto map_it = callbacks.find(currentItem);
            if ( map_it != callbacks.end() ) {

                for (const auto &pair : map_it->second)
                    invoke(pair.second, pair.first);

            }
        }

    }
}


void PlayerInput::willPollEvents()
{
    this->previousCursorPosition = this->cursorPosition;
}


glm::dvec2 PlayerInput::getCursorMovement() const
{
    return this->cursorPosition - this->previousCursorPosition;
}


