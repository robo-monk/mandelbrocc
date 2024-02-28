// #include "application.h"
#include "../lib/raylib/raylib.h"
#include "frontend.h"
#include "rendering.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_t.h>

void process_input_actions() {

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

    process_input_actions();

    UpdateTexture(texture, pixels);
    DrawTexture(texture, 0, 0, WHITE);

    DrawText(TextFormat("%02.02f ms", GetFrameTime() * 1000), 5, 5, 12, GREEN);

    draw_text();
    EndDrawing();
  }

  _quit = 1;
  pthread_join(thread_id, NULL);
  UnloadTexture(texture);
  free(pixels);
  CloseWindow();
  return 0;
}

void __platform_render_text_ui(TextUI *text_ui) {
  DrawText(text_ui->str, text_ui->x, text_ui->y, 12, GREEN);
}
