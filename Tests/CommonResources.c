#include "CommonResources.h"

U0 CommonPopulateWindow(MageApplicationWindowCreateInfo *info)
{
    memset(info, 0, sizeof(MageApplicationWindowCreateInfo));
    info->Title        = COMMON_WINDOW_TITLE; 
    info->Flags        = MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE | MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING;
    info->Width        = COMMON_WINDOW_WIDTH;
    info->Height       = COMMON_WINDOW_HEIGHT;
}
U0 CommonPopulateRenderer(MageApplicationWindowCreateInfo *info)
{
    memset(info, 0, sizeof(MageApplicationWindowCreateInfo));

}


