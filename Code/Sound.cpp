// ==================== LOCAL INCLUDES ====================

#include "Sound.h"

// ==================== STATIC VARIABLES ====================

sf::Sound Sound::sound;
sf::Music Sound::music;
bool Sound::didLoadSounds = false;
sf::SoundBuffer Sound::soundBuffers[NUM_SOUNDS];
std::string Sound::soundFileNames[NUM_SOUNDS] = {"Button_Hover.wav", "Button_Press.wav", "Check_Button_Press.wav", "Coding_Problem_Correct.wav", "Coding_Problem_Incorrect.wav"};

// ==================== CONSTRUCTOR / DESTRUCTOR ====================

//void Sound::Sound() {}
//void Sound::~Sound() {}

// ==================== INITIALIZE ====================

void Sound::Initialize() {
    
    // If already initialized, don't do anything
    if(didLoadSounds) {
        return;
    }
    
    // Initialize by loading all sounds from files into sound buffer objcts
    for(int soundBufferIndex = 0; soundBufferIndex < NUM_SOUNDS; soundBufferIndex++) {
        try {
            soundBuffers[soundBufferIndex].loadFromFile("Audio/" + soundFileNames[soundBufferIndex]);
        }
        catch(std::exception &e) {
            std::cout << "Error loading sound file \"" << soundFileNames[soundBufferIndex] << "\" error: " << e.what() << std::endl;
            return;
        }
    }
    
    // Set the flag to indicate that all the sounds were loaded into buffers
    didLoadSounds = true;
    
}


// ==================== PLAY SOUND ====================

void Sound::playSound(SoundType soundType) {
    
    // Create a sound instance which will use the specified sound buffer and play it
    sound.setBuffer(soundBuffers[static_cast<int>(soundType)]);
    sound.play();

}

void Sound::playMusic(std::string filename, bool loop) {
    
    // Load the specified music file from the "Audio" folder
    if (!music.openFromFile("Audio/" + filename)) {
        return;
    }
	
	// Control looping
	music.setLoop(loop);

    // Play it
    music.play();

}