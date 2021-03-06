// ==================== LOCAL INCLUDES ====================

#include "Controller.h"

// ==================== STATIC VARIABLES ====================

sf::Vector2f Controller::mousePosition = sf::Vector2f(0, 0);
bool Controller::mouseLeftIsPressed = false;
bool Controller::mouseLeftIsReleased = false;
bool Controller::mouseLeftIsDown = false;

bool Controller::keyUpIsPressed = false;
bool Controller::keyDownIsPressed = false;
bool Controller::keyLeftIsPressed = false;
bool Controller::keyRightIsPressed = false;

bool Controller::keyUpIsDown = false;
bool Controller::keyDownIsDown = false;
bool Controller::keyLeftIsDown = false;
bool Controller::keyRightIsDown = false;

bool Controller::keyActionIsPressed = false;
bool Controller::keyEscapeIsPressed = false;

bool Controller::keyActionIsDown = false;
bool Controller::keyEscapeIsDown = false;

bool Controller::keySelectIsPressed = false;
bool Controller::keyDeselectIsPressed = false;

bool Controller::keySelectIsDown = false;
bool Controller::keyDeselectIsDown = false;

bool Controller::keyPauseIsPressed = false;
bool Controller::keyPauseIsDown = false;

bool Controller::keySpaceIsPressed = false;
bool Controller::keySpaceIsDown = false;

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

Controller::Controller() : window(sf::VideoMode(WIDTH, HEIGHT), TITLE_TEXT, sf::Style::Titlebar | sf::Style::Close) {

	// Set the target frame rate
	window.setFramerateLimit(FRAME_LIMIT);

	// Initialize OpenGL
	window.resetGLStates();

    // Initialize state variables
    hasFocus = true;
    needToClose = false;
    codingWindowIsOpen = false;
    pauseWindowIsOpen = false;
    instructionsWindowIsOpen = false;

	// Start at the initialize state and level 0
    levelNumber = 0;
	gameState = INITIALIZE;

	// Initialize random seed
	srand(time(NULL));
	

}

Controller::~Controller() {

	// Get rid of the GUI windows
    delete(startMenu);
	delete(codingWindow);
    delete(pauseWindow);
    delete(instructionsWindow);

}

// ==================== START ====================

void Controller::start() {

    // Initialize all the questions as unsolved
    setAllQuestionsNotSolved();

	// Initialize (load) the sounds
	Sound::Initialize();
   
	// Play looping background music
	Sound::playMusic("BG_Music.wav", true);
   
	// Initialize the font and theme for GUI windows
	GUIWindow::Initialize();
   
	// Create the start menu, coding window, pause window, and instructions window, for use later
    startMenu = new StartMenu(this);
	codingWindow = new CodingWindow(this);
    pauseWindow = new PauseWindow(this);
    instructionsWindow = new InstructionsWindow(this);

    // Create level screen and load level 1
	levelScreen = new LevelScreen(this);
	levelScreen->LoadFromFile(std::string(LEVELS_DIRECTORY) + "Level1.tmx");

	// Create player and interactive objects
	dude = Character({64.0f, 0.0f});
	aDoor = TDoor({20.0f, 350.0f});
	aBlender = TBlender({740.0f, 190.0f});
	aKey = TKey({740.0f, 350.0f});
	aClock = TClock({225.0f, 95.0f});

	// Start at the start screen
    setGameState(START_SCREEN);

	// [Testing] Move to the game screen
	//setGameState(GAME_SCREEN);

    // [Testing] Open the coding window for the first puzzle
    //openCodingWindow(0);

    // [Testing] Open the instructions window
    //openInstructionsWindow();

	// Start the game loop
	update();

}

// ==================== EXIT ====================

void Controller::exitGame() {
    needToClose = true;
}

// ==================== UPDATE ====================

void Controller::update() {

	// ==================== MAIN GAME LOOP ====================

	// The current event that happened
	sf::Event event;

	// Timer to calculate time per frame
	sf::Clock timer;

	// Infinite loop while the window is open and don't need to close it
	while (window.isOpen() && !needToClose) {

		// ==================== HANDLE EVENTS ====================
			
		// Check all the events
		while (window.pollEvent(event)) {

			// Check to exit
			if (event.type == sf::Event::Closed){
				return;
			}
			
            // Update the update the start screen or end screen if they are open
            if(gameState == START_SCREEN) {
                startMenu->updateEvent(event);
            } else if(gameState == END_SCREEN) {
                // [TODO] Update the end window GUI with events
            }
            // If the instructions window is currently open, update it with events
            if(instructionsWindowIsOpen) {
                instructionsWindow->updateEvent(event);
            }
            // If the pause window is currently open, update it with events
            else if(pauseWindowIsOpen) {
                pauseWindow->updateEvent(event);
            }
            // If the coding window is currently open, update it with events
            else if(codingWindowIsOpen) {
                codingWindow->updateEvent(event);
            }

            // Check if window has focus
            if (event.type == sf::Event::GainedFocus) {
                hasFocus = true;
            } else if (event.type == sf::Event::LostFocus) {
                hasFocus = false;
            }
			
		}

		// ==================== GET & HANDLE KEYBOARD INPUT ====================

        getKeyboardInput();
		handleKeyboardInput(dude.getPosition());

		// ==================== GET MOUSE INPUT ====================

        getMouseInput();

		// ==================== UPDATE & DISPLAY ====================

		// Get the time per frame
		float deltaTime = timer.restart().asSeconds();
        static float currentTime = 0;
        currentTime += deltaTime;
        //std::cout << "FPS: " << (1/deltaTime) << std::endl;
        // Uncapped FPS was between ~150 and ~350 when only the coding screen was implemented (19-10-2018)

		// Clear the previous frame buffer
		window.clear(sf::Color(0,0,0));

        if(gameState == START_SCREEN) {
            startMenu->update(deltaTime);
            startMenu->draw(window);
        } else if(gameState == GAME_SCREEN) { 
            
            // Update and draw the first level screen
            if(levelNumber == 0) {

                // [Testing] Set viewport with mouse
//                int x = WIDTH / 2 - mousePosition.x + WIDTH / 2;
//                int y = HEIGHT / 2 - mousePosition.y + HEIGHT / 2;
//                sf::Vector2f centerPosition = sf::Vector2f(x, y);
//                levelScreen->setViewport(window, centerPosition, false); // Don't force fullscreen

                const float cameraSmoothing = 5.0f;

                // The current and previous posi
            	sf::Vector2f playerPosition = dude.getPosition();
            	static sf::Vector2f cameraPosition = playerPosition;
                
                // Calculate new camera position
            	int viewDistanceX = (playerPosition.x + 48 - cameraPosition.x);
            	int viewDistanceY = (playerPosition.y + 48 - cameraPosition.y);
                sf::Vector2f newCameraPosition(cameraPosition.x + viewDistanceX * deltaTime * cameraSmoothing, cameraPosition.y + viewDistanceY * deltaTime * cameraSmoothing);
                
                //std::cout << "Time: " << currentTime << "| Delta tiime: " << deltaTime <<  " | Player is at: " << playerPosition.x << ", " << playerPosition.y << std::endl;
                
                // Set the viewport and record new camera position
            	levelScreen->setViewport(window, newCameraPosition, false);
            	cameraPosition = newCameraPosition;
                
                // Only draw the level if currently not in the coding screen or is currently in the pause screen
                if(!codingWindowIsOpen || pauseWindowIsOpen) {
                    levelScreen->draw(window);
                }

            } else if(levelNumber == 1) {
                // [TODO] Update and draw the second level screen
            } else if(levelNumber == 2) {
                // [TODO] Update and draw the third level screen
            }
            
            updatePlayerMovement(deltaTime, currentTime);
            
            // Update the animations and movement
            dude.Updater(deltaTime);  
            aDoor.Updater(deltaTime);
            aBlender.Updater(deltaTime);
            aKey.Updater(deltaTime);
            aClock.Updater(deltaTime);
            
            // The
            aDoor.Draw( window );
            aBlender.Draw( window );
            aKey.Draw( window );
            aClock.Draw( window );
            dude.Draw( window ); // add all the other pieces
				
        } else if(gameState == END_SCREEN) {
            // [TODO] Update and draw the end screen
        }

        // If the instructions window is currently open, update and draw it
        if(instructionsWindowIsOpen) {
            instructionsWindow->update(deltaTime);
            instructionsWindow->draw(window);
        }
        // If the pause window is currently open, update and draw it
        else if(pauseWindowIsOpen) {
            pauseWindow->update(deltaTime);
            pauseWindow->draw(window);
        }
        // If the coding window is currently open, update and draw it
        else if(codingWindowIsOpen) {
            codingWindow->update(deltaTime);
            codingWindow->draw(window);
        }
        // If the instructions window is currently open, update and draw it
        else if(instructionsWindowIsOpen) {
            instructionsWindow->update(deltaTime);
            instructionsWindow->draw(window);
        }
        // If the coding window is currently open, update and draw it
        else if(codingWindowIsOpen) {
            codingWindow->update(deltaTime);
            codingWindow->draw(window);
        }

		// Draw the current frame
		window.display();

	}

}

void Controller::updatePlayerMovement(float deltaTime, float currentTime) {
    
    // If paused, don't move
    //if(pauseWindowIsOpen) {
        //return;
    //}
    
    // Possible movement flags
	bool isOnGround = false;
	bool canGoLeft = true;
	bool canGoRight = true;

    // Check collisions with each object
	for (Object currentObject : levelScreen->objects){ // for each collision rectangle
//		std::cout << "X: " << currentObject.rect.left << std::endl;
//		std::cout << "Y: " << currentObject.rect.top << std::endl;

		// Check floor collision
		if (currentObject.rect.contains(dude.getPosition() + sf::Vector2f(48,96))){
//			std::cout << "Intersects with: " << currentObject.name << std::endl;
			isOnGround = true;
		}

		// Check wall collision
		if (dude.getPosition().x <= 0){
			dude.setPosition(sf::Vector2f (0,dude.getPosition().y));
			canGoLeft = false;
		}
		if (dude.getPosition().x >= (levelScreen->width * levelScreen->tileWidth - 96)) {
			dude.setPosition(sf::Vector2f (levelScreen->width * levelScreen->tileWidth - 96,dude.getPosition().y));
			canGoRight = false;
		}
        
	}

    // Movement direction
	sf::Vector2f dir = {0.0f, 0.0f};
    
    // Fravity forces
    float gravity;
    const float gravityForceDown = 250.0f; 
    const float gravityForceJump = 400.0f; 
    const float gravityForceLimit = 1 + gravityForceDown * (1.0f / FRAME_LIMIT); //2.0f; 
    
    // Jump timings
    static float jumpStartTime = 0.0f; // s
    const float jumpForceApplyTime = 0.125f; // s
    
    //std::cout << "Delta time: " << (deltaTime) << std::endl;
    
    //std::cout << "Time since jump: " << (currentTime - jumpStartTime) << std::endl;
    
     // If jump force still applies, revese gravity to jump
    if(currentTime - jumpStartTime <= jumpForceApplyTime) {
        gravity = -gravityForceJump;
    }
    
    // If jump force does not apply
    else {

        // If is on ground
        if (isOnGround) {
            
            // If starting to jump, record time of jump
            if(keyUpIsPressed) {
                jumpStartTime = currentTime;
                //std::cout << "Jumped at time: " << jumpStartTime << std::endl;
            }
            
            // No gravity applies when standing on ground
            dir.y = 0;
           
        }
        
        // If not on ground
        else {
            gravity = gravityForceDown;
        }
    
    }
    
    // Apply gravity force if it is within the limit (to prevent fallsing through the ground!)
    float gravityToApply = gravity * deltaTime;
    if(gravityToApply <= gravityForceLimit) {
        dir.y = gravityToApply;
    }
    //std::cout << "Gravity limit: " << gravityForceLimit << " | Gravity to apply: " << (gravityToApply) << std::endl;
    
    // Check to move left
    if (keyLeftIsDown && canGoLeft) {
            dir.x -= 5.0f;
    }
    
    // Check to move right
    if (keyRightIsDown && canGoRight) {
            dir.x += 5.0f;
    }
    
    // Set the movement direction
    dude.setDirection(dir);
}

// ==================== HANDLE INPUT ====================
void Controller::getMouseInput() {

    // Get mouse position and convert from integers to floats in the window is focused
    if(hasFocus) {
        sf::Vector2i mousePositionInt = sf::Mouse::getPosition(window);

        if(mousePositionInt.x >= 0 && mousePositionInt.x < WIDTH && mousePositionInt.y >= 0 && mousePositionInt.y < HEIGHT) {
            mousePosition = sf::Vector2f(mousePositionInt.x, mousePositionInt.y);
        }
    }

    // Get left mouse button status
    mouseLeftIsPressed = (hasFocus && !mouseLeftIsDown) && (sf::Mouse::isButtonPressed(sf::Mouse::Left));
    mouseLeftIsReleased = mouseLeftIsDown && (!sf::Mouse::isButtonPressed(sf::Mouse::Left));
    mouseLeftIsDown = hasFocus && (sf::Mouse::isButtonPressed(sf::Mouse::Left));

    //std::cout << "Pos: (" << mousePosition.x << ", " << mousePosition.y << "), pressed: " << mouseLeftIsPressed << std::endl;

}

void Controller::getKeyboardInput() {

    keyUpIsPressed = (hasFocus && !keyUpIsDown) && (sf::Keyboard::isKeyPressed(MAIN_UP_KEY) || sf::Keyboard::isKeyPressed(ALT_UP_KEY));
    keyDownIsPressed = (hasFocus && !keyDownIsDown) && (sf::Keyboard::isKeyPressed(MAIN_DOWN_KEY) || sf::Keyboard::isKeyPressed(ALT_DOWN_KEY));
    keyLeftIsPressed = (hasFocus && !keyLeftIsDown) && (sf::Keyboard::isKeyPressed(MAIN_LEFT_KEY) || sf::Keyboard::isKeyPressed(ALT_LEFT_KEY));
    keyRightIsPressed = (hasFocus && !keyRightIsDown) && (sf::Keyboard::isKeyPressed(MAIN_RIGHT_KEY) || sf::Keyboard::isKeyPressed(ALT_RIGHT_KEY));

    keyUpIsDown = hasFocus && (sf::Keyboard::isKeyPressed(MAIN_UP_KEY) || sf::Keyboard::isKeyPressed(ALT_UP_KEY));
    keyDownIsDown = hasFocus && (sf::Keyboard::isKeyPressed(MAIN_DOWN_KEY) || sf::Keyboard::isKeyPressed(ALT_DOWN_KEY));
    keyLeftIsDown = hasFocus && (sf::Keyboard::isKeyPressed(MAIN_LEFT_KEY) || sf::Keyboard::isKeyPressed(ALT_LEFT_KEY));
    keyRightIsDown = hasFocus && (sf::Keyboard::isKeyPressed(MAIN_RIGHT_KEY) || sf::Keyboard::isKeyPressed(ALT_RIGHT_KEY));

    keyActionIsPressed = hasFocus && !keyActionIsDown && sf::Keyboard::isKeyPressed(MAIN_ACTION_KEY);
    keyEscapeIsPressed = hasFocus && !keyEscapeIsDown && sf::Keyboard::isKeyPressed(MAIN_ESCAPE_KEY);

    keyActionIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_ACTION_KEY);
    keyEscapeIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_ESCAPE_KEY);

    keySelectIsPressed = hasFocus && !keySelectIsDown && sf::Keyboard::isKeyPressed(MAIN_ENTER_KEY);
    keyDeselectIsPressed = hasFocus && !keyDeselectIsDown && sf::Keyboard::isKeyPressed(MAIN_EXIT_KEY);

    keySelectIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_ENTER_KEY);
    keyDeselectIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_EXIT_KEY);

    keyPauseIsPressed = hasFocus && !keyPauseIsDown && sf::Keyboard::isKeyPressed(MAIN_PAUSE_KEY);
    keyPauseIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_PAUSE_KEY);

    keySpaceIsPressed = hasFocus && !keySpaceIsDown && sf::Keyboard::isKeyPressed(MAIN_SPACE_KEY);
	keySpaceIsDown = hasFocus && sf::Keyboard::isKeyPressed(MAIN_SPACE_KEY);

}

void Controller::handleKeyboardInput(sf::Vector2f playerPosition) {

    // This function is for entering the pause state and testing only
    // All the actual player movement and other keyboard controls will be implemented in other classes, not here

	// When the pause key is pressed in the game state
    if(keyPauseIsPressed && gameState == GAME_SCREEN) {
        // If the pause window is not opened (not paused), open it (pause the game)
        if(!pauseWindowIsOpen) {
            openPauseWindow();
        }
        // If the pause window is opened (paused)
        else {
            // If the instructions window had been closed, the pause key unpauses the game
            if(!instructionsWindowIsOpen) {
                closePauseWindow();
            }
        }
    }

	// [Testing] If a number key + the deselect key is pressed, open the coding window for that question number
    if(keyDeselectIsPressed) {
        sf::Keyboard::Key numberKeys[] = {sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9};

        for(int numberKeyIndex = 0; numberKeyIndex < 10; numberKeyIndex++) {
            if(sf::Keyboard::isKeyPressed(numberKeys[numberKeyIndex])) {
                openCodingWindow(numberKeyIndex);
                break;
            }
        }

    }

    // Open coding window based on interactive object
	// Clock = 1	Blender = 2		Key = 3		Door = 4

	if(keySpaceIsPressed){
		if(aClock.getRect().intersects(dude.getRect())){
			openCodingWindow(1);
		}
		else if(aBlender.getRect().intersects(dude.getRect())){
			openCodingWindow(2);
		}
		else if(aKey.getRect().intersects(dude.getRect())){
			openCodingWindow(3);
		}
		else if(aDoor.getRect().intersects(dude.getRect())){
			openCodingWindow(4);
		}
	}

}

// ==================== OPEN / CLOSE GUI WINDWOS ====================

bool Controller::openStartMenuWindow() {

    // The start menu window can only be opened in the start screen state
    if(gameState == START_SCREEN) {

        // Show the start menu window
        startMenu->show();

        // Indicate that the start menu window was opened
        return true;

    }

    // If the above condition are not satisfied, the start menu window will not be opened
    return false;
}

void Controller::closeStartMenuWindow() {
    startMenu->close();
}

bool Controller::openCodingWindow(int questionNumber) {

    // The coding window can only be opened in the game state when the pause and instructions windows are closed
    if(gameState == GAME_SCREEN && !pauseWindowIsOpen && !instructionsWindowIsOpen) {

        // Try to open the coding window
        try {
            codingWindow->initializeCodingWindowForNewQuestion(questionNumber, firstTimeOpeningCodingWindow);
            codingWindow->show();
            codingWindowIsOpen = true;
            firstTimeOpeningCodingWindow = false;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        // Return if it was opened or not (it can fail is there is a problem with the current coding problem file)
        return codingWindowIsOpen;

    }

    // If the above conditions are not satisfied, the coding window will not be opened
    return false;

}

void Controller::closeCodingWindow() {
    if(codingWindowIsOpen) {
        codingWindow->close();
    }
}

bool Controller::openPauseWindow() {

    // The pause window can only be opened in the game state when the instructions screen is closed
    if(gameState == GAME_SCREEN && !instructionsWindowIsOpen) {

        // Hide the other windows
        startMenu->hide();
        codingWindow->hide();
        instructionsWindow->hide();

        // Show the pause window
        pauseWindow->show();

        pauseWindowIsOpen = true;
        return true;

    }

    // If the above condition are not satisfied, the pause window will not be opened
    return false;

}

void Controller::closePauseWindow() {
    if(pauseWindowIsOpen) {
        pauseWindow->close();
    }
}

bool Controller::openInstructionsWindow() {

    // The instructions window can only be opened in the start screen state or game state
    if(gameState == START_SCREEN || gameState == GAME_SCREEN) {

        // Hide the other windows
        startMenu->hide();
        codingWindow->hide();
        pauseWindow->hide();

        // Show the instructions window
        instructionsWindow->show();

        instructionsWindowIsOpen = true;
        return true;

    }

    // If the above condition are not satisfied, the instructions window will not be opened
    return false;

}

void Controller::closeInstructionsWindow() {
    if(instructionsWindowIsOpen) {
        instructionsWindow->close();

        // Hide the start screen in the start screen state while the instructions screen is open
        if(gameState == START_SCREEN) {
            startMenu->show();
        }

    }
}

// ==================== GETTERS ====================

GameState Controller::getGameState() {
	return gameState;
}

bool Controller::getCodingWindowState() {
    return codingWindowIsOpen;
}

bool Controller::getPauseWindowState() {
    return pauseWindowIsOpen;
}

bool Controller::getInstructionsWindowState() {
    return instructionsWindowIsOpen;
}

bool Controller::getQuestionSolvedStatus(int questionNumber) {
    return questionIsSolved[questionNumber];
}

// ==================== SETTERS ====================

bool Controller::setQuestionSolvedStatus(bool statusToSet, int questionNumber) {

    // If the question number is invalid, indicate that the status was not set
    if(questionNumber < 0 || questionNumber >= NUMBER_OF_QUESTIONS_TOTAL) {
        return false;
    }

    // Otherwise, set the status and indicate that the status was set
    questionIsSolved[questionNumber] = statusToSet;
    return true;

}

void Controller::setAllQuestionsNotSolved() {

    // Initialize all the questions as unsolved
    for(int questionIndex = 0; questionIndex < NUMBER_OF_QUESTIONS_TOTAL; questionIndex++) {
        questionIsSolved[questionIndex] = false;
    }

}

bool Controller::setGameState(GameState gameStateToSet) {

	bool isLegalTransition = false;

    // Initialize state -> Start screen is legal transition
	if(gameState == INITIALIZE) {
		if(gameStateToSet == START_SCREEN) {
			isLegalTransition = true;
		}
	}

	// Start screen -> Game screen are legal transitions
	else if(gameState == START_SCREEN) {
		if(gameStateToSet == GAME_SCREEN) {
			isLegalTransition = true;
		}
	}

	// Game screen -> Start screen or -> End screen are legal transitions
	else if(gameState == GAME_SCREEN) {
		if(gameStateToSet == START_SCREEN || gameStateToSet == END_SCREEN) {
			isLegalTransition = true;
		}
	}

    // End screen -> Start screen is legal transition
	else if(gameState == END_SCREEN) {
		if(gameStateToSet == START_SCREEN) {
			isLegalTransition = true;
		}
	}

	// If the transition is is legal
	if(isLegalTransition) {

        // Change the game state
		gameState = gameStateToSet;

        // If the game state is no longer active, close the coding, instructions, and pause windows
        if(gameState != GAME_SCREEN) {
            closeCodingWindow();
            closeInstructionsWindow();
            closePauseWindow();
        }
        // If the game screen is active now, close the start screen and open the first coding problem
        else {
            closeStartMenuWindow();
            openCodingWindow(0);
        }

        // If the game state is now the start scren, open the start menu window
        if(gameState == START_SCREEN) {

            // Initialize all the questions as unsolved
           setAllQuestionsNotSolved();

            // This is the first time the coding window will be opened
            firstTimeOpeningCodingWindow = true;
            gameStateDidChage_CodingWindow = true;

            // Force fullscreen viewport
            sf::Vector2f zeroPosition = sf::Vector2f(0, 0);
            levelScreen->setViewport(window, zeroPosition, true);

            // Open the start menu window
            openStartMenuWindow();
        }

        // Indicate that the transition was made
		return true;
	}

	// Otherwise, don't
	return false;
}
