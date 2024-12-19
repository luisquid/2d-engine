#include "Game.h"


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
        SDL_WINDOW_BORDERLESS
    );

    if(!window){
        std::cerr << "Error creating SDL2 window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        std::cerr << "Error creating SDL2 renderer" << std::endl;
        return;
    }

    isRunning = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup(){
    playerPosition = glm::vec2(10.0, 20.0);
    playerVelocity = glm::vec2(10.0, 0.0);
}

void Game::Run(){
    Setup();
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
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), millisecondsPreviousFrame + MILLISECS_PER_FRAME));
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if(timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME){
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0f;

    millisecondsPreviousFrame = SDL_GetTicks();

    playerPosition.x += playerVelocity.x * deltaTime;
    playerPosition.y += playerVelocity.y * deltaTime;
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect destRect = {
                         static_cast<int>(playerPosition.x), 
                         static_cast<int>(playerPosition.y), 
                         32, 
                         32};

    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player = {10, 10, 20, 20};
    SDL_RenderFillRect(renderer, &player);

    SDL_RenderPresent(renderer);
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}