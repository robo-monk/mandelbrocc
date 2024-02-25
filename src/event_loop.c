#include "event_loop.h"
#include "SDL_thread.h"
#include "rendering.h"
#include "util.h"

#define FPS_UPDATE_INTERVAL 15 // Update FPS every 10 frames, can be changed
#define TARGET_FPS 60          // Target FPS, can be changed
#define FRAME_DELAY (1000 / TARGET_FPS) // Target frame time in milliseconds

int draw_thread_function(void *ptr);

// Structure to pass data to the drawing thread
typedef struct {
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  Uint32 *pixels;
  int w_width;
  int w_height;
  SDL_atomic_t shutdown;
} DrawThreadData;

void event_loop(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {

  Uint32 startTick, endTick, frame_count = 0;
  Uint32 secondStart = SDL_GetTicks(), fps = 0;
  float frameTimes[FPS_UPDATE_INTERVAL]; // Array to store frame times
  int currentFrame = 0;

  SDL_bool done = SDL_FALSE;

  int w_width, w_height;
  SDL_GetWindowSize(window, &w_width, &w_height);

  rendering_setup(w_width, w_height, renderer, font);

  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, w_width, w_height);

  Uint32 *pixels = malloc(w_width * w_height * sizeof(Uint32));

  DrawThreadData data = {renderer, texture, pixels, w_width, w_height};

  // Create the drawing thread
  SDL_Thread *drawThread =
      SDL_CreateThread(draw_thread_function, "DrawingThread", (void *)&data);

  while (1) {
    startTick = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        done = SDL_TRUE;
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        handle_keyboard_event(&event.key);
        break;
      default:
        break;
      }
    }

    if (done) {
      SDL_AtomicSet(&data.shutdown, 1);
      break;
    }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // TODO dynamically adjust width and height
    // int w_width, w_height;
    // SDL_GetWindowSize(window, &w_width, &w_height);

    // Your drawing code here (or call a function)
    // draw(renderer, w_width, w_height);

    // draw(
    //   renderer,
    //   texture,
    //   pixels,
    //   w_width,
    //   w_height
    // );

    SDL_UpdateTexture(texture, NULL, pixels, w_width * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    endTick = SDL_GetTicks();

    Uint32 frameTime = endTick - startTick;
    currentFrame += 1;
    frameTimes[currentFrame % FPS_UPDATE_INTERVAL] = frameTime;

    if (currentFrame % FPS_UPDATE_INTERVAL == 0) {
      float totalFrameTime = 0;
      for (int i = 0; i < FPS_UPDATE_INTERVAL; i++) {
        totalFrameTime += frameTimes[i];
      }
      fps = (Uint32)(1000.0f / (totalFrameTime / FPS_UPDATE_INTERVAL));
      currentFrame = 0;
    }

    // render_fps(renderer, font, fps);
    draw_text(renderer, font, w_width, w_height);

    SDL_RenderPresent(renderer);
  }

  SDL_WaitThread(drawThread, NULL);
  free(pixels);
}

// this solves nothing since the draw_thread_function still
int draw_thread_function(void *ptr) {
  DrawThreadData *data = (DrawThreadData *)ptr;

  while (!SDL_AtomicGet(&data->shutdown)) {
    // Perform drawing operations here
    draw(data->pixels, data->w_width, data->w_height);
    // draw_text(
    //   data->renderer,
    //   font,
    //   w_width,
    //   w_height
    // );

    // SDL_Delay(FRAME_DELAY); // Control the drawing rate
  }

  return 0; // Exit the thread when shutdown signal is received
}
