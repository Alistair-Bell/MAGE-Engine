#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "CreateStructures.h"
#include "Includes.h"
#include "Window.h"

#define MAGE_MOUSE_STATE_COUNT    3
#define MAGE_KEYBOARD_STATE_COUNT 128

typedef enum MageKeyboardKey
{
    MAGE_KEYBOARD_KEY_A = 0,
    MAGE_KEYBOARD_KEY_C,
    MAGE_KEYBOARD_KEY_D,
    MAGE_KEYBOARD_KEY_E,
    MAGE_KEYBOARD_KEY_F,
    MAGE_KEYBOARD_KEY_G,
    MAGE_KEYBOARD_KEY_H,
    MAGE_KEYBOARD_KEY_I,
    MAGE_KEYBOARD_KEY_J,
    MAGE_KEYBOARD_KEY_K,
    MAGE_KEYBOARD_KEY_L,
    MAGE_KEYBOARD_KEY_M,
    MAGE_KEYBOARD_KEY_N,
    MAGE_KEYBOARD_KEY_O,
    MAGE_KEYBOARD_KEY_P,
    MAGE_KEYBOARD_KEY_Q,
    MAGE_KEYBOARD_KEY_R,
    MAGE_KEYBOARD_KEY_S,
    MAGE_KEYBOARD_KEY_T,
    MAGE_KEYBOARD_KEY_U,
    MAGE_KEYBOARD_KEY_W,
    MAGE_KEYBOARD_KEY_X,
    MAGE_KEYBOARD_KEY_Y,
    MAGE_KEYBOARD_KEY_Z,
} MageKeyboardKey;

typedef enum MageButtonState
{
    MAGE_BUTTON_STATE_PRESS = 0x01,
    MAGE_BUTTON_STATE_REPEAT = 0x02,
    MAGE_BUTTON_STATE_RELEASE = 0x04
} MageButtonState;

typedef struct MageInputHandler
{
    U8              MouseStates[MAGE_MOUSE_STATE_COUNT];
    U8              KeyboardStates[MAGE_KEYBOARD_STATE_COUNT];
    F32             MousePositionX;
    F32             MousePositionY;
} MageInputHandler;

extern U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler);
extern U8 MageInputHandlerPollEvents(MageInputHandler *handler);
extern U8 MageInputHandlerDestroy(MageInputHandler *handler);

#endif