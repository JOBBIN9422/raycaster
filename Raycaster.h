#include <SDL2/SDL.h>
#include <unordered_map>
#include <cmath>
#include "Player.h"
#include "Map.h"

class Raycaster
{
    private:
        //rendering
        SDL_Window* mainWindow;

        SDL_Texture* windowTexture;

        //map texture filenames to corresponding SDL textures (loop through game map in constructor)
        std::unordered_map<std::string, SDL_Texture*> textureMap;

        SDL_Renderer* renderer;

        uint* frameBuffer;

        //user input
        SDL_Event keyEvent;

        //game logic
        Player* player;
        Map* map;

        int windowWidth, windowHeight;
        
        double focalLength;

        double prevFrameTime;
        double currFrameTime;
        double elapsedFrameTime;

    public:
        Raycaster(int windowWidth, int windowHeight);

        void InitVideo(int windowWidth, int windowHeight);
        void RunFrame();
        void RenderFrame();
        void DrawVertLine(int x, int yStart, int yEnd, unsigned int color);
        void PollKeyboard();

        void DebugPrint();
};
