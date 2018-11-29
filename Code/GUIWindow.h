#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

// ==================== LOCAL INCLUDES ====================

#include "Constants.h"

// Controller.h is included in the derived classes
class Controller;

// ==================== GUI WINDOW CLASS ====================

class GUIWindow {
    
private:

    
protected:

    // Pointer to controller
    Controller *controller;
    
	// Variables for drawing
	static sfg::SFGUI sfgui;
	sfg::Desktop mainDesktop;
    
    // Font
    static std::shared_ptr<sf::Font> consolaFont;
    
    // Flag to indicate if this window has been shown befire being drown
    // If this is false when the draw() function in invoked, the show() function is invoked before the drawing happens
    bool isShown = false;
    
public:

	// Constructor / destructor
    GUIWindow(Controller *_controller);
    ~GUIWindow();
    
    // Initialize GUI, loads font and theme
    static void Initialize();

	// Update GUI, called from Controller in draw loop
	virtual void update(float deltaTime);
    
	// Update GUI events, called from Controller in event loop
	virtual void updateEvent(sf::Event event);
    
	// Draw GUI, called from Controller in draw loop
	static void draw(sf::RenderWindow &window);
    
    // Show function moves the window to the correct position on screen
    virtual void show() = 0;
    
    // Hide function moves the window off-screen
    virtual void hide() = 0;
    
    // Close function, hides the window and indicated to stop updating it from Controller class
    virtual void close() = 0;
    
};

