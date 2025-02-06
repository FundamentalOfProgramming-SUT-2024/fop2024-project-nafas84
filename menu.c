#include"menu.h"
#include"game.h"
// Window width & Height
int w, h;
char message[150];
int GAME_M = 0;
Setting current_user;

int show_menu(){
    // Start curses mode
    init_ncurses();
    // Set color
    set_color();
    
    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 3;
    const char* user_menu[] = {
    "Create New account",
    "Login Menu",
    "Exit"
    };

    strcpy(message , "\t Welcome to Rogue");

    while(!GAME_M) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);
        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        mvprintw(1 ,1 ,"%s",message);

        Print_Menu(highlight , user_menu , n_choices);
        int ch = getch();
    
        switch (ch) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = n_choices - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight >= n_choices) highlight = 0;
                break;

            case 10:
                if      (highlight == 0) {Creat_New_Account(); break;}
                else if (highlight == 1) {Login_Menu();        break;}
                else if (highlight == 2) {return 0;}
                break;
        }
    }
    return 1;
}
void Print_Menu(int highlight,const char* choices[], int n_choices) {
    for(int i = 0 ; i < n_choices ; i++){
        if(i == highlight)
            attron(A_REVERSE);
        mvprintw(h/2 + i*2, (w - strlen(choices[i]))/2 ,"%s", choices[i]);
        if(i == highlight)
            attroff(A_REVERSE);
    }
    refresh();
}
void Creat_New_Account(){
    clear();
    User user;
    int validate[3] = {0 ,0 ,0};
    strcpy(message, "");

    while (1) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);

        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);

        //Print Erors
        attron(COLOR_PAIR(2));
        mvprintw(1 ,1 ,"%s",message);
        attroff(COLOR_PAIR(2));

        // Get the username
        if (!validate[0]) {
            mvprintw(h/2, (w - 17)/2, "Enter Username: ");
            echo();
            getstr(user.username);
            noecho();

            if (Check_Username(user.username)) {
                strcpy(message, "\t Username already exists.");
                continue;
            }

            validate[0] = 1;
            strcpy(message, "");

        }else {
            mvprintw(h/2, (w - 17)/2, "Username: %s" , user.username);
        }

        // Get the password
        if (!validate[1]) {
            mvprintw(h/2 + 2, (w - 17)/2, "Enter Password: ");
            echo();
            getstr(user.password);
            noecho();

            if (Validate_Password(user.password)) {
                strcpy(message, "\t Password must be :at least 7 characters & capital letter & lowercase letter & digit.");
                continue;
            }

            validate[1] = 1;
            strcpy(message, "");
        }else {
            mvprintw(h/2 + 2, (w - 17)/2,"Password: *********");
        }

        // Get the email
        if (!validate[2]) {
            mvprintw(h/2 + 4, (w - 17)/2, "Enter Email: ");
            echo();
            getstr(user.email);
            noecho();

            if (Validate_Email(user.email)) {
                strcpy(message, "\t Invalid email format.");
                continue;
            }

            validate[2] = 1;
            strcpy(message, " ");
        }else {
            mvprintw(h/2 + 4, (w - 17)/2, "Email: %s", user.email);
        }

        //Save user's informations
        if (validate[0] && validate[1] && validate[2]) {
            Save_User(&user);
            clear();
            attron(COLOR_PAIR(3));
            mvprintw(h/2, (w -27)/2, "User created successfully!");
            attroff(COLOR_PAIR(3));
            mvprintw(h/2 + 2, (w -43)/2, "Press Enter to return to the main menu....");
            int ch = getch();
            if(ch == 10) return;
        }
    }
    strcpy(message, "\t Welcome back!");
    return;
}
void Login_Menu() {
    clear();

    int highlight = 0;
    int n_choices = 3;
    const char* login_menu[] = {
    "Guest",
    "Login",
    "Back to main menu"
    };

    strcpy(message , "\t Welcome to login menu!");

    while(!GAME_M) {

        // Get window width & Height
        getmaxyx(stdscr, h, w);
        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        mvprintw(1 ,1 ,"%s",message);

        Print_Menu(highlight , login_menu , n_choices);
        int ch = getch();
    
        switch (ch) {

            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = n_choices - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight >= n_choices) highlight = 0;
                break;

            case 10:
                if (highlight == 0) {
                    // Change directory for Guests
                    //char path[50] = "/home/nafise/Rogue/Guest";
                    //chdir(path);
                    // current user setting
                    strcpy(current_user.username, "Guest");
                    current_user.color = 0;
                    current_user.difficulty = 0;
                    Pre_Game_Menu();
                    break;
                }
                else if (highlight == 1) {Login(); break;}
                else if (highlight == 2) {strcpy(message , "\t Welcome back"); return;}
                break;
        }
    }
}
void Login() {
    clear();
    User person;
    int validate[2] = {0 ,0};
    strcpy(message, "");

    while (1) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);

        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        //Print Erors
        attron(COLOR_PAIR(2));
            mvprintw(1 ,1 ,"%s",message);
        attroff(COLOR_PAIR(2));

        // Get the username
        if (!validate[0]) {
            mvprintw(h/2, (w - 17)/2, "Enter Username: ");
            echo();
            getstr(person.username);
            noecho();

            if (!Check_Username(person.username)) {
                strcpy(message, "\t Username does not exist.");
                continue;
            }

            validate[0] = 1;
            strcpy(message, "");

        }else {
            mvprintw(h/2, (w - 17)/2, "Username: %s" , person.username);
        }

        // Get the password
        if (!validate[1]) {
            mvprintw(h/2 + 2, (w - 17)/2, "Enter Password: ");
            echo();
            getstr(person.password);
            noecho();

            if (Check_Password(person.username,person.password)) {
                strcpy(message, "\t The password is incorrect. Try again.");
                continue;
            }
            validate[1] = 1;
            strcpy(message, "");
        }

        if (validate[0] && validate[1]) {
            //Change Directory
            //char path[50];
            //sprintf(path ,"/home/nafise/Rogue/%s" ,person.username);
            //chdir(path);

            // current user setting
            strcpy(current_user.username, person.username);
            current_user.color = 0;
            current_user.difficulty = 0;

            clear();
            attron(COLOR_PAIR(3));
            mvprintw(h/2, (w -27)/2, "You have successfully logged in!");
            attroff(COLOR_PAIR(3));
            mvprintw(h/2 + 2, (w -28)/2, "Press Enter to continue....");
            int ch = getch();
            if(ch == 10) Pre_Game_Menu();
            if(GAME_M == 1) return;
        }
    }
}
void Pre_Game_Menu() {
    clear();
    sprintf(message,"\tWelcome to Pre-Menu game %s",current_user.username);

    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 4;
    const char* pre_game_menu[] = {
        "New Game",
        "Resume Game",
        "Scoreboard",
        "Setting"
    };  
    while(1) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);

        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        mvprintw(1 ,1 ,"%s",message);

        Print_Menu(highlight , pre_game_menu , n_choices);
        int ch = getch();
    
        switch (ch) {

            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = n_choices - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight >= n_choices) highlight = 0;
                break;

            case 10:
                if(highlight == 0){
                    current_user.load_game = 0;
                    GAME_M = 1;
                    return;
                }
                else if(highlight == 1 && strcmp(current_user.username,"Guest") != 0){
                    current_user.load_game = 1;
                    GAME_M = 1;
                    return;
                }
                else if(highlight == 2){
                    ScoreBoard();
                }
                else if(highlight == 3){
                    Setting_Menu();
                }
                break;
        }
    }
}
void ScoreBoard(){
    FILE* file = fopen("Scoreboard.txt","r");
    char line[250] = {0};

    typedef struct board{
        char name[50];
        int score;
        int gold;
        int games;
        int time;
    }Board;
    Board users[200];

    /*for(int z = 0; z < 200; z++){
        users[z].score = 0;
        users[z].games = 0;
        users[z].gold = 0;
        users[z].time = 0;
    }*/

    int i = 0;
    while(fgets(line,sizeof(line),file)){
        sscanf(line,"username:%[^,],score:%d,gold:%d,games:%d,time:%d",
        users[i].name, &users[i].score, &users[i].gold, &users[i].games, &users[i].time);        
        i++;
    }
    fclose(file);

    // sorting uses based on score +  gold (bubble sort)
    int cur_j = 0;
    for (int t = 0; t < i; t++) {
        for (int j = 0; j < i - 1; j++) {
            
            int totalScoreA = users[j].score + users[j].gold;
            int totalScoreB = users[j + 1].score + users[j + 1].gold;
            if(strcmp(users[j].name, current_user.username) == 0){
                cur_j = j;
            }
            if (totalScoreA < totalScoreB) {
                Board temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
    if(strcmp(users[i-1].name, current_user.username) == 0){
        cur_j = i-1;
    }

    // display scoreboard
    clear();
    attron(COLOR_PAIR(1));
    box(stdscr , 0 , 0);
    mvprintw(1, 1, "\tPress enter to back to Pre-Menu Game");
    mvprintw(3, (w - 70)/2 , "Rank    Username                Scores    Golds    Games    Wins");
    mvprintw(4, (w - 70)/2 , "________________________________________________________________");
    attroff(COLOR_PAIR(1));

    for(int j = 0; j < i && j < (h - 2)/2; j++){
        char curline[200];
        // rank1
        if(j == 0){
            attron(A_BOLD);
            attron(COLOR_PAIR(7));
            sprintf(curline,     "%-4d    🥇 Goat %-12s    %-6d    %-5d    %-5d    %-4d"
            ,j+1,users[j].name, users[j].score, users[j].gold, users[j].games, users[j].time);
        }
        // rank2
        else if(j == 1){
            attron(A_BOLD);
            attron(COLOR_PAIR(12));
            sprintf(curline,     "%-4d    🥈 Legend %-11s   %-6d    %-5d    %-5d    %-4d"
            ,j+1,users[j].name, users[j].score, users[j].gold, users[j].games, users[j].time);
        }
        // rank3
        else if(j == 2){
            attron(A_BOLD);
            attron(COLOR_PAIR(6));
            sprintf(curline,     "%-4d    🥉 Myth %-13s   %-6d    %-5d    %-5d    %-4d"
            ,j+1,users[j].name, users[j].score, users[j].gold, users[j].games, users[j].time);
        }
        // curent user
        else if(j == cur_j){
            attron(COLOR_PAIR(3));
            sprintf(curline,     "%-4d    %-20s    %-6d    %-5d    %-5d    %-4d"
            ,j+1,users[j].name, users[j].score, users[j].gold, users[j].games, users[j].time);
        }
        // others
        else{
            attron(COLOR_PAIR(0));
            sprintf(curline,     "%-4d    %-20s    %-6d    %-5d    %-5d    %-4d"
            ,j+1,users[j].name, users[j].score, users[j].gold, users[j].games, users[j].time);
        }

        mvprintw(6 + j*2 , (w - 70)/2,"%s", curline);

        if(j == 0)          {attroff(COLOR_PAIR(7));  attroff(A_BOLD);}
        else if(j == 1)     {attroff(COLOR_PAIR(12)); attroff(A_BOLD);}
        else if(j == 2)     {attroff(COLOR_PAIR(6));  attroff(A_BOLD);}
        else if(j == cur_j) {attroff(COLOR_PAIR(3));}
        else                {attroff(COLOR_PAIR(0));}
    }
    
    int ch = getch();
    while(ch != 10) ch = getch();
    return;
}
void Setting_Menu(){
    clear();
    strcpy(message,"\t Choose difficulty:");

    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 3;
    const char* setting_game_menu1[] = {
        "EASY",
        "MEDIUM",
        "HARD",
    };  
    int choose = 0;
    while(!choose) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);

        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        mvprintw(1 ,1 ,"%s",message);

        Print_Menu(highlight , setting_game_menu1 , n_choices);
        int ch = getch();
    
        switch (ch) {

            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = n_choices - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight >= n_choices) highlight = 0;
                break;

            case 10:
                if(highlight == 0){ 
                    current_user.difficulty = 0;
                }
                else if(highlight == 1){
                    current_user.difficulty = 1;
                }
                else if(highlight == 2){
                    current_user.difficulty = 2;
                }
                choose = 1;
                break;
        }
    }
    clear();
    strcpy(message,"\t Choose Your Hero color:");

    const char* setting_game_menu2[] = {
        "WHITE",
        "BLUE",
        "RED"
    };  
    choose = 0;
    while(!choose) {
        // Get window width & Height
        getmaxyx(stdscr, h, w);

        // Draw Box and Masseges
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        mvprintw(1 ,1 ,"%s",message);

        Print_Menu(highlight , setting_game_menu2 , n_choices);
        int ch = getch();
    
        switch (ch) {

            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = n_choices - 1;
                break;

            case KEY_DOWN:
                highlight++;
                if (highlight >= n_choices) highlight = 0;
                break;

            case 10:
                if(highlight == 0){ 
                    current_user.color = 0;
                }
                else if(highlight == 1){
                    current_user.color = 1;
                }
                else if(highlight == 2){
                    current_user.color = 2;
                }
                choose = 1;
                break;
        }
    }
}
int Check_Username(const char* username) {
    FILE *file = fopen("Users.txt", "r");

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[50];
        sscanf(line, "%[^,]", stored_username);
        if (strcmp(username, stored_username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
int Check_Password(const char* username ,const char *password) {
    FILE *file = fopen("Users.txt", "r");

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[50];
        sscanf(line, "%[^,]", stored_username);
        if (strcmp(username, stored_username) == 0) {
            char stored_password[50];
            sscanf(line + strlen(username) + 1,"%[^,]" ,stored_password);
            if (strcmp(password ,stored_password) == 0) {
                fclose(file);
                return 0;
            }else {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}
int Validate_Password(const char* password) {
    if (strlen(password) < 7) return 1;

    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (int i = 0; i < strlen(password); i++) {
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
        if (isdigit(password[i])) has_digit = 1;
    }
    
    if (has_upper && has_lower && has_digit) return 0;
    else return 1;
}
int Validate_Email(const char* email) {
    regex_t regex;
    int value;
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    value = regcomp(&regex, pattern, REG_EXTENDED);
    value = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    if (value) return 1;
    else return 0;
}
void Save_User(const User *user) {
    // Save user's information in Users.txt
    FILE *fileuser = fopen("Users.txt", "a");
    fprintf(fileuser, "%s,%s,%s\n", user->username, user->password, user->email);
    fclose(fileuser);
    // Save user's information in Scoreboard.txt
    FILE *filescore = fopen("Scoreboard.txt", "a");
    fprintf(filescore, "username:%s,score:0,gold:0,games:0,time:0\n", user->username);
    fclose(filescore);

    //Make new Folder for user
    //char username[100];
    //strcpy(username ,user->username);
    //mkdir(username , 0777);

}