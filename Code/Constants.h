#pragma once

// ==================== MAIN DATA ====================

// The current state of the game, only one of these is active at any given time
// The instructions, coding, and pause windows are overlayed on top of the game screen, they are not separate states
enum GameState {INITIALIZE, START_SCREEN, GAME_SCREEN, END_SCREEN};

// The number of questions for each of the levels
#define NUMBER_OF_QUESTIONS_LEVEL_1     5
#define NUMBER_OF_QUESTIONS_LEVEL_2     5
#define NUMBER_OF_QUESTIONS_LEVEL_3     5
#define NUMBER_OF_QUESTIONS_TOTAL       (NUMBER_OF_QUESTIONS_LEVEL_1 + NUMBER_OF_QUESTIONS_LEVEL_2 + NUMBER_OF_QUESTIONS_LEVEL_3)

// ==================== MAIN DATA ====================

#define LEVELS_DIRECTORY	    "./Levels/"
#define QUESTIONS_DIRECTORY	    "./Questions/"
#define IMAGES_DIRECTORY	    "./Images/"
#define AUDIO_DIRECTORY	        "./Audio/"
#define FONTS_DIRECTORY	        "./Fonts/"

#define FONT_NAME               "consola.ttf"

#define START_BG_IMAGE_NAME     "StartBG.png"
#define TITLE_IMAGE_NAME     	"Title.png"

#define PLAYER_IMAGE_NAME       "Player.png"
#define DOOR_IMAGE_NAME         "Door.png"
#define CLOCK_IMAGE_NAME        "Clock.png"
#define BLENDER_IMAGE_NAME      "Blender.png"
#define KEY_IMAGE_NAME          "Key.png"

// ==================== MAIN WINDOW ====================

// Title
#define TITLE_TEXT	            "i++"

// Target FPS
#define FRAME_LIMIT				60

// Main window width / height
#define WIDTH 					1200
#define HEIGHT 					1000

// ==================== LEVEL SCREEN ====================

#define LEVEL_SCREEN_VIEWPORT_WIDTH     (WIDTH / 3)
#define LEVEL_SCREEN_VIEWPORT_HEIGHT    (HEIGHT / 3)

// ==================== GUI WINDOWS ====================

// Margins from edges of the main window
#define WINDOW_MARGIN_X			32
#define WINDOW_MARGIN_Y 		32

// Padding of inside elements
#define WINDOW_PADDING			8

// Size of font
#define GUI_WINDOW_TITLE_FONT_SIZE      18
#define GUI_WINDOW_DEFAULT_FONT_SIZE    14

// ==================== START MENU WINDOW ====================

#define START_MENU_SCREEN_BUTTONS_FRAMES_HEIGHT 300

#define START_MENU_SCREEN_BUTTONS_HEIGHT        50

// ==================== PAUSE WINDOW ====================

#define PAUSE_WINDOW_WIDTH                      500
#define PAUSE_WINDOW_HEIGHT                     400

// ==================== INSTRUCTIONS WINDOW ====================

#define INSTRUCTIONS_WINDOW_WIDTH               800
#define INSTRUCTIONS_WINDOW_HEIGHT              600

#define INSTRUCTIONS_WINDOW_INSTRUCTIONS_TEXT \
"\
Welcome to i++ !\n\
\n\
You are a computer engineer who is trying to get to\n\
a very important job interview for your dream job!\n\
However, in this world, there are many obstacles along the way.\n\
To get past them, you will need to solve some coding puzzles\n\
and move on to the next levels. Each of the obstacles will test your knowledge of C++.\n\
At the end you will need to use all your skills to pass the interview and get the job.\n\
\n\
Go and get coding!\n\
\n\
"

#define INSTRUCTIONS_WINDOW_CONTROLS_TEXT \
"\
--------------------------------------- Movement -------------------------------------\n\
\n\
Movement                                                         [Arrow Keys] / [WASD]\n\
\n\
------------------------------------- Interaction ------------------------------------\n\
\n\
Open coding window                                               [Space]\n\
Pause                                                            [P]\n\
\n\
------------------------------------ Coding window -----------------------------------\n\
\n\
Move selection                                                   [Up]/[Down]\n\
Switch sides                                                     [Left]/[Right]\n\
Select / place line                                              [E]\n\
Cancel selection                                                 [Q]\n\
Run current code                                                 [Enter]\n\
Exit                                                             [Esc]"

// ==================== CODING SCREEN ====================

#define CODING_SCREEN_BOTTOM_FRAMES_HEIGHT          120

#define CODING_SCREEN_INFO_BOX_WIDTH                500
#define CODING_SCREEN_INFO_BOX_HEIGHT               150
    
#define CODING_SCREEN_LINE_SELECTION_TRANSPARANCY   0x20
#define CODING_SCREEN_DEFAULT_LINE_COLOR            0xC6, 0xCB, 0xC4
#define CODING_SCREEN_SELECTED_LINE_COLOR           0x10, 0xFF, 0x10
#define CODING_SCREEN_ACTIVATED_LINE_COLOR          0x10, 0xFF, 0xFF
#define CODING_SCREEN_WARNING_COLOR                 0xFF, 0x00, 0x00

#define CODING_SCREEN_CONTROLS_LEFT_TEXT \
"Move selection\n\
Switch sides\n\
Select / place line\n\
Cancel selection\n\
Run current code\n\
Exit"

#define CODING_SCREEN_CONTROLS_RIGHT_TEXT \
"[Up]/[Down]\n\
[Left]/[Right]\n\
[E]\n\
[Q]\n\
[Enter]\n\
[Esc]"

#define CODING_SCREEN_INFO_CORRECT_TEXT "Correct!\nPress the continue button to exit the coding window."
#define CODING_SCREEN_INFO_INCORRECT_TEXT "Incorrect!\nPress the continue button to retry."

// Comment out to prevent wrapping of line selection in coding screen
//#define USE_CODING_SCREEN_LINE_SELECTION_WRAP_AROUND

// ==================== INPUTS ====================

#define MAIN_ACTION_KEY sf::Keyboard::Enter
#define MAIN_ENTER_KEY sf::Keyboard::E
#define MAIN_EXIT_KEY sf::Keyboard::Q
#define MAIN_ESCAPE_KEY sf::Keyboard::Escape
#define MAIN_PAUSE_KEY sf::Keyboard::P
#define MAIN_SPACE_KEY sf::Keyboard::Space

#define MAIN_UP_KEY sf::Keyboard::Up
#define MAIN_DOWN_KEY sf::Keyboard::Down
#define MAIN_LEFT_KEY sf::Keyboard::Left
#define MAIN_RIGHT_KEY sf::Keyboard::Right

#define ALT_UP_KEY sf::Keyboard::W
#define ALT_DOWN_KEY sf::Keyboard::S
#define ALT_LEFT_KEY sf::Keyboard::A
#define ALT_RIGHT_KEY sf::Keyboard::D

