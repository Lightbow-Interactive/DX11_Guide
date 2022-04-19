#include <SDL.h>
#include <Windows.h>
#include <SDL_syswm.h>

#include "Graphics.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"

SDL_Window* window;
char windowTitle[] = "SDL Window";
int width = 960;
int height = 540;
Graphics gfx;

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_SetWindowResizable(window, SDL_FALSE);

    int realWidth, realHeight;
    SDL_GetWindowSize(window, &realWidth, &realHeight);

    SDL_SysWMinfo sysWMInfo;
    SDL_VERSION(&sysWMInfo.version)
	SDL_GetWindowWMInfo(window, &sysWMInfo);

    gfx.Init(sysWMInfo.info.win.window, window, realWidth, realHeight);

    bool run = true;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                run = false;
        }

        gfx.Render();

    }

    gfx.Shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}