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

void getGameInfo(int* numPlayers, int* wordSize, char wordInput[]);

int main() {
    bool GAMEOVER = false;
    bool wordComplete = false;
    int numPlayers = 4; // temporarily assigned value
    int guessesRemaining = 6; // temporary value
    int wordSize = 20; // temoporary value (maybe)
    char wordInput[wordSize];
    char wordGuess[wordSize];
    char *word = NULL; 
    char *wordDisplay = NULL;
    char availableGuesses[ALPHABET_SIZE];
    char currentGuess;
    
    // test word
    //strcpy(wordInput, "KEYBOARD");

    getGameInfo(&numPlayers, &wordSize, wordInput);

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
            printf("\nPlayer %d enter your guess or enter '?' to guess full word: ", i+1);
            scanf(" %c", &currentGuess);
            
            // check if guess is a valid characater
            if (currentGuess != '?' && isalpha(currentGuess) == 0){
                printf("Invalid Guess\n");
                continue;
            }
            
            // player attemps to guess word
            if (currentGuess == '?'){
                printf("Enter word: ");
                scanf("%s", wordGuess); /// MAKE ERROR MESSAGE IF GUESS IS TOO LONG
                // if(strlen(wordGuess) != wordSize){
                //     printf("\nIncorrect word length\n");
                //     continue;
                // }
                if (checkWordComplete(wordGuess, word, wordSize) == 1){
                    wordComplete = true;
                    break;
                }
                else{
                    printf("\nIncorrect\n");
                }
            }
            // player guesses and individual letter
            else{
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
                    printf("\nNot in word!\n");
                }

                // checks if word is complete
                if (checkWordComplete(wordDisplay, word, wordSize) == 1){
                    wordComplete = true;
                    break;
                }
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

void getGameInfo(int* numPlayers, int* wordSize, char wordInput[]){
    int difficulty = 0;
    printf("Welcome to Hangeman!\n");
    printf("Enter number of players: ");
    scanf("%d", numPlayers);
    printf("Select word difficulty\n1. Easy\n2. Medium\n3. Hard\n4. Custom\n");
    scanf("%d", &difficulty);
    if (difficulty == 1){
        strcpy(wordInput, "DUCK"); //temporary easy word
        printf("You selected Easy\n");
    }
    else if (difficulty == 2){
        strcpy(wordInput, "DIFFICULT"); //temporary medium word
        printf("You selected Medium\n");
    }
    else if (difficulty == 3){
        strcpy(wordInput,"TECHNOLOGICAL"); //temporary hard word
        printf("You selected Hard\n");
    }
    else if (difficulty == 4){
        printf("You selected Custom\n");
        printf("Enter costume word or type X to generate from custom list: ");
        scanf("%s", wordInput);
        if (strcmp(wordInput, "X") == 0 || strcmp(wordInput, "x") == 0){
            strcpy(wordInput, "CUSTOM"); // temporary random custom word
        }
        else{
            // converts user given word to uppercase
            for (int i = 0; i < strlen(wordInput); i++){
                wordInput[i] = toupper(wordInput[i]);
            }
        }
        
    }
    *wordSize = strlen(wordInput)+1;

}