#include "SpaceGame_II.h"

std::vector<const Ship*> Ships;
bool isGameOver_Fail = false;
bool isGameOVer_Success = false;

void CheckTouchDown(std::vector<const Ship*>& Ships)
{
    for (size_t i = 0; i < Ships.size(); i++)
    {
        if (Ships[i]->position.y > 655.0 && Ships[i]->v.y < 0.0f)
        {
            if (Ships[i]->v.y > Ships[i]->RequiredLandingSpeed)
            {
                isGameOVer_Success = true;
                return;
            }

            isGameOver_Fail = true;
        }
    }
}

void AsteroidCollision(Rectangle asteroid, std::vector<const Ship*>& Ships)
{
    bool isCollision = false;

    for (const Ship* ship : Ships)
    {
        isCollision = CheckCollisionRecs(asteroid, MovementController::GetShipRectangle(ship));

        if (isCollision == true)
        {
            isGameOver_Fail = true;
        }
    }
}

void SuccessMessage()
{
    BeginDrawing();
    ClearBackground(BLACK);
    std::string message = "Mission Success!";
    DrawText(message.c_str(), 50, 400, 100, GREEN);
    EndDrawing();
}

void FailMessage()
{
    BeginDrawing();
    ClearBackground(BLACK);
    std::string message = "Mission Failed!";
    DrawText(message.c_str(), 50, 400, 100, RED);
    EndDrawing();
}

void DisplayGameOver()
{
    if (isGameOVer_Success)
        SuccessMessage();

    if (isGameOver_Fail)
        FailMessage();
}

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 800;

    float scale = 2.0f;   

    char Title[]{ "SpaceHopper" };

    // Initialize the window
    InitWindow(windowWidth, windowHeight, Title);
    SetTargetFPS(60);

    GameMenu startMenu;

    // Space Background
    Texture2D spaceBackground = LoadTexture("backgroundImage.png");
    //// spaceBackgrounds original size
    Rectangle sourceSpaceBGRec = { 0.0f, 0.0f, (float)spaceBackground.width, (float)spaceBackground.height };
    //// spaceBackrgounds updated size
    Rectangle destinationSpaceBGRec = { 0.0f, 0.0f, (float)windowWidth * scale, (float)windowHeight * scale };

    Vector2 backgroundOrigin = { 0.0f, 0.0f };

    // Load the spaceship texture
    Ship shipData1;
    shipData1.Init("Ship1");
    Ships.push_back(&shipData1);
    shipData1.texture = LoadTexture("ship.png");
    shipData1.animatedTexture = LoadTexture("engineSpriteSheet.png");
        
    float scaledHeight = shipData1.texture.height * scale;

    Ship shipData2;
    shipData2.Init("Ship2");
    Ships.push_back(&shipData2);    
    shipData2.texture = LoadTexture("ship2.png");
    shipData2.animatedTexture = LoadTexture("engineSpriteSheet2.png");

    // Object Locations
    shipData1.position.x = windowWidth / 2 - scaledHeight / 2; // Center the ship
    int floorPositionY = windowHeight - 50;
    shipData1.position.y = 10.0f;

    shipData2.position.x = windowWidth / 2 - scaledHeight / 2; // Center the ship
    shipData2.position.x += 20.0f;
    shipData2.position.y = 10.0f;

    // Use to check the ship has a safe landing
    const float StarterPosition = shipData1.position.y;

    /*const float moveSpeed = 100.0f;*/
    float deltaTime = 0.06f; // 1 frame every 0.06ms, reflecting 60fps

    int thrustAnimationFrameIndex = 0;

    // Asteroid Texture

    Texture2D asteroids = LoadTexture("animated_asteroid.png");

    Asteroid asteroid({ 100, -100 }, asteroids, 80.0f, 0.5f);

    MovementController movementController(StarterPosition);    

    while (!WindowShouldClose())
    {
        startMenu.Update();

        if (startMenu.IsGameStartSelected() && IsKeyPressed(KEY_ENTER))
        {

            break;
        }
        else if (startMenu.IsGameQuitSelected() && IsKeyPressed(KEY_ENTER))
        {

            CloseWindow();
            break;
        }

        BeginDrawing();
        startMenu.Draw();
        EndDrawing();
    }
    // Main game loop
    while (!WindowShouldClose())
    {
        if (!isGameOver_Fail && !isGameOVer_Success)
        {
            BeginDrawing();
            ClearBackground(BLACK);

             // Update and Draw Asteroid
             asteroid.Update(deltaTime);

             // Reset asteroid position if it goes off-screen
             if (asteroid.IsOffScreen(windowHeight)) {
                 int textureWidth = asteroid.texture.width / 16;
                 asteroid.position.x = GetRandomValue(0, windowWidth - textureWidth);
                 asteroid.position.y = -asteroid.texture.height;
             }            

            // // ship and drawing logic here
            // movementController.UpdatePosition_1(shipData1, shipData1.position.y, deltaTime);  //** UNCOMMENT THIS TO BE ABLE TO PLAY WITH THE OTHER SHIP,
            movementController.AutoLand_Vertical(shipData1, deltaTime);
            ApplyGravity(shipData1, deltaTime);
            movementController.UpdatePosition_2(shipData2, shipData2.position.y, deltaTime);
            ApplyGravity(shipData2, deltaTime);

            movementController.UpdatePosition_Side_1(shipData1, deltaTime);
            ApplySideBoosters(shipData1, deltaTime);
            movementController.UpdatePosition_Side_2(shipData2, deltaTime);
            ApplySideBoosters(shipData2, deltaTime);

            Rectangle shipRect1 = MovementController::GetShipRectangle(Ships[0]);
            Rectangle shipRect2 = MovementController::GetShipRectangle(Ships[1]);
            Rectangle asteroidRect = asteroid.GetAsteroidRectangle();

            DrawTexturePro(spaceBackground, sourceSpaceBGRec, destinationSpaceBGRec, backgroundOrigin, 0.0f, WHITE);
            DrawTextureEx(shipData1.texture, shipData1.position, 0.0f, scale, WHITE);
            DrawTextureEx(shipData2.texture, shipData2.position, 0.0f, scale, WHITE);

            thrustAnimationFrameIndex = (thrustAnimationFrameIndex + 1) % 8; // Update this each frame to cycle through images

            Rectangle sourceRec = {
                (float)(thrustAnimationFrameIndex * (shipData1.animatedTexture.width / 8)),
                0.0f,
                (float)(shipData1.animatedTexture.width / 8),
                (float)shipData1.animatedTexture.height };

            Rectangle destRec1 = {
                shipData1.position.x,
                shipData1.position.y,
                sourceRec.width * scale,
                sourceRec.height * scale };

            Rectangle destRec2 = {
            shipData2.position.x,
            shipData2.position.y,
            sourceRec.width * scale,
            sourceRec.height * scale };

            Vector2 origin = { 0, 0 };

            if (movementController.thrustOn_1)
            {
                // Draw the current frame of the animation with scaling
                DrawTexturePro(shipData1.animatedTexture, sourceRec, destRec1, origin, 0.0f, WHITE);
            }

            if (movementController.thrustOn_2)
            {
                // Draw the current frame of the animation with scaling
                DrawTexturePro(shipData2.animatedTexture, sourceRec, destRec2, origin, 0.0f, WHITE);
            }

            DrawRectangle(0, floorPositionY, windowWidth, 10, WHITE);
            DrawText(TextFormat("Height: %0.2f miles", (windowHeight - shipData1.position.y)), 10, 10, 20, WHITE); // THIS IS UPSIDE DOWN... NEEDS FIXING
            DrawText(TextFormat("Fuel: %0.2f Liters", shipData1.fuel), 10, 30, 20, WHITE);
            DrawText(TextFormat("Speed: %0.2f Vertical", shipData1.v.y), 10, 50, 20, WHITE);            
            
            AsteroidCollision(asteroidRect, Ships); 

            CheckTouchDown(Ships);

            asteroid.Draw();

            EndDrawing();
        }

        DisplayGameOver();
    }

    // De-Initialization
    UnloadTexture(shipData1.texture); // Unload the texture
    CloseWindow();                    // Close the window

    return 0;
}
