#include "raylib.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)
#define BEGIN_LIFE (3)
#define BRICKS_COLUMNS (15)
#define BRICKS_LINES (6)
#define TOTAL_BRICKS (BRICKS_COLUMNS * BRICKS_LINES)
#define POINTS (3)
#define BEGIN_STATUS_BAR (SCREEN_HEIGHT * 8 / 9)

//COLOR PALLET
#define BACKGROUND_COLOR ((Color) { 250, 250, 250, 255})
#define HEART_COLOR ((Color) {173, 46, 36, 255})
#define DARK_COLOR ((Color) {46, 40, 42, 255})
#define LIGHT_COLOR ((Color) {250, 250, 249, 255})

typedef enum GameScreen {
    GAMEPLAY, ENDGAME,
} GameScreen;

typedef struct Player {
    int points;
    int life;
    bool alive;
} Player;

typedef struct Ball {
    float radius;
    Vector2 position;
    Vector2 speed;
    bool active;
} Ball;

typedef struct GameBar {
    Vector2 position;
    Vector2 size;
    Rectangle rectangle;
    bool active;
} GameBar;

typedef struct Bricks {
    Vector2 size;
    Vector2 position;
    bool active;
    Rectangle rectangle;
} Bricks;

//--------------------------------------------------
GameScreen screen = GAMEPLAY;

Player player;
Ball ball;
GameBar gameBar;
Bricks bricks[BRICKS_LINES][BRICKS_COLUMNS];

int inactiveBricks = 0;
bool gamePaused = false;
int framesCounter;
//-------------------------------------------------- function declaration
void initGame();
void drawGame();
void updateGame();
//-------------------------------------------------- MAIN
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout Game by Tati Muniz");
    SetTargetFPS(60);

    initGame();
    while (!WindowShouldClose()) {
        updateGame();
        drawGame();
    }

    CloseWindow();

    return 0;
}
//-------------------------------------------------- INIT
void initGame() {
    framesCounter = 0;
    player = (Player) {
            0,
            BEGIN_LIFE,
            true
    };


    gameBar = (GameBar) {
            (Vector2) {
                    (float) SCREEN_WIDTH / 2,
                    (float) SCREEN_HEIGHT * 7 / 8
            },
            (Vector2) {
                    90,
                    10,
            }
    };
    gameBar.active = false;

    ball = (Ball) {
            5,
            (Vector2) {
                    gameBar.position.x,
                    gameBar.position.y - 10
            },
            (Vector2) {0, 0},
            false
    };

    double columnSpace = 0;
    for (int i = 0; i < BRICKS_LINES; ++i) {
        double lineSpace = 0;
        for (int j = 0; j < BRICKS_COLUMNS; ++j) {
            bricks[i][j].size = (Vector2) {
                    40,
                    10,
            };
            bricks[i][j].position = (Vector2) {
                    (j * bricks[i][j].size.x) + 12.5 + lineSpace,
                    (i * bricks[i][j].size.y) + 12.5 + columnSpace
            };
            lineSpace += 12.5;
            bricks[i][j].rectangle = (Rectangle) {
                    bricks[i][j].position.x,
                    bricks[i][j].position.y,
                    bricks[i][j].size.x,
                    bricks[i][j].size.y
            };
            bricks[i][j].active = true;
        }
        columnSpace += 12.5;
    }

}
//-------------------------------------------------- UPDATE
void updateGame() {

    switch (screen) {
        case GAMEPLAY:
            if (IsKeyPressed('P')) gamePaused = !gamePaused;

            //---------------------------------------------------------------------------------------------BALL MOVEMENT

            if (!gamePaused) {

                if (!ball.active && IsKeyPressed(KEY_SPACE)) {
                    ball.active = true;
                    ball.speed.y = -5.0f;
                    gameBar.active = true;
                }

                if (ball.active) {
                    ball.position.y += ball.speed.y;
                    ball.position.x += ball.speed.x;


                    //------------------------------------------------------------------------------------------GAME BAR MOVEMENT
                    if (gameBar.active) {
                        if (IsKeyDown(KEY_RIGHT)) {
                            gameBar.position.x += 5.0f;
                        } else if (IsKeyDown(KEY_LEFT)) {
                            gameBar.position.x -= 5.0f;
                        }
                    }

                    //----------------------------------------------------------------------------------COLLISION GAME BAR -> WALL
                    if ((gameBar.position.x + (gameBar.size.x / 2)) >= SCREEN_WIDTH) {
                        gameBar.position.x -= 5.0f;
                    } else if ((gameBar.position.x - (gameBar.size.x / 2)) <= 0) {
                        gameBar.position.x += 5.0f;
                    }

                    if (IsKeyPressed(KEY_P)) {

                    }

                    //------------------------------------------------------------------------------------COLLISION BALL -> WALL
                    //UPPER
                    if (ball.position.y <= 0) {
                        ball.speed.y *= -1.0f;
                    }
                    //SIDES
                    if (ball.position.x + ball.radius >= SCREEN_WIDTH || ((ball.position.x - ball.radius) <= 0)) {
                        ball.speed.x *= -1;
                    }

                    //LOWER
                    if (ball.position.y >= SCREEN_HEIGHT) {
                        player.life -= 1;
                        ball.position.x = gameBar.position.x;
                        ball.position.y = gameBar.position.y - 10;
                        ball.active = false;
                    }

                    //-------------------------------------------------------------------------------COLLISION BALL -> GAME BAR

                    if (CheckCollisionCircleRec(ball.position, ball.radius,
                                                (Rectangle) {
                                                        gameBar.position.x - gameBar.size.x / 2,
                                                        gameBar.position.y - gameBar.size.y / 2,
                                                        gameBar.size.x,
                                                        gameBar.size.y
                                                })) {
                        ball.speed.y *= -1.0f;
                        ball.speed.x = (ball.position.x - gameBar.position.x) / (gameBar.size.x / 2) * 5;
                    }


                    //---------------------------------------------------------------------------------COLLISION BALL -> BRICK
                    for (int i = 0; i < BRICKS_LINES; ++i) {
                        for (int j = 0; j < BRICKS_COLUMNS; ++j) {

                            if (bricks[i][j].active &&
                                CheckCollisionCircleRec(ball.position, ball.radius, bricks[i][j].rectangle)) {
                                bricks[i][j].active = false;
                                player.points += POINTS;
                                ball.speed.y *= -1;
                                inactiveBricks += 1;
                            }

                        }
                    }
                    //-------------------------------------- BALL ACTIVE
                }

                if (player.life <= 0) {
                    player.alive = false;
                    screen = ENDGAME;
                } else if (inactiveBricks >= TOTAL_BRICKS) {
                    screen = ENDGAME;
                }
            }
            break;
        case ENDGAME:
            if (IsKeyPressed(KEY_ENTER)) {
                initGame();
                screen = GAMEPLAY;
            }
            break;
        default:
            break;
    }


}
//-------------------------------------------------- DRAW
void drawGame() {

    BeginDrawing();

    switch (screen) {
        case GAMEPLAY:
            //--------------------------------------------------DRAW STATUS BAR
            DrawRectangle(
                    0,
                    SCREEN_HEIGHT * 8 / 9,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    DARK_COLOR
            );
            int heartSpace = 0;
            int heartBarSize = 0;
            for (int k = 0; k < player.life; ++k) {
                DrawRectangle(
                        10 + heartSpace + heartBarSize,
                        (SCREEN_HEIGHT * 8 / 9) + 20,
                        30,
                        5,
                        HEART_COLOR
                );
                heartSpace += 10;
                heartBarSize += 30;
            }
            const Vector2 pointsTextSize = MeasureTextEx(GetFontDefault(), TextFormat("%i", player.points), 20, 1);
            DrawText(
                    TextFormat("%i", player.points),
                    SCREEN_WIDTH / 2 - pointsTextSize.x / 2,
                    BEGIN_STATUS_BAR + (25.0 - pointsTextSize.y / 2.0),
                    20,
                    LIGHT_COLOR
            );
            DrawText(
                    "Press P to pause",
                    SCREEN_WIDTH - (MeasureTextEx(GetFontDefault(), "Press P to pause", 20, 1).x + 40),
                    BEGIN_STATUS_BAR + (MeasureTextEx(GetFontDefault(), "Press P to pause", 20, 1).y / 2.0),
                    20,
                    LIGHT_COLOR
            );

            //--------------------------------------------------DRAW GAME BAR
            DrawRectangleRounded(
                    (Rectangle) {
                            gameBar.position.x - gameBar.size.x / 2,
                            gameBar.position.y - gameBar.size.y / 2,
                            gameBar.size.x,
                            gameBar.size.y
                    },
                    10,
                    4,
                    DARK_COLOR
                    );

            //--------------------------------------------------DRAW BALL
            DrawCircleLines(
                    ball.position.x,
                    ball.position.y,
                    ball.radius,
                    DARK_COLOR
            );

            //--------------------------------------------------DRAW BRICKS
            for (int i = 0; i < BRICKS_LINES; ++i) {
                for (int j = 0; j < BRICKS_COLUMNS; ++j) {
                    if (bricks[i][j].active) {
                        DrawRectangleRounded(
                                (Rectangle){
                                        bricks[i][j].position.x,
                                        bricks[i][j].position.y,
                                        bricks[i][j].size.x,
                                        bricks[i][j].size.y,
                                },
                                5,
                                4,
                                DARK_COLOR
                                );

                    }

                }
            }

            //----------------------------------------------------DRAW PAUSE PHRASE
            if (gamePaused) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {46, 40, 42, 150});
                DrawText(
                        "PAUSE",
                        SCREEN_WIDTH / 2.0 - (MeasureTextEx(GetFontDefault(), "PAUSE", 100, 1).x / 2),
                        SCREEN_HEIGHT / 2.0 - (MeasureTextEx(GetFontDefault(), "PAUSE", 100, 1).y / 2),
                        100,
                        DARK_COLOR
                );
            }

            ClearBackground(BACKGROUND_COLOR);

            break;
        case ENDGAME:
            framesCounter++;

             char* victoryText = "yasssss congrats!";
             Vector2 victoryTextSize = MeasureTextEx(GetFontDefault(), victoryText, 50, 1);
             char* defeatText = "oh no D:";
             Vector2 defeatTextSize = MeasureTextEx(GetFontDefault(), defeatText, 50, 1);
            char* text = "";
            Vector2 textSize = (Vector2){ 0,0};

            if (player.alive) {
                text = victoryText;
                textSize = victoryTextSize;
            } else {
                text = defeatText;
                textSize = defeatTextSize;
            }

            DrawText(
                    text,
                    SCREEN_WIDTH/2.0 - (textSize.x/2),
                    SCREEN_HEIGHT/3.0 - (textSize.y/2),
                    50,
                    DARK_COLOR
            );
            if ((framesCounter / 30) % 2 == 0) {
                DrawText(
                        "Press ENTER to play again.",
                        SCREEN_WIDTH / 2.0 -
                        (MeasureTextEx(GetFontDefault(), "Press ENTER to play again.", 20, 1).x / 2),
                        SCREEN_HEIGHT / 2.0 -
                        (MeasureTextEx(GetFontDefault(), "Press ENTER to play again.", 20, 1).y / 2),
                        20,
                        DARK_COLOR
                );
            }

            ClearBackground(BACKGROUND_COLOR);
            break;
        default: break;
    }

    EndDrawing();
}

