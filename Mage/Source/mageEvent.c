#include "mageAPI.h"



/*!************************
 * This is probaly the worst thing I have ever written
**************************/

static mageEventCategoryBit mageNoneCategories[] =                { MAGE_EVENT_CATEGORY_NONE };
static mageEventCategoryBit mageWindowCloseEventCategories[] =    { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageWindowFocusCategories[] =         { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageWindowLostFocusCategories[] =     { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageWindowMovedCategories[] =         { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageApplicationTickCategories[] =     { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageApplicationUpdateCategories[] =   { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageApplicationRenderCategories[] =   { MAGE_EVENT_CATEGORY_APPLICATION };
static mageEventCategoryBit mageKeyPressedCategories[] =          { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_KEYBOARD };
static mageEventCategoryBit mageKeyReleasedCategories[] =         { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_KEYBOARD };
static mageEventCategoryBit mageMouseButtonPressedCategories[] =  { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_MOUSE, MAGE_EVENT_CATEGORY_MOUSE_BUTTON };
static mageEventCategoryBit mageMouseButtonReleasedCategories[] = { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_MOUSE, MAGE_EVENT_CATEGORY_MOUSE_BUTTON };
static mageEventCategoryBit mageMouseButtonRepeatCategories[] = { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_MOUSE, MAGE_EVENT_CATEGORY_MOUSE_BUTTON };
static mageEventCategoryBit mageMouseMovedCategories[] =          { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_MOUSE };
static mageEventCategoryBit mageMouseScrolledCategories[] =       { MAGE_EVENT_CATEGORY_INPUT, MAGE_EVENT_CATEGORY_MOUSE };



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
        case MAGE_EVENT_NONE:
            return mageNoneCategories;
            break;
        case MAGE_EVENT_WINDOW_CLOSE:
            return mageWindowCloseEventCategories;
            break;
        case MAGE_EVENT_WINDOW_FOCUS:
            return mageWindowFocusCategories;
            break;
        case MAGE_EVENT_WINDOW_LOST_FOCUS:
            return mageWindowFocusCategories;
            break;
        case MAGE_EVENT_WINDOW_MOVED:
            return mageWindowMovedCategories;
            break;
        case MAGE_EVENT_APPLICATION_TICK:
            return mageApplicationTickCategories;
            break;
        case MAGE_EVENT_APPLICATION_UPDATE:
            return mageApplicationUpdateCategories;
            break;
        case MAGE_EVENT_APPLICATION_RENDER:
            return mageApplicationRenderCategories;
            break;
        case MAGE_EVENT_KEY_PRESSED:
            return mageKeyPressedCategories;
            break;
        case MAGE_EVENT_KEY_RELEASED:
            return mageKeyReleasedCategories;
            break;
        case MAGE_EVENT_KEY_REPEAT:
            return mageMouseButtonRepeatCategories;
            break;
        case MAGE_EVENT_MOUSE_BUTTON_PRESSED:
            return mageMouseButtonPressedCategories;
            break;
        case MAGE_EVENT_MOUSE_BUTTON_RELEASED:
            return mageMouseButtonReleasedCategories;
            break;
        case MAGE_EVENT_MOUSE_MOVED:
            return mageMouseMovedCategories;
            break;
        case MAGE_EVENT_MOUSE_SCROLLED:
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
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_WINDOW_CLOSE);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowFocus(void *buffer) 
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_WINDOW_FOCUS);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowLostFocus(void *buffer)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_WINDOW_LOST_FOCUS);
    memmove(buffer, &temp, sizeof(uint16_t));
}
void mageEventFormatWindowMoved(void *buffer, const int32_t x, const int32_t y)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_WINDOW_MOVED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &x, sizeof(int32_t));
    memmove(buffer + 6, &y, sizeof(int32_t));
}
void mageEventFormatKeyPressed(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_KEY_PRESSED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatKeyReleased(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_KEY_RELEASED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatKeyRepeat(void *buffer, const uint8_t keycode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_KEY_REPEAT);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &keycode, sizeof(uint8_t));
}
void mageEventFormatMouseButtonPressed(void *buffer, const uint8_t mousecode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_MOUSE_BUTTON_PRESSED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &mousecode, sizeof(uint8_t));
}
void mageEventFormatMouseButtonRelease(void *buffer, const uint8_t mousecode)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_MOUSE_BUTTON_RELEASED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &mousecode, sizeof(uint8_t));
}
void mageEventFormatMouseMoved(void *buffer, const double x, const double y)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_MOUSE_MOVED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &x, sizeof(double));
    memmove(buffer + 10, &y, sizeof(double));    
}
void mageEventFormatMouseWheelMoved(void *buffer, const double x, const double y)
{
    uint16_t temp = mageEventHandleCreate(MAGE_EVENT_MOUSE_SCROLLED);
    memmove(buffer, &temp, sizeof(uint16_t));
    memmove(buffer + 2, &x, sizeof(double));
    memmove(buffer + 10, &y, sizeof(double));
}
void mageEventDispatch(void *event)
{
    uint64_t i;
    uint16_t foo = 0;
    switch (EventMaster.ListenerCount)
    {
        case 0:
            return;
        default:
            memcpy(&foo, event, 2);
            for (i = 0; i < EventMaster.ListenerCount; i++) 
            {
                EventMaster.Listeners[i](event, mageEventExtractEventType(foo));
            }
    }
}

