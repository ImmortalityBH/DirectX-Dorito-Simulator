#pragma once

#include "Win.h"
#include <Xinput.h>

class Gamepad
{
private:
    UINT controllerID;
    XINPUT_STATE state;
    XINPUT_VIBRATION vibration;
    //XINPUT_BATTERY_INFORMATION battery;//not used in v1.4

    const float maxValue = 1.0f;

    float deadzoneX;
    float deadzoneY;

    float ApplyDeadzone(float value, float maxValue, float deadzone);
public:
    Gamepad(UINT id);
    Gamepad(UINT id, float deadzoneX, float deadzoneY);
    UINT getControllerID() const { return controllerID; }
    XINPUT_GAMEPAD* getGamepad();
    //XInputGetBatteryInformation is not supported for v1.4
    //XINPUT_BATTERY_INFORMATION* getBatteryInfo();
    bool isConnected();
    bool Update();
    void Vibrate(USHORT leftSpeed, USHORT rightSpeed);
    void Vibrate(USHORT speed);
    void resetVibration();
    bool isButtonPressed(UINT button) const;

    float leftStickX, leftStickY;
    float rightStickX, rightStickY;
    float leftTrigger, rightTrigger;
};