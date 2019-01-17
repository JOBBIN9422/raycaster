#include "Raycaster.h"

using namespace std;

Raycaster::Raycaster(int windowWidth, int windowHeight) 
: windowWidth(windowWidth), windowHeight(windowHeight), focalLength(0.5)
{
    this->player = new Player(10, 1);
    this->map = new Map(20, 4);
    this->InitVideo(windowWidth, windowHeight);
}

void Raycaster::InitVideo(int windowWidth, int windowHeight)
{
    this->mainWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, 
                                        SDL_WINDOW_SHOWN);
    this->renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    this->windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STATIC, windowWidth, windowHeight);
    this->frameBuffer = new unsigned int[windowWidth * windowHeight];
}

void Raycaster::RunFrame()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    //loop over each column of the display
    for (int x = 0; x < this->windowWidth; x++)
    {
        //calculate the offset (applied to player's current heading)
        double rayAngleOffset = atan2((double)x / (double)this->windowWidth - 0.5, this->focalLength);
        double rayHeading = this->player->GetHeading() + rayAngleOffset;
        double rayHeadingX = cos(rayHeading);
        double rayHeadingY = sin(rayHeading);

        //cast the ray from player position 
        arma::ivec gridPos = {(int)this->player->GetXYPosition()(0), (int)this->player->GetXYPosition()(1)};
        arma::vec rayOrigin = this->player->GetXYPosition();
        int stepX = 0;
        int stepY = 0;

        //length of ray from current point to next x/y grid line
        double gridDistX;
        double gridDistY;

        //length of ray from one x/y grid line to next x/y grid line
        double deltaDistX = abs(1.0 / rayHeadingX);
        double deltaDistY = abs(1.0 / rayHeadingY);

        //determine next grid to step to by determining ray direction
        if (rayHeadingX > 0)
        {
            stepX = 1;
            gridDistX = (gridPos(0) + 1.0 - rayOrigin(0)) * deltaDistX;
        }
        else
        {
            stepX = -1;
            gridDistX = (rayOrigin(0) - gridPos(0)) * deltaDistX;
        }

        if (rayHeadingY > 0)
        {
            stepY = 1;
            gridDistY = (gridPos(1) + 1.0 - rayOrigin(1)) * deltaDistY;
        }
        else
        {
            stepY = -1;
            gridDistY = (rayOrigin(1) - gridPos(1)) * deltaDistY;
        }

        bool hitWall = false;
        bool hitYSide;
        Tile currTile = this->map->GetMap().at(gridPos(1)).at(gridPos(0));
        while (!hitWall)
        {
            //check which grid line (x or y) is closer
            if (gridDistX < gridDistY)
            {
                gridDistX += deltaDistX;
                gridPos(0) += stepX;
                hitYSide = false;
            }
            else
            {
                gridDistY += deltaDistY;
                gridPos(1) += stepY;
                hitYSide = true;
            }

            //check for wall hits
            currTile = this->map->GetMap().at(gridPos(1)).at(gridPos(0));
            if (currTile.GetType() == TileType::WALL)
            {
                hitWall = true;
            }
        }

        //calculate euclidean and perpendicular distance of ray 
        double perpendicularDist;

        if (!hitYSide)
        {
            perpendicularDist = (gridPos(0) - rayOrigin(0) + (1 - stepX) / 2) / rayHeadingX;
        }
        else
        {
            perpendicularDist = (gridPos(1) - rayOrigin(1) + (1 - stepY) / 2) / rayHeadingY;
        }
        perpendicularDist *= cos(rayAngleOffset);

        //calculate vertical line size and start/end positions
        int lineSize  = round(this->windowHeight / perpendicularDist);
        int lineStart = -lineSize / 2 + this->windowHeight / 2;
        int lineEnd   = lineSize / 2 + this->windowHeight / 2;

        if (lineStart < 0)
        {
            lineStart = 0;
        }

        if (lineEnd >= this->windowHeight)
        {
            lineEnd = this->windowHeight - 1;
        }

        //draw line with wall color
        unsigned int color = currTile.GetColor();
        if (hitYSide)
        {
            unsigned char newR = ((color >> 16) & 0xFF) * 3 / 4;
            unsigned char newG = ((color >> 8) & 0xFF) * 3 / 4;
            unsigned char newB = (color & 0xFF) * 3 / 4;
            color = 0xFF000000 | (newR << 16) | (newG << 8) | newB;
        }

        this->DrawVertLine(x, lineStart, lineEnd, color);
    }

    this->prevFrameTime = this->currFrameTime;
    this->currFrameTime = SDL_GetTicks();
    this->elapsedFrameTime = (this->currFrameTime - this->prevFrameTime) / 1000;
    this->RenderFrame();

    //set speed modifiers
    this->player->SetMoveSpeed(this->elapsedFrameTime * 5.0);
    this->player->SetTurnSpeed(this->elapsedFrameTime * 3.0);

    this->PollKeyboard();
}

void Raycaster::RenderFrame()
{
    //SDL_UpdateTexture(windowTexture, NULL, frameBuffer, this->windowWidth * sizeof(uint));
    //SDL_RenderCopy(renderer, windowTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void Raycaster::DrawVertLine(int x, int yStart, int yEnd, unsigned int color)
{
    int R = (color >> 16) & 0xFF;
    int G = (color >> 8) & 0xFF;
    int B = color & 0xFF;

    SDL_SetRenderDrawColor(this->renderer, R, G, B, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(this->renderer, x, yStart, x, yEnd);
}

void Raycaster::PollKeyboard()
{
    arma::vec moveCoords = this->player->GetFrontXYPos();

    SDL_PollEvent(&keyEvent);
    switch (keyEvent.type)
    {
        case SDL_KEYDOWN:
            switch (keyEvent.key.keysym.sym)
            {
                case SDLK_RIGHT: 
                    this->player->TurnRight();
                    break;

                case SDLK_LEFT: 
                    this->player->TurnLeft();
                    break;

                case SDLK_UP: 
                    if (this->map->GetMap().at((int)moveCoords(1)).at((int)moveCoords(0)).GetType()
                        != TileType::WALL)
                    {
                        this->player->MoveForward();
                    }
                    break;

                case SDLK_DOWN: 
                    this->player->MoveBackward();
                    break;

                case SDLK_SPACE: 
                    break;

                case SDLK_RETURN: 
                    break;

                case SDLK_RSHIFT: 
                    break;

                case SDLK_TAB: 
                    break;

                case SDLK_ESCAPE: 
                    SDL_Quit();
                    break;
            }
            break;
    }
}

void Raycaster::DebugPrint()
{
    cout << "player pos:\n " << this->player->GetXYPosition() << "\n";
    cout << "player heading: " << this->player->GetHeading() * 180 / 3.1415 << "\n" << "\n";
}
