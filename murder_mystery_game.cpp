#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void showLevel1(sf::RenderWindow& window);
void showIntermediateMode(sf::RenderWindow& window);

void showSplashScreen(sf::RenderWindow& window, sf::Sound& sound) {
    sf::Texture splashTexture;
    if (!splashTexture.loadFromFile("splash_screen.png")) {
        std::cerr << "Failed to load splash screen image!" << std::endl;
        return;
    }

    sf::Sprite splashSprite;
    splashSprite.setTexture(splashTexture);
    splashSprite.setScale(
        static_cast<float>(window.getSize().x) / splashTexture.getSize().x,
        static_cast<float>(window.getSize().y) / splashTexture.getSize().y
    );

    sf::Font font;
    if (!font.loadFromFile("Montserrat.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    sf::Text gameTitle("Murder Mystery Puzzle", font, 50);
    gameTitle.setFillColor(sf::Color::White);
    gameTitle.setStyle(sf::Text::Bold);

    sf::RectangleShape textBox(sf::Vector2f(gameTitle.getGlobalBounds().width + 20, gameTitle.getGlobalBounds().height + 20));
    textBox.setFillColor(sf::Color(0, 0, 0, 150));
    textBox.setPosition(window.getSize().x / 2 - textBox.getSize().x / 2, window.getSize().y / 2 - textBox.getSize().y / 2);

    gameTitle.setPosition(
        window.getSize().x / 2 - gameTitle.getGlobalBounds().width / 2,
        window.getSize().y / 2 - gameTitle.getGlobalBounds().height / 2
    );

    // Play the sound for the splash screen
    sound.play();

    window.clear();
    window.draw(splashSprite);
    window.draw(textBox);
    window.draw(gameTitle);
    window.display();
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Pause for 3 seconds

    // Stop the sound after the splash screen
    sound.stop();
}

// Function to display the main menu
void showMainMenu(sf::RenderWindow& window) {
    sf::Texture menuBackground;
    if (!menuBackground.loadFromFile("menu.png")) {
        std::cerr << "Failed to load menu background image!" << std::endl;
        return;
    }

    sf::Sprite backgroundSprite(menuBackground);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / menuBackground.getSize().x,
        static_cast<float>(window.getSize().y) / menuBackground.getSize().y
    );

    sf::Font font;
    if (!font.loadFromFile("Montserrat.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    std::string options[] = {"Easy Mode", "Intermediate Mode", "Hard Mode", "Show Progress", "Quit Game"};
    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> optionBoxes;
    float rectangleWidth = 250.f;
    float rectangleHeight = 50.f;

    for (int i = 0; i < 5; ++i) {
        sf::Text text(options[i], font, 30);
        text.setFillColor(sf::Color::Red);
        text.setStyle(sf::Text::Bold);

        sf::RectangleShape optionBox(sf::Vector2f(rectangleWidth, rectangleHeight));
        optionBox.setFillColor(sf::Color(0, 0, 0, 150));
        optionBox.setPosition(window.getSize().x / 2 - rectangleWidth / 2, 200 + i * (rectangleHeight + 10));

        text.setPosition(optionBox.getPosition().x + (rectangleWidth - text.getGlobalBounds().width) / 2,
                         optionBox.getPosition().y + (rectangleHeight - text.getGlobalBounds().height) / 2);

        optionBoxes.push_back(optionBox);
        menuItems.push_back(text);
    }

    int selectedOption = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < 5; ++i) {
                        if (optionBoxes[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            if (i == 0) {  // Easy Mode
                                std::cout << "Starting Easy Mode..." << std::endl;
                                showLevel1(window);  // Call Easy Mode
                            } else if (i == 1) {  // Intermediate Mode
                                std::cout << "Starting Intermediate Mode..." << std::endl;
                                showIntermediateMode(window);  // Call Intermediate Mode
                            } else if (i == 2) {  // Hard Mode
                                std::cout << "Starting Hard Mode..." << std::endl;
                                // Add your hard mode logic here (e.g., showHardMode(window))
                            } else if (i == 3) {  // Show Progress
                                std::cout << "Viewing Progress..." << std::endl;
                                // Add your progress display logic here
                            } else if (i == 4) {  // Quit Game
                                window.close(); // Exit
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuItems[selectedOption].setFillColor(sf::Color::Red);
                    selectedOption = (selectedOption - 1 + menuItems.size()) % menuItems.size();
                    menuItems[selectedOption].setFillColor(sf::Color::White);
                } else if (event.key.code == sf::Keyboard::Down) {
                    menuItems[selectedOption].setFillColor(sf::Color::Red);
                    selectedOption = (selectedOption + 1) % menuItems.size();
                    menuItems[selectedOption].setFillColor(sf::Color::White);
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedOption == 0) { // Easy Mode
                        std::cout << "Starting Easy Mode..." << std::endl;
                        showLevel1(window);  // Call Easy Mode
                    } else if (selectedOption == 1) { // Intermediate Mode
                        std::cout << "Starting Intermediate Mode..." << std::endl;
                        showIntermediateMode(window);  // Call Intermediate Mode
                    } else if (selectedOption == 2) { // Hard Mode
                        std::cout << "Starting Hard Mode..." << std::endl;
                        // Add your hard mode logic here
                    } else if (selectedOption == 3) { // Show Progress
                        std::cout << "Viewing Progress..." << std::endl;
                        // Add your progress display logic here
                    } else if (selectedOption == 4) { // Quit Game
                        window.close(); // Exit
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        for (size_t i = 0; i < menuItems.size(); ++i) {
            window.draw(optionBoxes[i]);
            window.draw(menuItems[i]);
        }
        window.display();
    }
}

// Function to display Level 1
void showLevel1(sf::RenderWindow& window) {
    // Define the scrambled words and correct words
    std::string scrambledWord1 = "MONIS";   // Scrambled form of "Simon"
    std::string correctWord1 = "SIMON";     // Correct form of "Simon"

    std::string scrambledWord2 = "SYAS";    // Scrambled form of "Says"
    std::string correctWord2 = "SAYS";      // Correct form of "Says"

    std::string scrambledWord3 = "MEOG-REVA";  // Scrambled form of "Game-Over"
    std::string correctWord3 = "GAME-OVER";    // Correct form of "Game-Over"

    sf::Font font;
    if (!font.loadFromFile("Montserrat.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    // Load the background texture and create a sprite
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("simon.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return;
    }
    sf::Sprite background(backgroundTexture);

    // Create vectors to store the letter boxes and their positions for all three words
    std::vector<sf::RectangleShape> letterBoxes1, letterBoxes2, letterBoxes3;
    std::vector<sf::Text> letterTexts1, letterTexts2, letterTexts3;

    float boxWidth = 60.f;
    float boxHeight = 60.f;
    float gap = 10.f;

    // Create boxes and text for the first scrambled word (Simon)
    for (size_t i = 0; i < scrambledWord1.length(); ++i) {
        // First scrambled word letter boxes
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 200);  // Position boxes horizontally

        // Text for the first scrambled word
        sf::Text letterText(std::string(1, scrambledWord1[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes1.push_back(box);
        letterTexts1.push_back(letterText);
    }

    // Create boxes and text for the second scrambled word (Says)
    for (size_t i = 0; i < scrambledWord2.length(); ++i) {
        // Second scrambled word letter boxes
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 300);  // Position boxes below the first word

        // Text for the second scrambled word
        sf::Text letterText(std::string(1, scrambledWord2[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes2.push_back(box);
        letterTexts2.push_back(letterText);
    }

    // Create boxes and text for the third scrambled word (Game-Over)
    for (size_t i = 0; i < scrambledWord3.length(); ++i) {
        // Third scrambled word letter boxes
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 400);  // Position boxes below the second word

        // Text for the third scrambled word
        sf::Text letterText(std::string(1, scrambledWord3[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes3.push_back(box);
        letterTexts3.push_back(letterText);
    }

    std::vector<sf::RectangleShape> emptyBoxes;  // To track empty boxes for drag-and-drop functionality
    std::vector<sf::Text> emptyText;            // Text for empty boxes

    // Store the selected letter
    std::string playerGuess = "";
    int selectedIndex = -1;  // Track selected box

    // Feedback text that tells the player if they are correct
    sf::Text feedbackText("", font, 30);
    feedbackText.setFillColor(sf::Color::Green);
    feedbackText.setPosition(window.getSize().x / 2 - feedbackText.getGlobalBounds().width / 2, 450);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handling mouse clicks to select a letter box
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // First word (Simon)
                    for (size_t i = 0; i < letterBoxes1.size(); ++i) {
                        if (letterBoxes1[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            if (selectedIndex == -1) {
                                // Select the box for the first word
                                selectedIndex = i;
                                letterBoxes1[i].setFillColor(sf::Color::Yellow);  // Highlight selected box
                            } else {
                                // Swap letters between the selected box and clicked box
                                std::swap(scrambledWord1[selectedIndex], scrambledWord1[i]);

                                // Update text display for the swapped letters
                                letterTexts1[selectedIndex].setString(std::string(1, scrambledWord1[selectedIndex]));
                                letterTexts1[i].setString(std::string(1, scrambledWord1[i]));

                                
                                letterBoxes1[selectedIndex].setFillColor(sf::Color(50, 50, 50, 200));  // Unhighlight previous box
                                selectedIndex = -1;  
                            }
                        }
                    }
                   
                    for (size_t i = 0; i < letterBoxes2.size(); ++i) {
                        if (letterBoxes2[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            if (selectedIndex == -1) {
                                
                                selectedIndex = i + letterBoxes1.size();  
                                letterBoxes2[i].setFillColor(sf::Color::Yellow);  
                            } else {
                                
                                std::swap(scrambledWord2[selectedIndex - letterBoxes1.size()], scrambledWord2[i]);

                              
                                letterTexts2[selectedIndex - letterBoxes1.size()].setString(std::string(1, scrambledWord2[selectedIndex - letterBoxes1.size()]));
                                letterTexts2[i].setString(std::string(1, scrambledWord2[i]));

                               
                                letterBoxes2[selectedIndex - letterBoxes1.size()].setFillColor(sf::Color(50, 50, 50, 200));  // Unhighlight previous box
                                selectedIndex = -1;  
                            }
                        }
                    }
                    
                    for (size_t i = 0; i < letterBoxes3.size(); ++i) {
                        if (letterBoxes3[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            if (selectedIndex == -1) {
                                
                                selectedIndex = i + letterBoxes1.size() + letterBoxes2.size();  
                                letterBoxes3[i].setFillColor(sf::Color::Yellow);  
                            } else {
                                
                                std::swap(scrambledWord3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()], scrambledWord3[i]);

                                
                                letterTexts3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()].setString(std::string(1, scrambledWord3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()]));
                                letterTexts3[i].setString(std::string(1, scrambledWord3[i]));

                               
                                letterBoxes3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()].setFillColor(sf::Color(50, 50, 50, 200));  // Unhighlight previous box
                                selectedIndex = -1;  
                            }
                        }
                    }
                }
            }

    
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (scrambledWord1 == correctWord1 && scrambledWord2 == correctWord2 && scrambledWord3 == correctWord3) {
                    feedbackText.setString("Correct! Level 1 complete!");
                    feedbackText.setFillColor(sf::Color::Blue);  
                } else {
                    feedbackText.setString("Incorrect! Try again.");
                    feedbackText.setFillColor(sf::Color::Red);  
                }
            }
        }

        feedbackText.setPosition(window.getSize().x / 2 - feedbackText.getGlobalBounds().width / 2, 500);


       
        window.clear();
        window.draw(background);  
        for (size_t i = 0; i < letterBoxes1.size(); ++i) {
            window.draw(letterBoxes1[i]);
            window.draw(letterTexts1[i]);
        }
        for (size_t i = 0; i < letterBoxes2.size(); ++i) {
            window.draw(letterBoxes2[i]);
            window.draw(letterTexts2[i]);
        }
        for (size_t i = 0; i < letterBoxes3.size(); ++i) {
            window.draw(letterBoxes3[i]);
            window.draw(letterTexts3[i]);
        }
        window.draw(feedbackText);
        window.display();
    }
}

void showIntermediateMode(sf::RenderWindow& window) {
    // Define the scrambled words and correct words
    std::string scrambledWord1 = "IKME SI";   // Scrambled form
    std::string correctWord1 = "MIKE IS";     // Correct form
    std::string scrambledWord2 = "INGLY TOUAB";
    std::string correctWord2 = "LYING ABOUT";
    std::string scrambledWord3 = "REDRUM";


    sf::Font font;
    if (!font.loadFromFile("Montserrat.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    // Load the background texture and create a sprite
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("simon.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return;
    }
    sf::Sprite background(backgroundTexture);

    // Create vectors to store the letter boxes and their positions for each word
    std::vector<sf::RectangleShape> letterBoxes1, letterBoxes2, letterBoxes3;
    std::vector<sf::Text> letterTexts1, letterTexts2, letterTexts3;

    float boxWidth = 60.f;
    float boxHeight = 60.f;
    float gap = 10.f;

    // Create boxes and text for the first scrambled word (MIKE IS)
    for (size_t i = 0; i < scrambledWord1.length(); ++i) {
        if (scrambledWord1[i] == ' ') continue; // Skip spaces
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 200);

        sf::Text letterText(std::string(1, scrambledWord1[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes1.push_back(box);
        letterTexts1.push_back(letterText);
    }

    // Create boxes and text for the second scrambled word (LYING ABOUT)
    for (size_t i = 0; i < scrambledWord2.length(); ++i) {
        if (scrambledWord2[i] == ' ') continue; // Skip spaces
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 300);

        sf::Text letterText(std::string(1, scrambledWord2[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes2.push_back(box);
        letterTexts2.push_back(letterText);
    }

    // Create boxes and text for the third scrambled word (MURDER)
    for (size_t i = 0; i < scrambledWord3.length(); ++i) {
        if (scrambledWord3[i] == ' ') continue; // Skip spaces
        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setFillColor(sf::Color(50, 50, 50, 200));  // Grey color for the box
        box.setPosition(100 + i * (boxWidth + gap), 400);

        sf::Text letterText(std::string(1, scrambledWord3[i]), font, 40);
        letterText.setFillColor(sf::Color::White);
        letterText.setPosition(box.getPosition().x + (boxWidth - letterText.getGlobalBounds().width) / 2,
                               box.getPosition().y + (boxHeight - letterText.getGlobalBounds().height) / 2);

        letterBoxes3.push_back(box);
        letterTexts3.push_back(letterText);
    }

    std::vector<sf::RectangleShape> emptyBoxes;
    std::vector<sf::Text> emptyText;

    std::string playerGuess = "";
    int selectedIndex = -1;

    // Feedback text
    sf::Text feedbackText("", font, 30);
    feedbackText.setFillColor(sf::Color::Green);
    feedbackText.setPosition(window.getSize().x / 2 - feedbackText.getGlobalBounds().width / 2, 450);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handling mouse clicks to select a letter box
           if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        // First word (MIKE IS)
        for (size_t i = 0; i < letterBoxes1.size(); ++i) {
            if (letterBoxes1[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                if (selectedIndex == -1) {
                    // Select the box for the first word
                    selectedIndex = i;
                    letterBoxes1[i].setFillColor(sf::Color::Yellow);  // Highlight selected box
                } else {
                    // Swap the letters visually without modifying the original scrambled word
                    if (selectedIndex != i) {
                        char temp = scrambledWord1[selectedIndex];
                        scrambledWord1[selectedIndex] = scrambledWord1[i];
                        scrambledWord1[i] = temp;

                        // Update the text for the swapped letters
                        letterTexts1[selectedIndex].setString(std::string(1, scrambledWord1[selectedIndex]));
                        letterTexts1[i].setString(std::string(1, scrambledWord1[i]));

                        // Unhighlight previous box
                        letterBoxes1[selectedIndex].setFillColor(sf::Color(50, 50, 50, 200));
                        selectedIndex = -1;
                    }
                }
            }
        }

        // Repeat the same for the second and third words
        for (size_t i = 0; i < letterBoxes2.size(); ++i) {
            if (letterBoxes2[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                if (selectedIndex == -1) {
                    selectedIndex = i + letterBoxes1.size();
                    letterBoxes2[i].setFillColor(sf::Color::Yellow);
                } else {
                    if (selectedIndex != i + letterBoxes1.size()) {
                        char temp = scrambledWord2[selectedIndex - letterBoxes1.size()];
                        scrambledWord2[selectedIndex - letterBoxes1.size()] = scrambledWord2[i];
                        scrambledWord2[i] = temp;

                        letterTexts2[selectedIndex - letterBoxes1.size()].setString(std::string(1, scrambledWord2[selectedIndex - letterBoxes1.size()]));
                        letterTexts2[i].setString(std::string(1, scrambledWord2[i]));

                        letterBoxes2[selectedIndex - letterBoxes1.size()].setFillColor(sf::Color(50, 50, 50, 200));
                        selectedIndex = -1;
                    }
                }
            }
        }

        for (size_t i = 0; i < letterBoxes3.size(); ++i) {
            if (letterBoxes3[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                if (selectedIndex == -1) {
                    selectedIndex = i + letterBoxes1.size() + letterBoxes2.size();
                    letterBoxes3[i].setFillColor(sf::Color::Yellow);
                } else {
                    if (selectedIndex != i + letterBoxes1.size() + letterBoxes2.size()) {
                        char temp = scrambledWord3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()];
                        scrambledWord3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()] = scrambledWord3[i];
                        scrambledWord3[i] = temp;

                        letterTexts3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()].setString(std::string(1, scrambledWord3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()]));
                        letterTexts3[i].setString(std::string(1, scrambledWord3[i]));

                        letterBoxes3[selectedIndex - letterBoxes1.size() - letterBoxes2.size()].setFillColor(sf::Color(50, 50, 50, 200));
                        selectedIndex = -1;
                    }
                }
            }
        }
    }
}


        feedbackText.setPosition(window.getSize().x / 2 - feedbackText.getGlobalBounds().width / 2, 500);

        window.clear();
        window.draw(background);

        // Draw all the letter boxes and texts
        for (size_t i = 0; i < letterBoxes1.size(); ++i) {
            window.draw(letterBoxes1[i]);
            window.draw(letterTexts1[i]);
        }
        for (size_t i = 0; i < letterBoxes2.size(); ++i) {
            window.draw(letterBoxes2[i]);
            window.draw(letterTexts2[i]);
        }
        for (size_t i = 0; i < letterBoxes3.size(); ++i) {
            window.draw(letterBoxes3[i]);
            window.draw(letterTexts3[i]);
        }

        window.draw(feedbackText);
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Murder Mystery Puzzle Game");
    window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
    // Load the mysterious sound
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("mysterious.wav")) {
        std::cerr << "Failed to load audio!" << std::endl;
        return -1;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);

    showSplashScreen(window, sound);  
    showMainMenu(window);  

    return 0;
}



//g++ -std=c++11 murder_mystery_game.cpp -o murder_mystery_game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio










