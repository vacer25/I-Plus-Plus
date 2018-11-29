#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

// ==================== LOCAL INCLUDES ====================

#include "Controller.h"
#include "GUIWindow.h"

class Controller;
class CodeFrame;
class LineOfCode;

// ==================== CODING WINDOW CLASS ====================

class CodingWindow : public GUIWindow {

private:
    
	// Variables for drawing
    sfg::Window::Ptr codingWindow;
    sfg::Window::Ptr codingInfoWindow;
    sfg::Label::Ptr infoLabel;
    sfg::Button::Ptr infoButton;
    sfg::Label::Ptr instructionsLabel;
    
    // Variables for mouse input
    static sf::FloatRect availableLinesOfCodeRect;
    static sf::FloatRect currentLinesOfCodeRect;
    
    // Lines of code
    std::vector<LineOfCode*> availableLinesOfCode;
    std::vector<LineOfCode*> currentLinesOfCode;

    // State variables
    int currentQuestionNumber;
    bool currentSequenceIsCorrect;
    bool infoWindowWasOpenOnHide;
    
    // Initialization and lines of code placement
    void loadQuestionData(int questionNumber);
    void initializeLinesOfCode();

    // Functions for switching active code frames and entering/exiting activated line mode
    bool updateActivatedLineMode();
    void updateCurrentlySelectedFrame();
    void setActiveFrame(bool forceUpdate);
    
    // Function for checking if the current lines of code are correct
    void checkIfCurrentCodeIsCorrect();

    // Info window functions
    void openInfoWindow();
    void hideInfoWindow();    
    
public:

	// Constructor / destructor
	CodingWindow(Controller *_controller);
	virtual ~CodingWindow();

    // Initialization and lines of code placement
    void initializeCodingWindowForNewQuestion(int questionNumber, bool firstTimeOpeningCodingWindow);
    
    // Mouse functions
    static bool isMouseOverAvailableCodeFrame();
    static bool isMouseOverCurrentCodeFrame();

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
    
    // State variables
    static bool useMouseControl;
    static bool isCurrentlyInAvailableLinesFrame;
    static bool isCurrentlyInActivatedLineMode;
    static bool infoWindowIsOpen;
    
    // Code frames
    static CodeFrame *availableCodeFrame;
    static CodeFrame *currentCodeFrame;
    
};

// ==================== LINE OF CODE CLASS ====================

class LineOfCode {

private:
    
    // Instance variables
    std::string codeString;
    sfg::Label::Ptr label;
    std::string labelID;
    int sequenceID;
    
public:

	// Constructor / destructor
	LineOfCode(std::string _codeString, int _sequenceID) : codeString(_codeString), sequenceID(_sequenceID) {}
	//virtual ~LineOfCode() {delete label;}
    
    // Initialize
    inline void setLable(sfg::Label::Ptr _label, std::string _labelID) {
        label =_label;
        labelID = _labelID;
        //label->SetText("[" + std::to_string(sequenceID) + "] " + codeString);
        label->SetText(codeString);
        label->SetAlignment(sf::Vector2f(0.0f, 0.5f));
        label->SetId(labelID);
    }

	// Getters
	inline std::string getCodeString() {return codeString;}
    inline sfg::Label::Ptr getLable() {return label;}
	inline std::string getLabelID() {return labelID;}
	inline int getSequenceID() {return sequenceID;}

};

// ==================== CODE FRAME CLASS ====================

class CodeFrame {

private:
    
    // Instance variables
    bool isAvailableLinesFrame;
    std::vector<LineOfCode*> linesOfCode;
    sfg::Box::Ptr linesScrollBox;
    int numberOfTargetLines;
    int numberOfLines;
    int currentlySelectedLine;
    int currentlySelectedLineInsertion;
    
    // Variables for drawing
    sf::Vector2f linesStartPoint;
    sf::Vector2f linesDimensions;
    
    // Move current line of code to the other frame
    void moveCurrentlySelectedLineOfCode();
    
public:

	// Constructor / destructor
	CodeFrame(bool _isAvailableLinesFrame, sfg::Box::Ptr _linesScrollBox);
	virtual ~CodeFrame();
    
    // Initialization
    void initialize(sfg::ScrolledWindow::Ptr linesScrollWindow);
    void reInitialize();

	// Add / remove lines of code
	bool addLineOfCodeAtEnd(LineOfCode *lineOfCodeToAdd);
	bool addLineOfCode(LineOfCode *lineOfCodeToAdd);
	bool addLineOfCode(LineOfCode *lineOfCodeToAdd, int index);
    bool removeLineOfCode(int index);
    void removeAllLinesOfCode();
    
    // Check for correct sequence, only applicable to the current lines code frame
    bool hasCorrectSequenceOfLines();
    
    // Update & draw
    void update(bool canMoveLineNow);
    void draw(sf::RenderWindow &window);
    
    // Getters
    bool isEmpty();
    sf::Vector2f getLinesDimensions();
    
    // Setters
    void setNumberOfTargetLines(int _numberOfTargetLines);
    void incrementCurrentlySelectedLineInsertion();

};
