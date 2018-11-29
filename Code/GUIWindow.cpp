// ==================== LOCAL INCLUDES ====================

#include "GUIWindow.h"

// ==================== STATIC VARIABLES ====================

sfg::SFGUI GUIWindow::sfgui;
std::shared_ptr<sf::Font> GUIWindow::consolaFont;

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

GUIWindow::GUIWindow(Controller *_controller) : controller(_controller) {
}

GUIWindow::~GUIWindow() {
    // TODO Implement destructor
}

// ==================== INITIALIZE ====================

void GUIWindow::Initialize() {
    
	consolaFont = std::make_shared<sf::Font>();
	if (!consolaFont->loadFromFile(std::string(FONTS_DIRECTORY) + FONT_NAME)) {
		std::cerr << "Error loading font!" << std::endl;
		return;
	}
    
}

// ==================== UPDATE ====================

void GUIWindow::update(float deltaTime) {
	// Update the GUI to keep it interactive
	mainDesktop.Update(deltaTime);
}

void GUIWindow::updateEvent(sf::Event event) {
	// Update the GUI with new user input events
	mainDesktop.HandleEvent(event);
}

// ==================== DRAW ====================

void GUIWindow::draw(sf::RenderWindow &window) {
	// Draw all the GUI element to the window
	sfgui.Display(window);
}
