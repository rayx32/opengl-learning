#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <stdlib.h>
#include <iostream>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_GLContext context;

void drawRandomPixels() {
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8 * pixels = (Uint8 *)surface->pixels;

    for (int i=0; i < 1048576; i++) {
        char randomByte = rand() % 255;
        pixels[i] = randomByte;
    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(screenTexture);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);


#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    window = SDL_CreateWindow("ItchGameEngine",
                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                     1280, 720,
                     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE| SDL_WINDOW_SHOWN);

    if(!window) {
        std::cout << "Error creating SDL2 window" << std::endl;
        return -1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    surface = SDL_CreateRGBSurface(0, 512, 512, 32, 0, 0, 0, 0);

    /**  TODO: Context creation with SDL Accelerated 2D rendering fails the Web build */
#ifndef __EMSCRIPTEN__
    context = SDL_GL_CreateContext(window);

    if(!context) {
        std::cout << "Error creating OpenGL context" << std::endl;
        return -1;
    }

    SDL_GL_MakeCurrent(window, context);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
#endif

    const GLubyte* version = glGetString(GL_VERSION);

    if (version == 0) {
        printf(
                "Unable to get OpenGL ES version string: %d\n",
                glGetError()
        );
        return EXIT_FAILURE;
    }

    printf("Version string: %s\n", version);

    SDL_GL_DeleteContext(context);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(drawRandomPixels, 0, 1);
#else
    while(1) {
        drawRandomPixels();
        SDL_Delay(16);
    }
#endif
    return 0;
}
