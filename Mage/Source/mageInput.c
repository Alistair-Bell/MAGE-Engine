#include "mageAPI.h"

#define STR2(s) #s
#define STR(s) STR2(s)

void mageUserInputInquirerSetup(struct mageWindow *window, struct mageUserInputInquirerSetupInfo *info)
{
    /* Sets keyboard and mouse flags for glfw to use within the input system */
    
    MAGE_ASSERT(window != NULL);

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







}