#include "mageAPI.h"



/*!************************
 * This is probaly the worst thing I have ever written
**************************/

static mageEventCategoryBit mageNoneCategories[] =                { MAGE_NONE_CATEGORY };
static mageEventCategoryBit mageWindowCloseEventCategories[] =    { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageWindowFocusCategories[] =         { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageWindowLostFocusCategories[] =     { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageWindowMovedCategories[] =         { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageApplicationTickCategories[] =     { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageApplicationUpdateCategories[] =   { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageApplicationRenderCategories[] =   { MAGE_APPLICATION_CATEGORY };
static mageEventCategoryBit mageKeyPressedCategories[] =          { MAGE_INPUT_CATEGORY, MAGE_KEYBOARD_CATEGORY };
static mageEventCategoryBit mageKeyReleasedCategories[] =         { MAGE_INPUT_CATEGORY, MAGE_KEYBOARD_CATEGORY };
static mageEventCategoryBit mageMouseButtonPressedCategories[] =  { MAGE_INPUT_CATEGORY, MAGE_MOUSE_CATEGORY, MAGE_MOUSE_BUTTON_CATEGORY };
static mageEventCategoryBit mageMouseButtonReleasedCategories[] = { MAGE_INPUT_CATEGORY, MAGE_MOUSE_CATEGORY, MAGE_MOUSE_BUTTON_CATEGORY };
static mageEventCategoryBit mageMouseMovedCategories[] =          { MAGE_INPUT_CATEGORY, MAGE_MOUSE_CATEGORY };
static mageEventCategoryBit mageMouseScrolledCategories[] =       { MAGE_INPUT_CATEGORY, MAGE_MOUSE_CATEGORY };



struct mageEventMaster
{
    mageEventListenerCallback *Listeners;
    uint64_t                   ListenerCount;
};

static struct mageEventMaster EventMaster;

void mageEventSetupMaster()
{
    memset(&EventMaster, 0, sizeof(struct mageEventMaster));
    EventMaster.Listeners = calloc(0, sizeof(mageEventListenerCallback));
}
uint16_t mageEventHandleCreate(const mageEventType type)
{
    uint32_t i;
    uint16_t handle;
    handle = 0;
    mageEventCategoryBit *categories = mageEventGenerateCategories(type);
    uint32_t argumentCount = sizeof(*categories)  / sizeof(mageEventCategoryBit);
    
    handle = type;

    for (i = 0; i < argumentCount; i++)
    {
        MAGE_SET_BIT(handle, categories[i], 1);
    }
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
mageEventType mageEventExtractEventType(uint16_t handle)
{
    return handle &= 0b1111;
}
uint8_t mageEventInCategory(const uint16_t handle, const mageEventCategoryBit category)
{
    mageEventCategoryBit *categories = mageEventGenerateCategories(mageEventExtractEventType(handle));
    uint8_t categoryCount = (sizeof(*categories) / sizeof(mageEventCategoryBit));
    uint8_t i;
    for (i = 0; i < categoryCount; i++) 
    {
        if (categories[i] == category) return 1;
    }
    free(categories);
    return 0;
}
void mageEventRegisterListener(mageEventListenerCallback callback)
{
    EventMaster.ListenerCount++;
    EventMaster.Listeners = realloc(EventMaster.Listeners, EventMaster.ListenerCount * sizeof(mageEventListenerCallback));
    EventMaster.Listeners[EventMaster.ListenerCount - 1] = callback;
}
void mageEventFormatWindowClose(void *buffer)
{   
    uint16_t temp = mageEventHandleCreate(MAGE_WINDOW_CLOSE_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowFocus(void *buffer) 
{
    uint16_t temp = mageEventHandleCreate(MAGE_WINDOW_FOCUS_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowLostFocus(void *buffer)
{
    uint16_t temp = mageEventHandleCreate(MAGE_WINDOW_LOST_FOCUS_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowMoved(void *buffer, const int32_t x, const int32_t y)
{
    uint16_t temp = mageEventHandleCreate(MAGE_WINDOW_MOVED_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &x, sizeof(int32_t));
    memmove(buffer + 6, &y, sizeof(int32_t));
}
void mageEventFormatKeyPressed(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_KEY_PRESSED_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatKeyReleased(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_KEY_RELEASED_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatKeyRepeat(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_KEY_REPEAT_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatMouseMoved(void *buffer, const double x, const double y)
{
    uint16_t temp = mageEventHandleCreate(MAGE_MOUSE_MOVED_EVENT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &x, sizeof(double));
    memmove(buffer + 10, &y, sizeof(double));    
}
void mageEventDispatch(void *event)
{
    uint64_t i;
    uint16_t foo = 0;
    memcpy(&foo, event, 2);
    for (i = 0; i < EventMaster.ListenerCount; i++) 
    {
        EventMaster.Listeners[i](event, mageEventExtractEventType(foo));
    }
}

