// #include "application.h"
#include "../include/raylib.h"
#include "frontend.h"
#include "rendering.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_t.h>

void action_listeners() {

  const int len_actions = 8;
  const int action_keys[] = {

      // movement
      KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN,

      // zoom
      KEY_EQUAL, KEY_MINUS,

      // misc
      KEY_PERIOD, KEY_COMMA};

  const Action mapped_actions[] = {

      // movement
      ACTION_RIGHT, ACTION_LEFT, ACTION_UP, ACTION_DOWN,

      // zoom
      ACTION_ZOOM_IN, ACTION_ZOOM_OUT,

      // misc
      MAX_ITERATIONS_INCR, MAX_ITERATIONS_DECR

  };

  for (int i = 0; i < len_actions; i++) {
    if (IsKeyDown(action_keys[i])) {
      perform_action(mapped_actions[i]);
    }
  }
}

int screenWidth, screenHeight;
unsigned int *pixels;
int _quit = 0;
// unsigned int pixels[5000*1000];

void *draw_loop() {
  while (!_quit) {
    draw(pixels, screenWidth, screenHeight);
  }
}

void thread_function(int thread_count, ) {
  pthread_t *thread_ids = malloc(sizeof(pthread_t) * thread_count);

  for (int i = 0; i < thread_count; i++) {
  }

  free(thread_ids);
}

int main() {
  screenWidth = 800;
  screenHeight = 800;

  pixels = malloc(screenHeight * screenWidth * sizeof(unsigned int));

  InitWindow(screenWidth, screenHeight, "Mandelbrocc");
  SetTargetFPS(60);

  rendering_setup(screenWidth, screenHeight);

  Image image = GenImageWhiteNoise(screenWidth, screenHeight, 0.1);
  Texture2D texture = LoadTextureFromImage(image);

  pthread_t thread_id; // Variable to hold the thread ID
  int result = pthread_create(&thread_id, NULL, draw_loop, NULL);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    action_listeners();
    // draw(pixels, screenWidth, screenHeight);

    UpdateTexture(texture, pixels);
    DrawTexture(texture, 0, 0, WHITE);

    DrawText(TextFormat("%02.02f ms", GetFrameTime() * 1000), 5, 5, 12, GREEN);
    draw_text();

    EndDrawing();
  }

  _quit = 1;
  pthread_join(thread_id, NULL);
  UnloadTexture(texture); // Unload texture
  free(pixels);
  CloseWindow();

  return 0;
}

void __platform_render_text_ui(TextUI *text_ui) {
  // char str[256];
  // va_list args;
  // va_start(args, text_ui->format_str);
  // vsnprintf(str, sizeof(str), text_ui->format_str, args);
  // va_end(args);
  DrawText(text_ui->str, text_ui->x, text_ui->y, 12, GREEN);
}
