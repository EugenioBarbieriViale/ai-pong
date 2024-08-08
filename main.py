import pygame, sys

pygame.init()
clock = pygame.time.Clock()

width, height = 1000, 800
FPS = 60

screen = pygame.display.set_mode([width, height]) # wanted size
pygame.display.set_caption("AI Pong")


size = 20

bot_score = 0
ai_score = 0

vel = 4
vx = 2
vy = 2

ai = pygame.Rect(width - 50, height/2 - 50, size, 100)
bot = pygame.Rect(30, height/2 - 50, size, 100)
ball = pygame.Rect(width/2 - size/2, height/2 - size/2, size, size)

def draw_score(ai, bot):
    font = pygame.font.SysFont("Comic Sans MS", 60)

    draw_ai  = font.render(f"{ai}", 1, (255,255,255))
    draw_bot = font.render(f"{bot}", 1, (255,255,255))

    screen.blit(draw_ai, (width/2 - 50, 20))
    screen.blit(draw_bot, (width/2 + 50, 20))

def pixelmap():
    arr = []
    for y in range(height):
        for x in range(width):
            arr.append(screen.get_at((x, y)))
    return arr



while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    screen.fill((0,0,0))

    # print(len(pixelmap()))

    game_over = False;

    ball.x += vx;
    ball.y += vy;

    if ball.x <= 0:
        ai_score += 1
        game_over = True

    if ball.x >= width - size:
        bot_score += 1
        game_over = True

    if game_over:
        ai.x = width - 50
        ai.y = height/2 - 50

        bot.x = 30
        bot.y = height/2 - 50

        ball.x = width/2
        ball.y = height/2

    if ball.y <= 0 or ball.y >= height-size:
        vy *= -1;

    if pygame.Rect.colliderect(ai, ball) or pygame.Rect.colliderect(bot, ball):
        vx *= -1;

    bot.y += vy;

    pygame.draw.rect(screen, (255,0,0), ai)
    pygame.draw.rect(screen, (255,255,255), bot)
    pygame.draw.rect(screen, (0,0,255), ball)

    draw_score(ai_score, bot_score)

    pygame.display.flip()
    clock.tick(FPS)
    pygame.display.update()

pygame.quit()
