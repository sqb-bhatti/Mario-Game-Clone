

#include<iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


int main()
{
    static bool fullscreen = true;

    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
//    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);  // Not working
    RenderWindow window(vm, "Timber!!!");

    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;

    // Load a graphic into the texture
    if (!textureBackground.loadFromFile("graphics/background.png")) {
        cout << "Error \n";
    }
    else if (textureBackground.loadFromFile("graphics/background.png")) {
        cout << "FILE FOUND \n";
    }

    // Create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);


    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);


    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    // Is the bee currently moving?
    bool beeActive = false;

    // How fast can the bee fly
    float beeSpeed = 0.0f;  // This will hold the speed in pixels per second that our bee will fly across the screen at.

    // make 3 cloud sprites from 1 texture
    Texture textureCloud;

    // Load 1 new texture
    textureCloud.loadFromFile("graphics/cloud.png");

    // 3 New sprites with the same texture
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;

    // It is completely normal for multiple Sprite objects to share a texture.
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    // Position the clouds on the left of the screen at different heights
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // How fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;


    // Variables to control time itself
    Clock clock;

    while (window.isOpen())
    {
        /*
         ****************************************
         Handle the players input
         ****************************************
         */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                window.create(fullscreen ? VideoMode::getFullscreenModes()[0] : VideoMode(1920, 1080), "Non-zombie window");
                fullscreen = !fullscreen;
            }

            /*
             ****************************************
             Update the scene
             ****************************************
             */
            // Measure time
            Time dt = clock.restart();  // it returns the amount of time that has elapsed since the last time we restarted the clock.

            // Setup the bee
            if (!beeActive) {  // We only want to do this when the bee is inactive.
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 30) + 200;  // Get a random number between 200 and 399

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;  // Get a random number between 500 and 999
                spriteBee.setPosition(2000, height);
                beeActive = true;
            } else {
                // Move the bee
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                                      spriteBee.getPosition().y);

                // Has the bee reached the left-hand edge of the screen?
                // If the getPosition function returns less than -100, it will certainly be out of view of the player.
                // When this occurs, beeActive is set to false and, on the next frame, a "new" bee will be set flying
                // at a new random height and a new random speed.
                if (spriteBee.getPosition().x < -100) {
                    // Set it up ready to be a whole new bee next frame
                    beeActive = false;
                }
            }

            // Manage the clouds
            // Cloud 1
            if (!cloud1Active) {
                // How fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 30);

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            } else {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                                         spriteCloud1.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active) {
                // How fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 30);

                // How high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            } else {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                                         spriteCloud2.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active) {
                // How fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 30);

                // How high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            } else {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                                         spriteCloud3.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame
                    cloud3Active = false;
                }
            }


            /*
             ****************************************
             Draw the scene
             ****************************************
             */

        }

        // clear everything from the last frame
        window.clear();

        // Draw our Game scene here
        window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the tree
        window.draw(spriteTree);

        // Draw the insect
        window.draw(spriteBee);


        // Show everything we just drew
        window.display();
    }
}
