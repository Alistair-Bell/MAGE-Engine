#include "../Includes.h"
#include "../InputHandler.h"

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler) { return MageTrue; }
U8 MageInputHandlerPollEvents(MageInputHandler *handler, MageApplicationWindow *window) { return MageTrue; }
U8 MageInputHandlerUpdateKeyStatus(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state) { return MageTrue; }
U8 MageInputHandlerUpdateMouseButtonStatus(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state) { return MageTrue; }
U8 MageInputHandlerDestroy(MageInputHandler* handler) { return MageTrue; }
