//#pragma once

//#include "Controller.h"
class Controller;

class Animation {
	
public: 
	
		Animation() = default;
		Animation (int x, int y, int width, int height) {
			
			texture.loadFromFile(std::string(IMAGES_DIRECTORY) + PLAYER_IMAGE_NAME);
			blender.loadFromFile(std::string(IMAGES_DIRECTORY) + BLENDER_IMAGE_NAME);
			door.loadFromFile(std::string(IMAGES_DIRECTORY) + DOOR_IMAGE_NAME);
			key.loadFromFile(std::string(IMAGES_DIRECTORY) + KEY_IMAGE_NAME);
			theclock.loadFromFile(std::string(IMAGES_DIRECTORY) + CLOCK_IMAGE_NAME);			

			for (int i = 0; i < nFrames; i++){
				frames[i] = {x + i * width, y, width, height};
			}
			
		}
		
		void ApplyToSprite( sf::Sprite& s) {
			s.setTexture( texture );
			s.setTextureRect( frames[iFrame]);
		}
		
		// Items 
		void ApplyToBlender( sf::Sprite& s) {
			s.setTexture( blender );
			s.setTextureRect( frames[iFrame]);
		}
		
		void ApplyToDoor( sf::Sprite& s) {
			s.setTexture( door );
			s.setTextureRect( frames[iFrame]);
		}
		
		void ApplyToKey( sf::Sprite& s) {
			s.setTexture( key );
			s.setTextureRect( frames[iFrame]);
		}
		
		void ApplyToClock( sf::Sprite& s) {
			s.setTexture( theclock );
			s.setTextureRect( frames[iFrame]);
		}
		
		void Updater(float deltaTime) {
			time += deltaTime;
			while (time >= holdTime){
				time -= holdTime;
				Advance();
			}
		}
		
private:
	
		void Advance() {
			if (++iFrame >= nFrames){
				iFrame = 0;
			}
		}
		
private: 
	
		static constexpr int nFrames = 8;
		static constexpr float holdTime = 0.1f;
		sf::Texture texture;
		sf::Texture blender;
		sf::Texture door;
		sf::Texture key;
		sf::Texture theclock;
		sf::IntRect frames[nFrames];
		int iFrame = 0;
		float time = 0.0f;
};

class Character {
	
private: 
	
	enum class AnimationIndex {
		//Jump,
		Stand,
		WalkLeft,
		WalkRight,
		Count
	};
	
public: 
	
		Character() = default;
		Character (const sf::Vector2f& pos) : pos( pos ) {
			
			sprite.setTextureRect({0,0,96,96});
			
			//animations[int( AnimationIndex::Jump)] = Animation(96, 288, 96, 96); // below walking animations
			animations[int(AnimationIndex::Stand)] = Animation(0, 0, 96, 96);
			animations[int(AnimationIndex::WalkLeft)] = Animation(0, 96, 96, 96); //Bottom 
			animations[int(AnimationIndex::WalkRight)] = Animation(0, 196, 96, 96); // Top, (size of one sprite box, position on sheet (row*size), size of each box, box size)

		}
 
		void Draw( sf::RenderTarget& rt ) const {
			rt.draw( sprite );
		}
		
		void setDirection( const sf::Vector2f& dir ) {
			 vel = dir * speed;
			 
			if( dir.x > 0.0f ){
				curAnimation = AnimationIndex::WalkRight; 
			}
			else if( dir.x < 0.0f){
				curAnimation = AnimationIndex::WalkLeft;
			}
			else if( dir.x == 0.0f){
				curAnimation = AnimationIndex::Stand;
			}
			else if( dir.y < 0.0f){
				curAnimation = AnimationIndex::WalkLeft;
			}
		}
		
		void Updater(float deltaTime) {

			// [TODO] Only change position if not paused
			//if(!Controller::pauseWindowIsOpen) {
				pos += (vel * deltaTime); 
			//}
			
			animations[int( curAnimation )].Updater( deltaTime );
			animations[int( curAnimation )].ApplyToSprite( sprite );
			animations[int(curAnimation)].ApplyToBlender(spriteB);
			animations[int(curAnimation)].ApplyToDoor(spriteD);
			animations[int(curAnimation)].ApplyToKey(spriteK);
			animations[int(curAnimation)].ApplyToClock(spriteC);
			sprite.setPosition( pos );
		}

		// Accessor for player position to check if in front of interactive object
		sf::Vector2f getPosition() { return pos;}
		void setPosition(sf::Vector2f posToSet) { pos = posToSet; }

		sf::Rect<float> getRect() {
			sf::Rect<float> rect(pos, sf::Vector2f(96,96));
			return rect;
		}

private:
	
		static constexpr float speed = 100.0f;
		sf::Vector2f pos; 
		sf::Vector2f vel = {0.0f, 2.0f}; // updater to remove when with controller 
		sf::Sprite sprite;
		sf::Sprite spriteB;
		sf::Sprite spriteD;
		sf::Sprite spriteK;
		sf::Sprite spriteC;
		Animation animations[int( AnimationIndex::Count )]; //array of different animations 
		AnimationIndex curAnimation = AnimationIndex::Stand;
		
};

class TDoor {
	
private:
	
	enum class AnimationIndex {
		Stand,
		Count
	};
		
public:

	TDoor() = default;
	TDoor(const sf::Vector2f &pos) :  pos( pos ) {
		spriteD.setTextureRect({0, 0, 96, 96}); // 34:37 in video
		animations[int(AnimationIndex::Stand)] = Animation(0, 0, 96, 96);
	 }

	void Draw(sf::RenderTarget &rt) const {
		rt.draw(spriteD);
	}

	void Updater(float deltaTime) {
		animations[int(curAnimation)].Updater(0 /* deltaTime */); // Don't animate door for now
		animations[int(curAnimation)].ApplyToDoor(spriteD);
		spriteD.setPosition(pos);
	}

	// Function to get position to compare to player position
	sf::Vector2f getPosition() { return pos;}

	sf::Rect<float> getRect() {
		sf::Rect<float> rect(pos, sf::Vector2f(96,96));
		return rect;
	}

private:
  
	static constexpr float speed = 100.0f;
	sf::Vector2f vel = {0.0f, 0.0f}; // updater to remove when with controller 
	sf::Vector2f pos;
	sf::Sprite spriteD;
	Animation animations[int(AnimationIndex::Count)]; //array of different animations
	AnimationIndex curAnimation = AnimationIndex::Stand;
	
};

class TBlender {
	
private:
  
	enum class AnimationIndex {
		Stand,
		Count
	};

public:

	TBlender() = default;
	TBlender(const sf::Vector2f &pos) : pos(pos) {
		spriteB.setTextureRect({0, 0, 32, 32}); // 34:37 in video
		animations[int(AnimationIndex::Stand)] = Animation(0, 0, 32, 32);
	}

	void Draw(sf::RenderTarget &rt) const {
		rt.draw(spriteB);
	}

	void Updater(float deltaTime) {
		animations[int(curAnimation)].Updater(deltaTime);
		animations[int(curAnimation)].ApplyToBlender(spriteB);
		spriteB.setPosition(pos);
	}

	// Function to get position to compare to player position
	sf::Vector2f getPosition() { return pos;}

	sf::Rect<float> getRect() {
		sf::Rect<float> rect(pos, sf::Vector2f(32,32));
		return rect;
	}

private:
  
	static constexpr float speed = 100.0f;
	sf::Vector2f vel = {0.0f, 0.0f}; // updater to remove when with controller
	sf::Vector2f pos;
	sf::Sprite spriteB;
	Animation animations[int(AnimationIndex::Count)]; //array of different animations
	AnimationIndex curAnimation = AnimationIndex::Stand;
	
};

class TKey {
	
private:
  
	enum class AnimationIndex {
		Stand,
		Count
	};

public:
  
	TKey() = default;
	TKey(const sf::Vector2f &pos) : pos(pos) {
		spriteK.setTextureRect({0, 0, 32, 32}); // 34:37 in video
		animations[int(AnimationIndex::Stand)] = Animation(0, 0, 32, 32);
	}

	void Draw(sf::RenderTarget &rt) const {
		rt.draw(spriteK);
	}

	void Updater(float deltaTime) {
		animations[int(curAnimation)].Updater(deltaTime);
		animations[int(curAnimation)].ApplyToKey(spriteK);
		spriteK.setPosition(pos);
	}

	// Function to get position to compare to player position
	sf::Vector2f getPosition() { return pos;}

	sf::Rect<float> getRect() {
		sf::Rect<float> rect(pos, sf::Vector2f(32,32));
		return rect;
	}

private:
  
	static constexpr float speed = 200.0f;
	sf::Vector2f vel = {0.0f, 0.0f}; // updater to remove when with controller
	sf::Vector2f pos;
	sf::Sprite spriteK;
	Animation animations[int(AnimationIndex::Count)]; //array of different animations
	AnimationIndex curAnimation = AnimationIndex::Stand;
	
};

class TClock {
	
private:
  
	enum class AnimationIndex {
		Stand,
		Count
	};

public:

	TClock() = default;
	TClock(const sf::Vector2f &pos) : pos(pos) {
		spriteC.setTextureRect({0, 0, 32, 32}); // 34:37 in video
		animations[int(AnimationIndex::Stand)] = Animation(0, 0, 32, 32);
	}

	void Draw(sf::RenderTarget &rt) const {
		rt.draw(spriteC);
	}

	void Updater(float deltaTime) {
		animations[int(curAnimation)].Updater(deltaTime);
		animations[int(curAnimation)].ApplyToClock(spriteC);
		spriteC.setPosition(pos);
	}

	// Function to get position to compare to player position
	sf::Vector2f getPosition() { return pos;}

	sf::Rect<float> getRect() {
		sf::Rect<float> rect(pos, sf::Vector2f(32,32));
		return rect;
	}

private:
  
	static constexpr float speed = 100.0f;
	sf::Vector2f vel = {0.0f, 0.0f}; // updater to remove when with controller
	sf::Vector2f pos;
	sf::Sprite spriteC;
	Animation animations[int(AnimationIndex::Count)]; //array of different animations
	AnimationIndex curAnimation = AnimationIndex::Stand;
	
};
