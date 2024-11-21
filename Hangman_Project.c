#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// letters will always be represented in uppercase in guesses and words for consistency //

const int ALPHABET_SIZE = 26;

/*-----------------------------------------------------------------------------
-								Prototypes
-----------------------------------------------------------------------------*/
void printDisplay(char* display, char availableGuesses[], int wordSize);

int checkAvailableGuess(char guess, char availableGuesses[]);

int checkGuessInWord(char guess, char* word, int wordSize);

void updateDisplay(char* display, char* word, char guess, int wordSize);

int checkWordComplete(char* display, char* word, int wordSize);

int main() {
    bool GAMEOVER = false;
    bool wordComplete = false;
    int numPlayers = 4; // temporarily assigned value
    int guessesRemaining = 6; // temporary value
    int wordSize = 9; // temoporary value (maybe)
    char wordInput[wordSize];
    char *word = NULL; 
    char *wordDisplay = NULL;
    char availableGuesses[ALPHABET_SIZE];
    char currentGuess;
    
    // test word
    strcpy(wordInput, "KEYBOARD");

    // assigns secret word to input word
    word = (char*)malloc(wordSize * sizeof(char));
    for (int i = 0; i < wordSize; i++) {
        word[i] = wordInput[i];
    }

    wordDisplay = (char*)malloc(wordSize * sizeof(char));
    //sets displayed word to blank
    for (int i = 0; i < wordSize-1; i++) {
        wordDisplay[i] = '_';
    }

    //sets available guesses to alphabet
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        availableGuesses[i] = i + 'A';
    }
    

    while (!GAMEOVER){
        for (int i = 0; i < numPlayers; i++){
            printDisplay(wordDisplay, availableGuesses, wordSize);
            printf("\nPlayer %d enter your guess: ", i+1);
            scanf(" %c", &currentGuess);
            
            // check if guess is a letter
            if (isalpha(currentGuess) == 0){
                printf("Invalid Guess\n");
                continue;
            }
            // check is letter has already been guessed
            if (checkAvailableGuess(currentGuess, availableGuesses)==0){// UDPATE THIS
                printf("Already Guessed\n");
                continue;
            }

            // compares guess to word if letter is valid
            if (checkGuessInWord(currentGuess, word, wordSize)==1){
                printf("Correct!\n");
                updateDisplay(wordDisplay, word, currentGuess, wordSize);
            }
            else{
                printf("Not in word\n");
            }

            // checks if word is complete
            if (checkWordComplete(wordDisplay, word, wordSize) == 1){
                wordComplete = true;
                break;
            }

            //add delay
        }

        // exits game loop if word is complete
        if (wordComplete){
            GAMEOVER = true;
        }
    }

    // prints win message if word is completed
    if (wordComplete) {
        printf("\n\t************\n\t* You WIN! *\n\t************\n\n");
    }

}


/*-----------------------------------------------------------------------------
-								Functions
-----------------------------------------------------------------------------*/

// prints discovered letters in word and available letters
void printDisplay(char* display, char availableGuesses[], int wordSize){
    printf("\n\n");
    if (display != NULL){
        for (int i = 0; i < wordSize-1; i++){ // prints displayed word
            printf("%c ", display[i]);
        }
    }
    printf("\n\nAvailable Letters:\n"); // prints available letters
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (availableGuesses[i] != '-'){ // prints i if letter isn't removed/letter = '-'
            printf("%c ", availableGuesses[i]);
        }
    }
    printf("\n");
}


// compares current guess to available guesses and removes value from available guesses + returns one if guess is valid
int checkAvailableGuess(char guess, char availableGuesses[]){
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (toupper(guess) == availableGuesses[i]){
            availableGuesses[i] = '-';
            return 1;
        }
    }
    // if letter is not found return 0
    return 0;
}

// compares current guess to each letter in word and returns 1 if there is a match
int checkGuessInWord(char guess, char* word, int wordSize){
    for (int i = 0; i < wordSize; i++){
        if(word[i] == toupper(guess)){
            return 1;
        }
    }
    return 0;
}

// updates displayed word to include correctly guessed letter
void updateDisplay(char* display, char* word, char guess, int wordSize){
    for (int i = 0; i < wordSize; i++){
        if (toupper(guess) == word[i]){
            display[i] = toupper(guess);
        }
    }
}

// compares display word to actually word and returns 1 if every character matches
int checkWordComplete(char* display, char* word, int wordSize){
    //printf("Comparing %s with %s\n\n", display, word);
    for (int i = 0; i < wordSize; i++){
        if(word[i] != display[i]){
            return 0;
        }
    }
    return 1;
}

//Test