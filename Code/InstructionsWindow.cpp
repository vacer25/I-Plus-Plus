// ==================== LOCAL INCLUDES ====================

#include "InstructionsWindow.h"

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

InstructionsWindow::InstructionsWindow(Controller *_controller) : GUIWindow(_controller) {
    
	// ==================== CREATE INSTRUCTIONS WINDOW ====================
    
	// Create GUI for instructions window
	instructionsWindow = sfg::Window::Create();
    
	// Create the main box
	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
    
	// ==================== SET UP FONT(S) ====================

    // Set up the default font for all the labels
	mainDesktop.GetEngine().GetResourceManager().SetDefaultFont(consolaFont);
     
    // Set the font size of all elements
	mainDesktop.GetEngine().SetProperty("Frame#instructionsControlsFrame", "FontSize", GUI_WINDOW_TITLE_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Label#instructionsLabel", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Label#controlsLabel", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Button", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    
    // ==================== INSTRUCTIONS ====================

	// Create the instructions / controls frame
	auto instructionsControlsFrame = sfg::Frame::Create("Instructions");
	instructionsControlsFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    instructionsControlsFrame->SetId("instructionsControlsFrame");
    
	// Create the instructions / controls box
	auto instructionsControlsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
    
    // Create instructions lable
    auto instructionsLabel = sfg::Label::Create(INSTRUCTIONS_WINDOW_INSTRUCTIONS_TEXT);
	instructionsLabel->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    instructionsLabel->SetId("instructionsLabel");
    
    // Add the instructions label to the instructions / controls  box
    instructionsControlsBox->Pack(instructionsLabel, true, true);
    
    // ==================== SEPARATOR ====================
    
    // Add a horizontal separator to the instructions / controls  box
    instructionsControlsBox->Pack(sfg::Separator::Create(), false, true);
    
    // ==================== CONTROLS ====================

    // Create controls lable
    auto controlsLabel = sfg::Label::Create(INSTRUCTIONS_WINDOW_CONTROLS_TEXT);
    controlsLabel->SetId("controlsLabel");
    
    // Add the controls label to the instructions / controls box and add that to the instructions / controls frame
    instructionsControlsBox->Pack(controlsLabel, true, true);
    instructionsControlsFrame->Add(instructionsControlsBox);
    
	// Add the instructions / controls frame to the main box
    mainBox->Pack(instructionsControlsFrame, true, true);
    
    // ==================== EXIT BUTTON ====================
    
	// Create the back button
	backButton = sfg::Button::Create("Back");
    backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->close();});
    backButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    
    // Add the back button to the main box
    mainBox->Pack(backButton, false, true);
    
    // ==================== SET UP GUI WINDOW ====================

	// Add the main box to the instructions window
	instructionsWindow->Add(mainBox); 
    
	// Disable resizing
	instructionsWindow->SetStyle(instructionsWindow->GetStyle() ^ sfg::Window::TITLEBAR);
	instructionsWindow->SetStyle(instructionsWindow->GetStyle() ^ sfg::Window::RESIZE);

    // Add the instructions window to the main desktop to enable setting widget preperties
	mainDesktop.Add(instructionsWindow);
    
    // Hide the window when it is first created
    hide();
    
}

InstructionsWindow::~InstructionsWindow() {
	// TODO Implement destructor
}

// ==================== SHOW ====================

void InstructionsWindow::show() {
    
    // Show the window by moving it to the correct position
    instructionsWindow->SetAllocation(sf::FloatRect(WIDTH / 2 - INSTRUCTIONS_WINDOW_WIDTH / 2, HEIGHT / 2 - INSTRUCTIONS_WINDOW_HEIGHT / 2, INSTRUCTIONS_WINDOW_WIDTH, INSTRUCTIONS_WINDOW_HEIGHT)); 
    
    // Set the flag
    isShown = true;
    
}

// ==================== UPDATE ====================

void InstructionsWindow::update(float deltaTime) {
	// Update the window GUI to keep it interactive
    GUIWindow::update(deltaTime);
}

void InstructionsWindow::updateEvent(sf::Event event) {
	// Update the window GUI with new user input events
	GUIWindow::updateEvent(event);
}

// ==================== HIDE / CLOSE ====================

void InstructionsWindow::hide() {
    
    // Hide the window by moving it off-screen
	instructionsWindow->SetAllocation(sf::FloatRect(WIDTH, HEIGHT, INSTRUCTIONS_WINDOW_WIDTH, INSTRUCTIONS_WINDOW_HEIGHT));   
    
    // Clear the flag
    isShown = false;
    
}

void InstructionsWindow::close() {
    
    // Hide the window
    hide();
    
    // Make the controller stop updating it
    controller->instructionsWindowIsOpen = false;
    
}

// ==================== DRAW ====================

void InstructionsWindow::draw(sf::RenderWindow &window) {
    
    // If the screen is not shown yet (its not in the correct position on screen), show it before drawing
    if(!isShown) {
        show();
    }
    
	// Draw all the GUI element to the window
	GUIWindow::draw(window);
    
}
