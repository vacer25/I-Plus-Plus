#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <cmath>

// ==================== LOCAL INCLUDES ====================

#include "Entity.h"
#include "Sound.h"
#include "GUIWindow.h"
#include "StartMenu.h"
#include "CodingWindow.h"
#include "PauseWindow.h"
#include "InstructionsWindow.h"
#include "LevelScreen.h"
#include "Player.h"

class StartMenu;
class CodingWindow;
class PauseWindow;
class InstructionsWindow;
class LevelScreen;
class Character;
class TBlender;
class TClock;
class TKey;
class TDoor;

// ==================== CLASS ====================

class Controller {

private:

	// Main game loop
	void update();

	void updatePlayerMovement(float deltaTime, float currentTime);

	// Reading and handling of keyboard inputs
	void getKeyboardInput();
	void handleKeyboardInput(sf::Vector2f playerPos);

	// Reading of mouse inputs
	void getMouseInput();

    // Reset question solved status
    void setAllQuestionsNotSolved();

    Character dude;
    TDoor aDoor;
	TBlender aBlender;
	TKey aKey;
	TClock aClock;

	// Variables for drawing
	sf::RenderWindow window;

    // Variables for the GUI windows
    StartMenu *startMenu;
	CodingWindow *codingWindow;
	PauseWindow *pauseWindow;
	InstructionsWindow *instructionsWindow;

    // Level screen
	LevelScreen *levelScreen;

	// State variables
	GameState gameState;
    int levelNumber;
    bool questionIsSolved[NUMBER_OF_QUESTIONS_TOTAL];
    bool hasFocus;
    bool needToClose;
    bool firstTimeOpeningCodingWindow;

public:

	// Constructor / destructor
	Controller();
	virtual ~Controller();

	// Start game function
	void start();

    // Exit game function
    void exitGame();

	// Getters
	GameState getGameState();
	bool getCodingWindowState();
	bool getPauseWindowState();
	bool getInstructionsWindowState();
    bool getQuestionSolvedStatus(int questionNumber);

	// Setters
	bool setGameState(GameState gameStateToSet);
    bool setQuestionSolvedStatus(bool statusToSet, int questionNumber);

    // Open / close GUI windows
	bool openStartMenuWindow();
	void closeStartMenuWindow();
	bool openCodingWindow(int questionNumber);
    void closeCodingWindow();
	bool openPauseWindow();
	void closePauseWindow();
	bool openInstructionsWindow();
	void closeInstructionsWindow();

    // State variables
    bool codingWindowIsOpen;
    bool pauseWindowIsOpen;
    bool instructionsWindowIsOpen;
    bool gameStateDidChage_CodingWindow;

    // Keyboard input variables
    static sf::Vector2f mousePosition;
	static bool mouseLeftIsPressed;
	static bool mouseLeftIsReleased;
    static bool mouseLeftIsDown;

	// Keyboard input variables
	static bool keyUpIsPressed;
	static bool keyDownIsPressed;
	static bool keyLeftIsPressed;
	static bool keyRightIsPressed;

	static bool keyUpIsDown;
	static bool keyDownIsDown;
	static bool keyLeftIsDown;
	static bool keyRightIsDown;

	static bool keyActionIsPressed;
	static bool keyEscapeIsPressed;

	static bool keyActionIsDown;
	static bool keyEscapeIsDown;

	static bool keySelectIsPressed;
	static bool keyDeselectIsPressed;

	static bool keySelectIsDown;
	static bool keyDeselectIsDown;

	static bool keyPauseIsDown;
	static bool keyPauseIsPressed;

	static bool keySpaceIsDown;
	static bool keySpaceIsPressed;
};
