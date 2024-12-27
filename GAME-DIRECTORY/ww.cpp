#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <limits>

// Structure Definitions
struct Puzzle {
    std::string jumbledWord;
    std::string correctWord;
    std::string riddle;
};

struct Suspect {
    std::string name;
    std::string clue;
};

struct Murderer {
    std::string name;
    std::string clue;
};

// Difficulty Configuration
struct DifficultyConfig {
    int basePoints;
    int bonusPoints;
    int tryPenalty;
    int hintPenalty;
    int hintBonus;
};

// Global Difficulty Settings
const std::map<std::string, DifficultyConfig> difficultySettings = {
    {"intermediate", {5, 5, 3, 5, 2}},
    {"hard", {3, 3, 5, 10, 2}}
};

// Utility Functions
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string getHint(const std::string& correctWord, size_t& revealedChars) {
    std::string hint(correctWord.size(), '*');
    if (revealedChars < correctWord.size()) {
        hint[revealedChars] = correctWord[revealedChars];
        revealedChars++;
    }
    return hint;
}

void updatePoints(int& points, int tries, int hints, bool correct, const std::string& mode) {
    auto it = difficultySettings.find(mode);
    if (it == difficultySettings.end()) {
        std::cerr << "Error: Invalid mode '" << mode << "' provided to updatePoints.\n";
        return;
    }

    const auto& config = it->second;

    if (correct) {
        points += (tries < 3) ? (config.basePoints + config.bonusPoints) : config.basePoints;
    }
    else if (tries > 5) {
        points = std::max(points - config.tryPenalty, 0);
    }

    if (hints > 3) {
        points = std::max(points - config.hintPenalty, 0);
    }
    else {
        points += config.hintBonus;
    }
}

// Score Manager Class
class ScoreManager {
public:
    static void saveScore(const std::string& username, int score) {
        std::ofstream outFile("scores.txt", std::ios::app);
        if (outFile) {
            outFile << username << " " << score << std::endl;
            std::cout << "Score saved successfully!" << std::endl;
        }
        else {
            std::cerr << "Error opening file for writing. Scores will not be saved.\n";
        }
    }

    static void displayScores() {
        std::ifstream inFile("scores.txt");
        if (inFile) {
            std::string username;
            int score;
            std::vector<std::pair<int, std::string>> scores;

            while (inFile >> username >> score) {
                if (score >= 0) {
                    scores.emplace_back(score, username);
                }
            }

            std::sort(scores.rbegin(), scores.rend());

            std::cout << "User Scores:\n";
            for (const auto& entry : scores) {
                std::cout << entry.second << ": " << entry.first << "\n";
            }
        }
        else {
            std::cerr << "Error opening file for reading.\n";
        }
    }
};

void playLevel(const std::vector<Puzzle>& puzzles, const std::string& mode, int& points, const sf::Sprite& background) {
    int currentPuzzle = 0, tries = 0, hints = 0;
    size_t revealedChars = 0;
    std::string playerInput = "";

    sf::RenderWindow window(sf::VideoMode(800, 600), "Murder Mystery Game - " + mode);
    sf::Font font;

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font! Make sure the file exists.\n";
        return;
    }

    sf::Text jumbledWordText, inputText, statusText, hintText;
    jumbledWordText.setFont(font);
    inputText.setFont(font);
    statusText.setFont(font);
    hintText.setFont(font);

    jumbledWordText.setCharacterSize(30);
    jumbledWordText.setPosition(20, 150);
    jumbledWordText.setFillColor(sf::Color::White);

    inputText.setCharacterSize(25);
    inputText.setPosition(20, 350);
    inputText.setFillColor(sf::Color::Green);

    statusText.setCharacterSize(25);
    statusText.setPosition(20, 450);
    statusText.setFillColor(sf::Color::Yellow);

    hintText.setCharacterSize(20);
    hintText.setPosition(20, 500);
    hintText.setFillColor(sf::Color::Magenta);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!playerInput.empty())
                        playerInput.pop_back();
                } else if (event.text.unicode < 128) {
                    playerInput += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                std::string sanitizedInput = trim(playerInput);
                std::string correctWord = puzzles[currentPuzzle].correctWord;

                if (sanitizedInput == correctWord) {
                    updatePoints(points, tries, hints, true, mode);
                    currentPuzzle++;
                    if (currentPuzzle >= puzzles.size()) {
                        statusText.setString("All puzzles completed!");
                        window.close();
                    } else {
                        statusText.setString("Correct! Moving to the next puzzle.");
                    }
                    playerInput.clear();
                    revealedChars = 0;
                } else {
                    statusText.setString("Incorrect! Try again.");
                    tries++;
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
                hintText.setString("Hint: " + getHint(puzzles[currentPuzzle].correctWord, revealedChars));
                hints++;
            }
        }

        if (currentPuzzle < puzzles.size()) {
            jumbledWordText.setString("Unscramble: " + puzzles[currentPuzzle].jumbledWord);
            inputText.setString("Your Guess: " + playerInput);
        }

        // Render everything
        window.clear();
        window.draw(background); // Draw the background image first
        window.draw(jumbledWordText);
        window.draw(inputText);
        window.draw(statusText);
        window.draw(hintText);
        window.display();
    }
}

void displaySuspects(const std::vector<Suspect>& suspects) {
    std::cout << "Suspects List:\n";
    for (size_t i = 0; i < suspects.size(); ++i) {
        std::cout << i + 1 << ". " << suspects[i].name << ": " << suspects[i].clue << std::endl;
    }
}
bool guessMurdererSFML(std::vector<Suspect>& suspects, const Murderer& murderer, int& points, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Guess the Murderer");
    sf::Font font;

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font! Make sure the file exists.\n";
        return false;
    }

    sf::Text titleText, suspectText, feedbackText;
    titleText.setFont(font);
    titleText.setString("Who is the murderer?");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(20, 20);

    suspectText.setFont(font);
    suspectText.setCharacterSize(20);
    suspectText.setFillColor(sf::Color::White);
    suspectText.setPosition(20, 80);

    feedbackText.setFont(font);
    feedbackText.setCharacterSize(25);
    feedbackText.setFillColor(sf::Color::Yellow);
    feedbackText.setPosition(20, 500);

    bool guessedCorrectly = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                int selected = event.key.code - sf::Keyboard::Num1;
                if (selected >= 0 && selected < suspects.size()) {
                    if (suspects[selected].name == murderer.name) {
                        feedbackText.setString("Correct! The murderer is " + murderer.name + "!");
                        guessedCorrectly = true;
                        points += 10; // Reward points for correct guess
                        window.close();
                    } else {
                        feedbackText.setString("Wrong! Try again.");
                    }
                }
            }
        }

        // Display suspects
        std::string suspectsDisplay;
        for (size_t i = 0; i < suspects.size(); ++i) {
            suspectsDisplay += std::to_string(i + 1) + ". " + suspects[i].name + ": " + suspects[i].clue + "\n";
        }
        suspectText.setString(suspectsDisplay);

        // Render everything
        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(suspectText);
        window.draw(feedbackText);
        window.display();
    }

    if (!guessedCorrectly) {
        std::cout << "Too many wrong guesses. The murderer was " << murderer.name << ".\n";
    }

    return guessedCorrectly;
}

void displayLevelSelection(sf::Sprite& background, std::string& modeChoice) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Level Selection");
    sf::Font font;

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font! Make sure the file exists.\n";
        return;
    }

    sf::Text titleText, instructionText, level1Text, level2Text;
    titleText.setFont(font);
    titleText.setString("Select Level");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(20, 20);

    instructionText.setFont(font);
    instructionText.setString("Press 1 for Level 1 or 2 for Level 2");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20, 100);

    level1Text.setFont(font);
    level1Text.setString("1. Level 1");
    level1Text.setCharacterSize(25);
    level1Text.setFillColor(sf::Color::White);
    level1Text.setPosition(20, 150);

    level2Text.setFont(font);
    level2Text.setString("2. Level 2");
    level2Text.setCharacterSize(25);
    level2Text.setFillColor(sf::Color::White);
    level2Text.setPosition(20, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    modeChoice = "level1";
                    window.close();
                } else if (event.key.code == sf::Keyboard::Num2) {
                    modeChoice = "level2";
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(instructionText);
        window.draw(level1Text);
        window.draw(level2Text);
        window.display();
    }

    if (modeChoice.empty()) {
        std::cerr << "No level selected. Defaulting to level1.\n";
        modeChoice = "level1";
    }
}


void displayUsernameInput(sf::Sprite& background, std::string& username) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Username Input");
    sf::Font font;

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font! Make sure the file exists.\n";
        return;
    }

    sf::Text titleText, usernamePrompt, usernameInputText;
    titleText.setFont(font);
    titleText.setString("Enter your Username");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(20, 20);

    usernamePrompt.setFont(font);
    usernamePrompt.setString("Enter your username: ");
    usernamePrompt.setCharacterSize(20);
    usernamePrompt.setFillColor(sf::Color::White);
    usernamePrompt.setPosition(20, 100);

    usernameInputText.setFont(font);
    usernameInputText.setCharacterSize(20);
    usernameInputText.setFillColor(sf::Color::Green);
    usernameInputText.setPosition(250, 100);

    std::string playerInput = "";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Handle backspace
                    if (!playerInput.empty()) {
                        playerInput.pop_back();
                    }
                } else if (event.text.unicode < 128) {
                    playerInput += static_cast<char>(event.text.unicode);
                }
                usernameInputText.setString(playerInput);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    username = playerInput;
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(usernamePrompt);
        window.draw(usernameInputText);
        window.display();
    }

    if (username.empty()) {
        std::cerr << "No username entered. Defaulting to 'Player'.\n";
        username = "Player";
    }
}

int main() {
    int points = 0;
    std::string modeChoice, username;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("simon.png")) {
        std::cerr << "Failed to load background image!\n";
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("mysterious.wav")) {
        std::cerr << "Failed to load audio!\n";
        return -1;
    }
    sf::Sound backgroundSound;
    backgroundSound.setBuffer(soundBuffer);
    backgroundSound.setLoop(true); // Loop the audio
    backgroundSound.play();

    // Call the level selection function
    displayLevelSelection(background, modeChoice);

    // Call the username input function
    displayUsernameInput(background, username);

    // Set default difficulty to "level1" if invalid input is provided
    if (modeChoice != "level1" && modeChoice != "level2") {
        std::cout << "Invalid mode selected. Defaulting to 'level1'.\n";
        modeChoice = "level1";
    }

    // Puzzles for each level
    std::vector<Puzzle> level1Puzzles = {
        {"iomtve", "motive", "I drive people to commit crimes. What am I?"},
        {"adeviicnee", "evidence", "I confirm what really happened. What am I?"},
        {"lavibii", "alibi", "I can prove where someone was during the crime. What am I?"}
    };

    std::vector<Puzzle> level2Puzzles = {
        {"wettsnai", "witness", "I saw the crime unfold. Who am I?"},
        {"rtdistneoi", "distortion", "I can obscure the truth. What am I?"},
        {"loca", "clue", "I guide detectives in solving the case. What am I?"}
    };

    // Murderer and suspects information
    Murderer murderer = { "Inspector Clark", "Always one step ahead of everyone else, never caught in a lie." };
    std::vector<Suspect> suspects = {
        {"Detective Jane", "Quick thinking but overzealous."},
        {"Officer Bill", "Quiet and always present at crime scenes."},
        {"Inspector Clark", "One step ahead of everyone, never caught in a lie."},
        {"Dr. Sarah", "Renowned forensic expert, calm under pressure."},
        {"Mr. Edward", "Wealthy businessman, history of shady deals."},
        {"Ms. Olivia", "Charming socialite with a mysterious past."}
    };

    // Select puzzles based on mode
    std::vector<Puzzle> selectedPuzzles = (modeChoice == "level2") ? level2Puzzles : level1Puzzles;

    // Play the selected level
    playLevel(selectedPuzzles, modeChoice, points, background);

    // Display suspects and let the player guess the murderer
    displaySuspects(suspects);
    if (guessMurdererSFML(suspects, murderer, points, background)) {
        std::cout << "Congratulations! You solved the case!" << std::endl;
        ScoreManager::saveScore(username, points);
    } else {
        std::cout << "Better luck next time!" << std::endl;
    }

    // Display scores
    ScoreManager::displayScores();
    
    return 0;
}
//g++ ww.cpp -o ww -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
