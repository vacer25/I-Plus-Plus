// ==================== LOCAL INCLUDES ====================

#include "StartMenu.h"

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

StartMenu::StartMenu(Controller *_controller) : GUIWindow(_controller) {
    
	// ==================== CREATE START MENU WINDOW ====================
    
	// Create GUI for start menu
	startWindow = sfg::Window::Create();
    startWindow->SetId("startWindow");
    
	// Create the main box
	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
    
    // ==================== SET UP FONT(S) ====================
    
    // Set up the default font for all the labels
	mainDesktop.GetEngine().GetResourceManager().SetDefaultFont(consolaFont);
    
    mainDesktop.GetEngine().SetProperty("Button", "FontSize", 25);
    mainDesktop.GetEngine().SetProperty("Frame#titleFrame", "FontSize", 25);
    mainDesktop.GetEngine().SetProperty("Frame#titleFrame", "Color", sf::Color(0xFF, 0xFF, 0xFF));
    mainDesktop.GetEngine().SetProperty("Frame#buttonFrame", "FontSize", 25);
    mainDesktop.GetEngine().SetProperty("Label#titleLabel", "FontName" , "Library3am.otf");
    
    mainDesktop.GetEngine().SetProperty("Window#startWindow", "BackgroundColor", sf::Color(0,0,0,0));
    mainDesktop.GetEngine().SetProperty("Frame#titleFrame", "BorderColor", sf::Color(0,0,0,0));
    mainDesktop.GetEngine().SetProperty("Frame#buttonFrame", "BorderColor", sf::Color(0,0,0,0));

    // ==================== TITLE ====================

    // Create the title frame
	auto titleFrame = sfg::Frame::Create("So good we do it once more");
	titleFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
	titleFrame->SetId("titleFrame");
  
    // load the background image
    if (!backgroundTexture.loadFromFile(std::string(IMAGES_DIRECTORY) + START_BG_IMAGE_NAME)) {
		std::cerr << "Error loading background!" << std::endl;
		return;
    }
    backgroundSprite.setTexture(backgroundTexture);
    
    // load the title text image
    if (!titleTexture.loadFromFile(std::string(IMAGES_DIRECTORY) + TITLE_IMAGE_NAME)) {
		std::cerr << "Error loading background!" << std::endl;
		return;
    }
    titleSprite.setTexture(titleTexture);
    
    // Position the title text to be in the correct posititon in the middle of the title frame
    sf::Vector2u titleSpriteSize = titleTexture.getSize();
    titleSprite.setPosition(sf::Vector2f(WIDTH / 2 - titleSpriteSize.x / 2, (HEIGHT - START_MENU_SCREEN_BUTTONS_FRAMES_HEIGHT) / 2 - titleSpriteSize.y / 2));

    // Add the title frame to the main box
	mainBox->Pack(titleFrame, true, true);

    // ==================== BUTTONS ====================
    
	// Bottom Box that contains all of the buttons, inside the buttonFrame
	auto bottomBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
	bottomBox->SetRequisition(sf::Vector2f(0.0f, START_MENU_SCREEN_BUTTONS_FRAMES_HEIGHT));

	//Frame that the buttons will be inside
	auto buttonFrame = sfg::Frame::Create();
	buttonFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
	buttonFrame->SetId("buttonFrame");
	buttonFrame->SetRequisition(sf::Vector2f(WIDTH / 6 , 0.0f));
	buttonFrame->Add(bottomBox);
	mainBox->Pack(buttonFrame, false, true);

	//Create startButton
	auto startButtonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, WINDOW_PADDING);

	auto startLabelLeft  = sfg::Label::Create(" ");
	auto startLabelRight = sfg::Label::Create(" ");
	auto startButton = sfg::Button::Create("Start Game");
	startButton->SetId("startButton");
	startButton->SetRequisition(sf::Vector2f(WIDTH / 6 , START_MENU_SCREEN_BUTTONS_HEIGHT));
	startButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->startGame();});
    	startButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
	startButtonBox->Pack(startLabelLeft, true, true);
	startButtonBox->Pack(startButton, false, true);
	startButtonBox->Pack(startLabelRight, true, true);
	bottomBox->Pack(startButtonBox, false, true);

	//Create instructionButton
	auto instructionButtonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, WINDOW_PADDING);
	auto instructionLabelLeft  = sfg::Label::Create(" ");
	auto instructionLabelRight = sfg::Label::Create(" ");
	auto instructionButton = sfg::Button::Create("Instructions");
	instructionButton->SetId("InstructionButton");
	instructionButton->SetRequisition(sf::Vector2f(WIDTH / 6 , START_MENU_SCREEN_BUTTONS_HEIGHT));
	instructionButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]{this->openInstructionsWindow();});
    	instructionButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
	instructionButtonBox->Pack(instructionLabelLeft, true, true);
	instructionButtonBox->Pack(instructionButton, false, true);
	instructionButtonBox->Pack(instructionLabelRight, true, true);
	bottomBox->Pack(instructionButtonBox, false, true);

	//Create quitButton
	auto quitButton = sfg::Button::Create("Quit");
	quitButton->SetId("quitButton");
	quitButton->SetRequisition(sf::Vector2f(WIDTH / 6 , START_MENU_SCREEN_BUTTONS_HEIGHT));
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]{this->quitGame();});
    	quitButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    
	auto quitLabelLeft  = sfg::Label::Create(" ");
	auto quitLabelRight = sfg::Label::Create(" ");
    
	auto quitButtonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, WINDOW_PADDING);
	quitButtonBox->Pack(quitLabelLeft, true, true);
	quitButtonBox->Pack(quitButton, false, true);
	quitButtonBox->Pack(quitLabelRight, true, true);
	bottomBox->Pack(quitButtonBox, false, true);
    
    // ==================== SET UP GUI WINDOW ====================

    // Add the main box to the pause window
	startWindow->Add(mainBox);
    
	// Disable resizing
	startWindow->SetStyle(startWindow->GetStyle() ^ sfg::Window::TITLEBAR);
	startWindow->SetStyle(startWindow->GetStyle() ^ sfg::Window::RESIZE);

    // Add the start menu window to the main desktop
	mainDesktop.Add(startWindow);

}

StartMenu::~StartMenu() {
	// TODO Implement destructor
}

// ==================== SHOW ====================

void StartMenu::show() {
    
    // Show the window by moving it to the correct position (fullscreen)
    startWindow->SetAllocation(sf::FloatRect(0, 0, WIDTH, HEIGHT)); 
    
    // Set the flag
    isShown = true;
    
}

// ==================== USER INTERACTION ====================

void StartMenu::startGame() {
    
    // Play the button press sound
    Sound::playSound(Sound::BUTTON_PRESS);
    
    // Set the game state to the game screen
    controller->setGameState(GAME_SCREEN);
    
}

void StartMenu::openInstructionsWindow() {
    
    // Play the button press sound
    Sound::playSound(Sound::BUTTON_PRESS);
    
    // Open the instructions window through the controller which will auto hide the pause window
    controller->openInstructionsWindow();
    
}

void StartMenu::quitGame() {
    
    // Play the button press sound
    Sound::playSound(Sound::BUTTON_PRESS);
    
    // Invoke the coleing through the controller
    controller->exitGame();
    
}

// ==================== UPDATE ====================

void StartMenu::update(float deltaTime) {
	// Update the window GUI to keep it interactive
    GUIWindow::update(deltaTime);
}

void StartMenu::updateEvent(sf::Event event) {
	// Update the window GUI with new user input events
	GUIWindow::updateEvent(event);
}

// ==================== HIDE / CLOSE ====================

void StartMenu::hide() {
    
    // Hide the window by moving it off-screen
    startWindow->SetAllocation(sf::FloatRect(WIDTH, HEIGHT, WIDTH, HEIGHT)); 
    
    // Clear the flag
    isShown = false;
    
}

void StartMenu::close() {
    
    // Hide the window
    hide();
    
}

// ==================== DRAW ====================

void StartMenu::draw(sf::RenderWindow &window) {
    
    // If the screen is not shown yet and the instructions window is not currently open, show it before drawing
    if(!isShown && !controller->instructionsWindowIsOpen) {
        show();
    }
    
    // Draw the background image
    window.draw(backgroundSprite);
    
    // Draw the title text image
    window.draw(titleSprite);
    
    // Draw all the GUI element to the window
    GUIWindow::draw(window);
    
}
