#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <SFML/Graphics.hpp>

// ==================== LOCAL INCLUDES ====================

#include "Constants.h"

// ==================== CLASS ====================

class Entity {

protected:

	sf::Vector2f position;

public:

	// Update & draw overrides
	virtual void update(float deltaTime) {};
	virtual void draw(sf::RenderWindow &window) {};

	// Movement overrides
	virtual sf::Vector2f getPosition() = 0;
	virtual bool doesIntersect(sf::FloatRect inRect) = 0;

};
