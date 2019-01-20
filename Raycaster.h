#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <utility>
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Enums.h"

class Raycaster
{
    private:
        //rendering
        SDL_Window* mainWindow;
        SDL_Renderer* renderer;
        SDL_Texture* windowTexture;

        //map texture filenames to corresponding SDL textures (loop through game map in constructor)
        std::unordered_map<std::string, SDL_Texture*> textureMap;

        //user input
        SDL_Event event;

        //game logic
        Player* player;
        Map* map;

        int windowWidth, windowHeight;
        
        double focalLength;

        double targetFPS;
        double FPS;
        double lastFPSUpdate;
        double prevFrameTime;
        double currFrameTime;
        double elapsedFrameTime;

        //game state info
        bool done;
        bool strafeLeft;
        bool strafeRight;
        bool turnLeft;
        bool turnRight;
        bool moveForward;
        bool moveBackward;

    public:
        Raycaster(int windowWidth, int windowHeight);

        void InitVideo(int windowWidth, int windowHeight);
        void CastRays();
        void RenderFrame();
        void DrawVertLine(int x, int yStart, int yEnd, unsigned int color);
        void DrawFloorAndCeiling(unsigned int ceilingColor, unsigned int floorColor);
        void HandleEvents();
        void Cleanup();
        void RunGameLoop();
        void DebugPrint();
        void DoUpdates();
        void LoadTextures();

        void PlayerMove(MoveType move);

        bool IsDoneRendering();

        SDL_Texture* LoadTexture(std::string filename);
};
