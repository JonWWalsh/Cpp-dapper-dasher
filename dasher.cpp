#include "raylib.h"

struct AnimData // a.k.a an object
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height; // returns true if scarfy is on the ground, false if he's in the air.
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    // acceleration due to gravity (pixels/second)/second
    const int gravity{1'000}; 

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // Animdata for nebula
    // AnimData nebData{
    //     {0.0, 0.0, nebula.width/8, nebula.height/8}, // Rectangle rec
    //     {windowDimensions[0], windowDimensions[1] - nebula.height/8}, // Vector2 pos
    //     0, // int frame
    //     1.0/12.0, // float updateTime
    //     0 // float runningTime
    // }; // within the braces is a comma separated list of values to give nebData's member variables, these must be of the correct type and in correct order.
    //      /* You can use braced initialization to initialize variables on the same line we declare it.  The order of values must be correct, in this case: x, y, width, height.  This also ensures the variable will always be initialized*/

    // // Animdata for nebula2
    // AnimData neb2Data{
    //     {0.0, 0.0, nebula.width/8, nebula.height/8}, // Rectangle rec
    //     {windowDimensions[0] + 300, windowDimensions[1] - nebula.height/8}, // Vector2 pos
    //     0, // int frame
    //     1.0/16.0, // float updateTime
    //     0.0 // float runningTime
    // };

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{}; // this creates an array that holds 3 AnimData elements (uninitialized)  If you add {} after the [] then each element gets initialized w/ default values.  You can assign each element values with a comma separated list inside the braces.

    // this for loop loops through each element in the nebulae array and assign values to each AnimData in the array
    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }
    // here we initalize the Animdata not included in the for loop.  Now we've initalized the variables in each AnimData in the nebulae array and the original data in lines 27-44 are no longer needed since we removed the AnimData directly from the array on ln 46.

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x}; // this creates a finish line at the last nebula in the nebulae array

    // nebula X velocity in pixels/second
    int nebVel{-200};

    // scarfy variables - custom data types in the raylib library
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    // is rectangle in air?
    int isInAir{};
    // jump velocity (pixels/second)
    const int jumpVel{-600};
    
    int velocity{0};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{}; // This is a Vector2 variable (which must be a float) that is used to make the background img scroll.

    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};
        
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT; // scrolls background img at 20 p/s * dT which makes it framerate independent
        if (bgX <= -background.width*2) // checks to see if the 2nd img has reached the end and resets it back to 0.0
        {
            bgX = 0.0;
        }
        // scroll the midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
         //scroll the foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        // draw the background  ***These must be in order from back to front in order to be drawn correctly***
        Vector2 bg1Pos{bgX, 0.0};
        // DrawTextureEx (raylib) takes a Texture2D, Vector2, float rotation, float scale, and color
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        // Create a second Vector2 and DrawTextureEx call for the 2nd background img
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        //draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        // if(scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)  pre-bool check
        {
            // rectangle is on ground
            velocity = 0;
            isInAir = false;
        } else {
            // apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }
        // jump check - key press will only succeed if key is pressed and rectangle is on the ground
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {   // update nebula position
            nebulae[i].pos.x += nebVel * dT; // multiplying by delta time allows this img to move framerate independently 
        }

        // update finish line
        finishLine += nebVel * dT;

        // update scarfy position
        scarfyData.pos.y += velocity *dT;

    {
        for (int i = 0; i < sizeOfNebulae; i++)
        {
        // update nebula animation frame
        //This function (defined on ln 17) replaces the code below
        nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        // nebulae[i].runningTime += dT;
        // if (nebulae[i].runningTime >= nebulae[i].updateTime)
        // {
        //     nebulae[i].runningTime = 0.0;
        //     nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
        //     nebulae[i].frame++;
        //     if (nebulae[i].frame > 7)
        //     {
        //         nebulae[i].frame = 0;
        //     }
        // }
        }

        // this checks to see if scarfy is in the air and freezes his animation if so
        if(!isInAir)
        {
            //This function (defined on ln 17) replaces the code below
            scarfyData = updateAnimData(scarfyData, dT, 5);
            //  // update runningTime
            // scarfyData.runningTime += dT;
            // if (scarfyData.runningTime >= scarfyData.updateTime)
            // {
            //     scarfyData.runningTime = 0.0;
            //     // update animation frame. animation frame pattern: x = frame * w/6
            //     scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            //     scarfyData.frame++;
            //     if (scarfyData.frame > 5)
            //     {
            //     scarfyData.frame = 0;
            //     }
            // }
        }
        /* We update our running time each frame by adding delta time to it (ln 70).  Then we check to see if runningTime has reached 1/12th of a second or greater (ln71)
        if so, we update our animation frame and reset runningTime to 0.  Then we set scarfyRec.x = frame * scarfyRec.width and update frame (ln76) and if frame has surpassed 5
        we reset frame to 0. */
    }

        for (AnimData nebula : nebulae)
        {
            float pad{50}; //On the spritesheet there is extra buffer around each nebula, float pad shrinks that to a more representative size of the nebula
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2* pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }
            if (collision)
            {
                // Lose game
                DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 50, RED);
            } 
            else if (scarfyData.pos.x >= finishLine)
            {
                DrawText("You Win!", windowDimensions[0]/4, windowDimensions[1]/2, 50, RED);
            }
            else 
            {
               for (int i = 0; i < sizeOfNebulae; i++)
                {
                    // draw nebula
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                }   
                    // draw scarfy
                    DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            }

        EndDrawing();
    }
    UnloadTexture(scarfy); // any texture loaded must be unloaded at the end of the program
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow(); // raylib function to properly close window as program ends.
}