#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <string>
#include <iostream>

// ==================== LOCAL INCLUDES ====================

#include "Controller.h"
#include "GUIWindow.h"

class Controller;

// ==================== INSTRUCTIONS WINDOW CLASS ====================

class InstructionsWindow : public GUIWindow {

private:
    
	// Variables for drawing
    sfg::Window::Ptr instructionsWindow;
    sfg::Button::Ptr backButton;    
    
public:

	// Constructor / destructor
	InstructionsWindow(Controller *_controller);
	virtual ~InstructionsWindow();
    
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
