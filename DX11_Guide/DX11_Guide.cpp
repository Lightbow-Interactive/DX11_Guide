#include <SDL.h>

SDL_Window* window;
char windowTitle[] = "SDL Window";
int width = 960;
int height = 540;

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_SetWindowResizable(window, SDL_FALSE);

    bool run = true;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}