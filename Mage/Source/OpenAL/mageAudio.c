#include <mageAPI.h>

#if defined (MAGE_OPENAL_BACKEND)

inline MAGE_THREAD_RETURN_TYPE mageAudioDriverThreadedPlay(void *data);

struct mageAudioDriverThreadedData
{
    struct mageAudioDriver          Driver;
    struct mageAudioDriverPlayInfo  PlayInfo;
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
    MAGE_ASSERT(alcMakeContextCurrent(driver->Context) != MAGE_FALSE);

    alGetError();

    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, (float[]) { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f });

    return MAGE_RESULT_SUCCESS;
}
void mageAudioDriverPlay(struct mageAudioDriver *driver, struct mageAudioDriverPlayInfo *info)
{
    MAGE_ASSERT(driver != NULL);
    MAGE_ASSERT(info != NULL);

    struct mageAudioDriverThreadedData submitData;
    memset(&submitData, 0, sizeof(struct mageAudioDriverThreadedData));
    submitData.Driver     = *driver;
    submitData.PlayInfo   = *info;
    

    if (info->Threaded == MAGE_TRUE)
    {
        struct mageThread t;
        struct mageThreadCreateInfo i;
        memset(&i, 0, sizeof(struct mageThreadCreateInfo));
        mageThreadCreate(&t, &i);
        
        struct mageThreadBeginInfo begin;
        memset(&begin, 0, sizeof(struct mageThreadBeginInfo));
        begin.Job = mageAudioDriverThreadedPlay;
        begin.SubmitData    = &submitData;
        begin.ThreadFlags   = MAGE_THREAD_BEGIN_INFO_FLAGS_IMMEDIATE;
        mageThreadBegin(&t, &begin);
    }
    else
    {
        mageAudioDriverThreadedPlay(&submitData);
    }    
}
void *mageAudioDriverThreadedPlay(void *data)
{
    struct mageAudioDriverThreadedData raw = MAGE_VOID_POINTER_CAST(data, struct mageAudioDriverThreadedData);
    uint32_t frequency = raw.PlayInfo.Frequency;
    double seconds = raw.PlayInfo.Seconds;
    
    uint32_t sampleRate = 22050;
    size_t bufferSize = seconds * sampleRate;
    int16_t *samples = MAGE_MEMORY_ARRAY_ALLOCATE(bufferSize, sizeof(int16_t));

    uint32_t i;
    for (i = 0; i < bufferSize; i++)
    {
        samples[i] = 32760 * sin((2.f * MAGE_PI * frequency) / sampleRate * i);
    }

    uint32_t buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, samples, bufferSize, sampleRate);

    uint32_t source;
    alGenSources(1, &source);
    
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
    
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);

    while (state == AL_PLAYING)
        alGetSourcei(source, AL_SOURCE_STATE, &state);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    free(samples);
    return (MAGE_THREAD_RETURN_TYPE) 0;
}
void mageAudioDriverDestroy(struct mageAudioDriver *driver)
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(driver->Context);
    alcCloseDevice(driver->Device);
}

#endif