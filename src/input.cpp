//
// Created by Washington on 26/02/2020.
//

#include "input.hpp"
#include "opengl.hpp"
#include "application.hpp"

TInput::TInput() {
    std::fill(mAxisState.begin(), mAxisState.end(), 0.0f);
}

void TInput::Connect() {
    BIND_EVENT_CALLBACK(Events::MOUSE_BUTTON_PRESSED, this, OnMouseButtonPressed);
    BIND_EVENT_CALLBACK(Events::MOUSE_BUTTON_RELEASED, this, OnMouseButtonReleased);
    BIND_EVENT_CALLBACK(Events::KEY_PRESSED, this, OnKeyPressed);
    BIND_EVENT_CALLBACK(Events::KEY_RELEASED, this, OnKeyReleased);
    BIND_EVENT_CALLBACK(Events::MOUSE_POSITION_CHANGED, this, OnMousePositionChanged);
    BIND_EVENT_CALLBACK(Events::SCROLL_CHANGED, this, OnScrollChanged);
}

void TInput::Update() {
    mAxisState[INPUT_AXIS_X] = mKeyState[GLFW_KEY_LEFT] || mKeyState[GLFW_KEY_A] ? -1.0f :
                               mKeyState[GLFW_KEY_RIGHT] || mKeyState[GLFW_KEY_D] ? 1.0f : 0.0f;
    mAxisState[INPUT_AXIS_Y] = mKeyState[GLFW_KEY_UP] || mKeyState[GLFW_KEY_W] ? -1.0f :
                               mKeyState[GLFW_KEY_DOWN] || mKeyState[GLFW_KEY_S] ? 1.0f : 0.0f;

    const auto& appInfo = Application::sThisApplication->GetAppInfo();
    if(mFirstMovement) {
        mFirstMovement = false;
        mLastMouseX = mCurrentMouseX;
        mLastMouseY = mCurrentMouseY;
    }

    mAxisState[INPUT_AXIS_MOUSE_X] = (mLastMouseX - mCurrentMouseX)/(appInfo->windowWidth);
    mAxisState[INPUT_AXIS_MOUSE_Y] = (mCurrentMouseY - mLastMouseY)/(appInfo->windowHeight);

    mLastMouseX = mCurrentMouseX;
    mLastMouseY = mCurrentMouseY;

    if(mScrollConsumed) {
        mAxisState[INPUT_AXIS_SCROLL_WHEEL] = 0.0f;
    }

    mScrollConsumed = true;
}

void TInput::Disconnect() {
    UNBIND_CALLBACK(this);
}

float TInput::GetAxis(int axis) const {
    return mAxisState[axis];
}

bool TInput::GetMouseButtonDown(int button) const {
    return mMouseButtonState.test(button);
}

bool TInput::GetMouseButtonUp(int button) const {
    return !mMouseButtonState.test(button);
}

bool TInput::GetKeyDown(int key) const {
    return mKeyState.test(key);
}

bool TInput::GetKeyUp(int key) const {
    return !mKeyState.test(key);
}

bool TInput::GetAnyKeyDown() const {
    return mKeyState.any() || mMouseButtonState.any();
}

void TInput::GetMousePosition(float &x, float &y) {
    x = mCurrentMouseX;
    y = mCurrentMouseY;
}

void TInput::OnMouseButtonPressed(int, void *data) {
    auto button = (int*)data;
    if(*button < 0 || *button >= mMouseButtonState.size()) return;
    mMouseButtonState.set(*button, true);
}

void TInput::OnMouseButtonReleased(int, void *data) {
    auto button = (int*)data;
    if(*button < 0 || *button >= mMouseButtonState.size()) return;
    mMouseButtonState.set(*button, false);
}

void TInput::OnKeyPressed(int, void *data) {
    auto key = (int*)data;
    if(*key < 0 || *key >= mKeyState.size()) return;
    mKeyState.set(*key, true);
}

void TInput::OnKeyReleased(int, void *data) {
    auto key = (int*)data;
    if(*key < 0 || *key >= mKeyState.size()) return;
    mKeyState.set(*key, false);
}

void TInput::OnMousePositionChanged(int, void *data) {
    auto pos = (float*)data;
    mCurrentMouseX = pos[0];
    mCurrentMouseY = pos[1];
}

void TInput::OnScrollChanged(int, void *data) {
    auto dir = (float*)data;
    mAxisState[INPUT_AXIS_SCROLL_WHEEL] = dir[1];
    mScrollConsumed = false;
}
