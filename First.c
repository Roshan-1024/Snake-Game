#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a fixed console width for alignment
#define WIDTH 50

void clearScreen() {
    // Clears the screen (works in most terminals)
    system("cls"); // Use "clear" for Linux/macOS
}

void printCentered(const char *text) {
    int len = strlen(text);
    int spaces = (WIDTH - len) / 2;

    for (int i = 0; i < spaces; i++) printf(" ");
    printf("%s\n", text);
}

void displayMenu() {
    clearScreen();

    // Display game title
    printf("\n");
    printCentered("=== SNAKE GAME ===");
    printf("\n");

    // Display menu options
    printCentered("1. New User");
    printCentered("2. Existing User");
    printCentered("3. Leaderboard");
    printCentered("4. Play Game");
    printCentered("5. About Developers");
    printCentered("6. Exit");
    printf("\n");
}

void handleMenuSelection() {
    int choice;
    int running = 1;

    while (running) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter Name: \n");
                system("pause"); // Wait for user input
                break;
            case 2:
                printf("\nChoose the player: \n");
                system("pause");
                break;
            case 3:
                printf("\nLeaderboard: \n");
                system("pause");
                break;
            case 4:
                printf("\nGame will start in 5 seconds!\n");
                system("pause");
                break;
            case 5:
                printf("\nAbout Developers\n");
                system("pause");
                break;
            case 6:
                printf("\nExiting the game. Goodbye!\n");
                running = 0; // End the loop
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                system("pause");
        }
    }
}

int main() {
    handleMenuSelection();
    return 0;
}
