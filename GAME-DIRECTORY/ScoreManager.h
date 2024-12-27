#define SCOREMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ScoreManager {
public:
    // Function to save user scores
    static void saveScore(const string& username, int score) {
        ofstream outFile("scores.txt", ios::app); // Open file in append mode
        if (outFile) {
            outFile << username << " " << score << endl;
            cout << "Score saved successfully!" << endl;
        } else {
            cerr << "Error opening file for writing." << endl;
        }
        outFile.close();
    }

    // Function to display all scores
    static void displayScores() {
        ifstream inFile("scores.txt");
        if (inFile) {
            string username;
            int score;
            cout << "User Scores:" << endl;
            while (inFile >> username >> score) { // Read username and score from file
                cout << username << ": " << score << endl;
            }
        } else {
            cerr << "Error opening file for reading." << endl;
        }
        inFile.close();
    }
};

