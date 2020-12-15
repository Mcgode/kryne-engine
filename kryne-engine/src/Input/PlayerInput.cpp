/**
 * @file
 * @author Max Godefroy
 * @date 01/12/2020.
 */

#include "kryne-engine/Input/PlayerInput.h"


#ifdef KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW
#define playerInputPointer(pair)    pair->second
#else
#define playerInputPointer(pair)    pair->second.lock()
#endif

#ifdef KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW

unordered_map<GLFWwindow *, PlayerInput *> &PlayerInput::inputMap()
{
    static unordered_map<GLFWwindow *, PlayerInput *> inputMap {};
    return inputMap;
}

#else

unordered_map<GLFWwindow *, weak_ptr<PlayerInput>> &PlayerInput::inputMap()
{
    static unordered_map<GLFWwindow *, weak_ptr<PlayerInput>> inputMap {};
    return inputMap;
}

#endif


shared_ptr<PlayerInput> PlayerInput::tryMakeInput(GLFWwindow *window)
{
    auto &inputMap = PlayerInput::inputMap();

    const auto it = inputMap.find(window);
    if (it != inputMap.end()) {
        return nullptr;
    } else {
        shared_ptr<PlayerInput> newInput(new PlayerInput(window));

#ifdef KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW
        inputMap.emplace(make_pair(window, newInput.get()));
#else
        inputMap.emplace(make_pair(window, weak_ptr<PlayerInput>(newInput)));
#endif

        return newInput;
    }
}


PlayerInput::PlayerInput(GLFWwindow *window) : window(window)
{
    glfwSetKeyCallback(window, PlayerInput::handleKeyInput);
    glfwSetCharCallback(window, PlayerInput::handleTextInput);
    glfwSetCursorPosCallback(window, PlayerInput::handleCursorPosition);
    glfwSetMouseButtonCallback(window, PlayerInput::handleMouseButtonInput);
}


PlayerInput::~PlayerInput()
{
#ifdef KRYNE_ENGINE_PLAYER_INPUT_INPUT_MAP_RAW
    auto &inputMap = PlayerInput::inputMap();

    for (auto it = inputMap.begin(); it != inputMap.end(); it++) {
        if (it->second == this) {
            inputMap.erase(it);
            break;
        }
    }
#endif
}


void PlayerInput::handleKeyInput(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    auto &inputMap = PlayerInput::inputMap();
    const auto pair = inputMap.find(window);

    if (pair != inputMap.end()) {

        const auto playerInput = playerInputPointer(pair);

        if (playerInput)
        {
            KeyData keyData { key, mods };

            if (action == GLFW_PRESS) {
                playerInput->callCallbacks(keyData, playerInput->keyPressCallbacks);
                playerInput->keysDown.emplace(keyData);
            } else if (action == GLFW_RELEASE) {
                playerInput->callCallbacks(keyData, playerInput->keyReleaseCallbacks);
                playerInput->keysDown.erase(keyData);
            }
        }
        else
        {
            inputMap.erase(pair);
        }

    }
}


void PlayerInput::handleTextInput(GLFWwindow *window, uint32_t unicodeChar)
{
    auto &inputMap = PlayerInput::inputMap();
    const auto pair = inputMap.find(window);

    if (pair != inputMap.end()) {

        const auto playerInput = playerInputPointer(pair);

        if (playerInput)
        {
            playerInput->inputText += boost::lexical_cast<string>(unicodeChar);
        }
        else
        {
            inputMap.erase(pair);
        }

    }
}


void PlayerInput::handleCursorPosition(GLFWwindow *window, double x, double y)
{
    auto &inputMap = PlayerInput::inputMap();
    const auto pair = inputMap.find(window);

    if (pair != inputMap.end()) {

        const auto playerInput = playerInputPointer(pair);

        if (playerInput)
        {
            playerInput->cursorPosition = glm::dvec2(x, y);
        }
        else
        {
            inputMap.erase(pair);
        }

    }
}


void PlayerInput::handleMouseButtonInput(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
{
    auto &inputMap = PlayerInput::inputMap();
    const auto pair = inputMap.find(window);

    if (pair != inputMap.end())
    {
        const auto playerInput = playerInputPointer(pair);

        if (playerInput)
        {
            KeyData keyData { button, mods };

            if (action == GLFW_PRESS) {
                playerInput->callCallbacks(keyData, playerInput->keyPressCallbacks);
                playerInput->keysDown.emplace(keyData);
            } else if (action == GLFW_RELEASE) {
                playerInput->callCallbacks(keyData, playerInput->keyReleaseCallbacks);
                playerInput->keysDown.erase(keyData);
            }
        }
        else
        {
            inputMap.erase(pair);
        }
    }
}


void PlayerInput::registerKey(const string &name, int32_t key, int32_t mods)
{
    KeyMapItem item { name, KeyData(key, mods) };
    this->keyMap.insert(make_pair(name, item));

    const auto pair = this->keyToKeyMapItems.find(key);
    if (pair == this->keyToKeyMapItems.end()) {
        vector<KeyMapItem> v = { item };
        this->keyToKeyMapItems.emplace(key, v);
    } else
        pair->second.emplace_back(item);
}


void PlayerInput::addCallback(const string &keyName, CallbackPointer callback,
                              unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> &callbacksMap)
{
    const auto location = this->keyMap.find(keyName);
    if (location != this->keyMap.end())
    {
        const auto it = callbacksMap.find(location->second);

        if (it == callbacksMap.end()) {
            CallbackList list;
            list.push_back(std::move(callback));

            callbacksMap.emplace(location->second, list);
        } else
            it->second.push_back(std::move(callback));

        return;
    }
    cerr << "Unable to find key named " << keyName << endl;
}


void PlayerInput::callCallbacks(const KeyData &data, const unordered_map<KeyMapItem, CallbackList, KeyMapItem::Hasher> &callbacks) const
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
            auto map_it = callbacks.find(currentItem);
            if ( map_it != callbacks.end() ) {

                auto callbacksSet = map_it->second;
                auto setIterator = callbacksSet.begin();
                while (setIterator != callbacksSet.end())
                {
                    const auto p = setIterator->lock();
                    if (p) {
                        (*p) (); // Invoke callback
                        setIterator++;
                    } else
                        setIterator = callbacksSet.erase(setIterator);
                }

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



