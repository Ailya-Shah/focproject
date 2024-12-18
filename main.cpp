#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Puzzle structure for backend logic
struct Puzzle {
    std::string jumbledWord;
    std::string correctWord;
};

// Function to check if the guessed word is correct
bool isGuessCorrect(const std::string& guess, const std::string& correctWord) {
    return guess == correctWord;
}

// Main function
int main() {
    // Initialize puzzles
    std::vector<Puzzle> puzzles = {
        {"murdreer", "murderer"},
        {"lifaym", "family"},
        {"weran", "answer"}
    };

    int currentPuzzle = 0;
    int score = 0;
    std::string playerInput = "";

    // SFML setup
    sf::RenderWindow window(sf::VideoMode(800, 600), "Unscramble the Word Game");
    sf::Font font;
   if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
    std::cerr << "Error loading font!\n";
    return -1;
}


    // Text setup
    sf::Text jumbledWordText;
    jumbledWordText.setFont(font);
    jumbledWordText.setCharacterSize(50);
    jumbledWordText.setFillColor(sf::Color::White);
    jumbledWordText.setPosition(200, 150);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(40);
    inputText.setFillColor(sf::Color::Green);
    inputText.setPosition(200, 300);

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::Yellow);
    statusText.setPosition(200, 400);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle text input
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerInput.empty()) {
                    // Handle backspace
                    playerInput.pop_back();
                } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                    playerInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        // Update the current puzzle
        if (currentPuzzle < puzzles.size()) {
            jumbledWordText.setString("Unscramble: " + puzzles[currentPuzzle].jumbledWord);
            inputText.setString("Your Guess: " + playerInput);

            if (isGuessCorrect(playerInput, puzzles[currentPuzzle].correctWord)) {
                score += 10; // Add points for a correct guess
                statusText.setString("Correct! Press Enter for next puzzle.");
                playerInput = ""; // Clear input
                currentPuzzle++; // Move to the next puzzle
                if (currentPuzzle >= puzzles.size()) {
                    statusText.setString("Game Over! Your score: " + std::to_string(score));
                }
            } else {
                statusText.setString("Keep trying!");
            }
        }

        // Clear window and draw everything
        window.clear();
        window.draw(jumbledWordText);
        window.draw(inputText);
        window.draw(statusText);
        window.display();
    }

    return 0;
}






