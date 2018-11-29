#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

// ==================== LOCAL INCLUDES ====================

#include "Controller.h"
#include "GUIWindow.h"

class Controller;

// ==================== START MENU CLASS ====================

class StartMenu : public GUIWindow{
    
private:

	// Variables for drawing
    sfg::Window::Ptr startWindow;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture titleTexture;
    sf::Sprite titleSprite;

public:

	// Constructor / destructor
	StartMenu(Controller *_controller);
    virtual ~StartMenu();
    
    // Functions invoked by buttons
    void startGame();
    void openInstructionsWindow();
    void quitGame();
    
	// Update GUI, called from Controller in draw loop
	void update(float time);
    
	// Update GUI events, called from Controller in event loop
	void updateEvent(sf::Event event);
    
	// Draw GUI, called from Controller in draw loop
	void draw(sf::RenderWindow &window);
    
    // Show function
    void show();
    
    // Hide function
    void hide();
    
    // Exit function
    void close();

};
