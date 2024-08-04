#include <stdio.h>
#include <stdbool.h>
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

    int vx = 3;
    int vy = 3;

    unsigned int ai_score = 0;
    unsigned int bot_score = 0;


    while (!WindowShouldClose()) {
        bool game_over = false;

        ball.x += vx;
        ball.y += vy;

        if (ball.x <= 0) {
            ai_score++;
            game_over = true;
        }

        if (ball.x >= X-size) {
            bot_score++;
            game_over = true;
        }

        if (game_over) {
            ai.x = X - 50;
            ai.y = Y/2 - 50;

            bot.x = 30;
            bot.y = Y/2 - 50;

            ball.x = X/2;
            ball.y = Y/2;
        }

        if (ball.y <= 0 || ball.y >= Y-size)
            vy *= -1;

        if (IsKeyDown(KEY_UP))
            ai.y -= 4;

        if (IsKeyDown(KEY_DOWN))
            ai.y += 4;

        if (CheckCollisionRecs(ai, ball) || CheckCollisionRecs(bot, ball))
            vx *= -1;

        bot.y += vy;


        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRec(ai, RED);
        DrawRectangleRec(bot, WHITE);
        DrawRectangleRec(ball, BLUE);

        DrawText(TextFormat("%d", bot_score), (int)X/2 - 50, 20, 50, WHITE);
        DrawText(TextFormat("%d", ai_score), (int)X/2 + 50, 20, 50, WHITE);

        EndDrawing();
    }
}
