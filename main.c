#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define X 1000
#define Y 800

int main() {
    InitWindow(X, Y, "AI Pong");
    SetTargetFPS(60);


    Rectangle ai = {X - 50, Y/2 - 50, 20, 100};
    Rectangle bot = {30, Y/2 - 50, 20, 100};

    const int size = 20;
    Rectangle ball = {X/2, Y/2, size, size};

    int vx = 2;
    int vy = 2;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        ball.x += vx;
        ball.y += vy;

        if (ball.x <= 0 || ball.x >= X-size)
            vx *= -1;

        if (ball.y <= 0 || ball.y >= Y-size)
            vy *= -1;

        if (IsKeyDown(KEY_UP))
            ai.y -= 4;

        if (IsKeyDown(KEY_DOWN))
            ai.y += 4;

        if (CheckCollisionRecs(ai, ball) || CheckCollisionRecs(bot, ball))
            vx *= -1;

        DrawRectangleRec(ai, RED);
        DrawRectangleRec(bot, WHITE);
        DrawRectangleRec(ball, BLUE);

        EndDrawing();
    }
}
