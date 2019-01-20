#include "Raycaster.h"

using namespace std;

Raycaster::Raycaster(int windowWidth, int windowHeight) 
: windowWidth(windowWidth), windowHeight(windowHeight), focalLength(0.5), targetFPS(60.0), done(false)
{
    this->player = new Player(30, 20);
    this->map = new Map(40, 40);
    this->InitVideo(windowWidth, windowHeight);
    this->LoadTextures();
}

void Raycaster::InitVideo(int windowWidth, int windowHeight)
{
    this->mainWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, 
                                        SDL_WINDOW_SHOWN);
    this->renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED |
                                                        SDL_RENDERER_PRESENTVSYNC);
    //this->windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                            //SDL_TEXTUREACCESS_STATIC, windowWidth, windowHeight);
}

void Raycaster::LoadTextures()
{
    vector<vector<Tile>> worldMap = this->map->GetMap();
    for (int y = 0; y < worldMap.size(); y++)
    {
        for (int x = 0; x < worldMap.at(y).size(); x++)
        {
            //get filename and search the texture map for it
            string filename = worldMap.at(y).at(x).GetWallTextureFilename();
            unordered_map<string, SDL_Texture*>::const_iterator searchIter = 
            this->textureMap.find(filename);

            //if current tile has associated filename AND the same texture has not already been loaded
            if (filename != "" && searchIter == this->textureMap.end())
            {
                SDL_Texture* currTexture = this->LoadTexture(filename);
                //trim off path from filename (probably shouldn't hard-code this)
                this->textureMap.insert(make_pair(filename, currTexture));
            }
        }
    }
}

SDL_Texture* Raycaster::LoadTexture(std::string filename)
{
    SDL_Surface* tempSurface = IMG_Load(filename.c_str());
    if (!tempSurface)
    {
        cout << "Error loading " << filename << ": " << IMG_GetError() << "\n";
        return nullptr;
    }

    //if image loaded successfully
    SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(this->renderer, tempSurface);
    if (!loadedTexture)
    {
        cout << "Error creating texture from " << filename << ": " << SDL_GetError() << "\n";
        return nullptr;
    }

    SDL_FreeSurface(tempSurface);
    return loadedTexture;
}

bool Raycaster::IsDoneRendering()
{
    return this->done;
}

void Raycaster::RunGameLoop()
{
    while (!this->done)
    {
        this->CastRays();
        this->DoUpdates();
    }
}

//perform raycasting for each vertical column of the screen
//draw vertical stripes (to be rendered)
void Raycaster::CastRays()
{
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

        //if (lineStart < 0)
        //{
        //    lineStart = 0;
        //}

        //if (lineEnd >= this->windowHeight)
        //{
        //    lineEnd = this->windowHeight - 1;
        //}

        //draw line with wall color
        //unsigned int color = currTile.GetColor();
        //if (hitYSide)
        //{
        //    unsigned char newR = ((color >> 16) & 0xFF) * 3 / 4;
        //    unsigned char newG = ((color >> 8) & 0xFF) * 3 / 4;
        //    unsigned char newB = (color & 0xFF) * 3 / 4;
        //    color = 0xFF000000 | (newR << 16) | (newG << 8) | newB;
        //}

        //this->DrawVertLine(x, lineStart, lineEnd, color);

        //texture indexing
        double wallXCoord;
        if (!hitYSide) 
        {
            wallXCoord = rayOrigin(1) + perpendicularDist * rayHeadingY;
        }
        else
        {
            wallXCoord = rayOrigin(0) + perpendicularDist * rayHeadingX;
        }
        wallXCoord -= floor(wallXCoord);

        //get the texture of the intersected wall and its dimensions
        SDL_Texture* wallTexture = this->textureMap[currTile.GetWallTextureFilename()];
        int wallTextureWidth, wallTextureHeight;
        SDL_QueryTexture(wallTexture, nullptr, nullptr, &wallTextureWidth, &wallTextureHeight);

        //map the wall coordinate to the x coordinate to be drawn of the texture
        int textureXCoord = int(wallXCoord * double(wallTextureWidth));

        //reverse coordinate direction based on ray heading
        //if (!hitYSide && rayHeadingX > 0)
        //{
        //    textureXCoord = wallTextureWidth - textureXCoord - 1;
        //}
        //if (hitYSide && rayHeadingY < 0)
        //{
        //    textureXCoord = wallTextureWidth - textureXCoord - 1;
        //}
        
        //draw the texture slice in the appropriate location on screen
        SDL_Rect textureSource;
        SDL_Rect textureDest;

        //pull the appropriate strip from the wall texture
        textureSource.x = textureXCoord;
        textureSource.y = 0;
        textureSource.w = 1;
        textureSource.h = wallTextureHeight;

        //the destination (location drawn on screen)
        textureDest.x = x;
        textureDest.y = lineStart;
        textureDest.w = 1;
        textureDest.h = lineSize;
        
        SDL_RenderCopy(this->renderer, wallTexture, &textureSource, &textureDest);
    }
}

//do rendering and game updates at ~60fps
void Raycaster::DoUpdates()
{
    //frame timing
    this->prevFrameTime = this->currFrameTime;
    this->currFrameTime = SDL_GetTicks();

    //elapsed frame time in milliseconds
    this->elapsedFrameTime = (this->currFrameTime - this->prevFrameTime);
    this->FPS = 1000.0 / (double)this->elapsedFrameTime;
    double optimalFrameTime = 1000.0 / this->targetFPS;    
    double delta = this->elapsedFrameTime / optimalFrameTime;
    

    //set speed modifiers
    this->player->SetMoveSpeed(delta / 20.0);
    this->player->SetTurnSpeed(delta / 20.0);

    this->RenderFrame();
    this->HandleEvents();

    //update FPS counter every ~1 second
    if (this->currFrameTime - this->lastFPSUpdate > 1000.0)
    {
        SDL_SetWindowTitle(this->mainWindow, to_string(FPS).c_str());
        this->lastFPSUpdate = this->currFrameTime;
    }

    //limit FPS if we're going 2 fast
    if (this->elapsedFrameTime < optimalFrameTime)
    {
        SDL_Delay(optimalFrameTime - this->elapsedFrameTime);
    }
}

void Raycaster::RenderFrame()
{
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

void Raycaster::HandleEvents()
{
    arma::vec moveCoords = this->player->GetFrontXYPos();

    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
            this->done = true;
            this->Cleanup();
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
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
                    this->done = true;
                    this->Cleanup();
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

void Raycaster::Cleanup()
{
    SDL_DestroyWindow(this->mainWindow);
    SDL_DestroyRenderer(this->renderer);
}
