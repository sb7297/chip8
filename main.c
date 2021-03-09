#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int main() {
    chip8_init();
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init Error: %s\n" , SDL_GetError());
		return 1;
	}
    SDL_Window *win = SDL_CreateWindow("sdl2 pixel drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }
    SDL_Texture  *texture = SDL_CreateTexture(ren,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    if (texture == NULL) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 3;
    }

    uint8_t* pixels = malloc(640*480*sizeof(uint32_t));
    memset(pixels, 255, 640*480*sizeof(uint32_t));
    SDL_UpdateTexture(texture, NULL, pixels, 640*sizeof(uint32_t));

    FILE* fp = fopen("test_opcode.ch8", "rb");
    uint8_t buf[0xDFF] = {0};
    fread(buf, sizeof(buf), 1, fp);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, texture, NULL, NULL);
        SDL_RenderPresent(ren);
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}