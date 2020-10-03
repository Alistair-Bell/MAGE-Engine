#include <mageAPI.h>

/* 
    Wraps the native pulse audio library
    pulse audio is included in Core.h and should be installed on the system
*/

#if defined (MAGE_PULSE_AUDIO_BACKEND)

#define MAGE_PULSE_AUDIO_CONTEXT_NAME "Mage-Engine"

static void magePulseAudioStateCallback(pa_context *context, void *userData)
{

}
void mageAudioDriverDetectChannels(struct mageAudioDriver *driver)
{
    

}
mageResult mageAudioDriverCreate(struct mageAudioDriver *driver, void *userData)
{
    MAGE_LOG_CORE_INFORM("Creating pulse audio backend\n", NULL);
    return MAGE_RESULT_SUCCESS;
}
void mageAudioDriverDestroy(struct mageAudioDriver *driver)
{
    MAGE_LOG_CORE_INFORM("Destroying pulse audio backend\n", NULL);
}

#endif