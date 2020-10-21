#include "mageAPI.h"

#define SORT_NAME input_sort_algorithms
#define SORT_TYPE uint32_t
#include "../Externals/sort/sort.h"

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
                glfwSetInputMode(window->Context, GLFW_RAW_MOUSE_MOTION, MAGE_TRUE); break;
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
        goto callbacks;

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