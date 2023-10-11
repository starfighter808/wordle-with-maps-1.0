#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

void printOptions();
void optionsInterface();
void printChangeLives();
void changeLives();
void vectorSetup();
void randAnswerWord();
void displayMenu();
void menuInterface();
void displayCredits();
void setLives();
void mapInit();
void printGuessFeedback();
void getGuess();
void printPlayAgain();
void playGame();

map<char, map<char, map<char, vector<string>>>> firstLetter;
string answerWord;
int wordCount = 0;
//this variable is for tracking how many lives the user has at any given moment
int currentlifeCount = 6;
//this variable tracks how many lives the player will start the game with
int maxLives = 6;
bool exitFlag = false;
vector<string> wordVector;
bool playerWin = false;

int main()
{
    // set up our word vector (this requires access to answerlistsorted.txt)
    vectorSetup();
    // set up our nested maps (this also requires access to answerlistsorted.txt)
    mapInit();
    // Display the menu to the user
    displayMenu();
    // this accepts user input and starts they game if they choose
    menuInterface();

    return 0;
}
void playAgainInterface()
{
    int wait = 0;
    cin >> wait;
    switch (wait)
    {
    case 1:
        playGame();
        break;
    case 2:
        displayMenu();
        menuInterface();
        break;
    case 3:
        exitFlag = true;
        break;
    default:
        cout << "Please enter a single digit number. Other inputs not accepted." << endl;
        playAgainInterface();
    }
}
void printPlayAgain()
{
    cout << "Play again?" << endl;
    cout << "1. Play Again" << endl;
    cout << "2. Quit to Main Menu" << endl;
    cout << "3. Quit to Desktop" << endl;
}
void printGuessFeedback(string checkWord)
{
    const char *correctLetter = "\x1B[42;97m";
    const char *halfLetter = "\x1B[43;97m";
    const char *wrongLetter = "\x1B[0m";

    int deletedItems = 0;
    // this will store integers so that we can more easily track which position needs to be which color
    int colorArray[5] = {0, 0, 0, 0, 0};

    // first let's initialize a vector that will contain each of the letters from our answer and another that will contain the positions of those letters
    vector<char> answerLetterVector;
    // this loop pushes all letters in checkWord to our vector answerLetterVector and logs their positions in answerPositionVector
    for (int i = 0; i < 5; i++)
    {
        answerLetterVector.push_back(answerWord[i]);
    }

    // now we compare our answer vector to our original word and erase the character from our vector when we find it in the checkWord
    // after we do this for green letters, we can do the same for yellow letters and anything leftover after that we dont need

    // remember to prioritize green letters first
    // run for every letter once
    for (int n = 0; n < 5; n++)
    {
        // if the letter at position 'i' matches in 'checkWord' and 'answerLetterVector' then...
        if (checkWord[n] == answerLetterVector[n - deletedItems])
        {
            // change the color of that letter to green
            colorArray[n] = 2;
            // erase the letter from the vector at position 'n' because this moves all of the vector elements above it, we must increment our deleted items counter
            answerLetterVector.erase(answerLetterVector.begin() + (n - deletedItems));
            deletedItems++;
        }
    }

    // after the above code section we should be left with a partial that define which letters we have left
    // we need to scan the letter vector to see if any letters match and if they do, we need to color them yellow and remove them from the vector
    // remember that our vector could potentially be less than 5 here, so running a for loop bounded by i could run into memory we dont own, so we create two for loops
    for (int i = 0; i < 5; i++)
    {
        for (int n = 0; n < answerLetterVector.size(); n++)
        {
            if (checkWord[i] == answerLetterVector[n] && colorArray[i] == 0)
            {
                answerLetterVector.erase(answerLetterVector.begin() + n);
                colorArray[i] = 1;
            }
        }
    }
    // print the word using colorArray
    for (int i = 0; i <= 4; i++)
    {
        if (colorArray[i] == 0)
        {
            cout << wrongLetter << checkWord[i];
        }
        else if (colorArray[i] == 1)
        {
            cout << halfLetter << checkWord[i];
        }
        else if (colorArray[i] == 2)
        {
            cout << correctLetter << checkWord[i];
        }
        cout << wrongLetter;
    }
    cout << endl;
}
void vectorSetup()
{
    // create a file stream object labelled 'wordText'
    fstream wordText;

    // open our file labelled "answerlistsorted.txt"
    wordText.open("answerlistsorted.txt");

    // check to make sure the file opened properly and run the following code if it worked
    if (wordText.is_open() == true)
    {

        // begin a loop that prints every word in our list and counts the words
        while (wordText.eof() == false)
        {
            // declare a local string 'tempWord', this will be used to load words into our vector
            string tempWord;

            // This places the current word of the file into tempword
            wordText >> tempWord;

            // This places tempWord into "wordVector"
            wordVector.push_back(tempWord);

            // increment the counter that tracks how many words we have
            wordCount++;
        }
    }
    else
    {
        // print error message
        cout << "Error opening file." << endl;
    }
    // this line helps for making sure that all the words are read from the file
    cout << "I counted " << wordCount << " objects." << endl;

    // dont forget to close our file
    wordText.close();
}
void setLives()
{
    currentlifeCount = maxLives;
}
void randAnswerWord()
{
    // we need to seed the random number generator
    srand(time(NULL));

    // this declares a new integer 'answerNumber' then loads it with a random value from 0 to (count+1)
    int answerNumber = rand() % (wordCount + 1);

    // this loads 'answerWord' with the word located at position 'answerNumber' within the vector 'wordVector'
    answerWord = wordVector[answerNumber];
}
bool mapCheck(string checkWord)
{
    // first we scan the 'firstLetter' map for 'aLetter', this will return the end of our list if the letter is not found
    auto key1 = firstLetter.find(checkWord[0]);

    // this will not run if 'aLetter' is not found in the list
    if (key1 != firstLetter.end())
    {

        // locally name the middle map 'secondLetter', remember that 'key1->second' contains an address to the next map so we must put an '&' by 'auto'
        auto &secondLetter = key1->second;

        // next scan the 'secondLetter' map for 'anotherLetter' this will return the end of the map if the letter is not found
        auto key2 = secondLetter.find(checkWord[1]);

        // this will not run if 'anotherLetter'
        if (key2 != secondLetter.end())
        {

            // locally name the innermost map 'thirdLetter', remember that 'key2->second' contains an address to the next map so we must put an '&' by 'auto'
            auto &thirdLetter = key2->second;

            // search our map 'thirdLetter' for 'letterThree'
            auto key3 = thirdLetter.find(checkWord[2]);

            // this will not run if 'letterThree' is not in the list
            if (key3 != thirdLetter.end())
            {
                // locally name our vector 'innerWordList' remember that 'key3->second' is an address so we put an & next to 'vector<string>'
                const vector<string> &innerWordList = key3->second;

                // run this as many times as the size of innerWordList
                for (int i = 0; i < innerWordList.size(); ++i)
                {
                    if (innerWordList[i] == checkWord)
                    {
                        return 1;
                    }
                }
            }
            else
            {
                // error code (this will run if there are no words that start with those three letters.)
                cout << "There are no words that start with those three letters. Please try again." << endl;
            }
        }
        else
        {
            // error code (this will run if there are no words that start with those two letters.)
            cout << "There are no words that start with those two letters. Please try again." << endl;
        }
    }
    else
    {
        // error code (this will run if there are no words that start with that letter; however, given that our list has so many words in it, this error code will hopefully never be seen.)
        cout << "Please only type english letters." << endl;
    }
    // this will print if the vector is found but the word is not located within the vector
    cout << "This word is invalid. Please enter another." << endl;
    return 0;
}
void getGuess()
{
    string playerGuess;
    cin >> playerGuess;
    if (mapCheck(playerGuess) == true)
    {

        if (playerGuess == answerWord)
        {
            printGuessFeedback(playerGuess);
            cout << "You win!" << endl;
            playerWin = true;
        }
        else
        {
            printGuessFeedback(playerGuess);
            currentlifeCount--;
            cout << "You have " << currentlifeCount << " lives remaining." << endl;
        }
    }
    else
    {
        getGuess();
    }
}
void playGame()
{
    cout << "Game starting, Good Luck!" << endl;
    playerWin = false;
    randAnswerWord();
    setLives();
    // the line of code below is for developer testing
    // cout << "The word is " << answerWord << endl;
    while (currentlifeCount != 0 && playerWin != true)
    {
        getGuess();
    }
    if (currentlifeCount == 0)
    {
        cout << "Sorry, you are out of guesses and have lost." << endl;
        cout << "The word was " << answerWord << endl;
    }
    printPlayAgain();
    playAgainInterface();
}
void displayMenu()
{
    cout << "Welcome to Wordle with Maps!" << endl;
    cout << "Main Menu" << endl
    << endl;
    cout << "1. Start Game" << endl;
    cout << "2. Credits" << endl;
    cout << "3. Options" << endl;
    cout << "4. Quit" << endl;
}
void displayCredits()
{
    cout << "This code was made by Kyle Jungman in 2023 as an opportunity to test my coding skills and grow my portfolio." << endl;
    cout << "Special thanks to Mike McHugh, who taught me everything I know and inspired me to use maps to organize my data." << endl;
    cout << "Press any key to continue." << endl;
    cin.get();
    displayMenu();
}
void menuInterface()
{
    while (exitFlag != true){
    cout << "Please enter a number: ";
    int userMenuInput = 0;
    cin >> userMenuInput;
    switch (userMenuInput)
    {
    case 1:
        playGame();
        break;
    case 2:
        displayCredits();
        break;
    case 3:
        printOptions();
        optionsInterface();
        break;
    case 4:
        exitFlag = true;
        break;
    default:
        cout << "Please enter a single digit number. Other inputs not accepted." << endl;
        menuInterface();
    }
}
}
void mapInit()
{
    // this string will be used to transfer words from our txt file to our map.
    string tempWord;

    // create our fstream class object 'wordList'
    fstream wordText;

    // open our file labelled "answerlistsorted.txt"
    wordText.open("acceptedAnswerList.txt");

    // check to make sure the file opened properly and run the following code if it worked
    if (wordText.is_open() == true)
    {
        cout << "The file is now open" << endl;

        // begin a loop that prints every word in our list and counts the words
        while (wordText.eof() == false)
        {
            // This places the current word of the file into tempword
            wordText >> tempWord;

            // move tempWord into the map
            firstLetter[tempWord[0]][tempWord[1]][tempWord[2]].push_back(tempWord);
        }
        // close the text file
        wordText.close();
    }
}
void printChangeLives(){
    cout << "Please enter a number to change the number of lives you will start the game with." << endl;
    cout << "Walk in the Park = 10+" << endl;
    cout << "Very Easy = 9" << endl;
    cout << "Easy = 8" << endl;
    cout << "Wordle Lite = 7" << endl;
    cout << "Medium/Default = 6" << endl;
    cout << "Hard = 5" << endl;
    cout << "Insane = 4" << endl;
    cout << "Crazy = 3-" << endl;
}
void changeLives(){
    cout << "Change Lives to: ";
    cin >> maxLives;
        if(cin.fail()){
            cin.clear();
            cin.ignore();
            cout << "Please only enter numbers." << endl;
            changeLives();
        } else {
            cout << "Lives changed to: " << maxLives << endl << endl;
        }
}
void optionsInterface(){
    int userInput = 99;
    cout << "Please enter a number: ";
    cin >> userInput;
    switch (userInput)
    {
    case 1:
        printChangeLives();
        changeLives();
        printOptions();
        break;
    case 2:
        break;
    default:
        cout << "Please select an option from the list using a single digit number." << endl;
        break;
    }
}
void printOptions(){
    cout << "Welcome to the Options Menu" << endl << endl;
    cout << "1. Change Lives" << endl;
    cout << "2. Back to Menu" << endl;
}