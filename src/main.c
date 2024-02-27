// #include "application.h"
#include "../include/raylib.h"
#include "frontend.h"
#include "rendering.h"
#include <stdio.h>
#include <stdlib.h>

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

int main() {
  int screenWidth = 600;
  int screenHeight = 600;

  InitWindow(screenWidth, screenHeight, "Mandelbrocc");
  SetTargetFPS(60);

  unsigned int pixels[screenWidth * screenHeight]; // Your pixel data
  rendering_setup(screenWidth, screenHeight);

  Image image = GenImageWhiteNoise(screenWidth, screenHeight, 0.1);
  Texture2D texture = LoadTextureFromImage(image);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    action_listeners();
    draw(pixels, screenWidth, screenHeight);

    UpdateTexture(texture, pixels);
    DrawTexture(texture, 0, 0, WHITE);

    DrawText(TextFormat("%02.02f ms", GetFrameTime() * 1000), 5, 5, 12, GREEN);
    draw_text();

    EndDrawing();
  }

  UnloadTexture(texture); // Unload texture
  CloseWindow();

  return 0;
}

void __platform_render_text_ui(TextUI *text_ui, ...) {

  char str[256];
  va_list args;
  va_start(args, text_ui->format_str);
  vsnprintf(str, sizeof(str), text_ui->format_str, args);
  va_end(args);

  DrawText(str, text_ui->x, text_ui->y, 12, GREEN);
}
