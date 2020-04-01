#ifndef MAIN_UI_MANAGER_HPP
#define MAIN_UI_MANAGER_HPP

#include <SDL.h>
#include <SDL_ttf.h> // http://www.sdltutorials.com/sdl-ttf
#include "Texture.hpp"
class Scene;

class MainUiManager {
    protected:
        // The main window
        SDL_Window* mainWindow;
        SDL_Renderer* renderer;

        TTF_Font* mainFont;

        Uint32 uiTick;

        int mousePosX, mousePosY;

        bool initSDL();
        bool initWindow(); // requires initSDL()
        bool initRenderer();
        bool initFont();
        bool initTextures(); // requires initWindow()

        void deinitSDL();
        void deinitWindow();
        void deinitRenderer();
        void deinitFont();
        void deinitTextures();

    public:
        const char *WINDOW_TITLE;
        const int SCREEN_WIDTH;
        const int SCREEN_HEIGHT;
        const SDL_Rect SCREEN_RECT;

        float fps; // For display purposes only; is written to by MainApp.

        MainUiManager();
        ~MainUiManager();
        bool init();
        void deinit();

        Uint32 getUiTick();
        void sleep(float duration); // in seconds, capped at millisecond precision

        // Collect hardware inputs and pass them to scene for interpretation
        void getInputs(Scene* scene);

        // Draws the scene to screen
        void draw(Scene* scene);

        //void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        //void drawPixel(int x, int y);
        void renderTextToScreen(const char *text, SDL_Color fg, int x, int y);
        void renderImageToScreen(SDL_Surface* image, int x, int y);
};

#endif // MAIN_UI_MANAGER_HPP
