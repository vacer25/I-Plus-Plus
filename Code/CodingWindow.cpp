// ==================== LOCAL INCLUDES ====================

#include "CodingWindow.h"

// ==================== STATIC VARIABLES ====================

bool CodingWindow::useMouseControl = true;
bool CodingWindow::isCurrentlyInAvailableLinesFrame = true;
bool CodingWindow::isCurrentlyInActivatedLineMode = false;
bool CodingWindow::infoWindowIsOpen = false;

CodeFrame* CodingWindow::availableCodeFrame = nullptr;
CodeFrame* CodingWindow::currentCodeFrame = nullptr;

sf::FloatRect CodingWindow::availableLinesOfCodeRect = sf::FloatRect(0, 0, 0, 0);
sf::FloatRect CodingWindow::currentLinesOfCodeRect = sf::FloatRect(0, 0, 0, 0);

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

CodingWindow::CodingWindow(Controller *_controller) : GUIWindow(_controller), infoWindowWasOpenOnHide(false) {
    
	// ==================== CREATE CODING WINDOW ====================

	// Create GUI for coding window
	codingWindow = sfg::Window::Create();
    
	// Create the main box
	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, WINDOW_PADDING);
    
	// Create the left and right box
	auto leftBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
	auto rightBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);

    float boxWidth = ((WIDTH - WINDOW_MARGIN_X * 2) - WINDOW_PADDING) / 2 - WINDOW_PADDING;
	leftBox->SetRequisition(sf::Vector2f(boxWidth, 0.0f));
	rightBox->SetRequisition(sf::Vector2f(boxWidth, 0.0f));
    
	// ==================== SET UP FONT(S) ====================

    // Set up the default font for all the labels
	mainDesktop.GetEngine().GetResourceManager().SetDefaultFont(consolaFont);
    
    // Set the font size of all elements
    mainDesktop.GetEngine().SetProperty("Label#LineOfCode", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
	mainDesktop.GetEngine().SetProperty("Frame#availableLinesFrame", "FontSize", GUI_WINDOW_TITLE_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Frame#currentLinesFrame", "FontSize", GUI_WINDOW_TITLE_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Frame#controlsFrame", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Label#controlsLabel", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Frame#instructionsFrame", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Label#instructionsLabel", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Button#controlButton", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Frame#infoFrame", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Label#infoLable", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("Button#infoButton", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    mainDesktop.GetEngine().SetProperty("CheckButton#mouseControlCheckButton", "FontSize", GUI_WINDOW_DEFAULT_FONT_SIZE);
    
    // Set the colors for the run and exit buttons
    mainDesktop.GetEngine().SetProperty("Button#runButton", "Color", sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR));
    mainDesktop.GetEngine().SetProperty("Button#exitButton", "Color", sf::Color(CODING_SCREEN_WARNING_COLOR));
    //mainDesktop.GetEngine().SetProperty("Button#runButton", "BorderColor", sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR));
    //mainDesktop.GetEngine().SetProperty("Button#exitButton", "BorderColor", sf::Color(CODING_SCREEN_WARNING_COLOR));
    
	// ==================== AVAILABLE LINES ====================

	// Create the available lines frame
	auto availableLinesFrame = sfg::Frame::Create("Available Lines");
	availableLinesFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    availableLinesFrame->SetId("availableLinesFrame");
    
	// Create the available lines scrollbox and scrollwindow
	auto availableLinesScrollBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
	auto availableLinesScrollWindow = sfg::ScrolledWindow::Create();
	availableLinesScrollWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

	// Add the available lines scrollbox to the available lines scrollwindow, add that to the frame, and add that to the main box
	availableLinesScrollWindow->AddWithViewport(availableLinesScrollBox);
	availableLinesFrame->Add(availableLinesScrollWindow);
	leftBox->Pack(availableLinesFrame, true, true);

    // Create the CodeFrame object for the available lines of code
    availableCodeFrame = new CodeFrame(true, availableLinesScrollBox);
    
	// ==================== CURRENT LINES ====================

	// Create the current lines frame
	auto currentLinesFrame = sfg::Frame::Create("Current Lines");
	currentLinesFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    currentLinesFrame->SetId("currentLinesFrame");
    
	// Create the current lines scrollbox and scrollwindow
	auto currentLinesScrollBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
	auto currentLinesScrollWindow = sfg::ScrolledWindow::Create();
	currentLinesScrollWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
    
	// Add the current lines scrollbox to the current lines scrollwindow, add that to the frame, and add that to the main box
	currentLinesScrollWindow->AddWithViewport(currentLinesScrollBox);
	currentLinesFrame->Add(currentLinesScrollWindow);
	rightBox->Pack(currentLinesFrame, true, true);

    // Create the CodeFrame object for the current lines of code
    currentCodeFrame = new CodeFrame(false, currentLinesScrollBox);
    
	// ==================== INSTRUCTIONS ====================

	// Create the instructions frame
	auto instructionsFrame = sfg::Frame::Create("Instructions");
	instructionsFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
	instructionsFrame->SetRequisition(sf::Vector2f(0.0f,CODING_SCREEN_BOTTOM_FRAMES_HEIGHT));
    instructionsFrame->SetId("instructionsFrame");
    
	// Create the instructions label
    instructionsLabel = sfg::Label::Create();
    instructionsLabel->SetText("You got this!");
	instructionsLabel->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    instructionsLabel->SetId("instructionsLabel");
    
    // Add the instructions label to the instructions frame and add instructions frame to the left box
    instructionsFrame->Add(instructionsLabel);
	leftBox->Pack(instructionsFrame, false, false);
    
	// ==================== CONTROLS ====================
    
    // Create the controls outer box to hold both the controls frame and buttons box
	auto controlsOuterBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, WINDOW_PADDING);
    
    // Create the controls box to hold both left and right controls labels
	auto controlsBox = sfg::Box::Create();
    
	// Create the controls frame
	auto controlsFrame = sfg::Frame::Create("Controls");
	controlsFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
	controlsFrame->SetRequisition(sf::Vector2f(0.0f, CODING_SCREEN_BOTTOM_FRAMES_HEIGHT));
    controlsFrame->SetId("controlsFrame");
    
	// Create the left controls label and add it to the controls box
    auto controlsLeftLabel = sfg::Label::Create();
    controlsLeftLabel->SetText(CODING_SCREEN_CONTROLS_LEFT_TEXT);
	controlsLeftLabel->SetAlignment(sf::Vector2f(0.0f, 0.5f));
    controlsLeftLabel->SetId("controlsLabel");
    controlsBox->Pack(controlsLeftLabel, true, true);
    
	// Create the right controls label and add it to the controls box
    auto controlsRightLabel = sfg::Label::Create();
    controlsRightLabel->SetText(CODING_SCREEN_CONTROLS_RIGHT_TEXT);
	controlsRightLabel->SetAlignment(sf::Vector2f(1.0f, 0.5f));
    controlsRightLabel->SetId("controlsLabel");
    controlsBox->Pack(controlsRightLabel, true, true);
    
    // Add the controls box to the controls frame and add that to the controls outer box
    controlsFrame->Add(controlsBox);
    controlsOuterBox->Pack(controlsFrame, true, true);
    
    // Create the controls buttons frame to hold the controls buttons box
	auto controlsButtonsFrame = sfg::Frame::Create("Actions");
	controlsButtonsFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    controlsButtonsFrame->SetId("controlsFrame");
    
    // Create the controls buttons box to hold the run and exit buttons
	auto controlsButtonBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);

    // Create the run button and add it to the controls buttons box
    auto runButton = sfg::Button::Create("Run");
    runButton->SetId("runButton");
    runButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->checkIfCurrentCodeIsCorrect();});
    runButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    controlsButtonBox->Pack(runButton, true, true);
    
    // Create the exit button and add it to the controls buttons box
    auto exitButton = sfg::Button::Create("Exit");
    exitButton->SetId("exitButton");
    exitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->close();});
    exitButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    controlsButtonBox->Pack(exitButton, true, true);
    
    auto mouseControlCheckButton = sfg::CheckButton::Create("Use mouse");
    mouseControlCheckButton->SetId("mouseControlCheckButton");
    mouseControlCheckButton->SetActive(true);
    mouseControlCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, &mouseControlCheckButton] {
        Sound::playSound(Sound::CHECK_BUTTON_PRESS);
		this->useMouseControl = !this->useMouseControl;
	});
    mouseControlCheckButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    controlsButtonBox->Pack(mouseControlCheckButton, true, true);
    
    // Add the controls button box to the controls buttons frame and add that to the controls outer box and add that to the right box
    controlsButtonsFrame->Add(controlsButtonBox);
    controlsOuterBox->Pack(controlsButtonsFrame, false, false);
	rightBox->Pack(controlsOuterBox, false, false);
    
	// ==================== SET UP CODING WINDOW ====================
    
	// Add left and right boxes to the main box
	mainBox->Pack(leftBox, true, false);
	mainBox->Pack(rightBox, true, false);

	// Add the main box to the coding window
	codingWindow->Add(mainBox);
   
	// Disable resizing
	codingWindow->SetStyle(codingWindow->GetStyle() ^ sfg::Window::TITLEBAR);
	codingWindow->SetStyle(codingWindow->GetStyle() ^ sfg::Window::RESIZE);

    // Add the coding window to the coding desktop to enable setting widget preperties
	mainDesktop.Add(codingWindow);

    // ==================== CREATE CODING INFO WINDOW ====================

	// Create GUI for coding window
	codingInfoWindow = sfg::Window::Create();
    
	// Create the main info box
	auto mainInfoBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, WINDOW_PADDING);
    
    // Create the info frame to hold the info label and add it to the info box
	auto infoFrame = sfg::Frame::Create("Info");
	infoFrame->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    infoFrame->SetId("infoFrame");
    mainInfoBox->Pack(infoFrame, true, true);

    // Create the info label and add it to the info frame
	infoLabel = sfg::Label::Create();
	infoLabel->SetAlignment(sf::Vector2f(0.5f, 0.5f));
    infoLabel->SetId("infoLable");
    infoFrame->Add(infoLabel);
    
    // Create the continue button and add it to the main info box
    infoButton = sfg::Button::Create("Continue");
    infoButton->SetId("infoButton");
    infoButton->GetSignal(sfg::Widget::OnMouseEnter).Connect([] {Sound::playSound(Sound::BUTTON_HOVER);});
    mainInfoBox->Pack(infoButton, false, true);
    
	// Add the main info box to the coding info window
	codingInfoWindow->Add(mainInfoBox);
   
	// Disable resizing
	codingInfoWindow->SetStyle(codingInfoWindow->GetStyle() ^ sfg::Window::TITLEBAR);
	codingInfoWindow->SetStyle(codingInfoWindow->GetStyle() ^ sfg::Window::RESIZE);
    
    // Add the coding info window to the coding desktop to enable setting widget preperties, hiding it because it is now needed yet
	mainDesktop.Add(codingInfoWindow);
    //hideInfoWindow();
    
	// ==================== INITIALIZATION ====================
    
    // Temporarly show the coding window to get the correct on-screen positions
    show();
    
    // Get the layout rectangles of the available and current lines frames for mouse input
    availableLinesOfCodeRect = availableLinesFrame->GetAllocation();
    availableLinesOfCodeRect.left += availableLinesFrame->GetAbsolutePosition().x;
    availableLinesOfCodeRect.top += availableLinesFrame->GetAbsolutePosition().y;
    currentLinesOfCodeRect = currentLinesFrame->GetAllocation();
    currentLinesOfCodeRect.left += currentLinesFrame->GetAbsolutePosition().x;
    currentLinesOfCodeRect.top += currentLinesFrame->GetAbsolutePosition().y;
    
    // Prepare coding window
    initializeCodingWindowForNewQuestion(0, true);
    
    // Get the line dimensions, only needed once
    availableCodeFrame->initialize(availableLinesScrollWindow);
    currentCodeFrame->initialize(currentLinesScrollWindow);
    
    // Hide the window when it is first created
    hide();
    
}

CodingWindow::~CodingWindow() {
	// TODO Implement destructor
}
// ==================== SHOW ====================

void CodingWindow::show() {
        
    // Static variable to keep track of the previous question number
    static int prev_questionNumber = -1;
    
    // If the game state was switched, the last question state becomes invalid
    if(controller->gameStateDidChage_CodingWindow) {
        prev_questionNumber = -1;
        infoWindowWasOpenOnHide = false;
        controller->gameStateDidChage_CodingWindow = false;
    }
    
    // Get the solved status of the current question
    currentSequenceIsCorrect = controller->getQuestionSolvedStatus(currentQuestionNumber);
    
    // Set the correct position and size of the coding window to be on screen
	codingWindow->SetAllocation(sf::FloatRect(WINDOW_MARGIN_X, WINDOW_MARGIN_Y, WIDTH - WINDOW_MARGIN_X * 2, HEIGHT - WINDOW_MARGIN_Y * 2));
    
    // If the question is already solved, open the info window to indicate so and prevent re-solving the question
    // This way, once a question is solved, the coding window just shows that it was solved, preventing further interaction
    // Also, if the coding window was open when the coding window was being hidded (either on closing or on overlaying another GUI window on top) 
    // and the same question is reterned to, open the info window again (restore its state)
    if(currentSequenceIsCorrect || (infoWindowWasOpenOnHide && currentQuestionNumber == prev_questionNumber)) {
        openInfoWindow();
    }
    // Otherwise, the info window is hidden as the start
    else {
        hideInfoWindow();
    }
    
    // Record the previous question number
    prev_questionNumber = currentQuestionNumber;
    
    // Set the flag
    isShown = true;
    
}

// ==================== INITIALIZATION ====================

void CodingWindow::initializeCodingWindowForNewQuestion(int questionNumber, bool firstTimeOpeningCodingWindow) {

    currentQuestionNumber = questionNumber;

    // Get the solved status of the current question
    currentSequenceIsCorrect = controller->getQuestionSolvedStatus(currentQuestionNumber);
    
    // Static variable to keep track of the previous question number
    static int prev_questionNumber = -1;
    
    // Only load lines and re-initialize if the previous question was different than the current one or this is the first time the coding window is opened
    if(questionNumber != prev_questionNumber || firstTimeOpeningCodingWindow) {
    
        // By default, the coding window starts with the available lines frame and line 0 being selected
        isCurrentlyInAvailableLinesFrame = true;
        isCurrentlyInActivatedLineMode = false;
        
        // Load the lines of code for the current question
        loadQuestionData(questionNumber);
        
        // Add all the available and current lines of code
        initializeLinesOfCode();
    
    }
    
    // Record the previous question number
    prev_questionNumber = questionNumber;
    
}

void CodingWindow::loadQuestionData(int questionNumber) {

    // Open the file for the current question
    std::ifstream questionFile(std::string(QUESTIONS_DIRECTORY) + std::to_string(questionNumber) + ".txt");
    
    // If it could not be opened, throw an exception
    if(!questionFile.is_open()) {
        // TODO: catch this exception!
        throw std::invalid_argument("Could not load question file for question " + std::to_string(questionNumber));
    }
    
    // Prepare variables
    std::string currentLine;
    int readingStage = -1;
    std::string instructionsText = "";
    std::vector<std::string> targetLinesOfCode;
    
    // Remove all the current lines of code to prevent duplicates
    availableLinesOfCode.clear();
    currentLinesOfCode.clear();
    
    // While the end of the file is not reached
    while(questionFile.good()) {

        // Read the current line, if it is empty, skip it
        getline(questionFile, currentLine);
        if(currentLine.empty()) {
            continue;
        }
        
        // Determine what vector to put the current line into
        if(currentLine == "[Target]") {
            readingStage = 0;            
        } else if(currentLine == "[Available]") {
            // Once all the target lines have been read in, set the number of target lines in the current lines crame to the number of target lines read in
            currentCodeFrame->setNumberOfTargetLines(targetLinesOfCode.size());
            readingStage = 1;            
        } else if(currentLine == "[Current]") {
            readingStage = 2;            
        } else if(currentLine == "[Instructions]") {
            readingStage = 3;            
        } else {
            
            // Read the current line in to the target lines vector
            if(readingStage == 0) {
                targetLinesOfCode.push_back(currentLine);
            }
            // Read the current line into the available lines vector, where [All] means all target lines and [None] means no lines
            else if(readingStage == 1) {
                // Only load the available lines if the current question is not solved yet
                if(!currentSequenceIsCorrect) {
                    // If the [ALL] keyword is used, copy all the target lines to the available lines, preserving the sequence ID
                    if(currentLine == "[All]") {
                        unsigned int sequenceID = 0;
                        for(std::string lineOfCode : targetLinesOfCode) {
                            availableLinesOfCode.push_back(new LineOfCode(lineOfCode, sequenceID));
                            sequenceID++;
                        }
                    }
                    // If the [None] keyword is used, the available lines stays empty
                    else if(currentLine == "[None]") {
                        continue;
                    }
                    // Otherwise, read input line-by line
                    else {
                        // If the current line is just a number, add the target line at that index as an available line, with that sequence ID
                        if(std::all_of(currentLine.begin(), currentLine.end(), ::isdigit)) {
                            unsigned int sequenceID = std::stoi(currentLine);
                            if(sequenceID < targetLinesOfCode.size()) {
                                availableLinesOfCode.push_back(new LineOfCode(targetLinesOfCode[sequenceID], sequenceID));
                            }
                        }
                        // Otherwise, add the current line as text with a sequence ID of -1 since this line is not part of the solution
                        else {
                            availableLinesOfCode.push_back(new LineOfCode(currentLine, -1));
                        }
                    }
                }
            }
            // Read the current line into the current lines vector, where [All] means all target lines and [None] means no lines
            else if(readingStage == 2) {
                // If the question is already solved or the [ALL] keyword is used, copy all the target lines to the current lines, preserving the sequence ID
                if((currentSequenceIsCorrect && currentLinesOfCode.empty()) || (!currentSequenceIsCorrect && currentLine == "[All]")) {
                    unsigned int sequenceID = 0;
                    for(std::string lineOfCode : targetLinesOfCode) {
                        currentLinesOfCode.push_back(new LineOfCode(lineOfCode, sequenceID));
                        sequenceID++;
                    }
                } 
                // Only load the available lines if the current question is not solved yet
                if(!currentSequenceIsCorrect) {
                    // If the [None] keyword is used, the current lines stays empty
                    if(currentLine == "[None]") {
                        continue;
                    }
                    // Otherwise, read input line-by line
                    else {
                        // If the current line is just a number, add the target line at that index as a current line, with that sequence ID
                        if(std::all_of(currentLine.begin(), currentLine.end(), ::isdigit)) {
                            unsigned int sequenceID = std::stoi(currentLine);
                            if(sequenceID < targetLinesOfCode.size()) {
                                currentLinesOfCode.push_back(new LineOfCode(targetLinesOfCode[sequenceID], sequenceID));
                            }
                        }
                        // Otherwise, add the current line as text with a sequence ID of -1 since this line is not part of the solution
                        else {
                            currentLinesOfCode.push_back(new LineOfCode(currentLine, -1));
                        }
                    }
                }
            }
            // Read the instructions text in to the instructions text string line by line
            if(readingStage == 3) {
                instructionsText += (instructionsText.empty() ? "" : "\n") + currentLine;
            }
            
        }
        
    }
    
    // Set the instructions label text to the read in instructions text
    instructionsLabel->SetText(instructionsText);
        
    // Close the question file
    questionFile.close();
    
}
    
void CodingWindow::initializeLinesOfCode() {
    
    // Remove all the current lines of code to prevent duplicates
    availableCodeFrame->removeAllLinesOfCode();
    currentCodeFrame->removeAllLinesOfCode();
    
    // The index of the current line
    int lineIndex = 0;
    
    // Add labels to the available lines and add all the available lines to the available code frame
    for(LineOfCode* currentAvailableLineOfCode : availableLinesOfCode) {
		auto lineLabel = sfg::Label::Create();
        currentAvailableLineOfCode->setLable(lineLabel, "LineOfCode");
        availableCodeFrame->addLineOfCodeAtEnd(currentAvailableLineOfCode);
        lineIndex++;
    }
    
    // Add labels to the current lines and add all the current lines to the current code frame
    for(LineOfCode* currentCurrentLineOfCode : currentLinesOfCode) {
		auto lineLabel = sfg::Label::Create();
        currentCurrentLineOfCode->setLable(lineLabel, "LineOfCode");
        currentCodeFrame->addLineOfCodeAtEnd(currentCurrentLineOfCode);
        lineIndex++;
    }
    
    // Reset the selected line position and insertion point to the start 
    availableCodeFrame->reInitialize();
    currentCodeFrame->reInitialize();
    
}

// ==================== UPDATE ====================

void CodingWindow::update(float deltaTime) {
    
    // Only update the main coding window if the info window is closed
    if(!infoWindowIsOpen) {
        
        // Handle entering/exiting activated line mode
        bool didJustEnterActivatedLineMode = updateActivatedLineMode();
        
        // Handle switching the currently selected frame
        updateCurrentlySelectedFrame();
        
        // Update the code frames to handle selection and movement of lines (can move lines only if activated line mode was not entered on this frame)
        availableCodeFrame->update(!didJustEnterActivatedLineMode);
        currentCodeFrame->update(!didJustEnterActivatedLineMode);
        
        // If the action key was pressed in while out of activated lines mode, check if the current code frame contains the correct sequence of lines to solve the question
        if(!isCurrentlyInActivatedLineMode && Controller::keyActionIsPressed) {
            checkIfCurrentCodeIsCorrect();
        }

    }
    // If the info window is open, keep it info window in the front
    else {
        mainDesktop.BringToFront(codingInfoWindow);
        
        // If the action key was pressed, close the info window if the current code is not correct and the whole coding window if it is correct
        if(Controller::keyActionIsPressed) {
            if(currentSequenceIsCorrect) {
                close();
            } else {
                hideInfoWindow();
            }
        }
        
    }
    
    // If the escape key is pressed, exit
    if(Controller::keyEscapeIsPressed) {
        close();
    }
    
	// Update the coding window GUI to keep it interactive
    GUIWindow::update(deltaTime);
    
}

void CodingWindow::updateEvent(sf::Event event) {
	// Update the coding window GUI with new user input events
	GUIWindow::updateEvent(event);
}

// ==================== USER INTERACTION ====================

void CodingWindow::checkIfCurrentCodeIsCorrect() {
    
    // If the run button is pressed while the info window is open, don't do anything
    if(infoWindowIsOpen) {
        return;
    }
    
    // Check if the current code frame contains the correct sequence of lines to solve the question
    currentSequenceIsCorrect = currentCodeFrame->hasCorrectSequenceOfLines();
    //std::cout << "Correct: " << sequenceIsCorrect << std::endl;
    
    // If the current sequence was correct, indicate that the question was solved in the controller
    if(currentSequenceIsCorrect) {
        controller->setQuestionSolvedStatus(true, currentQuestionNumber);
    }
    
    // Open the info window
    openInfoWindow();
    
}

bool CodingWindow::updateActivatedLineMode() {
    
    // Activated line mode was just switched
    bool activatedLineModeWasJustSwitched = false;
    
    // Update using mouse if mouse controls are active
    if(useMouseControl) {
     
        // If the left mouse button is pressed
        if(Controller::mouseLeftIsPressed) {
            // If the activated line mode is not entered, enter it
            if(!isCurrentlyInActivatedLineMode) {
                // Only enter the activated line mode if the currently selected frome is not empty and the mouse is currently over it
                if((isCurrentlyInAvailableLinesFrame && isMouseOverAvailableCodeFrame() && !availableCodeFrame->isEmpty()) || (!isCurrentlyInAvailableLinesFrame && isMouseOverCurrentCodeFrame() && !currentCodeFrame->isEmpty())) {
                    isCurrentlyInActivatedLineMode = true;
                    activatedLineModeWasJustSwitched = true;
                }
            }
        }
        
        // If the left mouse button is released
        else if(Controller::mouseLeftIsReleased) {
            // If the activated line mode is entered
            if(isCurrentlyInActivatedLineMode) {
                // Only exit the activated line mode if the mouse is currently not over the non-selected frome (cancel line movement)
                if((isCurrentlyInAvailableLinesFrame && !CodingWindow::isMouseOverCurrentCodeFrame()) || (!isCurrentlyInAvailableLinesFrame && !CodingWindow::isMouseOverAvailableCodeFrame())) {
                    isCurrentlyInActivatedLineMode = false;
                    activatedLineModeWasJustSwitched = true;
                }
            }
        }
   
    }
    // Otherwise update using the keyboard
    else {
        
        // If the select key is pressed
        if(Controller::keySelectIsPressed) {
            // If the activated line mode is not entered, enter it
            if(!isCurrentlyInActivatedLineMode) {
                // Only enter the activated line mode if the currently selected frome is not empty
                if((isCurrentlyInAvailableLinesFrame && !availableCodeFrame->isEmpty()) || (!isCurrentlyInAvailableLinesFrame && !currentCodeFrame->isEmpty())) {
                    isCurrentlyInActivatedLineMode = true;
                    activatedLineModeWasJustSwitched = true;
                }
            }
        }
        
        // If the deselect key is pressed
        else if(Controller::keyDeselectIsPressed) {
            // If the activated line mode is entered, exit it (cancel line movement)
            if(isCurrentlyInActivatedLineMode) {
                isCurrentlyInActivatedLineMode = false;
                activatedLineModeWasJustSwitched = true;
            }
        }

    }
    
    // If there is currently an activated line
    if(isCurrentlyInActivatedLineMode) {
    
        // If the line was just activated, set the frame border color to indicate the activation has happened
        if(activatedLineModeWasJustSwitched) {
            // If the available lines frame is selected, this means an available line is being placed into the current lines frame
            if(isCurrentlyInAvailableLinesFrame) {
                mainDesktop.GetEngine().SetProperty("Frame#availableLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR));  
            } 
            // If the current lines frame is selected, this means a current line is being placed back into the available lines frame
            else {
                mainDesktop.GetEngine().SetProperty("Frame#currentLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR));
            }
        }
        
    }
    
    // Return true is the activated line mode was just entered
    return isCurrentlyInActivatedLineMode && activatedLineModeWasJustSwitched;
    
}

void CodingWindow::updateCurrentlySelectedFrame() {
    
    // Flag to determine if this is the first time the frames are updated
    static bool prev_isCurrentlyInActivatedLineMode = !isCurrentlyInActivatedLineMode;
        
    // Flag to indicate if the current frame was switched (true on the first time)
    bool frameWasJustSwitched = (prev_isCurrentlyInActivatedLineMode != isCurrentlyInActivatedLineMode);
        
    // Update using mouse if mouse controls are active
    if(useMouseControl) {
        
        // If activated line mode is not active and was not just switched, the currently selected frame can be switched
        if(!frameWasJustSwitched && !isCurrentlyInActivatedLineMode) {
        
            // Determine which frame the mouse if over
            if(isMouseOverAvailableCodeFrame()) {
                isCurrentlyInAvailableLinesFrame = true;
            } else if(isMouseOverCurrentCodeFrame()) {
                isCurrentlyInAvailableLinesFrame = false;
            }
            
            // Update the frame border colors if the active frame changed
            setActiveFrame(false);
        
        }
        
    }
    // Otherwise update using the keyboard
    else {
        
        // If no key was pressed and activated line mode status did not change, don't update anything
        if(prev_isCurrentlyInActivatedLineMode == isCurrentlyInActivatedLineMode) {
            if(!Controller::keyLeftIsPressed && !Controller::keyRightIsPressed) {
                return;
            }
        }
        
        // If activated line mode is not active, the currently selected frame can be switched
        if(!isCurrentlyInActivatedLineMode) {
            
            // If the left / right keys are pressed, move the currently selected frame between available and current lines
            if(Controller::keyLeftIsPressed && !isCurrentlyInAvailableLinesFrame) {
                isCurrentlyInAvailableLinesFrame = true;
                frameWasJustSwitched = true;
            } else if(Controller::keyRightIsPressed && isCurrentlyInAvailableLinesFrame) {
                isCurrentlyInAvailableLinesFrame = false;
                frameWasJustSwitched = true;
            }
            
        }
        
    }
    
    // If the currently selected frame was just switched and activated line mode is deactivated, update the frame border colors to their normal colors
    if(frameWasJustSwitched && !isCurrentlyInActivatedLineMode) {
        setActiveFrame(true);
    }
    
    // Keep track of the last activated line mode status
    prev_isCurrentlyInActivatedLineMode = isCurrentlyInActivatedLineMode;
    
}

void CodingWindow::setActiveFrame(bool forceUpdate) {
    
    // Flag to determine if this is the first time the frames are switched
    static bool prev_isCurrentlyInAvailableLinesFrame = !isCurrentlyInAvailableLinesFrame;
    
    // If need to force update or the active frames are switched, update the frame border colors
    if(forceUpdate || (prev_isCurrentlyInAvailableLinesFrame != isCurrentlyInAvailableLinesFrame)) {
    
        if(isCurrentlyInAvailableLinesFrame) {
            // Set the available lines frame border color to selected and current lines frame border color to default
            mainDesktop.GetEngine().SetProperty("Frame#availableLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR));
            mainDesktop.GetEngine().SetProperty("Frame#currentLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_DEFAULT_LINE_COLOR));            
        } else {
            // Set the available lines frame border color to default and current lines frame border color to selected
            mainDesktop.GetEngine().SetProperty("Frame#availableLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_DEFAULT_LINE_COLOR));
            mainDesktop.GetEngine().SetProperty("Frame#currentLinesFrame", "BorderColor", sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR));
        }
    
    }
    
    // Keep track of the last active frame
    prev_isCurrentlyInAvailableLinesFrame = isCurrentlyInAvailableLinesFrame;
    
}

bool CodingWindow::isMouseOverAvailableCodeFrame() {
    return availableLinesOfCodeRect.contains(Controller::mousePosition);
}

bool CodingWindow::isMouseOverCurrentCodeFrame() {
    return currentLinesOfCodeRect.contains(Controller::mousePosition);
}

void CodingWindow::openInfoWindow() {
    
    // Remove the left click signal ID to always keep only one function assigned to the info button
    static int infoButtonSignalID = -1;
    infoButton->GetSignal(sfg::Widget::OnLeftClick).Disconnect(infoButtonSignalID);
    
    if(currentSequenceIsCorrect) {
        
        // Set the info frame color to green
        mainDesktop.GetEngine().SetProperty("Frame#infoFrame", "BorderColor", sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR));
        
        // Set the info label text to indicate that the current code is correct
        infoLabel->SetText(CODING_SCREEN_INFO_CORRECT_TEXT);
        
        // Set the info button to close the whole coding window
        infoButtonSignalID = infoButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->close();});
		
	// PLay the correct solution sound
	if(controller->getGameState() != INITIALIZE) {
		Sound::playSound(Sound::CODING_PROBLEM_CORRECT);
	}
		
    } else {
        
        // Set the info frame color to red
        mainDesktop.GetEngine().SetProperty("Frame#infoFrame", "BorderColor", sf::Color(CODING_SCREEN_WARNING_COLOR));
        
        // Set the info label text to indicate that the current code is not correct
        infoLabel->SetText(CODING_SCREEN_INFO_INCORRECT_TEXT);
        
        // Set the info button to set the flag to close the coding info window
        infoButtonSignalID = infoButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->hideInfoWindow();});
        
	// PLay the incorrect solution sound
	if(controller->getGameState() != INITIALIZE) {
		Sound::playSound(Sound::CODING_PROBLEM_INCORRECT);
	}
		
    }
    
    // Open the coding info window and set the flag to indicate that it's open to ignore keyboard controls and the run button
    mainDesktop.BringToFront(codingInfoWindow);
    codingInfoWindow->SetAllocation(sf::FloatRect((WIDTH - CODING_SCREEN_INFO_BOX_WIDTH) / 2, (HEIGHT - CODING_SCREEN_INFO_BOX_HEIGHT) / 2, CODING_SCREEN_INFO_BOX_WIDTH, CODING_SCREEN_INFO_BOX_HEIGHT));
    infoWindowIsOpen = true;
    
}

void CodingWindow::hideInfoWindow() {
    
    //mainDesktop.Remove(codingInfoWindow);
    codingInfoWindow->SetAllocation(sf::FloatRect(WIDTH, HEIGHT, CODING_SCREEN_INFO_BOX_WIDTH, CODING_SCREEN_INFO_BOX_HEIGHT));
    mainDesktop.BringToFront(codingWindow);
    mainDesktop.Refresh();
    infoWindowIsOpen = false;
    
}

// ==================== HIDE / CLOSE ====================

void CodingWindow::hide() {
    
    // If the window is already hidden, don't do anything
    if(!isShown) {
        return;
    }
    
    // Record if the info window was oben before hiding the coding window
    // Required to restore the info window's state the next time the coding window is opened to the same question
    infoWindowWasOpenOnHide = infoWindowIsOpen;
    
    // Hide the info window so it does now show up the next time the coding window is opened
    hideInfoWindow();
    
    // Hide the coding window by moving it off-screen
	codingWindow->SetAllocation(sf::FloatRect(WIDTH, HEIGHT, WIDTH - WINDOW_MARGIN_X * 2, HEIGHT - WINDOW_MARGIN_Y * 2));
    
    // Clear the flag
    isShown = false;
    
}

void CodingWindow::close() {
    
    // Hide the window
    hide();
    
    // Hide the coding window
    controller->codingWindowIsOpen = false;
}

// ==================== DRAW ====================

void CodingWindow::draw(sf::RenderWindow &window) {
        
    // If the screen is not shown yet (its not in the correct position on screen), show it before drawing
    if(!isShown) {
        show();
    }
    
	// Draw all the GUI element to the window
	GUIWindow::draw(window);
    availableCodeFrame->draw(window);
    currentCodeFrame->draw(window);
    
}

// ==================== CODE FRAME CLASS ====================

CodeFrame::CodeFrame(bool _isAvailableLinesFrame, sfg::Box::Ptr _linesScrollBox) : isAvailableLinesFrame(_isAvailableLinesFrame), linesScrollBox(_linesScrollBox), numberOfLines(0) {}

CodeFrame::~CodeFrame() { /* TODO */ }

// ==================== INITIALIZE ====================

void CodeFrame::initialize(sfg::ScrolledWindow::Ptr linesScrollWindow) {
    
    // Calculate the positions of the available lines
    linesStartPoint = linesScrollWindow->GetAbsolutePosition();
    
    // If this is the available code frame, get the line dimensions from the position of the 2nd line of code's label
    if(isAvailableLinesFrame) {
        if(numberOfLines < 2) {
            // TODO: catch this exception!
            throw std::length_error("At least 2 lines of code must be present in the available lines frame!");
        }
        linesDimensions = sf::Vector2f(linesScrollWindow->GetAllocation().width , linesOfCode[1]->getLable()->GetAbsolutePosition().y);
    } 
    // If the is is the current code frame, get the line y dimensions from the available code frame's line dimensions since the current code frame may be initially empty
    else {
        linesDimensions = sf::Vector2f(linesScrollWindow->GetAllocation().width , CodingWindow::availableCodeFrame->getLinesDimensions().y);
    }
    
}

void CodeFrame::reInitialize() {
    
    // Reset the selected line position and insertion point to the start of the list of lines of code
    currentlySelectedLine = 0;
    currentlySelectedLineInsertion = 0;
    
}

// ==================== UPDATE ====================

void CodeFrame::update(bool canMoveLineNow) {
    
    // Update using mouse if mouse controls are active
    if(CodingWindow::useMouseControl) {
        
        // If the coding window is not in activated line mode
        if(!CodingWindow::isCurrentlyInActivatedLineMode) {
            
            // If the mouse is currently over the selected frame, update the currently selected line index
            if(isAvailableLinesFrame == CodingWindow::isCurrentlyInAvailableLinesFrame && isAvailableLinesFrame == CodingWindow::isMouseOverAvailableCodeFrame() && isAvailableLinesFrame != CodingWindow::isMouseOverCurrentCodeFrame()) {
            
                // Set the selected line to the one with the mouse over it
                currentlySelectedLine = (Controller::mousePosition.y - linesStartPoint.y) / linesDimensions.y;
                
                // Make sure the selection stays in range
                if(currentlySelectedLine >= numberOfLines) {
                    currentlySelectedLine = std::max(numberOfLines - 1, 0);
                } else if(currentlySelectedLine < 0) {
                    currentlySelectedLine = 0;
                }
            
            }
            
        }
        // If the coding window is in activated line mode
        else {
            
            // If the mouse is currently over the non-selected frame
            if(isAvailableLinesFrame == CodingWindow::isMouseOverAvailableCodeFrame() && isAvailableLinesFrame != CodingWindow::isMouseOverCurrentCodeFrame()) {
            
                // In the non-selected frame, update the currently selected insertion point index
                if(isAvailableLinesFrame != CodingWindow::isCurrentlyInAvailableLinesFrame) {
                
                    // Set the currently selected insertion point index to the one with the mouse over it
                    currentlySelectedLineInsertion = (Controller::mousePosition.y - linesStartPoint.y + linesDimensions.y / 2) / linesDimensions.y;
                    
                    // Make sure the selection stays in range
                    if(currentlySelectedLineInsertion >= numberOfLines + 1) {
                        currentlySelectedLineInsertion = std::max(numberOfLines, 0);
                    } else if(currentlySelectedLineInsertion < 0) {
                        currentlySelectedLineInsertion = 0;
                    }
                
                    // If the left mouse button is released
                    if(Controller::mouseLeftIsReleased) {
                        // If the activated line mode was already entered on a previous frame, place it into the other frame (available -> current or current -> available) and exit activated line mode
                        if(canMoveLineNow) {
                            // Call the move line function in the selected frame to move the line to the currently non-selected frame
                            if(isAvailableLinesFrame) {
                                CodingWindow::currentCodeFrame->moveCurrentlySelectedLineOfCode();
                            } else {
                                CodingWindow::availableCodeFrame->moveCurrentlySelectedLineOfCode();
                            }                            
                        }
                    }
                    
                }
                
            }
            
        }
        
    }
    // Otherwise update using the keyboard
    else {
        
        // If no key was pressed, don't do anything
        if(!Controller::keyUpIsPressed && !Controller::keyDownIsPressed && !Controller::keySelectIsPressed) {
            return;
        }
        
        // If the coding window is in activated line mode and the select key was not pressed, don't change the selected line in the currently selected frame
        if(CodingWindow::isCurrentlyInActivatedLineMode && isAvailableLinesFrame == CodingWindow::isCurrentlyInAvailableLinesFrame && !Controller::keySelectIsPressed) {
            return;   
        }
        
        // If the coding window is not in activated line mode, don't change the selected line in the non-selected selected frame
        if(!CodingWindow::isCurrentlyInActivatedLineMode && isAvailableLinesFrame != CodingWindow::isCurrentlyInAvailableLinesFrame) {
            return;   
        }
        
        // If the coding window is in activated line mode, change line insertion point, otherwise, change currently selected line
        int allowInsertingAtLastIndex = 0;
        int *currentSelectionToChange = &currentlySelectedLine;
        if(CodingWindow::isCurrentlyInActivatedLineMode) {
            // In the activated line mode, the selection of the insertion point can be at the last index, so 1 must be added to the number of lines
            currentSelectionToChange = &currentlySelectedLineInsertion;
            allowInsertingAtLastIndex = 1;
        }
        
        // If the up / down keys are pressed, move the current selection up/down
        if(Controller::keyDownIsPressed) {
            (*currentSelectionToChange)++;
        } else if(Controller::keyUpIsPressed) {
            (*currentSelectionToChange)--;
        }
        
        // Make sure the selection stays in range
        if(*currentSelectionToChange >= numberOfLines + allowInsertingAtLastIndex) {
    #ifdef USE_CODING_SCREEN_LINE_SELECTION_WRAP_AROUND
            *currentSelectionToChange = 0;
    #else
            *currentSelectionToChange = std::max(numberOfLines - 1 + allowInsertingAtLastIndex, 0);
    #endif
        } else if(*currentSelectionToChange < 0 + allowInsertingAtLastIndex) {
    #ifdef USE_CODING_SCREEN_LINE_SELECTION_WRAP_AROUND
            *currentSelectionToChange = std::max(numberOfLines - 1 + allowInsertingAtLastIndex, 0);
    #else
            *currentSelectionToChange = 0;
    #endif
        }
        
        // If the select key is pressed and the current frame is selected
        if(Controller::keySelectIsPressed && isAvailableLinesFrame == CodingWindow::isCurrentlyInAvailableLinesFrame) {
            
            // If the activated line mode was already entered on a previous frame, place it into the other frame (available -> current or current -> available) and exit activated line mode
            if(CodingWindow::isCurrentlyInActivatedLineMode && canMoveLineNow) {
                moveCurrentlySelectedLineOfCode();
            }
            
        }
    
    }
    
}

void CodeFrame::moveCurrentlySelectedLineOfCode() {
    
    // Get the line of code to move
    LineOfCode *lineOfCodeToMove = linesOfCode[currentlySelectedLine];
    
    // Remove it from the current frame
    removeLineOfCode(currentlySelectedLine);
    
    // If the last line was moved, set the selected line to the new last one
    if(currentlySelectedLine >= numberOfLines) {
        currentlySelectedLine = std::max(numberOfLines - 1, 0);
    }
    
    // Add it to the other frame at its insertion point
    // Increment its current insertion point to automatically move on to the next line
    if(isAvailableLinesFrame) {
        CodingWindow::currentCodeFrame->addLineOfCode(lineOfCodeToMove);
        CodingWindow::currentCodeFrame->incrementCurrentlySelectedLineInsertion();
    } else {
        CodingWindow::availableCodeFrame->addLineOfCode(lineOfCodeToMove);
        CodingWindow::availableCodeFrame->incrementCurrentlySelectedLineInsertion();
    }
    
    // Exit activated line mode
    CodingWindow::isCurrentlyInActivatedLineMode = false;
    
}

// ==================== DRAW ====================

void CodeFrame::draw(sf::RenderWindow &window) {
    
    // If the info window is open, don't draw any selections
    if(CodingWindow::infoWindowIsOpen) {
        return;
    }

    // If this frame is not selected and the coding window is in activated line mode, draw the insertion selection line where the line from the currently selected frame will be inserted
    if(CodingWindow::isCurrentlyInActivatedLineMode && isAvailableLinesFrame != CodingWindow::isCurrentlyInAvailableLinesFrame) {
        sf::Vertex insertionSelectionLine[2];
        insertionSelectionLine[0].position = sf::Vector2f(linesStartPoint.x, linesStartPoint.y + linesDimensions.y * currentlySelectedLineInsertion);
        insertionSelectionLine[0].color  = sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR);
        insertionSelectionLine[1].position = sf::Vector2f(linesStartPoint.x + linesDimensions.x, linesStartPoint.y + linesDimensions.y * currentlySelectedLineInsertion);
        insertionSelectionLine[1].color = sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR);
        window.draw(insertionSelectionLine, 2, sf::LinesStrip);
        
    }
    
    // If this frame is selected
    if(isAvailableLinesFrame == CodingWindow::isCurrentlyInAvailableLinesFrame) {
        
        // Set the outline and fill colors of the selection rectange to the right colors depending on if the coding window is in activated line mode or not
        sf::Color fillColor = sf::Color(sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR, CODING_SCREEN_LINE_SELECTION_TRANSPARANCY));
        sf::Color outlineColor = sf::Color(CODING_SCREEN_SELECTED_LINE_COLOR);
        if(CodingWindow::isCurrentlyInActivatedLineMode) {
            fillColor = sf::Color(sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR, CODING_SCREEN_LINE_SELECTION_TRANSPARANCY));
            outlineColor = sf::Color(CODING_SCREEN_ACTIVATED_LINE_COLOR);
        }
        
        // Draw the selection rectange on top of the currently selected line
        sf::RectangleShape currentSelectionRectange;
        currentSelectionRectange.setSize(sf::Vector2f(linesDimensions.x, linesDimensions.y));
        currentSelectionRectange.setOutlineThickness(1);
        currentSelectionRectange.setFillColor(fillColor);
        currentSelectionRectange.setOutlineColor(outlineColor);
        currentSelectionRectange.setPosition(sf::Vector2f(linesStartPoint.x, linesStartPoint.y + linesDimensions .y * currentlySelectedLine));
        window.draw(currentSelectionRectange);
        
    }
    
}

// ==================== CHECK FOR CORRECT SEQUENCE OF LINES ====================

bool CodeFrame::hasCorrectSequenceOfLines() {
    
    // Only check the correct sequence on the current lines code frame
    if(isAvailableLinesFrame) {
        return false;
    }
    
    // If the current number of lines is not the target number of lines, current code frame does not have the correct sequence of lines
    if(numberOfLines != numberOfTargetLines) {
        return false;
    }
    
    // Sequence counter
    int currentExpectedSequence = 0;
    
    // Loop over all the lines of code in the current code frame
    for(LineOfCode *lineOfCode: linesOfCode) {
        // If a lines does not match the expected sequence, the current code frame does not have the correct sequence of lines
        if(lineOfCode->getSequenceID() != currentExpectedSequence) {
            return false;
        }
        currentExpectedSequence++;
    }
    
    // If all the lines matched the expected sequence, the current code frame has the correct sequence of lines (the question has been solved)
    return true;
    
}

// ==================== ADD / REMOVE LINES OF CODE ====================

bool CodeFrame::addLineOfCode(LineOfCode *lineOfCodeToAdd){
    
    // Add the line of code at the insertion point
    return addLineOfCode(lineOfCodeToAdd, currentlySelectedLineInsertion);
    
}

bool CodeFrame::addLineOfCodeAtEnd(LineOfCode *lineOfCodeToAdd){
    
    // Add the line of code at the end
    return addLineOfCode(lineOfCodeToAdd, numberOfLines);
    
}
    
bool CodeFrame::addLineOfCode(LineOfCode *lineOfCodeToAdd, int index){
    
    // Flag to indicate if the line was inserted
    bool didAddLine = false;
    
    // If the index to insert at is negative, don't do anything
    if(index < 0) {
        return false;
    }
    
    // If inserting in the middle, use the insert function
    if(index < numberOfLines) {
        linesOfCode.insert(linesOfCode.begin() + index, lineOfCodeToAdd);
        numberOfLines++;
        didAddLine = true;
    }
    
    // If inserting in the middle, use the push_back function
    else if(index == numberOfLines) {
        linesOfCode.push_back(lineOfCodeToAdd);
        numberOfLines++;
        didAddLine = true;
    }
    
    // If a line was added, add its label to the scroll box at the current index
    if(didAddLine) {
        
        // If the line was added at the end, simply add it ti the scroll box
        if(index == numberOfLines) {
            linesScrollBox->Pack(lineOfCodeToAdd->getLable());
        } 
        // Otherwise remove all the entries and add them all in order since SFGUI does not support adding at a certain index
        else {
            linesScrollBox->RemoveAll();
            for(LineOfCode *lineOfCodeToAdd : linesOfCode) {
                linesScrollBox->Pack(lineOfCodeToAdd->getLable());
            }
        }
        
    }
    
    // Return if the line was inserted or not
    return didAddLine;
    
}

bool CodeFrame::removeLineOfCode(int index) {
    
    // If the index to remove is negative or greater than the size of the list, don't do anything
    if(index < 0 || index >= numberOfLines) {
        return false;
    }
      
    // Remove the line's label from the scroll box
    linesScrollBox->Remove(linesOfCode[index]->getLable());
    
    // Remove the line from the list
    linesOfCode.erase(linesOfCode.begin() + index);
    numberOfLines--;
    
    // Make sure the currently selected insertion point index stays in range
    if(currentlySelectedLineInsertion >= numberOfLines + 1) {
        currentlySelectedLineInsertion = std::max(numberOfLines, 0);
    } else if(currentlySelectedLineInsertion < 0) {
        currentlySelectedLineInsertion = 0;
    }
    
    // Return success
    return true;
    
}

void CodeFrame::removeAllLinesOfCode() {

    // Remove all line's label from the scroll box
    linesScrollBox->RemoveAll();
    
    // Remove all the lines from the list
    linesOfCode.clear();
    numberOfLines = 0;

}

// ==================== GETTERS ====================

bool CodeFrame::isEmpty() {
    // Empty if the number of lines is zero
    return numberOfLines == 0;
}

sf::Vector2f CodeFrame::getLinesDimensions() {
    // Return the lines dimensions
    return linesDimensions;
}

// ==================== SETTERS ====================

void CodeFrame::setNumberOfTargetLines(int _numberOfTargetLines) {
    if(_numberOfTargetLines > 0) {
        numberOfTargetLines = _numberOfTargetLines;
    }
}

void CodeFrame::incrementCurrentlySelectedLineInsertion() {
    
    // Increment the current insertion point 
    currentlySelectedLineInsertion++;
    
    // Make sure it stays in bounds
    if(currentlySelectedLineInsertion > numberOfLines + 1) {
        currentlySelectedLineInsertion = numberOfLines + 1;
    }
    
}
