// ==================== LOCAL INCLUDES ====================

#include "PauseWindow.h"

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

PauseWindow::PauseWindow(Controller *_controller) : GUIWindow(_controller) {
    
	// ==================== CREATE PAUSE WINDOW ====================
    
	// Create GUI for pause window
    pauseWindow = sfg::Window::Create();
    
    // ==================== SET UP FONT(S) ====================

    // Set up the default font for all the labels
	mainDesktop.GetEngine().GetResourceManager().SetDefaultFont(consolaFont);
    
    // Set the font size of all elements
	mainDesktop.GetEngine().SetProperty("Frame#mainFrame", "FontSize", GUI_WINDOW_TITLE_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Button", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    
    // ==================== SET UP BUTTONS AND WINDOW TO HOLD BUTTON ====================
    
	// Create the main frame
	auto mainFrame = sfg::Frame::Create("Pause");
	mainFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    mainFrame->SetId("mainFrame");
    
    // Create the controls buttons box to hold the run and exit buttons
	auto buttonBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING); // Orientation and padding

    // Create the return to game button and add it to the controls buttons box
    auto returnButton = sfg::Button::Create("Return To Game");
    returnButton->SetId("returnButton");
    returnButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->close();});
    returnButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    buttonBox->Pack(returnButton, true, true);
    
    //Create instructions window
    auto instructionsButton = sfg::Button::Create("Instruction");
    instructionsButton->SetId("instructionsButton");
    instructionsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->openInstructionsWindow();});
    instructionsButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    buttonBox->Pack(instructionsButton, true, true);
    
    // Create the exit button and add it to the controls buttons box
    auto exitButton = sfg::Button::Create("Exit To Main Menu");
    exitButton->SetId("exitButton");
    exitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->exitToStartMenu();});
    exitButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    buttonBox->Pack(exitButton, true, true);
    
    // ==================== SET UP GUI WINDOW ====================
    
    // Add the button box to the main frame and add that to the pause window
    mainFrame->Add(buttonBox);
    pauseWindow->Add(mainFrame);
    
	// Disable resizing
	pauseWindow->SetStyle(pauseWindow->GetStyle() ^ sfg::Window::TITLEBAR);
	pauseWindow->SetStyle(pauseWindow->GetStyle() ^ sfg::Window::RESIZE);
    
    // Add the pause window to the main desktop
    mainDesktop.Add(pauseWindow);
    
    // Hide the window when it is first created
    hide();
    
}

PauseWindow::~PauseWindow() {
	// TODO Implement destructor
}

// ==================== SHOW ====================

void PauseWindow::show() {
    
    // Show the window by moving it to the correct position
    pauseWindow->SetAllocation(sf::FloatRect(WIDTH / 2 - PAUSE_WINDOW_WIDTH / 2, HEIGHT / 2 - PAUSE_WINDOW_HEIGHT / 2, PAUSE_WINDOW_WIDTH, PAUSE_WINDOW_HEIGHT)); 
    
    // Set the flag
    isShown = true;
    
}

// ==================== USER INTERACTION ====================

void PauseWindow::exitToStartMenu() {
    
    // Set the game state to the start screen and (TODO) open the start screen
    controller->setGameState(START_SCREEN);
    
}

void PauseWindow::openInstructionsWindow() {
    
    // Open the instructions window through the controller which will auto hide the pause window
    controller->openInstructionsWindow();
    
}

// ==================== UPDATE ====================

void PauseWindow::update(float deltaTime) {
	// Update the window GUI to keep it interactive
    GUIWindow::update(deltaTime);
}

void PauseWindow::updateEvent(sf::Event event) {
	// Update the window GUI with new user input events
	GUIWindow::updateEvent(event);
}

// ==================== HIDE / CLOSE ====================

void PauseWindow::hide() {
    
    // Hide the window by moving it off-screen
    pauseWindow->SetAllocation(sf::FloatRect(WIDTH, HEIGHT, PAUSE_WINDOW_WIDTH, PAUSE_WINDOW_HEIGHT)); 
    
    // Clear the flag
    isShown = false;
    
}

void PauseWindow::close() {
    
    // Hide the window
    hide();
    
    // Make the controller stop updating it
    controller->pauseWindowIsOpen = false; 
    
}

// ==================== DRAW ====================

void PauseWindow::draw(sf::RenderWindow &window) {
    
    // If the screen is not shown yet (its not in the correct position on screen), show it before drawing
    if(!isShown) {
        show();
    }
    
	// Draw all the GUI element to the window
	GUIWindow::draw(window);
    
}
