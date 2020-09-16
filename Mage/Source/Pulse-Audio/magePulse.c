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
    driver->Loop = pa_mainloop_new();
    MAGE_ASSERT(driver->Loop != NULL);
    driver->Context = pa_context_new(pa_mainloop_get_api(driver->Loop), MAGE_PULSE_AUDIO_CONTEXT_NAME);
    MAGE_ASSERT(driver->Context != NULL);

    pa_context_set_state_callback(driver->Context, magePulseAudioStateCallback, driver);
    
    return MAGE_RESULT_SUCCESS;
}
void mageAudioDriverDestroy(struct mageAudioDriver *driver)
{
    pa_mainloop_free(driver->Loop);
    pa_context_disconnect(driver->Context);
    pa_context_unref(driver->Context);
    
    MAGE_LOG_CORE_INFORM("Destroying pulse audio backend\n", NULL);
}

#endif