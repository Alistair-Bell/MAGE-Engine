#include "mageAPI.h"

#define SORT_NAME input_sort_algorithms
#define SORT_TYPE uint32_t
#include "../Externals/sort/sort.h"

int32_t mageToGLFWKey(mageKeyCode code);

void mageUserInputInquirerSetup(struct mageWindow *window, struct mageUserInputInquirerSetupInfo *info)
{
    /* Sets keyboard and mouse flags for glfw to use within the input system */
    
    MAGE_ASSERT(window != NULL);
    MAGE_ASSERT(info != NULL);

    uint32_t i;
    for (i = 0; i < info->KeyboardFlagsCount; i++)
    {
        switch (info->KeyboardFlags[i])
        {
            case MAGE_KEYBOARD_SETUP_FLAGS_NONE:
                break;
            
            case MAGE_KEYBOARD_SETUP_FLAGS_ENABLE_STICKY:
                glfwSetInputMode(window->Context, GLFW_STICKY_KEYS, MAGE_TRUE); break;
        }
    }
    for (i = 0; i < info->MouseFlagsCount; i++)
    {
        switch (info->MouseFlags[i])
        {
            case MAGE_MOUSE_SETUP_FLAGS_NONE:
                break;
    
            case MAGE_MOUSE_SETUP_FLAGS_RAW_MOTION:
            {
                if (glfwRawMouseMotionSupported())
                    glfwSetInputMode(window->Context, GLFW_RAW_MOUSE_MOTION, MAGE_TRUE); 
                else
                    MAGE_LOG_CORE_ERROR("Raw motion not supported, native window framework may lack support\n", NULL);
                break;
            }
        }
    }
    for (i = 0; i < info->CursorFlagsCount; i++)
    {
        switch (info->CursorFlags[i])
        {
            case MAGE_MOUSE_CURSOR_SETUP_FLAGS_NONE:
                break;

            case MAGE_MOUSE_CURSOR_SETUP_FLAGS_ENABLED:
                glfwSetInputMode(window->Context, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
            
            case MAGE_MOUSE_CURSOR_SETUP_FLAGS_HIDDEN:
                glfwSetInputMode(window->Context, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
            
            case MAGE_MOUSE_CURSOR_SETUP_FLAGS_DISABLED:
                glfwSetInputMode(window->Context, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
        }
    }


    input_sort_algorithms_quick_sort(info->ExternalInputFlags, info->ExtenalInputFlagsCount);
    if (0 < info->ExtenalInputFlagsCount && info->ExternalInputFlags[0] == MAGE_EXTERNAL_INPUT_SETUP_FLAGS_NONE)
        goto callbacks; /* Goto is not evil, people just use it wrong */

    struct mageJoystickInfo joys[GLFW_JOYSTICK_LAST];
    memset(joys, 0, sizeof(joys));

    uint8_t foundJoystick   = MAGE_FALSE;
    uint8_t foundGamePad    = MAGE_FALSE;
    uint8_t gamePadCount, joystickCount;

    /* Searching for all available joysticks at ports 0 -> 15 */
    for (i = 0; i < GLFW_JOYSTICK_LAST; i++)
    {
        joys[i].Present         = glfwJoystickPresent(i);
        joys[i].Index           = i;
        joys[i].Gamepad         = glfwJoystickIsGamepad(i);
    
        /* Joystick has been found */
        if (joys[i].Present && joys[i].Gamepad == MAGE_FALSE)
        {
            MAGE_LOG_CORE_INFORM("Found joystick %s residing at port %d of %d\n", glfwGetJoystickName(i), i, GLFW_JOYSTICK_LAST);
            joystickCount++;
            foundJoystick = MAGE_TRUE;
        }
        else if (joys[i].Present && joys[i].Gamepad == MAGE_TRUE)
        {
            MAGE_LOG_CORE_INFORM("Found gamepad %s residing at port %d of %d\n", glfwGetJoystickName(i), i, GLFW_JOYSTICK_LAST);
            gamePadCount++;
            foundGamePad = MAGE_TRUE;
        }
    }
    MAGE_LOG_CORE_INFORM("External input sources report: [Joysticks %d] [Gamepads %d]\n", joystickCount, gamePadCount);

    for (i = 1; i < info->ExtenalInputFlagsCount; i++)
    {
        if (info->ExternalInputFlags[i] == MAGE_EXTERNAL_INPUT_SETUP_REQUIRE_PRESENT_JOYSTICK) 
        {
            MAGE_ASSERT_MESSAGE(foundJoystick != MAGE_FALSE, "Unable to find required joystick!\n", NULL);
        }
        else if (info->ExternalInputFlags[i] == MAGE_EXTERNAL_INPUT_SETUP_REQUIRE_PRESENT_GAMEPAD)
        {
            MAGE_ASSERT_MESSAGE(foundGamePad != MAGE_FALSE, "Unable to find required gamepad!\n", NULL);
        }
    }
    
    /* Allows users to set callbacks */
    callbacks:
    {
        if (info->Callbacks != NULL)
        {
            struct mageUserInputCallbacks calls = *info->Callbacks;
            if (calls.KeyBoardPress != NULL)
                glfwSetKeyCallback(window->Context, calls.KeyBoardPress);
            if (calls.MouseButtonPress != NULL)
                glfwSetMouseButtonCallback(window->Context, calls.MouseButtonPress);
        }
    }
}
/* Not my proudest moment */
#define MAGE_SWITCH_CASE_BREAK_MACRO(value, returning) \
    case value: \
        return (int32_t)returning

inline int32_t mageToGLFWKey(mageKeyCode code)
{
    switch (code)
    {
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_A, GLFW_KEY_A);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_B, GLFW_KEY_B);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_C, GLFW_KEY_C);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_D, GLFW_KEY_D);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_E, GLFW_KEY_E);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_F, GLFW_KEY_F);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_G, GLFW_KEY_G);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_H, GLFW_KEY_H);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_I, GLFW_KEY_I);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_J, GLFW_KEY_J);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_K, GLFW_KEY_K);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_L, GLFW_KEY_L);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_M, GLFW_KEY_M);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_N, GLFW_KEY_N);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_O, GLFW_KEY_O);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_P, GLFW_KEY_P);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_Q, GLFW_KEY_Q);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_R, GLFW_KEY_R);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_S, GLFW_KEY_S);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_T, GLFW_KEY_T);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_U, GLFW_KEY_U);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_V, GLFW_KEY_V);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_W, GLFW_KEY_W);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_X, GLFW_KEY_X);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_Y, GLFW_KEY_Y);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_Z, GLFW_KEY_Z);

        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_SPACE, GLFW_KEY_SPACE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_APOSTROPHE, GLFW_KEY_APOSTROPHE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_COMMA, GLFW_KEY_COMMA);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_MINUS, GLFW_KEY_MINUS);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_PERIOD, GLFW_KEY_PERIOD);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_SLASH, GLFW_KEY_SLASH);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_EQUAL, GLFW_KEY_EQUAL);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_SEMI_COLON, GLFW_KEY_SEMICOLON);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_LEFT_BRACKET, GLFW_KEY_LEFT_BRACKET);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_BACKSLASH, GLFW_KEY_BACKSLASH);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_RIGHT_BRACKET, GLFW_KEY_RIGHT_BRACKET);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_GRAVE_ACCENT, GLFW_KEY_GRAVE_ACCENT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_ESCAPE, GLFW_KEY_ESCAPE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_ENTER, GLFW_KEY_ENTER);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_TAB, GLFW_KEY_TAB);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_BACKSPACE, GLFW_KEY_BACKSPACE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_INSERT, GLFW_KEY_INSERT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_DELETE, GLFW_KEY_DELETE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_DIRECTIONAL_RIGHT, GLFW_KEY_RIGHT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_DIRECTIONAL_LEFT, GLFW_KEY_LEFT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_DIRECTIONAL_DOWN, GLFW_KEY_DOWN);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_DIRECTIONAL_UP, GLFW_KEY_UP);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_PAGE_UP, GLFW_KEY_PAGE_UP);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_PAGE_DOWN, GLFW_KEY_PAGE_DOWN);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_HOME, GLFW_KEY_HOME);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_END, GLFW_KEY_END);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_CAPS_LOCK, GLFW_KEY_CAPS_LOCK);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_NUM_LOCK, GLFW_KEY_NUM_LOCK);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_PRINT_SCREEN, GLFW_KEY_PRINT_SCREEN);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_PAUSE, GLFW_KEY_PAUSE);

        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_0, GLFW_KEY_0);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_1, GLFW_KEY_1);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_2, GLFW_KEY_2);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_3, GLFW_KEY_3);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_4, GLFW_KEY_4);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_5, GLFW_KEY_5);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_6, GLFW_KEY_6);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_7, GLFW_KEY_7);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_8, GLFW_KEY_8);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_9, GLFW_KEY_9);

        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_0, GLFW_KEY_KP_0);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_1, GLFW_KEY_KP_1);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_2, GLFW_KEY_KP_2);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_3, GLFW_KEY_KP_3);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_4, GLFW_KEY_KP_4);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_5, GLFW_KEY_KP_5);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_6, GLFW_KEY_KP_6);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_7, GLFW_KEY_KP_7);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_8, GLFW_KEY_KP_8);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_9, GLFW_KEY_KP_9);

        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_DECIMAL, GLFW_KEY_KP_DECIMAL);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_DIVIDE, GLFW_KEY_KP_DIVIDE);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_MULTIPLY, GLFW_KEY_KP_MULTIPLY);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_SUBTRACT, GLFW_KEY_KP_SUBTRACT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_ADD, GLFW_KEY_KP_ADD);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_ENTER, GLFW_KEY_KP_ENTER);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_KEYPAD_EQUAL, GLFW_KEY_KP_EQUAL);

	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_LEFT_CONTROL, GLFW_KEY_LEFT_CONTROL);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_LEFT_ALT, GLFW_KEY_LEFT_ALT);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_LEFT_SUPER, GLFW_KEY_LEFT_SUPER);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_RIGHT_SHIFT, GLFW_KEY_RIGHT_SHIFT);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_RIGHT_CONTROL, GLFW_KEY_RIGHT_CONTROL);
	    MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_RIGHT_ALT, GLFW_KEY_RIGHT_ALT);
        MAGE_SWITCH_CASE_BREAK_MACRO(MAGE_KEYCODE_RIGHT_SUPER, GLFW_KEY_RIGHT_SUPER);
        //default:
        //    MAGE_LOG_CORE_ERROR("Invalid keycode %lu, expected values to range between %lu\n", MAGE_KEYCODE_RIGHT_SUPER - MAGE_KEYCODE_A);

    }
    return MAGE_KEYCODE_APOSTROPHE;
}

struct mageKeyState mageUserInputInquireKey(struct mageWindow *window, mageKeyCode code)
{
    return (struct mageKeyState) { .KeyCode = (uint8_t)code, .KeyState = (uint8_t)glfwGetKey(window->Context, mageToGLFWKey(code)) };
}