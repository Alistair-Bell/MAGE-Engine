#include <mageAPI.h>

#if defined(MAGE_MONO_EXTERNALS)

    void *mageMonoHandlerAllocate()
    {
        return malloc(sizeof(struct MAGE_MONO_HANDLER_STRUCT));
    }
    void mageMonoInterpretLogInform(MonoString *message)
    {
        MAGE_LOG_CLIENT_INFORM("%s", mono_string_to_utf8(message));
    }
    void mageMonoInterpretLogWarning(MonoString *message)
    {
        MAGE_LOG_CLIENT_WARNING("%s", mono_string_to_utf8(message));
    }
    void mageMonoInterpretLogError(MonoString *message)
    {
        MAGE_LOG_CLIENT_ERROR("%s", mono_string_to_utf8(message));
    }
    void mageMonoInterpretLogFatalError(MonoString *message)
    {
        MAGE_LOG_CLIENT_FATAL_ERROR("%s", mono_string_to_utf8(message));
    }
    MonoMethod *mageMonoHandlerFindMethod(MonoClass *monoClass, const char *name)
    {
        MonoMethod* method = NULL;
        MonoMethod* m = NULL;
        void* iterator = NULL;

        while ((m = mono_class_get_methods(monoClass, &iterator))) {
            if (strcmp(mono_method_get_name(m), name) == 0) {
                method = m;
            }
        }

        return method;
    }
    mageResult mageMonoHandlerInitialise(mageMonoHandler *handler, const char *builtLibrary)
    {
        handler->Domain = mono_jit_init("MAGE-Engine");
        handler->Assembler = mono_domain_assembly_open(handler->Domain, builtLibrary);

        if (handler->Assembler == NULL)
        {
            MAGE_LOG_CORE_ERROR("Invalid mono library\n", NULL);
            return MAGE_INVALID_INPUT;
        }

        handler->Image = mono_assembly_get_image(handler->Assembler);

        mono_add_internal_call("Mage.Log::Inform", mageMonoInterpretLogInform);
        mono_add_internal_call("Mage.Log::Warning", mageMonoInterpretLogWarning);
        mono_add_internal_call("Mage.Log::Error", mageMonoInterpretLogError);
        mono_add_internal_call("Mage.Log::FatalError", mageMonoInterpretLogFatalError);

        return MAGE_SUCCESS;
    }
    void mageMonoCleanup(mageMonoHandler *handler)
    {
        mono_jit_cleanup(handler->Domain);
    }

#endif