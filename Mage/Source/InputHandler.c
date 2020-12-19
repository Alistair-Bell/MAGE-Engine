#include "InputHandler.h"

U8 MageInputHandlerInterrogateKeyState(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state)
{
    return (handler->KeyboardStates[(U8)key] &= state);
}
U8 MageInputHandlerUpdateKeyStatus(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state)
{
	return MageTrue;
}
U8 MageInputHandlerUpdateMouseButtonStatus(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state)
{ 
	return MageTrue; 
}
U8 MageInputHandlerInterrogateMouseButtonState(MageInputHandler *handler, const MageMouseButton button, const MageButtonState state)
{
    return (handler->MouseStates[(U8)button] &= state);
}

F32 MageInputHandlerInterrogateMousePositionX(MageInputHandler* handler)
{
	return 1.0f;
}

F32 MageInputHandlerInterrogateMousePositionY(MageInputHandler* handler)
{
	return 1.0f;
}
