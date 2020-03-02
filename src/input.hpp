//
// Created by Washington on 26/02/2020.
//

#ifndef ENGINE3D_SRC_INPUT_HPP
#define ENGINE3D_SRC_INPUT_HPP

#include <bitset>
#include "messagehandler.hpp"
#include "singleton.hpp"
#include "inputstates.hpp"

class TInput : public MessageHandler {
public:
    TInput();

    void Connect();
    void Update();
    void Disconnect();

    [[nodiscard]] float GetAxis(int axis) const;
    [[nodiscard]] bool GetMouseButtonDown(int button) const;
    [[nodiscard]] bool GetMouseButtonUp(int button) const;
    [[nodiscard]] bool GetKeyDown(int key) const;
    [[nodiscard]] bool GetKeyUp(int key) const;
    [[nodiscard]] bool GetAnyKeyDown() const;
    void GetMousePosition(float &x, float &y);

    enum {NUM_BUTTON_STATES = MOUSE_BUTTON_LAST + 1};
    enum {NUM_KEY_STATES = KEY_LAST + 1};

    void OnMouseButtonPressed(int event, void* data);
    void OnMouseButtonReleased(int event, void* data);
    void OnKeyPressed(int event, void* data);
    void OnKeyReleased(int event, void* data);
    void OnMousePositionChanged(int event, void* data);
    void OnScrollChanged(int event, void* data);
private:
    std::bitset<NUM_BUTTON_STATES> mMouseButtonState{0};
    std::bitset<NUM_KEY_STATES> mKeyState{0};
    std::array<float, NUM_INPUT_AXIS> mAxisState{};
    float mLastMouseX{0.0f};
    float mLastMouseY{0.0f};
    float mCurrentMouseX{0.0f};
    float mCurrentMouseY{0.0f};
    bool mFirstMovement{true};
    bool mScrollConsumed{false};
};

namespace Input {
    inline void Connect() {Singleton<TInput>::GetInstance()->Connect();}
    inline void Update() {Singleton<TInput>::GetInstance()->Update();}
    inline void Disconnect() {Singleton<TInput>::GetInstance()->Disconnect();}
    inline float GetAxis(int axis) {return Singleton<TInput>::GetInstance()->GetAxis(axis);}
    inline bool GetMouseButtonDown(int button) {return Singleton<TInput>::GetInstance()->GetMouseButtonDown(button);}
    inline bool GetMouseButtonUp(int button) {return Singleton<TInput>::GetInstance()->GetMouseButtonUp(button);}
    inline bool GetKeyDown(int key) {return Singleton<TInput>::GetInstance()->GetKeyDown(key);}
    inline bool GetKeyUp(int key) {return Singleton<TInput>::GetInstance()->GetKeyUp(key);}
    inline bool GetAnyKeyDown() {return Singleton<TInput>::GetInstance()->GetAnyKeyDown();}
}

#endif //ENGINE3D_SRC_INPUT_HPP
