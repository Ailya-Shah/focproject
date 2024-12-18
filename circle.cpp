#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    // Create a sound buffer to hold the sound data
    sf::SoundBuffer buffer;

    // Load the WAV file into the buffer (make sure the file path is correct)
    if (!buffer.loadFromFile("test_data_1ch_44100_24_test.wav"))  // Replace with your sound file path
    {
        std::cerr << "Failed to load sound effect!" << std::endl;
        return -1;  // Exit the program if sound file loading fails
    }

    // Create a sound object and set its buffer
    sf::Sound sound;
    sound.setBuffer(buffer);

    // Play the sound
    sound.play();

    // Wait for the sound to finish playing
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < sound.getBuffer()->getDuration().asSeconds())
    {
        // Loop to keep the program running until the sound is finished
    }

    std::cout << "Sound played successfully!" << std::endl;

    return 0;
}

//g++ -o circle circle.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio




