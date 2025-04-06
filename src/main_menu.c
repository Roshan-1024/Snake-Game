#include "../include/main_menu.h"
#include "../include/common.h"

#define MAX_USERS 100

static char name[50]; // static variable to make the value persist even after function exits

void print_centered(const char *text){
    int len = strlen(text);
    int spaces = (SCREEN_WIDTH - len) / 2;

    for (int i = 0; i < spaces; i++) printf(" ");
    printf("%s\n", text);
}

void display_main_menu(){
    printf("\n");
    print_centered("=== SNAKE GAME ===");
    printf("\n");
    print_centered("Existing User");
    print_centered("New User");
    print_centered("Leaderboard");
    print_centered("How To Play");
    print_centered("Exit");
    printf("\n");
}

void usernameInput(){
    printf("Enter your username: ");
    fgets(name,sizeof(name),stdin);
    name[strcspn(name, "\n")] = '\0';
}

bool usernameExists() {
    FILE *fptr = fopen("data/usernames.txt", "r");
    char existing_name[50];
    while (fgets(existing_name, sizeof(existing_name), fptr) != NULL) {
        existing_name[strcspn(existing_name, "\n")] = '\0';
        if (strcmp(name, existing_name) == 0) {
            fclose(fptr);
            return true;
        }
    }    
    fclose(fptr);
    return false;    
}

void print_Leaderboard(){
    system("clear || cls");

    typedef struct{
        char name[50];
        uint16_t score;
    } Player;

    Player players[MAX_USERS];
    int count = 0;
    FILE *file = fopen("data/highscores.csv", "r");
    while (fscanf(file, " %50[^,],%hu", players[count].name, &players[count].score) == 2 && count < MAX_USERS){
        count++;
    }
    fclose(file);

    // Sort players by score (descending) using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // Print leaderboard
    print_centered("\n    Leaderboard    \n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - %hu\n", i + 1, players[i].name, players[i].score);
    }
    printf("\n\nPress <Enter> to go back to Main Menu...");
}

void print_HowToPlay(){
    system("clear || cls");

    print_centered("How To Play");
    printf("\n1. Move the snake with h-j-k-l controls\n");
    printf("    Up    : k\n");
    printf("    Down  : j\n");
    printf("    Left  : h\n");
    printf("    Right : l\n");
    printf("2. The snake cannot move in reverse direction.\n");
    printf("3. Eat the food (+) to grow longer and score up.\n");
    printf("4. When the snake collides with the boundary or itself, the game ends.\n");
    printf("5. Press 'p' to pause/play the game.\n");
    printf("6. Press 'q' to quit the game.\n\n");
    printf("Press <Enter> to go back to Main Menu...");    
}

const char* run_main_menu() {
    int choice;
    bool running = true;

    while(running){ 
        system("clear || cls");
        display_main_menu();
        printf("Enter your choice (1-5): ");
        scanf("%d",&choice);
        while(getchar()!='\n');

        switch(choice){
            
            // EXISTING USER
            case 1:{
                usernameInput();            
                usernameExists();
                if(usernameExists()) {
                    running = false;
                }
                else {
                    printf("User does not exist.\nPress Enter to continue...");
                    getchar();
                }
                break;
            }
            
            // REGISTER NEW USER
            case 2:{
                usernameInput();
                usernameExists();
                if(!usernameExists()){
                    FILE* fptr = fopen("data/usernames.txt", "a");
                    fprintf(fptr, "%s\n", name);
                    fclose(fptr);
                    printf("New user added.\nPress Enter to continue...");
                    while (getchar() != '\n');
                }
                else {
                    printf("Username already exists.\nPress Enter to continue...");
                    while (getchar() != '\n');
                }
                break;
            }

            // Display LEADERBOARD
            case 3: {
                print_Leaderboard();
                while (getchar() != '\n');
                break;
            }
            
            // Display How To Play
            case 4:{
                print_HowToPlay();
                while (getchar() != '\n');
                break;
            }
            
            // EXIT THE GAME
            case 5:{
                printf("Exiting the game...\n");
                running = false;
                exit(0);
            }
            
            default:{
                printf("Invalid choice. Please try again.");
                getchar();
            }
        }
    }
return name;
}
