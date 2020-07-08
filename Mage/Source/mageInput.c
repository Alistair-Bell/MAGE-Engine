#include "mageAPI.h"

static struct mageApplication *App;

static void mageGLFWKeyCallback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t modifiers)
{
    void *buffer = malloc(MAGE_KEY_PRESSED_EVENT);
    memset(buffer, 0, MAGE_WINDOW_CLOSE_EVENT_BYTE_SIZE);
        
    switch (action)
    {
        case GLFW_PRESS:
            break;

        case GLFW_RELEASE:
            mageEventFormatKeyReleased(buffer, key);
            break;

        case GLFW_REPEAT:
            mageEventFormatKeyRepeat(buffer, key);
            break;
    }
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWMouseMoveCallback(GLFWwindow *window, double x, double y)
{
    void *buffer = malloc(MAGE_MOUSE_MOVED_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_MOUSE_MOVED_EVENT_BYTE_SIZE);   
    mageEventFormatMouseMoved(buffer, x, y);
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWMouseButtonCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t modifiers)
{
    void *buffer = malloc(MAGE_MOUSE_BUTTON_PRESSED_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_MOUSE_BUTTON_PRESSED_EVENT_BYTE_SIZE);
    switch (action)
    {
        case GLFW_PRESS:
            mageEventFormatMouseButtonPressed(buffer, button);
            break;

        case GLFW_RELEASE:
            mageEventFormatMouseButtonRelease(buffer, button);
            break;
    }
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWWindowCloseCallback(GLFWwindow *window)
{
    void *buffer = malloc(MAGE_WINDOW_CLOSE_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_WINDOW_CLOSE_EVENT_BYTE_SIZE);
    mageEventFormatWindowClose(buffer);
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWWindowFocusCallback(GLFWwindow *window, int32_t focused)
{
    void *buffer = malloc(MAGE_WINDOW_FOCUS_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_WINDOW_MOVED_EVENT_BYTE_SIZE);
    
    switch (focused)
    {
        case 1:
            mageEventFormatWindowFocus(buffer);
            break;

        default:
            mageEventFormatWindowLostFocus(buffer);
            break;
    }   
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWCursorEnterCallback(GLFWwindow *window, int32_t entered)
{
    
}   
static void mageGLFWWindowMovesCallback(GLFWwindow *window, int32_t x, int32_t y)
{
    void *buffer = malloc(MAGE_WINDOW_MOVED_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_WINDOW_MOVED_EVENT_BYTE_SIZE);
    mageEventFormatWindowMoved(buffer, x, y);
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWScrollWheelCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    void *buffer = malloc(MAGE_MOUSE_SCROLLED_EVENT_BYTE_SIZE);
    memset(buffer, 0, MAGE_MOUSE_SCROLLED_EVENT_BYTE_SIZE);
    mageEventFormatMouseWheelMoved(buffer, xOffset, yOffset);
    mageEventDispatch(buffer);
    free(buffer);
}
static void mageGLFWWindowResizeCallback(GLFWwindow *window, int32_t width, int32_t height)
{
    App->Window->Width = width;
    App->Window->Height = height;
    mageRendererResize(App->Renderer, App->Window, &App->RendererProps);
}
void mageInputSetup(struct mageApplication *application)
{
    App = application;
    glfwSetKeyCallback          (application->Window->Context, mageGLFWKeyCallback);
    glfwSetCursorPosCallback    (application->Window->Context, mageGLFWMouseMoveCallback);
    glfwSetMouseButtonCallback  (application->Window->Context, mageGLFWMouseButtonCallback);
    glfwSetWindowCloseCallback  (application->Window->Context, mageGLFWWindowCloseCallback);
    glfwSetWindowFocusCallback  (application->Window->Context, mageGLFWWindowFocusCallback);
    glfwSetCursorEnterCallback  (application->Window->Context, mageGLFWCursorEnterCallback);
    glfwSetWindowPosCallback    (application->Window->Context, mageGLFWWindowMovesCallback);
    glfwSetScrollCallback       (application->Window->Context, mageGLFWScrollWheelCallback);
    glfwSetWindowSizeCallback   (application->Window->Context, mageGLFWWindowResizeCallback);
}
