#include "mageAPI.h"

int main(void)
{
	
	mageWindow *window = mageWindowAllocate();

	mageWindowInitialise(window, 1080, 720, "Hello World", NULL);
	uint8 flag;	

	while(window->Running)
	{
		while (SDL_PollEvent(&window->Events))
		{
			switch (window->Events.type)
			{
			case SDL_QUIT:  
				window->Running = 0;
				break;
			}

		}		
	}
	mageWindowDestroy(window);
	mageLogReset();
	return 0;
}

