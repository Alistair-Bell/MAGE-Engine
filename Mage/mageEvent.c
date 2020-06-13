#include "mageAPI.h"

static mageEventCategoryBit mageNoneCategories[] = 
{
    MAGE_NONE_CATEGORY,
};
static mageEventCategoryBit mageWindowCloseEventCategories[] =
{
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageWindowFocusCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY
};
static mageEventCategoryBit mageWindowLostFocusCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageWindowMovedCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageApplicationTickCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageApplicationUpdateCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageApplicationRenderCategories[] = 
{ 
    MAGE_APPLICATION_CATEGORY,
};
static mageEventCategoryBit mageKeyPressedCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_KEYBOARD_CATEGORY,
};
static mageEventCategoryBit mageKeyReleasedCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_KEYBOARD_CATEGORY,
};
static mageEventCategoryBit mageMouseButtonPressedCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_MOUSE_CATEGORY,
    MAGE_MOUSE_BUTTON_CATEGORY,
};
static mageEventCategoryBit mageMouseButtonReleasedCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_MOUSE_CATEGORY,
    MAGE_MOUSE_BUTTON_CATEGORY,
};
static mageEventCategoryBit mageMouseMovedCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_MOUSE_CATEGORY,
};
static mageEventCategoryBit mageMouseScrolledCategories[] = 
{ 
    MAGE_INPUT_CATEGORY,
    MAGE_MOUSE_CATEGORY,
};
	

mageEventHandle mageEventHandleCreate(const mageEventType type)
{
    uint32_t i;
    mageEventHandle handle;
    handle = 0;
    mageEventCategoryBit *categories = mageEventGenerateCategories(type);
    uint32_t argumentCount = sizeof(*categories)  / sizeof(mageEventCategoryBit);
    
    handle = type;

    for (i = 0; i < argumentCount; i++)
    {
        MAGE_SET_BIT(handle, categories[i], 1);
    }
    MAGE_SET_BIT(handle, 15, 0);
    return handle;
}
mageEventCategoryBit *mageEventGenerateCategories(const mageEventType type)
{
    switch (type)
    {
        case MAGE_NONE_EVENT:
            return mageNoneCategories;
            break;
        case MAGE_WINDOW_CLOSE_EVENT:
            return mageWindowCloseEventCategories;
            break;
        case MAGE_WINDOW_FOCUS_EVENT:
            return mageWindowFocusCategories;
            break;
        case MAGE_WINDOW_LOST_FOCUS_EVENT:
            return mageWindowFocusCategories;
            break;
        case MAGE_WINDOW_MOVED_EVENT:
            return mageWindowMovedCategories;
            break;
        case MAGE_APPLICATION_TICK_EVENT:
            return mageApplicationTickCategories;
            break;
        case MAGE_APPLICATION_UPDATE_EVENT:
            return mageApplicationUpdateCategories;
            break;
        case MAGE_APPLICATION_RENDER_EVENT:
            return mageApplicationRenderCategories;
            break;
        case MAGE_KEY_PRESSED_EVENT:
            return mageKeyPressedCategories;
            break;
        case MAGE_KEY_RELEASED_EVENT:
            return mageKeyReleasedCategories;
            break;
        case MAGE_MOUSE_BUTTON_PRESSED_EVENT:
            return mageMouseButtonPressedCategories;
            break;
        case MAGE_MOUSE_BUTTON_RELEASED_EVENT:
            return mageMouseButtonReleasedCategories;
            break;
        case MAGE_MOUSE_MOVED_EVENT:
            return mageMouseMovedCategories;
            break;
        case MAGE_MOUSE_SCROLLED_EVENT:
            return mageMouseScrolledCategories;
            break;
    }
}
mageEventType mageEventExtractEventType(mageEventHandle handle)
{
    return handle &= 0b1111;
}
uint8_t mageEventInCategory(const mageEventHandle handle, const mageEventCategoryBit category)
{
    mageEventCategoryBit *categories = mageEventGenerateCategories(mageEventExtractEventType(handle));
    uint8_t categoryCount = (sizeof(*categories) / sizeof(mageEventCategoryBit));
    uint8_t i;
    for (i = 0; i < categoryCount; i++) 
    {
        if (categories[i] == category) return 1;
    }
    return 0;
}


