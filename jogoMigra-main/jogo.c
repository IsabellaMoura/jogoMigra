#include "raylib.h"

#define SCREEN_WIDTH 1600 //800
#define SCREEN_HEIGHT 900 //600

#define PLAYER_SIZE 50
#define IMAGE_SIZE 190
#define OBSTACLE_WIDTH 100
#define OBSTACLE_HEIGHT 50

#define NUM_FRAMES 1 //Numero de frames para a animação do botão

typedef enum GameScreen {MAINMENU = 0, CREDITS, INGAME, GAMEOVER, CONTROLS, SCORE} GameScreen;
typedef enum buttonSprite {NORMAL = 0, JOGAR_PRESSED, CREDITOS_PRESSED, MENU_PRESSED, REJOGAR_PRESSED, SAIR_PRESSED, CLOSE_PRESSED} buttonSprite;

typedef struct {
    Rectangle rect;
    Vector2 speed;
} Obstacle;

void InitObstacles(Obstacle obstacles[], int count);
void UpdateObstacles(Obstacle obstacles[], int count, float delta, float scrollSpeed);
void DrawObstacles(Obstacle obstacles[], int count, Texture2D textureLixo, Texture2D textureCanudo, Texture2D textureLata, Texture2D textureChinelo, Texture2D textureBola);

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
    Texture2D background = LoadTexture("images/areia2.png");
    Texture2D tituloMigra = LoadTexture("images/titulo.png");
    Texture2D creditosBackground = LoadTexture("images/CreditosBackground.png");
    Texture2D restartBackground = LoadTexture("images/RestartBackground.png");
    Texture2D controlsBackground = LoadTexture("images/controlsBackground.png");
    Texture2D scoreBackground = LoadTexture("images/scoreBackground.png");
    
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
    Texture2D btnCloseControls = LoadTexture("images/closeControls.png");
    Texture2D btnCloseControlsPressed = LoadTexture("images/closeControlsPressed.png");
    
    
    //Carrega a fonte
    Font beachdayFont = LoadFont("images/beachday.ttf");
    
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
    
    int currentAnimFramePlayer = 0;
    int frameDelay = 7;
    int frameCounter = 0;
    
    int currentAnimFrame = 0;
    int frameCounterLixo =0;
    int frameCounterCanudo = 0;
    int frameCounterLata = 0;
    int frameCounterBola = 0;
    int frameCounterChinelo = 0;
        
    int lixoFrames = 0;
    int canudoFrames = 0;
    int lataFrames = 0;
    int chineloFrames =0;
    int bolaFrames = 0;
        
    //Carregar Textura dos Obstaculos
    Image lixoImage = LoadImageAnim("images/sacoDeLixo.gif", &lixoFrames);
    Texture2D lixoTexture = LoadTextureFromImage(lixoImage);
    

    Image canudoImage = LoadImageAnim("images/canudo.gif", &canudoFrames);
    Texture2D canudoTexture = LoadTextureFromImage(canudoImage);
    
    Image lataImage = LoadImageAnim("images/lata.gif", &lataFrames);
    Texture2D lataTexture = LoadTextureFromImage(lataImage);
    
    Image chineloImage = LoadImageAnim("images/chinelo.gif", &chineloFrames);
    Texture2D chineloTexture = LoadTextureFromImage(chineloImage);
    
    Image bolaImage = LoadImageAnim("images/bola.gif", &bolaFrames);
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
    Rectangle positionBtnCloseControls = {1470, 10, (float)btnCloseControls.width, (float)btnCloseControls.height};
    Vector2 mousePosition;
    
    //Posiçãos dos bakcgrounds (somente precise rescalar com DrawTextureEx)
    Vector2 backgroundPosition = {0,0};
    
    //Corte dos sprites dos botões (sourceRec)
    Rectangle spriteBtnJogar = {0, 0, btnJogar.width, btnJogar.height/NUM_FRAMES};
    Rectangle spriteBtnCreditos = {0, 0, btnCreditos.width, btnCreditos.height/NUM_FRAMES};
    Rectangle spriteBtnSair = {0, 0, btnSair.width, btnSair.height/NUM_FRAMES};
    Rectangle spriteBtnRejogar = {0, 0, btnRejogar.width, btnRejogar.height/NUM_FRAMES};
    Rectangle spriteBtnMenu = {0,0, btnMenu.width, btnMenu.height/NUM_FRAMES };
    Rectangle spriteBtnCloseControls = {0,0, btnCloseControls.width, btnCloseControls.height/NUM_FRAMES};
    
    //highscore
    float distance = 0.0f;
    float highdistance = 0.0f;
    bool newHighscore = false;
    float backgroundOffset = 0.0f;

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
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        buttonSprite = JOGAR_PRESSED;   
                        //Soluçaõ temporária                         
                        for(int i = 0; i < obstacleCount; i++){
                            obstacles[i].rect.x = 0;
                            obstacles[i].rect.y = GetRandomValue(0, SCREEN_HEIGHT / 2);
                        }
                    }
                    else buttonSprite = 0;
                    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))currentScreen = CONTROLS; 
                    
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
                    currentAnimFramePlayer++;
                    if(currentAnimFramePlayer >= playerFrames) currentAnimFramePlayer = 0;
                    
                    //Pega a posição do offset da memoria para o proximo frame em image.data
                    nextFrameDataOffset = playerTexture.width * playerTexture.height * 4 * currentAnimFramePlayer;
                    
                    
                    //Atualiza a data da textura na GPU com o proximo frame em image data                    
                    UpdateTexture(playerTexture, ((unsigned char *) playerImage.data) + nextFrameDataOffset);
                    
                    frameCounter = 0;
                                      
                }
                               
                // Movimento do jogador
                if (IsKeyDown(KEY_RIGHT) && player.x + player.width < SCREEN_WIDTH) player.x += playerSpeed.x * deltaTime;
                if (IsKeyDown(KEY_LEFT) && player.x > 0) player.x -= playerSpeed.x * deltaTime;
                if (IsKeyDown(KEY_DOWN) && player.y + player.height < SCREEN_HEIGHT) player.y = playerSpeed.y * deltaTime;

                // Movimento do jogador
                Vector2 movement = { 0.0f, 0.0f };
                if (IsKeyDown(KEY_RIGHT)) movement.x += playerSpeed.x * deltaTime;
                if (IsKeyDown(KEY_LEFT)) movement.x -= playerSpeed.x * deltaTime;
                if (IsKeyDown(KEY_UP)){
                    movement.y += playerSpeed.y * deltaTime; // Invertido para descer 
                    distance += 0.2f; //Controla a pontuação, toda vez que o jogador andar pra cima, ela aumenta
                }                                   

                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)){
                    UpdateMusicStream(sfxMovimento);
                    timePlayed = GetMusicTimePlayed(sfxMovimento)/GetMusicTimeLength(sfxMovimento);
                    if(timePlayed > 1.0f) timePlayed = 0.0f;             
                }else timePlayed = 0.0f;             
                
                

                // Atualizar deslocamento do fundo e obstáculos
                backgroundOffset += movement.y;
                if (backgroundOffset >= SCREEN_HEIGHT) backgroundOffset = 0;

                               
                
                // Atualizar obstáculos                
                UpdateObstacles(obstacles, obstacleCount, deltaTime, movement.y);             

                // Colisão
                for (int i = 0; i < obstacleCount; i++)
                {
                    if (CheckCollisionRecs(player, obstacles[i].rect))
                    {
                        // Reiniciar o jogo (pode ser mais elaborado)
                        //Se player acertar obstáculo ==  GAME OVER
                        currentScreen = SCORE;
                        PlaySound(sfxMorte);                        
                        if(distance > highdistance){
                            ///Define o higscore
                            highdistance = distance;
                            newHighscore = true;
                            
                        }
                        break;
                    }
                }
            }   
            
            
            //Mecânica da tela de Game Over
            case GAMEOVER:
            {
                if(CheckCollisionPointRec(mousePosition, positionBtnMenu))
                {
                    
                   if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                       buttonSprite = MENU_PRESSED;
                       distance = 0.0f;
                       newHighscore = false;                                              
                   }
                   else buttonSprite = 0;
                   if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))  currentScreen = MAINMENU;
                    
                }
                
                if(CheckCollisionPointRec(mousePosition, positionBtnRejogar))
                {
                   if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        buttonSprite = REJOGAR_PRESSED;
                        distance = 0.0f;
                        newHighscore = false;
                        //Reseta os obstáculos se jogador decidir rejogar
                        for(int i = 0; i <obstacleCount; i++){
                            obstacles[i].rect.x = 0;
                            obstacles[i].rect.y = GetRandomValue(0, SCREEN_HEIGHT / 2);
                        }
                   }
                   else buttonSprite = 0;
                   if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))  currentScreen = INGAME;
                    
                }

            } break;
            
            case CONTROLS:
            {
                if(CheckCollisionPointRec(mousePosition, positionBtnCloseControls))
                {
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) buttonSprite = CLOSE_PRESSED;
                    else buttonSprite = 0;
                    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) currentScreen = INGAME;
                }
                
            } break;
            
            case SCORE:
            {
                if(IsKeyPressed(KEY_ENTER)) currentScreen = GAMEOVER;
                
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
                    //DrawTextEx(beachdayFont, "Pressione ESC para voltar...", (Vector2){10, 850}, 36.0f, 2, ORANGE);  

                    
                } break;
                
                //Gráficos do jogo
                case INGAME:
                {
                    
                     // Desenhar a imagem de fundo
                    DrawTexture(background, 0, backgroundOffset, WHITE);
                    DrawTexture(background, 0, backgroundOffset - SCREEN_HEIGHT, WHITE);
                    
                    //Desenhar o score, o 04 é formatação quer dizer que vai mostrar numeros com até 4 digitos(ex: 2500)
                    DrawTextEx(beachdayFont, TextFormat("SCORE: %04i", (int)distance), (Vector2){SCREEN_WIDTH/3, 0}, 46.0f, 2, ORANGE);
                        
                    // Desenhar jogador redimensionado
                    Rectangle sourceRect = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };                                    
                    Rectangle destRect = { player.x + (PLAYER_SIZE - IMAGE_SIZE) / 2, player.y + (PLAYER_SIZE - IMAGE_SIZE) / 2, IMAGE_SIZE, IMAGE_SIZE };
                    Vector2 origin = { 0.0f, 0.0f };
                    
                    // Colisão do player, bom pra debugar
                    //DrawRectangleLinesEx(player, 1, YELLOW); 
                    DrawTexturePro(playerTexture, sourceRect, destRect, origin, 0, WHITE);
                    
                    
                    //Animação do lixo                
                    frameCounterLixo++;
                    if(frameCounterLixo >= frameDelay)
                    {                                   
                        currentAnimFrame++;
                        if(currentAnimFrame >= lixoFrames) currentAnimFrame = 0;
                                    
                        //Pega a posição do offset da memoria para o proximo frame em image.data
                        nextFrameDataOffset = lixoTexture.width * lixoTexture.height * 4 * currentAnimFrame;
                                    
                                    
                        //Atualiza a data da textura na GPU com o proximo frame em image data                    
                        UpdateTexture(lixoTexture, ((unsigned char *) lixoImage.data) + nextFrameDataOffset);
                        frameCounterLixo = 0;
                    }
                    
                    //Animação da lata                
                    frameCounterLata++;
                    if(frameCounterLata >= frameDelay)
                    {                                   
                        currentAnimFrame++;
                        if(currentAnimFrame >= lataFrames) currentAnimFrame = 0;
                                    
                        //Pega a posição do offset da memoria para o proximo frame em image.data
                       nextFrameDataOffset = lataTexture.width * lataTexture.height * 4 * currentAnimFrame;
                                    
                                    
                        //Atualiza a data da textura na GPU com o proximo frame em image data                    
                        UpdateTexture(lataTexture, ((unsigned char *) lataImage.data) + nextFrameDataOffset);
                        frameCounterLata = 0;
                    }
                    
                    //Animação do Canudo                
                    frameCounterCanudo++;
                    if(frameCounterCanudo >= frameDelay)
                    {                                   
                        currentAnimFrame++;
                        if(currentAnimFrame >= canudoFrames) currentAnimFrame = 0;
                                    
                        //Pega a posição do offset da memoria para o proximo frame em image.data
                        nextFrameDataOffset = canudoTexture.width * canudoTexture.height * 4 * currentAnimFrame;
                                    
                                    
                        //Atualiza a data da textura na GPU com o proximo frame em image data                    
                        UpdateTexture(canudoTexture, ((unsigned char *) canudoImage.data) + nextFrameDataOffset);
                        frameCounterCanudo = 0;
                    }
                    
                    //Animação do Chinelo                
                    frameCounterChinelo++;
                    if(frameCounterChinelo >= frameDelay)
                    {                                   
                        currentAnimFrame++;
                        if(currentAnimFrame >= chineloFrames) currentAnimFrame = 0;
                                    
                        //Pega a posição do offset da memoria para o proximo frame em image.data
                        nextFrameDataOffset = chineloTexture.width * chineloTexture.height * 4 * currentAnimFrame;
                                    
                                    
                        //Atualiza a data da textura na GPU com o proximo frame em image data                    
                        UpdateTexture(chineloTexture, ((unsigned char *) chineloImage.data) + nextFrameDataOffset);
                        frameCounterChinelo = 0;
                    }
                    
                    //Animação da Bola                
                    frameCounterBola++;
                    if(frameCounterBola >= frameDelay)
                    {                                   
                        currentAnimFrame++;
                        if(currentAnimFrame >= bolaFrames) currentAnimFrame = 0;
                                    
                        //Pega a posição do offset da memoria para o proximo frame em image.data
                        nextFrameDataOffset = bolaTexture.width * bolaTexture.height * 4 * currentAnimFrame;
                                    
                                    
                        //Atualiza a data da textura na GPU com o proximo frame em image data                    
                        UpdateTexture(bolaTexture, ((unsigned char *) bolaImage.data) + nextFrameDataOffset);
                        frameCounterBola = 0;
                    }
                        

                    // Desenhar obstáculos                                        
                    DrawObstacles(obstacles, obstacleCount, lixoTexture, canudoTexture, lataTexture, chineloTexture, bolaTexture);
                    
                                        
                    
                    
                } break;
                
                //Gráficos da tela de GAME OVER
                case GAMEOVER:
                {
                    DrawTexture(restartBackground, 0, 0, WHITE);
                    DrawTextureRec(btnMenu, spriteBtnMenu, (Vector2){positionBtnMenu.x, positionBtnMenu.y}, WHITE);
                    DrawTextureRec(btnRejogar, spriteBtnRejogar, (Vector2){positionBtnRejogar.x, positionBtnRejogar.y}, WHITE);                    
                        
                    if(buttonSprite == MENU_PRESSED) DrawTextureRec(btnMenuPressed, spriteBtnMenu, (Vector2){positionBtnMenu.x, positionBtnMenu.y}, WHITE);
                    if(buttonSprite == REJOGAR_PRESSED) DrawTextureRec(btnRejogarPressed, spriteBtnRejogar, (Vector2){positionBtnRejogar.x, positionBtnRejogar.y}, WHITE);
                    
                } break;
                
                case CONTROLS:
                {
                    DrawTexture(controlsBackground, 0, 0, WHITE);
                    DrawTextureRec(btnCloseControls, spriteBtnCloseControls, (Vector2){positionBtnCloseControls.x, positionBtnCloseControls.y}, WHITE);
                    
                    if(buttonSprite == CLOSE_PRESSED) DrawTextureRec(btnCloseControlsPressed, spriteBtnCloseControls, (Vector2){positionBtnCloseControls.x, positionBtnCloseControls.y}, WHITE);
                    
                } break;
                
                case SCORE:
                {
                    DrawTexture(scoreBackground, 0, 0, WHITE);
                    if(newHighscore == true) DrawTextEx(beachdayFont, "NEW HIGHSCORE!", (Vector2){400, 100}, 80.0f, 2, RED);  
                    DrawTextEx(beachdayFont, TextFormat("HIGHSCORE: %04i", (int)highdistance), (Vector2){400, 200}, 80.0f, 2, ORANGE);                                                 
                    DrawTextEx(beachdayFont, TextFormat("SCORE: %04i", (int)distance), (Vector2){554, 300}, 80.0f, 2, ORANGE);
                    
                    DrawTextEx(beachdayFont, "Pressione ENTER para continuar...", (Vector2){10, 850}, 46.0f, 2, ORANGE);                                                                         
                    
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
    UnloadTexture(controlsBackground);
    UnloadTexture(btnCloseControls);
    UnloadTexture(btnCloseControlsPressed);
    UnloadFont(beachdayFont);
    UnloadTexture(scoreBackground);
    
    //Fecha o device de audio
    CloseAudioDevice();
    
    // Fechar a janela
    CloseWindow();

    return 0;
}

void UpdateObstacles(Obstacle obstacles[], int count, float delta, float scrollSpeed)
{
    for (int i = 0; i < count; i++)
    {
        obstacles[i].rect.y += scrollSpeed;
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
        
        //DrawRectangleLinesEx(obstacles[i].rect, 1, RED); //bom pra debug, da pra ver o rect a colisão kkk       
        
        //Corta o sprite na textura
        Rectangle lixoRect = {0,0, textureLixo.width, textureLixo.height/NUM_FRAMES};
        Rectangle canudoRect = {0,0, textureCanudo.width, textureCanudo.height/NUM_FRAMES};
        Rectangle lataRect = {0,0, textureLata.width, textureLata.height/NUM_FRAMES};
        Rectangle chineloRect = {0,0, textureChinelo.width, textureChinelo.height/NUM_FRAMES};
        Rectangle bolaRect = {0,0, textureBola.width, textureBola.height/NUM_FRAMES};
        
        float scale = 4.0f;
        
        
        //Desenha os objetos, dentro dos rects de colisão
        if(i == 0 || i == 1) DrawTexturePro(textureLixo, lixoRect, (Rectangle){obstacles[i].rect.x - 85.0f, obstacles[i].rect.y - 100.0f, obstacles[i].rect.width * scale, obstacles[i].rect.height * scale}, (Vector2){ 0.0f, 0.0f }, 0, WHITE);        
        //Descer/Direita diminua o offset . Subir/Esquerda aumente o offset
        if(i == 2 || i == 3) DrawTexturePro(textureCanudo, canudoRect, (Rectangle){obstacles[i].rect.x - 60.0f, obstacles[i].rect.y - 50.0f, obstacles[i].rect.width * 2.5f, obstacles[i].rect.height * 2.5f}, (Vector2){ 0.0f, 0.0f }, 0, WHITE); 
        if(i == 4 || i == 5) DrawTexturePro(textureLata, lataRect, (Rectangle){obstacles[i].rect.x - 150.0f, obstacles[i].rect.y - 150.0f, obstacles[i].rect.width * 6.0f, obstacles[i].rect.height * 6.0f}, (Vector2){ 0.0f, 0.0f }, 0, WHITE); 
        if(i == 6 || i == 7) DrawTexturePro(textureChinelo, chineloRect, (Rectangle){obstacles[i].rect.x - 100.0f, obstacles[i].rect.y - 100.0f, obstacles[i].rect.width * 4.2f, obstacles[i].rect.height * 4.2f}, (Vector2){ 0.0f, 0.0f }, 0, WHITE);
        if(i == 8 || i == 9) DrawTexturePro(textureBola, bolaRect, (Rectangle){obstacles[i].rect.x - 130.0f, obstacles[i].rect.y - 140.0f, obstacles[i].rect.width * 5.6f, obstacles[i].rect.height * 5.6f}, (Vector2){ 0.0f, 0.0f }, 0, WHITE);                          
    }
    
}