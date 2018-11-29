#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

// ==================== LOCAL INCLUDES ====================

#include "Controller.h"
#include "TinyXML/tinyxml.h"

// ==================== OBJECT CLASS ====================

class Object {
public:
    // Getters
	int GetPropertyInt(std::string name);
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);


    // Attributes
	std::string name;
	std::string type;
	
    // Additional properties
	std::map<std::string, std::string> properties;

    // The image and positions + size
	sf::Sprite sprite;
    sf::Rect<float> rect;
    
};

// ==================== LAYER CLASS ====================

struct Layer {
    
    // The opacity of all the tiles on this layer
	int opacity;
    
    // All the tiles on this level
	std::vector<sf::Sprite> tiles;
    
};

// ==================== LEVEL SCREEN CLASS ====================

class LevelScreen {

private:

    // Attributes
	int firstTileID;
    
    // The viewport
    sf::View viewport;
    
	sf::Rect<float> drawBoundary;
    
    // The tileset image that all the objects are drawn using
	sf::Texture tilesetImage;
	std::vector<Layer> layers;

protected:

	// Pointer to controller
	Controller *controller;

public:

	// Constructor / destructor
	LevelScreen(Controller *_controller);
	~LevelScreen();

    // Load objects from leel file
	bool LoadFromFile(std::string filename);
    
    // Set the viewport (camera position)
    void setViewport(sf::RenderWindow &window, sf::Vector2f &centerPosition, bool forceFullscreen);
    
    // Getters
	Object GetObject(std::string name);
	std::vector<Object> GetObjects(std::string name);
	std::vector<Object> GetAllObjects();
	sf::Vector2i GetTileSize();
    
	// Draw GUI, called from Controller in draw loop
	void draw(sf::RenderWindow &window);
	void update(float deltaTime);
	void updateEvent(sf::Event event);

	std::vector<Object> objects;
	int width, height, tileWidth, tileHeight;
};
