#include <mageAPI.h>

#if defined (MAGE_OPENAL_BACKEND)

inline MAGE_THREAD_RETURN_TYPE mageAudioDriverThreadedPlay(void *data);

struct mageAudioDriverThreadedData
{
    struct mageAudioDriver          *Driver;
    struct mageAudioDriverPlayInfo  *PlayInfo;
};

mageResult mageAudioDriverCreate(struct mageAudioDriver *driver, struct mageAudioDriverCreateInfo *info)
{
    MAGE_ASSERT(driver != NULL);
    MAGE_ASSERT(info != NULL);
    
    driver->OuputDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    MAGE_ASSERT_MESSAGE(driver->OuputDevice != NULL, "Could not open audio device\n", NULL);
    MAGE_LOG_CORE_INFORM("Using audio device %s\n", driver->OuputDevice);

    driver->Device  = alcOpenDevice(driver->OuputDevice);
    driver->Context = alcCreateContext(driver->Device, NULL);
    MAGE_ASSERT(driver->Device != NULL);
    MAGE_ASSERT(driver->Context != NULL);

    return MAGE_RESULT_SUCCESS;
}
void mageAudioDriverPlay(struct mageAudioDriver *driver, struct mageAudioDriverPlayInfo *info)
{
    MAGE_ASSERT(driver != NULL);
    MAGE_ASSERT(info != NULL);

    struct mageAudioDriverThreadedData submitData;
    memset(&submitData, 0, sizeof(struct mageAudioDriverThreadedData));
    submitData.Driver     = driver;
    submitData.PlayInfo   = info;


    if (info->Threaded == MAGE_TRUE)
    {
        mageThread t = mageThreadCreate();
        struct mageThreadBeginInfo beginInfo;
        memset(&beginInfo, 0, sizeof(struct mageThreadBeginInfo));
        beginInfo.Job           = mageAudioDriverThreadedPlay;
        beginInfo.SubmitData    = &submitData;
        beginInfo.ThreadFlags   = MAGE_THREAD_BEGIN_INFO_FLAGS_IMMEDIATE;
        mageThreadBegin(t, &beginInfo);
        MAGE_MEMORY_FREE(t);
    }
    else
    {
        mageAudioDriverThreadedPlay(&submitData);
    }
}
void *mageAudioDriverThreadedPlay(void *data)
{
    struct mageAudioDriverThreadedData *raw = (struct mageAudioDriverThreadedData *)data;
    uint32_t frequency = raw->PlayInfo->Frequency;
    double seconds = raw->PlayInfo->Seconds;

    uint32_t buffer, i;
    alGenBuffers(1, &buffer);

    uint32_t sampleRate = 22050;
    size_t bufferSize = seconds * sampleRate;
    int16_t *samples = MAGE_MEMORY_ARRAY_ALLOCATE(bufferSize, sizeof(int16_t));

    for (i = 0; i < bufferSize; i++)
        samples[i] = 32760 * sin((2.f * MAGE_PI * frequency) / sampleRate * i);

    alBufferData(buffer, AL_FORMAT_MONO16, samples, bufferSize, sampleRate);


    uint32_t source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);

    /* While sound is playing, sleep */
    sleep(seconds);

    free(samples);
    alDeleteBuffers(1, &buffer);
    return (MAGE_THREAD_RETURN_TYPE) 0;
}
void mageAudioDriverDestroy(struct mageAudioDriver *driver)
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(driver->Context);
    alcCloseDevice(driver->Device);
}

#endif