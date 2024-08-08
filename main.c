#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define X 1000
#define Y 800

#define N_INPUTS 6
#define EPOCS 100

int vel = 4;
int vx  = 2;
int vy  = 2;

const float h = 1e-2;
const float rate = 1e-2;

// inputs: xball, yball, vxball, vyball, yplayer, ybot
float inputs[N_INPUTS] = {0, 0, 0, 0, 0, 0};

float model[N_INPUTS]; // Model

// Gradient
float g[N_INPUTS];

float sigmoid(float x) {
    return 1.f/(1.f + expf(-x));
}

float rand_float(void) {
    return (float) rand() / (float) RAND_MAX;
}

void init_model(float model[]) {
    for (int i=0; i<N_INPUTS; i++)
        model[i] = rand_float();
}

float forward(float model[]) {
    float ans = 0;
    float b = rand_float();

    for (int i=0; i<N_INPUTS; i++)
        ans += (model[i] * inputs[i]);
    return sigmoid(ans*1e-3 + b);
}

int main() {
    InitWindow(X, Y, "AI Pong");
    SetTargetFPS(60);
    
    init_model(model);

    Rectangle ai = {X - 50, Y/2 - 50, 20, 100};
    Rectangle bot = {30, Y/2 - 50, 20, 100};

    const int size = 20;
    Rectangle ball = {X/2, Y/2, size, size};

    unsigned int ai_score = 0;
    unsigned int bot_score = 0;

    while (!WindowShouldClose()) {

        inputs[0] = ball.x/100;
        inputs[1] = ball.y/100;
        inputs[2] = vx;
        inputs[3] = vy;
        inputs[4] = ai.y/100;
        inputs[5] = bot.y/100;

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

        if (CheckCollisionRecs(ai, ball) || CheckCollisionRecs(bot, ball))
            vx *= -1;

        bot.y += vy;


        float dir = forward(model);
        printf("%f\n", dir);

        if (dir > 0.5)
            ai.y += vel;
        else
            ai.y -= vel;

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
