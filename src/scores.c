#include "../include/scores.h"
#include "../include/common.h"

#define MAX_USERS 100 // no. of lines

void updatehighscore(const char *user, uint16_t curr_score) {

    typedef struct{
        char name[50];
        uint16_t score;
    } Player;

    Player players[MAX_USERS];
    int linecount = 0;
    bool user_found = false;

    FILE *file = fopen("data/highscores.csv", "r");
    if(!file){  // Assuming first user registers their score
        // make a new file
        fopen("data/highscores.csv", "w+");
    }

    ////////////////////////////////////////////////////////////////////////////
    //      CHECK IF USER HAS PRE-EXISTING HIGHSCORE IN highscore.csv
    ////////////////////////////////////////////////////////////////////////////

    // store each username in highscore.csv
    while (fscanf(file, " %49[^,],%hu", players[linecount].name, &players[linecount].score) == 2 && linecount < MAX_USERS){
        linecount++;
    }
    fclose(file);
    
    for (int i = 0; i < linecount; i++) {
        if(strcmp(players[i].name, user) == 0){   // if current user found in highscores.csv (=> has a pre-existitng highscore)
            user_found = true;
            // new highscore created
            if(curr_score > players[i].score){
                printf("You made a new high score: %hu\n", curr_score);
                players[i].score = curr_score;
            }
            else    // not a highscore
                printf("Your high score is: %hu\n", players[i].score);
        }
    }   

    // if no highscore exists for the current user
    if(!user_found) {
        if(linecount < MAX_USERS){
            strcpy(players[linecount].name, user);
            players[linecount].score = curr_score;  // Adds current score as new high score
            printf("You made a new high score: %hu\n", curr_score);
            linecount++;
        } 
        else{
            printf("Error: High score list is full!\n");
            return;
        }
    }

    ///////////////////////////////////////////////////////
    //      UPDATE THE highscore.csv file
   ///////////////////////////////////////////////////////

   FILE *tempFile = fopen("data/highscores_temp.csv", "w");
   if(!tempFile){
      perror("Error opening highscores_temp.csv");
      return;
    }

    for(int i = 0; i < linecount; i++){
       fprintf(tempFile, "%s,%hu\n", players[i].name, players[i].score);
    }   
    fclose(tempFile);

    remove("data/highscores.csv");
    rename("data/highscores_temp.csv", "data/highscores.csv");
} 
