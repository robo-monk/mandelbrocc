#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h> // Include SDL_ttf for font rendering

int w_width = 640;
int w_height = 480;

int get_rand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}

void render_fps(SDL_Renderer *renderer, TTF_Font *font, int fps)
{
  SDL_Color color = {255, 255, 255, 255}; // White color for the FPS counter text
  char fpsText[15];
  sprintf(fpsText, "FPS: %d", fps); // Create a string for the FPS text

  SDL_Surface *surface = TTF_RenderText_Solid(font, fpsText, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {5, 5, surface->w, surface->h}; // Position: top left
  SDL_FreeSurface(surface);                           // No longer needed

  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_DestroyTexture(texture); // Clean up
}

void draw(SDL_Renderer *renderer, int screenWidth, int screenHeight)
{
  // Create a texture for pixel manipulation
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           screenWidth, screenHeight);

  // Allocate a buffer for pixel data
  Uint32 *pixels = malloc(screenWidth * screenHeight * sizeof(Uint32));
  if (!pixels)
  {
    // Handle allocation failure
    return;
  }

  // Fill the buffer with random colors
  for (int i = 0; i < screenWidth * screenHeight; i++)
  {
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;
    pixels[i] = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
  }

  // Update texture with pixel buffer
  SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(Uint32));

  // Free the pixel buffer
  free(pixels);

  // Copy the texture to the renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL);

  // Destroy the texture after use
  SDL_DestroyTexture(texture);
}

void event_loop(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font)
{
  Uint32 startTick, endTick, frameCount = 0;
  Uint32 secondStart = SDL_GetTicks(), fps = 0;

  SDL_bool done = SDL_FALSE;
  while (!done)
  {
    startTick = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        done = SDL_TRUE;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Your drawing code here (or call a function)
    draw(renderer, w_width, w_height);

    // Calculate and render FPS
    endTick = SDL_GetTicks();
    if (endTick - secondStart >= 1000)
    {
      fps = frameCount;
      frameCount = 0;
      secondStart = endTick;
    }
    render_fps(renderer, font, fps);
    frameCount++;

    SDL_RenderPresent(renderer);
    SDL_Delay(1); // Delay to limit FPS (if necessary)
  }
}

void init_sdl()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0)
  {
    printf("SDL or SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());
    return;
  }


  SDL_Window *window = SDL_CreateWindow("Mandelbrocc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (window == NULL || renderer == NULL)
  {
    printf("Window or Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    TTF_Font *font = TTF_OpenFont("./assets/mononoki-Regular.ttf", 24); // Load your font
    if (!font)
    {
      printf("Failed to load font: %s\n", TTF_GetError());
    }
    else
    {
      event_loop(window, renderer, font);
      TTF_CloseFont(font); // Clean up font
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
  TTF_Quit();
  SDL_Quit();
}
