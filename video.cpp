#include "video.h"
#include "raylib.h"

int render(const int pixels[]) {
    const int scale = 8;

    InitWindow(32 * scale, 32 * scale, "Video");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                int index = y * 32 + x;

                if (pixels[index] != 0) {
                    DrawRectangle(x * scale, y * scale, scale, scale, RED);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
