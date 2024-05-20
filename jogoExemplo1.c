#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_SIZE 50
#define IMAGE_SIZE 190
#define OBSTACLE_WIDTH 100
#define OBSTACLE_HEIGHT 50

typedef struct {
    Rectangle rect;
    Vector2 speed;
} Obstacle;

void InitObstacles(Obstacle obstacles[], int count);
void UpdateObstacles(Obstacle obstacles[], int count, float delta);
void DrawObstacles(Obstacle obstacles[], int count);

int main(void)
{
    // Inicializar a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Migra");

    // Carregar a imagem de fundo
    Texture2D background = LoadTexture("images/areia.png");

    // Carregar a imagem do jogador
    Texture2D playerTexture = LoadTexture("images/player.png");
    Rectangle player = { SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT - PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE };
    Vector2 playerSpeed = { 200.0f, 200.0f };

    // Configurar obstáculos
    int obstacleCount = 10;
    Obstacle obstacles[10];
    InitObstacles(obstacles, obstacleCount);

    // Configurar a taxa de quadros
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Atualizar
        float deltaTime = GetFrameTime();
        
        // Movimento do jogador
        if (IsKeyDown(KEY_RIGHT) && player.x + player.width < SCREEN_WIDTH) player.x += playerSpeed.x * deltaTime;
        if (IsKeyDown(KEY_LEFT) && player.x > 0) player.x -= playerSpeed.x * deltaTime;
        if (IsKeyDown(KEY_UP) && player.y > 0) player.y -= playerSpeed.y * deltaTime;
        if (IsKeyDown(KEY_DOWN) && player.y + player.height < SCREEN_HEIGHT) player.y += playerSpeed.y * deltaTime;

        // Atualizar obstáculos
        UpdateObstacles(obstacles, obstacleCount, deltaTime);

        // Colisão
        for (int i = 0; i < obstacleCount; i++)
        {
            if (CheckCollisionRecs(player, obstacles[i].rect))
            {
                // Reiniciar o jogo (pode ser mais elaborado)
                player.x = SCREEN_WIDTH / 2 - PLAYER_SIZE / 2;
                player.y = SCREEN_HEIGHT - PLAYER_SIZE;
                break;
            }
        }

        // Desenhar
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Desenhar a imagem de fundo
            DrawTexture(background, 0, 0, WHITE);

            // Desenhar jogador redimensionado
            Rectangle sourceRect = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };
            Rectangle destRect = { player.x + (PLAYER_SIZE - IMAGE_SIZE) / 2, player.y + (PLAYER_SIZE - IMAGE_SIZE) / 2, IMAGE_SIZE, IMAGE_SIZE };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(playerTexture, sourceRect, destRect, origin, 0.0f, WHITE);

            // Desenhar obstáculos
            DrawObstacles(obstacles, obstacleCount);

        EndDrawing();
    }

    // Descarregar texturas
    UnloadTexture(background);
    UnloadTexture(playerTexture);

    // Fechar a janela
    CloseWindow();

    return 0;
}

void UpdateObstacles(Obstacle obstacles[], int count, float delta)
{
    for (int i = 0; i < count; i++)
    {
        obstacles[i].rect.x += obstacles[i].speed.x * delta;

        // Se o obstáculo sair pela direita, reposicioná-lo no lado esquerdo da tela
        if (obstacles[i].rect.x > SCREEN_WIDTH)
        {
            // Reposicionar o obstáculo antes da tela começar
            obstacles[i].rect.x = -obstacles[i].rect.width;

            // Definir uma nova posição vertical para o obstáculo
            obstacles[i].rect.y = GetRandomValue(0, SCREEN_HEIGHT / 2);
        }
    }
}

void InitObstacles(Obstacle obstacles[], int count)
{
    // Definir um padrão para a geração de obstáculos
    int spacing = SCREEN_WIDTH / count;
    for (int i = 0; i < count; i++)
    {
        obstacles[i].rect.x = i * spacing;
        obstacles[i].rect.y = GetRandomValue(0, SCREEN_HEIGHT / 2);
        obstacles[i].rect.width = OBSTACLE_WIDTH;
        obstacles[i].rect.height = OBSTACLE_HEIGHT;
        obstacles[i].speed.x = GetRandomValue(100, 300);
        obstacles[i].speed.y = 0;
    }
}


void DrawObstacles(Obstacle obstacles[], int count)
{
    for (int i = 0; i < count; i++)
    {
        DrawRectangleRec(obstacles[i].rect, RED);
    }
}
