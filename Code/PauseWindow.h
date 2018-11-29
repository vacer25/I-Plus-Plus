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

// ==================== PAUSE WINDOW CLASS ====================

class PauseWindow : public GUIWindow {

private:

	// Variables for drawing
    sfg::Window::Ptr pauseWindow;
    
public:

	// Constructor / destructor
	PauseWindow(Controller *_controller);
	virtual ~PauseWindow();
    
    // Functions invoked by buttons
    void exitToStartMenu();
    void openInstructionsWindow();
    
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