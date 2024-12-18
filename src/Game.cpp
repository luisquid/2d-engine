#include "Game.h"
#include <iostream>

Game::Game(){
    std::cout << "Game constructor called!"<< std::endl;
    isRunning = false;
}

Game::~Game(){
    std::cout << "Game destructor called!"<< std::endl;
}

void Game::Initialize(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL!" << std::endl;
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = 800;
    windowHeight = 600;

    window = SDL_CreateWindow(NULL, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        windowWidth,
        windowHeight,
        SDL_WINDOW_FULLSCREEN
    );

    if(!window){
        std::cerr << "Error creating SDL2 window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        std::cerr << "Error creating SDL2 renderer" << std::endl;
        return;
    }

    isRunning = true;
}
void Game::Run(){
    while(isRunning){
        ProcessInput();
        Update();
        Render();
    }
}
void Game::ProcessInput(){
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent)){
        switch(sdlEvent.type){
            case SDL_QUIT:
                isRunning = false;
            break;
            case SDL_KEYDOWN:
                if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                    isRunning = false;
                }
            break;
        }
    }
}

void Game::Update(){

}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}