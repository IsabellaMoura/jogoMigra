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
typedef enum Objects {LIXO = 0, CANUDO, LATA} Objects;

typedef struct {
    Rectangle rect;
    Vector2 speed;
} Obstacle;

void InitObstacles(Obstacle obstacles[], int count);
void UpdateObstacles(Obstacle obstacles[], int count, float delta);
void DrawObstacles(Obstacle obstacles[], int count, Texture2D textureLixo, Texture2D textureCanudo, Texture2D textureLata, Texture2D textureChinelo, Texture2D textureBola);
void LoadObjectAnimation(int objectFrames, int frameDelay, int textureWidth, int textureHeight, Texture2D ObjectTexture, void *ObjectImageData);

int main(void)
{
    // Inicializar a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Migra");
    
    //Inicializa o audio
    InitAudioDevice(); 
    
    //Disabilita o ESC para sair
    SetExitKey(KEY_NULL);
    
    //Booleano de fechar a tela
    bool exitWindow = false;
    
    //Inicializa o Menu
    GameScreen currentScreen = MAINMENU;
    
    // Carregar a imagem de fundo
    Texture2D background = LoadTexture("images/areia.png");
    Texture2D tituloMigra = LoadTexture("images/titulo.png");
    Texture2D creditosBackground = LoadTexture("images/CreditosBackground.png");
    Texture2D restartBackground = LoadTexture("images/RestartBackground.png");
    //Carregar as imagens dos botões
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
    
    //Carregar animação de fundo
    int animFrames = 0;    
    Image menuAnimado  = LoadImageAnim("images/MENU.gif", &animFrames);//Carregar todos os frames do GIF em uma só imagem
    Texture2D texMenuAnimado = LoadTextureFromImage(menuAnimado);    //Carrega a textura da imagem

    
    // Carregar a imagem do jogador
    int playerFrames = 0;
    Image playerImage = LoadImageAnim("images/Purpurinha.gif", &playerFrames);
    Texture2D playerTexture = LoadTextureFromImage(playerImage);
    Rectangle player = { (SCREEN_WIDTH / 2 - PLAYER_SIZE /2 ) , (SCREEN_HEIGHT - PLAYER_SIZE), PLAYER_SIZE, PLAYER_SIZE }; //Rect Colisor do Jogador (775X, 850Y, 40, 40)
    Vector2 playerSpeed = { 200.0f, 200.0f };
    
    //Config dos frames das animações
    
    unsigned int nextFrameDataOffset = 0;
    int currentAnimFrame = 0;
    int frameDelay = 4;
    int frameCounter = 0;
        
    //Carregar Textura dos Obstaculos
    int lixoFrames = 0;
    Image lixoImage = LoadImageAnim("images/sacoDeLixo.gif", &lixoFrames);
    Texture2D lixoTexture = LoadTextureFromImage(lixoImage);
    

    Image canudoImage = LoadImageAnim("images/canudo.gif", &lixoFrames);
    Texture2D canudoTexture = LoadTextureFromImage(canudoImage);
    
    Image lataImage = LoadImageAnim("images/lata.gif", &lixoFrames);
    Texture2D lataTexture = LoadTextureFromImage(lataImage);
    
    Image chineloImage = LoadImageAnim("images/chinelo.gif", &lixoFrames);
    Texture2D chineloTexture = LoadTextureFromImage(chineloImage);
    
    Image bolaImage = LoadImageAnim("images/bola.gif", &lixoFrames);
    Texture2D bolaTexture = LoadTextureFromImage(bolaImage);
    
    //Carrega as músicas
    Music musicaMenu = LoadMusicStream("sounds/musicaMenu.mp3");
    PlayMusicStream(musicaMenu);
    
    Music gameplayAmbience = LoadMusicStream("sounds/gameplayAmbience.mp3");
    PlayMusicStream(gameplayAmbience);
    
    Music sfxMovimento = LoadMusicStream("sounds/sfxMovimento.mp3");
    PlayMusicStream(sfxMovimento);
    
    float timePlayed = 0.0f; // Tempo tocado normalizado
    
    //Carrega os efeitos sonoros
    Sound sfxMorte = LoadSound("sounds/sfxMorte.mp3");
    
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
    
    //Posiçãos dos bakcgrounds (somente precise rescalar com DrawTextureEx)
    Vector2 backgroundPosition = {0,0};
    
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
                UpdateMusicStream(musicaMenu);
                //Pega o tempo tocado normalizado da musica atual
                timePlayed = GetMusicTimePlayed(musicaMenu)/GetMusicTimeLength(musicaMenu);
                if(timePlayed > 1.0f) timePlayed = 1.0f;
                
                //Animação do Menu
                frameCounter++;
                if(frameCounter >= 3)
                {
                    //Vai para o proximo frame
                    //se chegar ao ultimo frmae, volta para o primeiro
                    currentAnimFrame++;
                    if(currentAnimFrame >= animFrames) currentAnimFrame = 0;
                    
                    //Pega a posição do offset da memoria para o proximo frame em image.data
                    nextFrameDataOffset = menuAnimado.width * menuAnimado.height * 4 * currentAnimFrame;
                    
                    
                    //Atualiza a data da textura na GPU com o proximo frame em image data                    
                    UpdateTexture(texMenuAnimado, ((unsigned char *) menuAnimado.data) + nextFrameDataOffset);
                    
                    frameCounter = 0;
                    
                }
                
                //Mecânica dos Botões
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
                int frameDelayPlayer = 3;
                
                //Musica
                UpdateMusicStream(gameplayAmbience);                
                timePlayed = GetMusicTimePlayed(gameplayAmbience)/GetMusicTimeLength(gameplayAmbience);
                if(timePlayed > 1.0f) timePlayed = 1.0f;
                
                frameCounter++;
                if(frameCounter >= frameDelayPlayer && (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)))
                {
                    //Vai para o proximo frame
                    //se chegar ao ultimo frame, volta para o primeiro
                    currentAnimFrame++;
                    if(currentAnimFrame >= playerFrames) currentAnimFrame = 0;
                    
                    //Pega a posição do offset da memoria para o proximo frame em image.data
                    nextFrameDataOffset = playerTexture.width * playerTexture.height * 4 * currentAnimFrame;
                    
                    
                    //Atualiza a data da textura na GPU com o proximo frame em image data                    
                    UpdateTexture(playerTexture, ((unsigned char *) playerImage.data) + nextFrameDataOffset);
                    
                    frameCounter = 0;
                                      
                }
                               
                // Movimento do jogador
                if (IsKeyDown(KEY_RIGHT) && player.x + player.width < SCREEN_WIDTH) player.x += playerSpeed.x * deltaTime;                
                if (IsKeyDown(KEY_LEFT) && player.x > 0) player.x -= playerSpeed.x * deltaTime;                    
                if (IsKeyDown(KEY_UP) && player.y > 0) player.y -= playerSpeed.y * deltaTime;                                    
                if (IsKeyDown(KEY_DOWN) && player.y + player.height < SCREEN_HEIGHT) player.y += playerSpeed.y * deltaTime;
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)){
                    UpdateMusicStream(sfxMovimento);
                    timePlayed = GetMusicTimePlayed(sfxMovimento)/GetMusicTimeLength(sfxMovimento);
                    if(timePlayed > 1.0f) timePlayed = 0.0f;             
                }                  
                
                //Frames do lixo - esse anima o lixo bonitinho
                
                frameCounter++;
                if(frameCounter >= frameDelay)
                {                                   
                    currentAnimFrame++;
                    if(currentAnimFrame >= lixoFrames) currentAnimFrame = 0;
                                    
                    //Pega a posição do offset da memoria para o proximo frame em image.data
                    nextFrameDataOffset = lixoTexture.width * lixoTexture.height * 4 * currentAnimFrame;
                                    
                                    
                    //Atualiza a data da textura na GPU com o proximo frame em image data                    
                    UpdateTexture(lixoTexture, ((unsigned char *) lixoImage.data) + nextFrameDataOffset);
                    frameCounter = 0;
                }
                
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
                        PlaySound(sfxMorte);
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
                    DrawTextureEx(texMenuAnimado, backgroundPosition, 0, 0.834f, WHITE);
                    DrawTexture(tituloMigra, SCREEN_WIDTH/3, 0, WHITE);
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
                    DrawTexture(creditosBackground, 0, 0, WHITE);                    

                    
                } break;
                
                //Gráficos do jogo
                case INGAME:
                {
                  
                    // Desenhar a imagem de fundo
                    DrawTexture(background, 0, 0, WHITE);
                    
                        
                    // Desenhar jogador redimensionado
                    Rectangle sourceRect = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };                
                    Vector2 origin = { 0.0f, 0.0f };
                    
                    DrawRectangleLinesEx(player, 1, YELLOW); // Colisão do player, bom pra debugar
                    DrawTexturePro(playerTexture, sourceRect,(Rectangle){player.x - 155.0f, player.y + 25.0f, player.width*5.0f, player.height*5.0f}, origin, -45.0f, WHITE);
                    

                    // Desenhar obstáculos                    
                    LoadObjectAnimation(lixoFrames, 2, lixoTexture.width, lixoTexture.height, lixoTexture, lixoImage.data); //Não funciona
                    DrawObstacles(obstacles, obstacleCount, lixoTexture, canudoTexture, lataTexture, chineloTexture, bolaTexture);
                    
                                        
                    
                    
                } break;
                
                //Gáficos da tela de GAME OVER
                case GAMEOVER:
                {
                    DrawTexture(restartBackground, 0, 0, WHITE);
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
    UnloadImage(playerImage);
    UnloadTexture(creditosBackground);
    UnloadTexture(restartBackground);
    UnloadTexture(tituloMigra);
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
    UnloadImage(menuAnimado);
    UnloadTexture(texMenuAnimado);
    UnloadImage(lixoImage);
    UnloadTexture(lixoTexture);
    UnloadImage(canudoImage);
    UnloadTexture(canudoTexture);
    UnloadImage(lataImage);
    UnloadTexture(lataTexture);
    UnloadImage(chineloImage);
    UnloadTexture(chineloTexture);
    UnloadImage(bolaImage);
    UnloadTexture(bolaTexture);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(gameplayAmbience);
    UnloadSound(sfxMorte);
    UnloadMusicStream(sfxMovimento);
    
    //Fecha o device de audio
    CloseAudioDevice();
    
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
        obstacles[i].speed.x = GetRandomValue(100, 300);
        obstacles[i].speed.y = 0;
        obstacles[i].rect.width = 60; //Altera o tamanho dos rects de colisão
        obstacles[i].rect.height = 60; 
                    
        
    }
}


void DrawObstacles(Obstacle obstacles[], int count, Texture2D textureLixo, Texture2D textureCanudo, Texture2D textureLata, Texture2D textureChinelo, Texture2D textureBola)
{
    for (int i = 0; i < count; i++)
    {
        DrawRectangleLinesEx(obstacles[i].rect, 1, RED); //bom pra debug, da pra ver o rect a colisão kkk
        DrawText("0", obstacles[0].rect.x-5, obstacles[0].rect.y-5, 10, BLUE);
        DrawText("1", obstacles[1].rect.x-5, obstacles[1].rect.y-5, 10, BLUE);
        DrawText("2", obstacles[2].rect.x-5, obstacles[2].rect.y-5, 10, BLUE);
        DrawText("3", obstacles[3].rect.x-5, obstacles[3].rect.y-5, 10, BLUE);
        
        
        //Corta o sprite na textura
        Rectangle lixoRect = {0,0, textureLixo.width, textureLixo.height/NUM_FRAMES};
        Rectangle canudoRect = {0,0, textureCanudo.width, textureCanudo.height/NUM_FRAMES};
        Rectangle lataRect = {0,0, textureLata.width, textureLata.height/NUM_FRAMES};
        Rectangle chineloRect = {0,0, textureChinelo.width, textureChinelo.height/NUM_FRAMES};
        Rectangle bolaRect = {0,0, textureBola.width, textureBola.height/NUM_FRAMES};
        
        float scale = 3.5f; 
        float offset = 75.0f; 
        
        //Desenha os objetos, dentro dos rects de colisão
        if(i == 0 || i == 1) DrawTexturePro(textureLixo, lixoRect, (Rectangle){obstacles[i].rect.x - offset, obstacles[i].rect.y - offset, obstacles[i].rect.width * scale, obstacles[i].rect.height * scale}, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);        
        
        if(i == 2 || i == 3) DrawTexturePro(textureCanudo, canudoRect, (Rectangle){obstacles[i].rect.x - 70.0f, obstacles[i].rect.y - 100.0f, obstacles[i].rect.width * scale, obstacles[i].rect.height * scale}, (Vector2){ 0.0f, 0.0f }, 0, WHITE); 
        if(i == 4 || i == 5) DrawTexturePro(textureLata, lataRect, (Rectangle){obstacles[i].rect.x - offset, obstacles[i].rect.y - offset, obstacles[i].rect.width * scale, obstacles[i].rect.height * scale}, (Vector2){ 0.0f, 0.0f }, 0, WHITE); 
        if(i == 6 || i == 7) DrawTexturePro(textureChinelo, chineloRect, (Rectangle){obstacles[i].rect.x - 70.0f, obstacles[i].rect.y - 65.0f, obstacles[i].rect.width * scale, obstacles[i].rect.height * scale}, (Vector2){ 0.0f, 0.0f }, 0, WHITE);
        if(i == 8 || i == 9) DrawTexturePro(textureBola, bolaRect, (Rectangle){obstacles[i].rect.x - 150.0f, obstacles[i].rect.y - 190.0f, obstacles[i].rect.width * 6.0f, obstacles[i].rect.height * 6.0f}, (Vector2){ 0.0f, 0.0f }, 0, WHITE);                                 
    }
    
}

//Tentativa de encurtar o processo
void LoadObjectAnimation(int objectFrames, int frameDelay, int textureWidth, int textureHeight, Texture2D ObjectTexture, void *ObjectImageData)
{
    unsigned int nextFrameDataOffset = 0;
    int frameCounter = 0;
    int currentAnimFrame = 0;
    
    frameCounter++;
    if(frameCounter >= frameDelay)
    {
        
        //Vai para o proximo frame
        //se chegar ao ultimo frame, volta para o primeiro
        currentAnimFrame++;
        if(currentAnimFrame >= objectFrames) currentAnimFrame = 0;
                        
        //Pega a posição do offset da memoria para o proximo frame em image.data
        nextFrameDataOffset = textureWidth * textureHeight * 4 * currentAnimFrame;
                        
                        
        //Atualiza a data da textura na GPU com o proximo frame em image data                    
        UpdateTexture(ObjectTexture, ((unsigned char *) ObjectImageData) + nextFrameDataOffset);
        frameCounter = 0;
    }
    
    
}
