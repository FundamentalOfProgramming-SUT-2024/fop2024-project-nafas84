#ifndef MENU_H
#define MENU_H

#include<ncurses.h>
#include<string.h>
#include<ctype.h>
#include<regex.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<time.h>

extern int GAME_M;
typedef struct{
    char username[50];
    int difficulty; // 0 -> easy , 1 -> medium , 2 -> hard
    int color;     //  0 -> white ,1 -> blue  , 2 -> red 
    int load_game;
}Setting;

extern Setting current_user;
// User information
typedef struct {
    char username[50];
    char password[50];
    char email[50];
} User;

// Functions
int show_menu();
void Print_Menu(int highlight, const char* choices[], int n_choices);
    // Main Menu
    void Creat_New_Account();
    int Check_Username(const char *username);
    int Validate_Password(const char *password);
    int Validate_Email(const char *email);
    void Save_User(const User *user);
    // Login Menu
    void Login_Menu();
    int Check_Password(const char* username , const char *password);
    void Login();
    // Pre Menu
    void Pre_Game_Menu();
    void ScoreBoard();
    void Setting_Menu();
    void Profile_Menu();


#endif