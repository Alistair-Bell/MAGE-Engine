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
			if (window->Events.type == SDL_QUIT) window->Running = 0;
			else if (window->Events.type == SDL_KEYDOWN)
			{
				switch (window->Events.key.keysym.sym)
				{
				
				}
			}
		}		
		
	}
	mageWindowDestroy(window);
	mageLogReset();
	return 0;
}

