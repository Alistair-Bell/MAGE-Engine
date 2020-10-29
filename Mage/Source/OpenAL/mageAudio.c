#include <mageAPI.h>

#if defined (MAGE_OPENAL_BACKEND)

inline MAGE_THREAD_RETURN_TYPE mageAudioDriverThreadedPlay(void *data);

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
void mageAudioDriverPlayLoadSource(struct mageAudioDriver *driver, struct mageAudioDriverSoundSourceLoadInfo *info, struct mageAudioDriverPlayInfo *playInfo)
{
    MAGE_ASSERT(driver != NULL);
    MAGE_ASSERT(info != NULL);
    MAGE_ASSERT(playInfo != NULL);

    int32_t *data;
    uint32_t dataSize;
    uint32_t sampleRate;
    uint32_t format;
    uint64_t samplesDecoded;

    switch (info->Flags)
    {
        case MAGE_AUDIO_DRIVER_SOUND_SOURCE_FLAGS_FORMAT_WAV:
        {
            drwav wav;
            MAGE_ASSERT(drwav_init_file(&wav, info->Source, NULL) != MAGE_FALSE);
            
            dataSize = wav.totalPCMFrameCount * wav.channels * sizeof(int32_t);
            data = MAGE_MEMORY_ALLOCATE(dataSize);
            samplesDecoded = drwav_read_pcm_frames_s32(&wav, wav.totalPCMFrameCount, data);
            
            sampleRate = wav.sampleRate;
            
            playInfo->Length = (float)samplesDecoded / wav.sampleRate;
            drwav_uninit(&wav);
            break;
        }
        default:
            MAGE_ASSERT_MESSAGE(MAGE_TRUE == MAGE_TRUE, "Only wav files currently supported, watch out for support in later commits\n", NULL);
    }
    
    alGenBuffers(1, &playInfo->BufferHandle);
    alBufferData(playInfo->BufferHandle, AL_FORMAT_STEREO16, data, dataSize, sampleRate);

    alGenSources(1, &playInfo->SourceHandle);
    alSourcei(playInfo->SourceHandle, AL_BUFFER, playInfo->BufferHandle);

    MAGE_LOG_CORE_INFORM("Creating audio clip from file %s, duration %f seconds\n", info->Source, playInfo->Length);
    MAGE_MEMORY_FREE(data);
}
void mageAudioDriverPlay(struct mageAudioDriver *driver, struct mageAudioDriverPlayInfo *info)
{
    MAGE_ASSERT(driver != NULL);
    MAGE_ASSERT(info != NULL);


    if (info->Flags & MAGE_AUDIO_DRIVER_PLAY_FLAGS_NEW_THREAD)
    {
        struct mageThread t;
        struct mageThreadCreateInfo i;
        memset(&i, 0, sizeof(struct mageThreadCreateInfo));
        mageThreadCreate(&t, &i);
        
        struct mageThreadBeginInfo begin;
        memset(&begin, 0, sizeof(struct mageThreadBeginInfo));
        begin.Job = mageAudioDriverThreadedPlay;
        begin.SubmitData    = info;
        begin.ThreadFlags   = MAGE_THREAD_BEGIN_INFO_FLAGS_IMMEDIATE;

        MAGE_LOG_CORE_INFORM("Playing audio clip on new thread\n", NULL);
        mageThreadBegin(&t, &begin);
    }
    else
    {
        mageAudioDriverThreadedPlay(info);
    }    
}
void *mageAudioDriverThreadedPlay(void *data)
{
    struct mageAudioDriverPlayInfo raw = MAGE_VOID_POINTER_CAST(data, struct mageAudioDriverPlayInfo);
    
    alSourcePlay(raw.SourceHandle);
    return (MAGE_THREAD_RETURN_TYPE) 0;
}
void mageAudioDriverPlayDestroy(struct mageAudioDriver *driver, struct mageAudioDriverPlayInfo *info)
{
    MAGE_LOG_CORE_INFORM("Destroying audio clip with duration of %f seconds\n", info->Length);
    alDeleteSources(1, &info->SourceHandle);
    alDeleteBuffers(1, &info->BufferHandle);
}
void mageAudioDriverDestroy(struct mageAudioDriver *driver)
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(driver->Context);
    alcCloseDevice(driver->Device);
}

#endif