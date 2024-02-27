// #include "application.h"
#include "../include/raylib.h"
#include "rendering.h"
#include <stdio.h>
#include <stdlib.h>

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

    draw(pixels, screenWidth, screenHeight);
    UpdateTexture(texture, pixels);
    DrawTexture(texture, 0, 0, WHITE);

    // Image image = LoadImageEx((Color*)pixels, width, height);
    // Texture2D texture = LoadTextureFromImage(image);
    // UnloadImage(image); // Once the texture is created, the image can be
    // unloaded

    EndDrawing();
  }

  UnloadTexture(texture); // Unload texture
  CloseWindow();

  return 0;
}
