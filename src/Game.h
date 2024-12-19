#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <glm/glm.hpp>

const int FPS = 120;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game{
    private:
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        int millisecondsPreviousFrame = 0;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        int windowWidth;
        int windowHeight;
};

#endif