#include "window.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

struct Window {
	SDL_Window* window;
	SDL_GLContext gl_context;
};

static int resizeEventWatch(void* data, SDL_Event* event);

Window* createWindow(const char* window_title, int window_width, int window_height)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	   SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "%s : SDL could not be initialized\n", __func__);
	   return NULL;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

	Window* window_handle = (Window*)malloc(sizeof(Window));
	window_handle->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if(window_handle->window == NULL)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "%s : Window could not be created\n", __func__);
		free(window_handle);
	    return NULL;
	}

	window_handle->gl_context = SDL_GL_CreateContext(window_handle->window);
	if(window_handle->gl_context == NULL)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "%s : OpenGL context could not be created\n", __func__);
		free(window_handle);
		return NULL;
	}

    gladSetGLOnDemandLoader((GLADloadfunc)SDL_GL_GetProcAddress);

	SDL_GL_MakeCurrent(window_handle->window, window_handle->gl_context);
	SDL_AddEventWatch(resizeEventWatch, window_handle->window);
	SDL_SetRelativeMouseMode(SDL_ENABLE);

	return window_handle;
}

void swapWindow(Window* window_handle)
{
	SDL_GL_SwapWindow(window_handle->window);
}

void destroyWindow(Window* window_handle)
{
	if(window_handle->window == NULL)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "%s : Window is not initialized, cannot destroy!", __func__);
		return;
	}
	SDL_GL_DeleteContext(window_handle->gl_context);
	SDL_DestroyWindow(window_handle->window);
	SDL_Quit();

	free(window_handle);
}

static int resizeEventWatch(void* data, SDL_Event* event)
{
    if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
		int width, height;
        SDL_GetWindowSize((SDL_Window*)data, &width, &height);

        glViewport(0, 0, width, height);
        return 1;
    } else {
        return 0;
    }
}
