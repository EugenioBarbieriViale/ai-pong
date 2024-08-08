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

const float h = 2;
const float rate = 1e-3;

float model[N_INPUTS];
float gradient[N_INPUTS];


// inputs: xball, yball, vxball, vyball, yplayer, ybot


float sigmoid(float x) {
	return 1.f/(1.f + expf(-x));
}

float rand_float(void) {
    return (float) rand() / (float) RAND_MAX;
}

float forward(float model[], int xball, int yball, int yplayer, int ybot) {
    float ans = 0;

    ans += model[0] * xball;
    ans += model[1] * yball;
    ans += model[2] * vx;
    ans += model[3] * vy;
    ans += model[4] * yplayer;
    ans += model[5] * ybot;

    return sigmoid(ans);
}

float loss(float model[], int xball, int yball, int yplayer, int ybot) {
    float e = 0.f;
    for (int i=0; i<N_INPUTS; i++) {
        float out = forward(model, xball, yball, yplayer, ybot);

        float expected_value = vy * 2;

        e += (expected_value - out) * (expected_value - out);
    }
    e /= N_INPUTS;
    
    return e;
}

float *init_model(float model[]) {
    for (int i=0; i<N_INPUTS; i++) {
        model[i] = rand_float();
    }
    return model;
}

float *applay_diff(float model[], float gradient[]) {
    for (int i=0; i<N_INPUTS; i++) {
        model[i] -= rate * gradient[i];
    }
    return model;
}

float *calc_gradient(float model[], int xball, int yball, int yplayer, int ybot) {
    float prev_loss = loss(model, xball, yball, yplayer, ybot);

    for (int i=0; i<N_INPUTS; i++) {
        model[i] += h;
        /* printf("%f %f\n", loss(model, xball, yball, yplayer, ybot), prev_loss); */
        gradient[i] = (loss(model, xball, yball, yplayer, ybot) - prev_loss) / h;
        model[i] -= h;
    }
    return gradient;
}

int main() {
    InitWindow(X, Y, "AI Pong");
    SetTargetFPS(60);
    
    float *m;
    m = init_model(model);
    float *g;

    Rectangle ai = {X - 50, Y/2 - 50, 20, 100};
    Rectangle bot = {30, Y/2 - 50, 20, 100};

    const int size = 20;
    Rectangle ball = {X/2, Y/2, size, size};

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

        g = calc_gradient(m, ball.x, ball.y, ai.y, bot.y);
        m = applay_diff(m, g);

        float dir = forward(model, ball.x, ball.y, ai.y, bot.y);

        /* printf("%f\n", loss(model, ball.x, ball.y, ai.y, bot.y)); */
        printf("%f\n", gradient[3]);
        ai.y += dir;

        /* if (IsKeyDown(KEY_UP)) */
        /*     ai.y -= vel; */

        /* if (IsKeyDown(KEY_DOWN)) */
        /*     ai.y += vel; */

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
