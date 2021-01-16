#include "../Threading.h"

U8 MageThreadCreate(MageThreadCreateInfo *info, MageThread *thread)
{
	thread->Handle = CreateThread(
		NULL,
		0,
		info->Callback,
		info->CallbackData,
		0,
		&thread->Identifier
	);
	MAGE_HANDLE_ERROR_MESSAGE(thread->Handle == NULL, printf("Error: Failed to create Win32 thread\n"));
	return MageTrue;
}
U8 MageThreadEnd(MageThread *thread)
{
	TerminateThread(thread->Handle, MAGE_THREAD_RETURN_FAILURE);
	return MageTrue;
}
U8 MageThreadDestroy(MageThread *thread)
{
	WaitForMultipleObjects(1, &thread->Handle, TRUE, UINT32_MAX); /* Wait for the thread to end */
	DWORD r;
	GetExitCodeThread(thread->Handle, &r);
	CloseHandle(thread->Handle);
	return r == MAGE_THREAD_RETURN_SUCCESS;
}