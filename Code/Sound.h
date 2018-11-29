#pragma once

// ==================== LIBRARY INCLUDES ====================

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

// ==================== SOUND CLASS ====================

class Sound {
    
private:

    // The number of sounds used
    const static int NUM_SOUNDS = 5;
    
    // The files for the types of sound that can be played
    static std::string soundFileNames[NUM_SOUNDS];

    // The sound buffers that hold the actual sound data
    static sf::SoundBuffer soundBuffers[NUM_SOUNDS];
    
    // The sound that will play
    static sf::Sound sound;
    
    // The music that will play
    static sf::Music music;

    // Flag used to make sure sounds are only loaded once
    static bool didLoadSounds;

public:

	// Constructor / destructor
	//Sound();
	//virtual ~Sound();
    
    // The types of sound that can be played
    enum SoundType {BUTTON_HOVER = 0, BUTTON_PRESS = 1, CHECK_BUTTON_PRESS = 2, CODING_PROBLEM_CORRECT = 3, CODING_PROBLEM_INCORRECT = 4};
    
    // Initialize by loading all sounds into sound buffer objcts
    static void Initialize();
    
    // Play a short buffered sound
    static void playSound(SoundType soundType);
    
    // Play a long streamed music file
    static void playMusic(std::string filename, bool loop);
    
};

