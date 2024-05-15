

#include<iostream>
#include <sstream>  // the sstream class adds some really useful functionality for combining Strings and other
                    // variable types into a String.
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;



// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];  // array of Sprite objects called branches that can hold six Sprite instances.

// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };

side branchPositions[NUM_BRANCHES];  // array called branchPositions with six values. Each of these values is of the
// side type and can be either LEFT, RIGHT, or NONE.



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

    Sprite spriteTree1;
    Sprite spriteTree2;
    Sprite spriteTree3;

    spriteTree1.setTexture(textureTree);
    spriteTree2.setTexture(textureTree);
    spriteTree3.setTexture(textureTree);

    spriteTree1.setPosition(810, 0);
    spriteTree2.setPosition(210, -200);
    spriteTree3.setPosition(1200, -200);


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


    // Track whether the game is running
    bool paused = true;


    // Draw some text
    int score = 0;

    Text messageText;
    Text scoreText;

    // We need to choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);


    // Position the text.
    // remember that the origin of everything we draw is at the top left-hand corner. So, if we simply divide the
    // screen width and height by two and use the results in mesageText.setPosition..., then the top left of the text
    // will be in the center of the screen and it will spread out untidily to the right.

    FloatRect textRect = messageText.getLocalBounds(); // getLocalBounds function returns the coordinates of the
    // rectangle that wraps messageText.

    // What the code does is set the center of messageText to the center of the screen
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
//    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);


    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);  // hide it off-screen with setPosition.

        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }


    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // The player starts on the left
    side playerSide = side::LEFT;  // playerSide, to keep track of where the player is currently standing.(Left/Right)

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up with the tree. Determine the horizontal position the axe will be drawn at, depending on whether
    // the player is on the left-or right-hand side of the tree.
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false; // will be used to determine when to listen for chops and when to ignore them


    /*
     SFML plays sound effects using two different classes. The first class is the SoundBuffer class. This is the class
     that holds the actual audio data from the sound file. It is SoundBuffer that is responsible for loading the .wav
     files into the PC's RAM in a format that can be played without any further decoding work.

     Once we have a SoundBuffer object with our sound stored in it, we will then create another object of the Sound
     type. We can then associate this Sound object with a SoundBuffer object. Then, at the appropriate moment in our
     code, we will be able to call the play function of the appropriate Sound object.
     */
    // Prepare the sounds
    // The player chopping sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    // The player has met his end under a branch
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

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
            // The following code uses the method of detecting when a key is released.
            // executes when both a key has been released and the game is not paused.
            if (event.type == Event::KeyReleased && !paused) {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
            if (event.type == Event::Closed)
                window.close();
//            if (event.type == Event::KeyPressed)
//            {
//                window.create(fullscreen ? VideoMode::getFullscreenModes()[0] : VideoMode(1920, 1080), "Non-zombie window");
//                fullscreen = !fullscreen;
//            }

            // Start the game
            if (Keyboard::isKeyPressed(Keyboard::Return)) {
                paused = false;

                // Reset the time and the score
                score = 0;

                // Make all the branches disappear - we are using a for loop to prepare the tree with no branches.
                // This is fair to the player because, if the game started with a branch right above their head,
                // it would be considered unsporting.
                for (int i = 1; i < NUM_BRANCHES; i++) {
                    branchPositions[i] = side::NONE;
                }

                // Make sure the gravestone is hidden
                spriteRIP.setPosition(675, 2000);

                // Move the player into position
                spritePlayer.setPosition(580, 720);

                acceptInput = true;  // We are now ready to receive chopping key presses.
            }

            // Wrap the player controls to Make sure we are accepting input
            if (acceptInput) {
                // what happens when the player presses the right cursor key on the keyboard
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    // Make sure the player is on the right
                    playerSide = side::RIGHT;
                    score ++;
                    spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(1200, 720);

                    // Update the branches to move all the branches down one place and spawn a new random branch
                    // (or space) at the top of the tree.
                    updateBranches(score);

                    // Set the log flying to the left
                    spriteLog.setPosition(810, 720);
                    logSpeedX = -5000;
                    logActive = true;

                    acceptInput = false;

                    // Play a chop sound
                    chop.play();
                }

                // Handle the left cursor key
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    // Make sure the player is on the left
                    playerSide = side::LEFT;
                    score++;
                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(580, 720);

                    // update the branches
                    updateBranches(score);

                    // Set the log flying
                    spriteLog.setPosition(810, 720);
                    logSpeedX = 5000;
                    logActive = true;

                    acceptInput = false;

                    // Play a chop sound
                    chop.play();
                }
            }

            /*
             ****************************************
             Update the scene
             ****************************************
             */

            if (!paused) {
                // Measure time
                Time dt = clock.restart();  // it returns the amount of time that has elapsed since the last time we
                // restarted the clock.


                // Setup the bee
                if (!beeActive) {  // We only want to do this when the bee is inactive.
                    // How fast is the bee
                    srand((int)time(0));
                    beeSpeed = (rand() % 200) + 200;  // Get a random number between 200 and 399

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
                    cloud1Speed = (rand() % 200);

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
                    cloud2Speed = (rand() % 200);

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
                    cloud3Speed = (rand() % 200);

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
                // Update the score text
                stringstream ss;
                ss<< "Score = " << score;
                scoreText.setString(ss.str()); // This line of code simply sets the String contained in ss to scoreText:

                // update the branch sprites
                for (int i = 0; i < NUM_BRANCHES; i++) {
                    float height = i * 150;
                    if (branchPositions[i] == side::LEFT) {
                        // Move the sprite to the left side
                        branches[i].setPosition(610, height);

                        // Flip the sprite by 180 degrees because the branch.png graphic "hangs" to the right by default.
                        branches[i].setRotation(180);
                    }
                    else if(branchPositions[i] == side::RIGHT) {
                        // Move the sprite to the right side
                        branches[i].setPosition(1330, height);

                        // Set the sprite rotation to zero degrees, just in case it had previously been at 180 degrees.
                        branches[i].setRotation(0);
                    }
                    else {
                        // branchPosition must be NONE so hides the branch off-screen at 3,000 pixels.
                        branches[i].setPosition(3000, height);
                    }
                }

                // Handle a flying log
                if (logActive) {
                    spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                                          spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                    // Has the log reached the right hand edge?
                    if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
                        // Set it up ready to be a whole new log next frame
                        logActive = false;
                        spriteLog.setPosition(810, 720);
                    }
                }

                // Detecting the player getting squashed is really simple. All we want to know is: does the last
                // branch in the branchPositions array equal playerSide? If it does, the player is dead.
                // has the player been squished by a branch?
                if (branchPositions[5] == playerSide) {
                    // death
                    paused = true;
                    acceptInput = false;

                    // Draw the gravestone
                    spriteRIP.setPosition(525, 760);

                    // hide the player
                    spritePlayer.setPosition(2000, 660);

                    // Change the text of the message
                    messageText.setString("SQUISHED!!");

                    // Center it on the screen
                    FloatRect textRect = messageText.getLocalBounds();
                    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                                          textRect.top + textRect.height / 2.0f);

                    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                    // Play a death sound
                    death.play();
                }

            } // End if(!paused)



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

        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }
        // Draw the trees
        window.draw(spriteTree1);
        window.draw(spriteTree2);
        window.draw(spriteTree3);

        // Draw the player
        window.draw(spritePlayer);

        // Draw the axe
        window.draw(spriteAxe);

        // Draw the flying log
        window.draw(spriteLog);

        // Draw the gravestone
        window.draw(spriteRIP);

        // Draw the bee
        window.draw(spriteBee);

        // Draw the score
        window.draw(scoreText);

        // This if statement causes messageText to only be drawn when the game is paused.
        if (paused) {
            // Draw our message
            window.draw(messageText);
        }


        // Show everything we just drew
        window.display();
    }
}



// Function definition
void updateBranches(int seed) {
    // Simply move all the branches down one position, one at a time, starting with the sixth branch.
    // This is achieved by making the for loop count from 5 through to 0.
    // Other thing to note with this previous code is that after we have moved the branch in position 4 to position 5,
    // then the branch in position 3 to position 4, and so on, we will need to add a new branch at position 0,
    // which is the top of the tree.
    for (int j = NUM_BRANCHES-1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0)+seed);
    int r = (rand() % 5);

    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}