#include "gameover.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void saveHighScore(int score){
    string line;
    int highScore;
    string name;

    ifstream myfile;
    myfile.open("../highscore.txt");
    getline(myfile, line);
    highScore = stoi(line);
    getline(myfile, name);
    myfile.close();
    cout << "The High Score is: " << highScore << " By: " << name << endl;
    if (highScore < score){
        cout << "Congradulations - You've made a hew high score! ";
        ofstream myfile;
        myfile.open("../highscore.txt");
        cout << "Please Input Player Name: ";
        cin >> name;
        myfile << to_string(score) << endl;
        myfile << name << endl;
        myfile.close(); 
    }  
    cout << "CTL + C to EXIT!  Thanks for Playing!" << endl;
}