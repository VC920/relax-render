#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "game.h"
#include "renderer.h"
#include "input.h"

// Settings
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int main()
{
    // Initialize window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow("",
					  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
					  WINDOW_WIDTH, WINDOW_HEIGHT,
					  SDL_WINDOW_OPENGL |
					  SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    // Setting
    SDL_GL_SetSwapInterval(1);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 last_frame_time = 0;
    Uint64 current_frame_time = 0;
    float delta_time = 0;
    
    bool running = true;

    // Initialize game
    game_init();

    // Main loop
    while (running)
    {
        // Process Input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: 
                running = false;
                break;
            case SDL_KEYDOWN: 
		input_set_key(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP: 
		input_set_key(event.key.keysym.sym, false);
                break;
            case SDL_MOUSEMOTION:
                input_set_mouse_rel_pos(event.motion.xrel, event.motion.yrel);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;
                    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
                }
                break;
            default:
                break;
            }
	}

    	// Update
        current_frame_time = SDL_GetPerformanceCounter();
        delta_time = (float)(current_frame_time - last_frame_time) / frequency;
        last_frame_time = current_frame_time;
	game_update(delta_time);

	// Renbder
	game_render();
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}
