 // this is to run controller stuff 
// ==================== LOCAL INCLUDES ====================

#include "Controller.h"

// ==================== MAIN====================

int main() {

	// ==================== CREATE AND RUN GAME ====================

	Controller controller;
	controller.start();

	// ==================== EXIT AFTER GAME WINDOW CLOSED ====================

	return EXIT_SUCCESS;
}

/*
// ================== Below is testing stuff =================
#include "Controller.h"
#include "Player.h"
#include <chrono>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

	Character dude( { 800.0f, 100.0f } );
	auto tp = std::chrono::steady_clock::now();
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		//get dt 
		float dt;
		{
			const auto new_tp = std::chrono::steady_clock::now();
			dt = std::chrono::duration<float>( new_tp - tp ).count();
			tp = new_tp;
		}
		
		// Handle input 
		// Input the keyboard commands, but this will be handled by the controller in the future
		//Controller will handle the animation calls 
			sf::Vector2f dir = { 0.0f, 0.0f };
			if(sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
			{
				dir.x -= 1.0f;
			}
			if(sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
			{
				dir.x += 1.0f;
			}
			dude.setDirection( dir );
			
			dude.Update( dt );
		// Clear screen
        window.clear();
        // Draw the sprite
        dude.Draw( window );
		
       // Update the window
        window.display();
		
    }
    return EXIT_SUCCESS;
}
*/