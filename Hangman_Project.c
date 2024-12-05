#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_WORD_LENGTH 50
#define LEN 1024       
#define INIT 10        
#define GROW 2         
#define MAX_TRIES 7    
#define MAX_NAME_LENGTH 3
#define LEADERBOARD_FILE "leaderboard.txt"

// Structure for leaderboard
typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    int score;
    char difficulty[10];
} LeaderboardEntry;

// Structure for dictionary
typedef struct {
    int nval;     // Current number of words
    int max;      // Maximum capacity
    char **words; // Array of words
} Diction;

// Function Prototypes
Diction* file_open();
void free_mem(Diction* dictionary);
char* get_word_by_difficulty(Diction* dictionary, int difficulty);
void displayWord(const char secretWord[], const char guessedWord[]);
void drawHangman(int incorrectGuesses);
void printFalseGuesses(char falseGuesses[], int falseCount);
void save_to_leaderboard(const char* name, int score, const char* difficulty);
void display_leaderboard();

int main() {
    srand(time(NULL));

    // Opening dictionary
    Diction* dictionary = file_open();

    printf("Welcome to Hangman!\n");

    // Choose game mode
    int gameMode;
    printf("Select Game Mode:\n1. Singleplayer\n2. Multiplayer (2-4 players)\n");
    do {
        scanf("%d", &gameMode);
    } while (gameMode < 1 || gameMode > 2);

    // Number of players for multiplayer
    int numPlayers = 1;
    if (gameMode == 2) {
        printf("Enter the number of players (2-4): ");
        do {
            scanf("%d", &numPlayers);
        } while (numPlayers < 2 || numPlayers > 4);
    }

    // Players choose the difficulty
    int difficulty;
    printf("Select difficulty:\n1. Easy (4 letters or fewer)\n2. Medium (5-6 letters)\n3. Hard (7+ letters)\n");
    do {
        scanf("%d", &difficulty);
    } while (difficulty < 1 || difficulty > 3);

    char difficultyLabel[10];
    strcpy(difficultyLabel, difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");

    // Selects a word based on difficulty
    char* secretWord = get_word_by_difficulty(dictionary, difficulty);
    int wordLength = strlen(secretWord);

    char guessedWord[MAX_WORD_LENGTH] = {0};
    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }

    char falseGuesses[MAX_TRIES] = {0};
    bool guessedLetters[26] = {false};
    int incorrectGuesses = 0, falseCount = 0;

    int currentPlayer = 1;
    int totalGuesses = 0;

    while (incorrectGuesses < MAX_TRIES) {
        printf("\n");
        if (gameMode == 2) {
            printf("Player %d's turn\n", currentPlayer);
        }
        displayWord(secretWord, guessedWord);
        drawHangman(incorrectGuesses);
        printFalseGuesses(falseGuesses, falseCount);

        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);
        totalGuesses++;

        if (!isalpha(guess)) {
            printf("Invalid input. Please enter a letter.\n");
            continue;
        }

        if (guessedLetters[guess - 'a']) {
            printf("You've already guessed that letter. Try again.\n");
            continue;
        }

        guessedLetters[guess - 'a'] = true;

        bool found = false;
        for (int i = 0; i < wordLength; i++) {
            if (secretWord[i] == guess) {
                found = true;
                guessedWord[i] = guess;
            }
        }

        if (found) {
            printf("Good guess!\n");
        } else {
            printf("Sorry, '%c' is not in the word.\n", guess);
            falseGuesses[falseCount++] = guess;
            incorrectGuesses++;
        }

        if (strcmp(secretWord, guessedWord) == 0) {
            printf("\nCongratulations! ");
            if (gameMode == 1) {
                printf("You've guessed the word: %s\n", secretWord);
            } else {
                printf("Player %d guessed the word: %s\n", currentPlayer, secretWord);
            }

            // Saves to leaderboard
            char playerName[MAX_NAME_LENGTH + 1];
            printf("Enter your name (3 characters max): ");
            scanf("%s", playerName);
            playerName[MAX_NAME_LENGTH] = '\0'; // Ensure name is 3 characters max
            save_to_leaderboard(playerName, totalGuesses, difficultyLabel);

            break;
        }

        if (gameMode == 2) {
            currentPlayer = (currentPlayer % numPlayers) + 1; // Move to the next player
        }
    }

    if (incorrectGuesses >= MAX_TRIES) {
        printf("\nGame Over! The word was: %s\n", secretWord);
    }

    // Display leaderboard
    printf("\nLeaderboard:\n");
    display_leaderboard();

    // Free dictionary memory
    free_mem(dictionary);
    return 0;
}

// Function to save to leaderboard
void save_to_leaderboard(const char* name, int score, const char* difficulty) {
    FILE* fp = fopen(LEADERBOARD_FILE, "a");
    if (!fp) {
        perror("Error opening leaderboard file");
        return;
    }
    fprintf(fp, "%s %d %s\n", name, score, difficulty);
    fclose(fp);
}

// Function to display leaderboard
void display_leaderboard() {
    FILE* fp = fopen(LEADERBOARD_FILE, "r");
    if (!fp) {
        printf("No leaderboard data found.\n");
        return;
    }

    char name[MAX_NAME_LENGTH + 1];
    int score;
    char difficulty[10];
    printf("%-10s %-10s %-10s\n", "Name", "Guesses", "Difficulty");
    printf("--------------------------------\n");

    while (fscanf(fp, "%s %d %s", name, &score, difficulty) == 3) {
        printf("%-10s %-10d %-10s\n", name, score, difficulty);
    }
    fclose(fp);
}

// Function to display the current state of the word
void displayWord(const char secretWord[], const char guessedWord[]) {
    printf("\nWord: ");
    for (int i = 0; i < strlen(secretWord); i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}

// Hangman visuals for incorrect guesses
void drawHangman(int incorrectGuesses) {
    const char* hangmanStages[] = {
        "  _______\n |/      |\n |\n |\n |\n |\n_|___",         
        "  _______\n |/      |\n |      (_)\n |\n |\n |\n_|___",      
        "  _______\n |/      |\n |      (_)\n |       |\n |\n |\n_|___",  
        "  _______\n |/      |\n |      (_)\n |      \\|\n |\n |\n_|___",  
        "  _______\n |/      |\n |      (_)\n |      \\|/\n |\n |\n_|___", 
        "  _______\n |/      |\n |      (_)\n |      \\|/\n |       |\n |\n_|___",  
        "  _______\n |/      |\n |      (_)\n |      \\|/\n |       |\n |      /\n_|___", 
        "  _______\n |/      |\n |      (_)\n |      \\|/\n |       |\n |      / \\\n_|___"  
    };
    printf("%s\n", hangmanStages[incorrectGuesses]);
}

// Function to print wrong guesses
void printFalseGuesses(char falseGuesses[], int falseCount) {
    printf("False Letters: ");
    if (falseCount == 0) {
        printf("None");
    } else {
        for (int i = 0; i < falseCount; i++) {
            printf("%c ", falseGuesses[i]);
        }
    }
    printf("\n");
}

// Function to open and load the dictionary from a file
Diction* file_open() {
    FILE* fp = fopen("dictionary.txt", "r");
    if (!fp) {
        perror("Failed to open dictionary file");
        exit(EXIT_FAILURE);
    }

    Diction* dictionary = malloc(sizeof(Diction));
    assert(dictionary);

    dictionary->nval = 0;
    dictionary->max = INIT;
    dictionary->words = malloc(dictionary->max * sizeof(char*));
    assert(dictionary->words);

    char buf[LEN];
    while (fgets(buf, LEN, fp)) {
        if (dictionary->nval >= dictionary->max) {
            dictionary->max *= GROW;
            dictionary->words = realloc(dictionary->words, dictionary->max * sizeof(char*));
            assert(dictionary->words);
        }

        buf[strcspn(buf, "\n")] = '\0'; // Remove newline character
        dictionary->words[dictionary->nval] = strdup(buf);
        dictionary->nval++;
    }

    fclose(fp);
    return dictionary;
}

// Function to free memory allocated for the dictionary
void free_mem(Diction* dictionary) {
    for (int i = 0; i < dictionary->nval; i++) {
        free(dictionary->words[i]);
    }
    free(dictionary->words);
    free(dictionary);
}

// Function to get a word based on difficulty
char* get_word_by_difficulty(Diction* dictionary, int difficulty) {
    char* selectedWord = NULL;
    int maxLength = (difficulty == 1) ? 4 : (difficulty == 2) ? 6 : INT_MAX;
    int minLength = (difficulty == 1) ? 0 : (difficulty == 2) ? 5 : 7;

    // Loop for correct word length
    while (selectedWord == NULL) {
        int randomIndex = rand() % dictionary->nval;
        char* candidate = dictionary->words[randomIndex];
        int length = strlen(candidate);

        if (length >= minLength && length <= maxLength) {
            selectedWord = candidate;
        }
    }

    return selectedWord;
}
