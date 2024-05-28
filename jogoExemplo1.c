#include "raylib.h"

#define SCREEN_WIDTH 1600 //800
#define SCREEN_HEIGHT 900 //600

#define PLAYER_SIZE 50
#define IMAGE_SIZE 190
#define OBSTACLE_WIDTH 100
#define OBSTACLE_HEIGHT 50

#define NUM_FRAMES 1 //Numero de frames para a animação do botão


typedef enum GameScreen {MAINMENU = 0, CREDITS, INGAME, GAMEOVER} GameScreen;
typedef enum buttonSprite {NORMAL = 0, JOGAR_PRESSED, CREDITOS_PRESSED, MENU_PRESSED, REJOGAR_PRESSED, SAIR_PRESSED} buttonSprite;

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
    
    //Disabilita o ESC para sair
    SetExitKey(KEY_NULL);
    
    //Booleano de fechar a tela
    bool exitWindow = false;
    
    //Inicializa o Menu
    GameScreen currentScreen = MAINMENU;
    
    // Carregar a imagem de fundo
    Texture2D background = LoadTexture("images/areia.png");
    Vector2 backgroundPosition = {0,0}; //Para usar DrawTextureEx e ser possível escalar no futuro
    Texture2D menuBackground = LoadTexture("images/MenuBackground.png");
    Texture2D creditosBackground = LoadTexture("images/CreditosBackground.png");
    Texture2D restartBackground = LoadTexture("images/RestartBackground.png");
    Texture2D btnJogar = LoadTexture("images/btnJogar.png");
    Texture2D btnJogarPressed = LoadTexture("images/btnJogarPressed.png");
    Texture2D btnCreditos = LoadTexture("images/btnCreditos.png");
    Texture2D btnCreditosPressed = LoadTexture("images/btnCreditosPressed.png");
    Texture2D btnSair = LoadTexture("images/btnSair.png");
    Texture2D btnSairPressed = LoadTexture("images/btnSairPressed.png");
    Texture2D btnRejogar = LoadTexture("images/btnRejogar.png");
    Texture2D btnRejogarPressed = LoadTexture("images/btnRejogarPressed.png");
    Texture2D btnMenu = LoadTexture("images/btnMenu.png");
    Texture2D btnMenuPressed = LoadTexture("images/btnMenuPressed.png");
    

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
    
    
    //Posição dos Botões na Tela
    Rectangle positionBtnJogar = {120, 280, (float)btnJogar.width, (float)btnJogar.height};
    Rectangle positionBtnCreditos = {1100, 280, (float)btnCreditos.width, (float)btnCreditos.height};
    Rectangle positionBtnSair = {680, 280, (float)btnSair.width, (float)btnSair.height};
    Rectangle positionBtnRejogar = {380, 540, (float)btnRejogar.width, (float)btnRejogar.height};
    Rectangle positionBtnMenu = {500, 320, (float)btnMenu.width, (float)btnMenu.height};
    Vector2 mousePosition;
    
    //Corte dos sprites dos botões (sourceRec)
    Rectangle spriteBtnJogar = {0, 0, btnJogar.width, btnJogar.height/NUM_FRAMES};
    Rectangle spriteBtnCreditos = {0, 0, btnCreditos.width, btnCreditos.height/NUM_FRAMES};
    Rectangle spriteBtnSair = {0, 0, btnSair.width, btnSair.height/NUM_FRAMES};
    Rectangle spriteBtnRejogar = {0, 0, btnRejogar.width, btnRejogar.height/NUM_FRAMES};
    Rectangle spriteBtnMenu = {0,0, btnMenu.width, btnMenu.height/NUM_FRAMES };
    

    

    while (!exitWindow && !WindowShouldClose())
    {
        int buttonSprite = 0; //Decide qual sprite do botão vai desenhar 0-NORMAL, 1 -PRESSIONADO
        mousePosition = GetMousePosition();
        switch (currentScreen)
        {
            //Mecânica da tela do menu principal
            case MAINMENU:
            {
                if (CheckCollisionPointRec(mousePosition, positionBtnJogar))
                {
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = JOGAR_PRESSED;                 
                    else buttonSprite = 0;
                    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))currentScreen = INGAME; 
                    
                }
                
                if(CheckCollisionPointRec(mousePosition, positionBtnCreditos))
                {
                     
                     if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = CREDITOS_PRESSED;
                     else buttonSprite = 0;
                     if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))currentScreen = CREDITS; 
                    
                }
                
                if(CheckCollisionPointRec(mousePosition, positionBtnSair))
                {
                   if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = SAIR_PRESSED;
                   else buttonSprite = 0;
                   if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) exitWindow = true;
                   
                }
                
            } break;
            
            case CREDITS:
            {
                 if(IsKeyPressed(KEY_ESCAPE)) currentScreen = MAINMENU;  
 
            } break;
            
            //Mecânica do jogo
            case INGAME:
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
                        //Se player acertar obstáculo ==  GAME OVER
                        currentScreen = GAMEOVER;
                        player.x = SCREEN_WIDTH / 2 - PLAYER_SIZE / 2;
                        player.y = SCREEN_HEIGHT - PLAYER_SIZE;
                        break;
                    }
                }
            }   
            
            
            //Mecânica da tela de Game Over
            case GAMEOVER:
            {
                if(CheckCollisionPointRec(mousePosition, positionBtnMenu))
                {
                    
                   if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = MENU_PRESSED;
                   else buttonSprite = 0;
                   if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))  currentScreen = MAINMENU;
                    
                }
                
                if(CheckCollisionPointRec(mousePosition, positionBtnRejogar))
                {
                   if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = REJOGAR_PRESSED;
                   else buttonSprite = 0;
                   if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))  currentScreen = INGAME;
                    
                }

            } break;
        }
        
        // Desenhar
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            switch (currentScreen)
            {   
                //Gráficos da tela do menu principal
                case MAINMENU:
                {
                    DrawTextureEx(menuBackground, backgroundPosition, 0, 1, WHITE);                                       
                    DrawTextureRec(btnJogar, spriteBtnJogar, (Vector2){positionBtnJogar.x, positionBtnJogar.y}, WHITE);
                    DrawTextureRec(btnCreditos, spriteBtnCreditos, (Vector2){positionBtnCreditos.x, positionBtnCreditos.y}, WHITE);
                    DrawTextureRec(btnSair, spriteBtnSair, (Vector2){positionBtnSair.x, positionBtnSair.y}, WHITE);
                       
                    //Altera a textura se o botão for apertado
                    if(buttonSprite == JOGAR_PRESSED) DrawTextureRec(btnJogarPressed, spriteBtnJogar, (Vector2){positionBtnJogar.x, positionBtnJogar.y}, WHITE);
                    if(buttonSprite == CREDITOS_PRESSED) DrawTextureRec(btnCreditosPressed, spriteBtnCreditos, (Vector2){positionBtnCreditos.x, positionBtnCreditos.y}, WHITE);
                    if(buttonSprite == SAIR_PRESSED) DrawTextureRec(btnSairPressed, spriteBtnSair, (Vector2){positionBtnSair.x, positionBtnSair.y}, WHITE);
                                                 
                   
                        
                    
                } break;
                
                //Gráficos da tela de créditos
                case CREDITS:
                {
                    DrawTextureEx(creditosBackground, backgroundPosition, 0, 1, WHITE);                    

                    
                } break;
                
                //Gráficos do jogo
                case INGAME:
                {
                  
                    // Desenhar a imagem de fundo
                    DrawTexture(background, 0, 0, WHITE);

                    // Desenhar jogador redimensionado
                    Rectangle sourceRect = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };
                    Rectangle destRect = { player.x + (PLAYER_SIZE - IMAGE_SIZE) / 2, player.y + (PLAYER_SIZE - IMAGE_SIZE) / 2, IMAGE_SIZE, IMAGE_SIZE };
                    Vector2 origin = { 0.0f, 0.0f };
                    DrawTexturePro(playerTexture, sourceRect, destRect, origin, 0.0f, WHITE);

                    // Desenhar obstáculos
                    DrawObstacles(obstacles, obstacleCount);
    
                } break;
                
                //Gáficos da tela de GAME OVER
                case GAMEOVER:
                {
                    DrawTextureEx(restartBackground, backgroundPosition, 0, 1, WHITE);
                    DrawTextureRec(btnMenu, spriteBtnMenu, (Vector2){positionBtnMenu.x, positionBtnMenu.y}, WHITE);
                    DrawTextureRec(btnRejogar, spriteBtnRejogar, (Vector2){positionBtnRejogar.x, positionBtnRejogar.y}, WHITE);
                    
                    if(buttonSprite == MENU_PRESSED) DrawTextureRec(btnMenuPressed, spriteBtnMenu, (Vector2){positionBtnMenu.x, positionBtnMenu.y}, WHITE);
                    if(buttonSprite == REJOGAR_PRESSED) DrawTextureRec(btnRejogarPressed, spriteBtnRejogar, (Vector2){positionBtnRejogar.x, positionBtnRejogar.y}, WHITE);
                    
                } break;
                
            
            }
        EndDrawing();
    }

    // Descarregar texturas
    UnloadTexture(background);
    UnloadTexture(playerTexture);
    UnloadTexture(menuBackground);
    UnloadTexture(creditosBackground);
    UnloadTexture(restartBackground);
    UnloadTexture(btnJogar);
    UnloadTexture(btnJogarPressed);
    UnloadTexture(btnCreditos);
    UnloadTexture(btnCreditosPressed);
    UnloadTexture(btnSair);
    UnloadTexture(btnSairPressed);
    UnloadTexture(btnRejogar);
    UnloadTexture(btnRejogarPressed);
    UnloadTexture(btnMenu);
    UnloadTexture(btnMenuPressed);

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
