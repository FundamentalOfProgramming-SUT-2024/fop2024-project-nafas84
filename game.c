#include"game.h"
#include"menu.h"
// Window width & Height
int W, H;
// Map
Cell map[60][160];
int Floor;
Room Rooms[6];
point Stair;
// player
Player player;
Monster Monsters[6];

char Message[150];
char Message2[150] = "";

int play_game() {
    // Start curses mode
    init_ncurses();
    // Set color
    set_color();

    // Make player
    if(current_user.load_game == 0){
        Floor = 0;
        player.life = 100;
        player.hunger = 0;
        player.score = 0;
        player.gold = 0;
        
        player.backpack.normal_food = 0;
        player.backpack.exellent_food = 0;
        player.backpack.magical_food = 0;
        player.backpack.sword_weapon = 0;
        player.backpack.dagger_weapon = 0;
        player.backpack.magic_wond_weapon = 0;
        player.backpack.normal_arrow_weapon = 0;
        player.backpack.health_potion = 0;
        player.backpack.speed_potion = 0;
        player.backpack.damage_potion = 0;

        strcpy(player.current_weapon,"mace");
        strcpy(player.current_abilaty, "_");
        player.move_count = 0;
    }else{
        Load_Game();
    }
    int s_key = 0;
    int treasure = 0;
    while((player.life > 0) && (!s_key) && (!treasure)){
        // Make map
        if(current_user.load_game == 0){
            make_map();
        }else{
            current_user.load_game = 0;
        }
        
        // Main loop
        int new_floor = 1;
        int m_key = 0;
        while(new_floor && (player.life > 0) && (!s_key) && (!treasure)){
            print_game(m_key);
            int ch = getch();
            switch(ch){
                // Cheats
                case 10: // Enter (exit)
                    return 0;
                case 109: // m (show map)
                    m_key = ((m_key == 1) ? 0 : 1);
                    break;
                case 102: // f (fast move)
                    int sh = getch();
                    while(move_player(sh));
                    break;
                case 115: // s (save game)
                    s_key = 1;
                    break;
                case 62: // > (stair) & (win in treasure room)
                    if(strcmp(map[player.y][player.x].type,s_stair) == 0){
                        Floor++;
                        new_floor = 0;
                    }else if(strcmp(map[player.y][player.x].type,s_treasure) == 0){
                        treasure = 1;
                    }
                    break;
                default:
                    collect_backpack(ch);
                    move_player(ch);
                    break;
            }
        }
    }

    GAME_M = 0; // for back to main menu
    if(s_key){
        Save_Game();
        if(!current_user.music){
            char music_path[50];
            if(current_user.music == 1){
                strcpy(music_path,"sonso.mp3");
            }else if(current_user.music == 2){
                strcpy(music_path,"ensemble.mp3");
            }
            Mix_Music *music = Mix_LoadMUS(music_path);
            // Cleanup
            Mix_FreeMusic(music);
            Mix_CloseAudio();
            SDL_Quit();
        }
        return 0;
    }
    else{
        if(!current_user.music){
            char music_path[50];
            if(current_user.music == 1){
                strcpy(music_path,"sonso.mp3");
            }else if(current_user.music == 2){
                strcpy(music_path,"ensemble.mp3");
            }
            Mix_Music *music = Mix_LoadMUS(music_path);
            // Cleanup
            Mix_FreeMusic(music);
            Mix_CloseAudio();
            SDL_Quit();
        }
        Update_Scoreboard(player.life);
        End_Game_Menu(player.life);
        return 1;
    }
    
}
void init_ncurses(){
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    leaveok(stdscr, TRUE);
    curs_set(0);
    cbreak();
}
void set_color(){
    start_color();
    // RGB
    init_color(20, 1000, 500, 0);
    init_color(21, 1000, 1000, 0);
    init_color(22, 500, 500, 1000);
    init_color(23, 1000, 500, 500);
    init_color(24, 0, 1000, 500);
    init_color(25, 0, 0, 1000);
    init_color(26, 941, 902, 549);
    init_color(27, 1000, 714, 757);
    init_color(28, 1000, 627, 690);
    init_color(29 ,1000, 412, 706);
    init_color(30, 800, 600, 1000);
    init_color(31, 1000, 800, 0);
    init_color(32, 502, 0, 125);

    init_pair(0, COLOR_WHITE, COLOR_BLACK); // 
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // box
    init_pair(2, COLOR_RED, COLOR_BLACK);  // monsters & gameover
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // rankcurrent & messaga2 & exellent food & profile menu
    init_pair(4, 31, COLOR_BLACK); // زرد بدرنگ TREASURE
    init_pair(5, 30, COLOR_BLACK); // بنفش  ENCHANT
    init_pair(6, 20, COLOR_BLACK); // نارنجی پررنگ rank3
    init_pair(7, 21, COLOR_BLACK); // زرد health bar & normal gold & rank1 & wingame
    init_pair(8, 22, COLOR_BLACK); // بنفش کمرنگ mace weapon & dagger weapon & magic weapon & normal arrow weapon & sword weapon
    init_pair(9, 23, COLOR_BLACK); // صورتی/قرمز کمرنگ trap
    init_pair(10, 24, COLOR_BLACK); // سبز کمرنگ   stair & treasure
    init_pair(11, 32, COLOR_BLACK); // black gold
    init_pair(12, COLOR_CYAN, COLOR_BLACK); // نقره ای  rank2
    init_pair(13, 26, COLOR_BLACK); //  REGULAR  & castle profile
    init_pair(14, 27, COLOR_BLACK); // health potion
    init_pair(15, 28, COLOR_BLACK); // speed potion
    init_pair(16, 29, COLOR_BLACK); // damage potion
    init_pair(17, COLOR_YELLOW, COLOR_BLACK); // magical food
}
void make_map(){
    clear();
    // clear map & update message
    sprintf(Message,"\tWELCOME TO %d FLOOR",Floor);
    for(int i = 1; i < W; i++){
        for(int j = 2; j < H - 2; j++){
            strcpy(map[j][i].type," ");
            map[j][i].visible = 0;
            map[j][i].color = 0;
        }
    }
    // Get window width & Height
    getmaxyx(stdscr, H, W);

    // Make rooms
    srand(time(NULL));
    for(int j = 0; j < 2 ;j++){
        for(int i = 0; i < 3; i++){
            int x_start = i * (W-1) / 3 + 3;
            int y_start = j * (H-4) / 2 + ((j == 0) ? 4 : 3);
            int I = (j == 0) ? i : -i + 5;
            make_room(x_start, y_start, I);
        }
    }
    // Theme of rooms & make stair
    int number[6] = {0, 1, 2, 3, 4, 5};
    int list[3] = {0, 0, 0};
    int size = 6;
    int stair = 0;
    for(int i = 0; i < 6 ; i++)
        add_theme(number, &size, list, &stair);
    
    // Make Doors and Windows
    for(int I = 0; I < 6 ; I++){
        switch(I){
            case 0:
            Rooms[I].doors[1] = make_doorXwindow(Rooms[I], I, 1);
            Rooms[I].doors[2] = make_doorXwindow(Rooms[I], I, 2);
            break;
            case 1:
            Rooms[I].doors[1] = make_doorXwindow(Rooms[I], I, 1);
            Rooms[I].doors[3] = make_doorXwindow(Rooms[I], I, 3);
            Rooms[I].doors[2] = make_doorXwindow(Rooms[I], I, 2);
            break;
            case 2:
            Rooms[I].doors[2] = make_doorXwindow(Rooms[I], I, 2);
            Rooms[I].doors[3] = make_doorXwindow(Rooms[I], I, 3);
            break;
            case 3:
            Rooms[I].doors[0] = make_doorXwindow(Rooms[I], I, 0);
            Rooms[I].doors[3] = make_doorXwindow(Rooms[I], I, 3);
            break;
            case 4:
            Rooms[I].doors[1] = make_doorXwindow(Rooms[I], I, 1);
            Rooms[I].doors[3] = make_doorXwindow(Rooms[I], I, 3);
            Rooms[I].doors[0] = make_doorXwindow(Rooms[I], I, 0);
            break;
            case 5:
            Rooms[I].doors[1] = make_doorXwindow(Rooms[I], I, 1);
            Rooms[I].doors[0] = make_doorXwindow(Rooms[I], I, 0);
            break;
        }
        // Make window
        //int dir = rand() % 4;
        //Rooms[I].windows[dir] = make_doorXwindow(Rooms[I], I, dir);
    }
    // Save rooms & add obj 7 add monster
    for(int I = 0; I < 6; I++){
        save_room(Rooms[I], I);
        add_obj(Rooms[I], I);
        add_monster(Rooms[I], I);
    }
    // Make path
    int curv = rand() % 3;
    make_path(Rooms[0].doors[1], Rooms[1].doors[3], 3, curv);
    make_path(Rooms[1].doors[1], Rooms[2].doors[3], 3, curv);
    make_path(Rooms[2].doors[2], Rooms[3].doors[0], 0, curv);
    make_path(Rooms[3].doors[3], Rooms[4].doors[1], 1, curv);
    make_path(Rooms[4].doors[3], Rooms[5].doors[1], 1, curv);
    make_path(Rooms[5].doors[0], Rooms[0].doors[2], 2, curv);
    make_path(Rooms[4].doors[0], Rooms[1].doors[2], 2, curv);

    // move player
    player.y = Rooms[4].h / 2 + Rooms[4].y;
    player.x = Rooms[4].w / 2 + Rooms[4].x;
}
void make_room(int x_start, int y_start, int i){
    int h = (H-4) / 2 - 4;
    int w = (W-1) / 3 - 4;
    for(int attempt = 0; attempt < 15; attempt++){
        point p1,p2;

        p1.x = rand() % w + x_start;
        p1.y = rand() % h + y_start;
        p2.x = rand() % w + x_start;
        p2.y = rand() % h + y_start;
        
        int w_room = abs(p1.x - p2.x);
        int h_room = abs(p1.y - p2.y);
        if(w_room > 5 && h_room > 5){
            Rooms[i].x = (p1.x < p2.x) ? p1.x : p2.x;
            Rooms[i].y = (p1.y < p2.y) ? p1.y : p2.y;
            Rooms[i].w = w_room;
            Rooms[i].h = h_room;
            Rooms[i].visibality = (i == 4) ? 1 : 0;
            return;
        }
    }
    Rooms[i].x = x_start + 1;
    Rooms[i].y = y_start + 1;
    Rooms[i].w = w / 2;
    Rooms[i].h = h / 2;
    Rooms[i].visibality = (i == 4) ? 1 : 0;

}
void add_theme(int number[6], int* size, int list[3], int* stair){
    int i = rand() % *size;
    int I_n = number[i];
    // make stair
    if((*stair == 0)  && (I_n % 2 == 1) && Floor != 4){
        point s;
        s.x = rand() % (Rooms[I_n].w - 2) + Rooms[I_n].x + 1;
        s.y = rand() % (Rooms[I_n].h - 2) + Rooms[I_n].y + 1;
        Stair = s;
        *stair = 1;
    }
    if((list[0] == 0 || list[1] == 0 )){ // Make Enchant Room
        strcpy(Rooms[I_n].theme , "ENCHANT");
        if(list[0] == 0) list[0] = 1;
        else list[1] = 1;
    }
    else if(list[2] == 0 && Floor == 4 && I_n != 4){ // Make Treasure Room
        strcpy(Rooms[I_n].theme , "TREASURE");
        list[2] = 1;
    }
    else{ // Make Regular Room
        strcpy(Rooms[I_n].theme , "REGULAR");
    }
    number[i] = number[*size -1];
    (*size)--;
}
point make_doorXwindow(Room room , int I, int dir){
    point p;
    switch(dir){
        case 0:
        // up door
        p.x = rand() % (Rooms[I].w -1) + Rooms[I].x + 1;
        p.y = room.y;
        break;
        case 1:
        // right door
        p.x = room.x + room.w;
        p.y = rand() % (Rooms[I].h -1) + Rooms[I].y + 1;
        break;
        case 2:
        // down door
        p.x = rand() % (Rooms[I].w -1) + Rooms[I].x + 1;
        p.y = room.y + room.h;
        break;
        case 3:
        // left door
        p.x = room.x;
        p.y = rand() % (Rooms[I].h -1) + Rooms[I].y + 1;
        break;
    }
    //if((Rooms[I].doors[dir].x != p.x) && (Rooms[I].doors[dir].y != p.y))
        return p;
}
void make_path(point p1, point p2, int dir, int curv ){
    switch(dir){
        case 0: p2.y--; break;
        case 1: p2.x++; break;
        case 2: p2.y++; break;
        case 3: p2.x--; break;
    }
    if(abs(p1.x - p2.x) > 1 && abs(p1.y - p2.y) > 1 && curv > 0){
        point obstacle;
        int randx = rand() % (abs(p1.x - p2.x) - 1) +  1;
        int randy = rand() % (abs(p1.y - p2.y) - 1) +  1;
        obstacle.x = p1.x + ((p1.x  > p2.x) ? -randx : randx);
        obstacle.y = p1.y + ((p1.y  > p2.y) ? -randy : randy);

        strcpy(map[obstacle.y][obstacle.x].type, "s");
        curv--;
        make_path(p1, obstacle, -1, curv);
        make_path(obstacle, p2, -1, curv);
    }else{
        while(1){
            // step right
            if((abs((p1.x + 1) - p2.x) < abs(p1.x - p2.x))   && 
                strcmp(map[p1.y][p1.x + 1].type, s_wall_a) != 0    &&
                strcmp(map[p1.y][p1.x + 1].type, s_wall_o) != 0){
                p1.x += 1;
            }
            // step left
            else if((abs((p1.x - 1) - p2.x) < abs(p1.x - p2.x))  &&
                    strcmp(map[p1.y][p1.x - 1].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y][p1.x - 1].type, s_wall_o) != 0){
                p1.x -= 1;
            }
            // step up
            else if((abs((p1.y - 1) - p2.y) < abs(p1.y - p2.y))  &&
                    strcmp(map[p1.y - 1][p1.x].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y - 1][p1.x].type, s_wall_o) != 0){
                p1.y -= 1;
            }
            // step down
            else if((abs((p1.y + 1) - p2.y) < abs(p1.y - p2.y))  &&
                    strcmp(map[p1.y + 1][p1.x].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y + 1][p1.x].type, s_wall_o) != 0){
                p1.y += 1;
            }else{
                return;
            }
            // save path
            strcpy(map[p1.y][p1.x].type, s_path);
            map[p1.y][p1.x].visible = 0;
        }
    }
}
void save_room(Room room, int I){
    // save wall & floor
    for(int i = room.x; i <= room.w + room.x; i++){
        for(int j = room.y; j <= room.h + room.y;j++){
            map[j][i].visible = (I == 4) ? 1 : 0;
            map[j][i].color   = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
            if(j == room.y || j == room.h + room.y){
                strcpy(map[j][i].type, s_wall_o);
            }else if(i == room.x || i == room.w + room.x){
                strcpy(map[j][i].type, s_wall_a);
            }else{
                strcpy(map[j][i].type, s_floor);
            }
        }
    }
    // make and save pillar
    int numb = rand() % 3; // number of pillar
        for(int i = 0; i < numb; i++){
            int x = rand() % (Rooms[I].w - 3) + Rooms[I].x + 2;
            int y = rand() % (Rooms[I].h - 3) + Rooms[I].y + 2;
            if((strcmp(map[y][x].type,s_floor) == 0)){

                strcpy(map[y][x].type,s_pillar);
                map[y][x].visible = (I == 4) ? 1 : 0;
                map[y][x].color   = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
            }
        }   
    int n_trap = (current_user.difficulty == 0) ?  0 : (current_user.difficulty == 1) ? 5 : 10;
    // make and save trap
    numb = rand() % 6 + n_trap; // number of trap
    numb += (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? -15 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 5 : 0;
        for(int i = 0; i < numb; i++){
            int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
            int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
            if((strcmp(map[y][x].type,s_floor) == 0)){

                strcpy(map[y][x].type,s_trap);
                map[y][x].visible = 0;
                map[y][x].color   =  9;
            }          
        }
    // save stair
    if(Floor != 4){
        strcpy(map[Stair.y][Stair.x].type, s_stair);
        map[Stair.y][Stair.x].visible = 0;
        map[Stair.y][Stair.x].color = 10;
    }
    // save doors & windows
    for(int i = 0; i < 4; i++){
        int rn = rand() % 100;
        if(rn > 40){
        strcpy(map[room.doors[i].y][room.doors[i].x].type, s_normal_door);
        map[room.doors[i].y][room.doors[i].x].visible = (I == 4)  ? 1 : 0;
        }else{
        strcpy(map[room.doors[i].y][room.doors[i].x].type, s_secret_door);
        map[room.doors[i].y][room.doors[i].x].visible = 0;           
        }
        map[room.doors[i].y][room.doors[i].x].color   = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
        // save windows
        strcpy(map[room.windows[i].y][room.windows[i].x].type, s_window);
        map[room.windows[i].y][room.windows[i].x].visible = (I == 4)  ? 1 : 0;
        map[room.windows[i].y][room.windows[i].x].color   = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
    }
}
void add_obj(Room room, int I){
    int theme = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 0 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 1 : 2;
    int numb;
    // make and save gold
    int n_gold = (current_user.difficulty == 0) ?  6 : (current_user.difficulty == 1) ? 3 : 0;
    numb = rand() % 4 + n_gold; // number of gold
    numb += (theme == 0) ? 3 : 0;
    for(int i = 0; i < numb; i++){
        int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
        int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
        if((strcmp(map[y][x].type,s_floor) == 0)){
            int pos = rand() % 100;
            if(pos < 15){
                strcpy(map[y][x].type,s_black_gold);
                map[y][x].visible = (I == 4) ? 1 : 0;
                map[y][x].color   =  11;
            }else{
                strcpy(map[y][x].type,s_normal_gold);
                map[y][x].visible = (I == 4) ? 1 : 0;
                map[y][x].color   =  7;
            }
        }          
    }
    // make and save treasure
    if(theme == 1){
        int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
        int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;  
        strcpy(map[y][x].type,s_treasure);
        map[y][x].visible = 0;
        map[y][x].color   = 10;
    }

    // make and save foods
    int n_food = (current_user.difficulty == 0) ?  5 : (current_user.difficulty == 1) ? 3 : 1;
    numb = rand() % 4 + n_food; // number of foods
    numb += (theme == 0) ? -10 : 1;
    for(int i = 0; i < numb; i++){
        int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
        int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
        if((strcmp(map[y][x].type,s_floor) == 0)){
            int pos = rand() % 3; // 0 -> normal , 1 -> excellent , 2 -> magical
            map[y][x].visible = (I == 4) ? 1 : 0;
            switch(pos){
                case 0:
                    strcpy(map[y][x].type,s_normal_food);
                    map[y][x].color   =  12;
                    break;
                case 1:
                    strcpy(map[y][x].type,s_exellent_food);
                    map[y][x].color   =  3;
                    break;
                case 2:
                    strcpy(map[y][x].type,s_magical_food);
                    map[y][x].color   =  17;
                    break;            
            }
        }          
    }

    // make and save weapons
    int n_weapon = (current_user.difficulty == 0) ?  2 : (current_user.difficulty == 1) ? 1 : 0;
    numb = rand() % 2 + n_weapon; // number of weapons
    numb += (theme == 1) ? -2 : 0;
    for(int i = 0; i < numb; i++){
        int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
        int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
        if((strcmp(map[y][x].type,s_floor) == 0)){
            int pos = rand() % 4; // 0 -> dagger , 1 -> magic , 2 -> noraml , 3 -> sword
            map[y][x].visible = (I == 4) ? 1 : 0;
            switch(pos){
                case 0:
                    strcpy(map[y][x].type,s_dagger_weapon);
                    map[y][x].color   =  8;
                    break;
                case 1:
                    strcpy(map[y][x].type,s_magic_wond_weapon);
                    map[y][x].color   =  8;
                    break;
                case 2:
                    strcpy(map[y][x].type,s_normal_arrow_weapon);
                    map[y][x].color   =  8;
                    break;   
                case 3:
                    strcpy(map[y][x].type,s_sword_weapon);
                    map[y][x].color   =  8;
                    break;            
            }
        }          
    }

    // make and save potion
    int n_potion = (current_user.difficulty == 0) ?  4 : (current_user.difficulty == 1) ? 2 : 0;
    numb = rand() % 3 + n_potion; // number of potion
    numb += (theme == 0) ? 4 : 0;
    for(int i = 0; i < numb; i++){
        int x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
        int y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
        if((strcmp(map[y][x].type,s_floor) == 0)){
            int pos = rand() % 4; // 0 -> healt , 1 -> speed , 2 -> damage
            map[y][x].visible = (I == 4) ? 1 : 0;
            switch(pos){
                case 0:
                    strcpy(map[y][x].type,s_health_potion);
                    map[y][x].color   =  14;
                    break;
                case 1:
                    strcpy(map[y][x].type,s_speed_potion);
                    map[y][x].color   =  15;
                    break;
                case 2:
                    strcpy(map[y][x].type,s_damage_potion);
                    map[y][x].color   =  16;
                    break;              
            }
        }          
    }

}
void add_monster(Room room, int I){
    Monsters[I].x = rand() % (Rooms[I].w - 2) + Rooms[I].x + 1;
    Monsters[I].y = rand() % (Rooms[I].h - 2) + Rooms[I].y + 1;
    Monsters[I].visibality = (I == 4) ? 1 : 0;

    int type_monster = rand() % 5; // 0 -> deamon , 1 -> fire_breathing , 2 -> giant , 3 -> snake , 4 -> undeed
    switch (type_monster){
        case 0:
            Monsters[I].type =  'D';
            Monsters[I].life = 5;
            Monsters[I].n_move = 0;
            Monsters[I].damage = 1;
        break;
        case 1:
            Monsters[I].type =  'F';
            Monsters[I].life = 10;
            Monsters[I].n_move = 0;
            Monsters[I].damage = 2;
        break;
        case 2:
            Monsters[I].type =  'G';
            Monsters[I].life = 15;
            Monsters[I].n_move = 5;
            Monsters[I].damage = 5;
        break;
        case 3:
            Monsters[I].type =  'S';
            Monsters[I].life = 20;
            Monsters[I].n_move = -1;
            Monsters[I].damage = 10;
        break;
        case 4:
            Monsters[I].type =  'U';
            Monsters[I].life = 30;
            Monsters[I].n_move = 5;
            Monsters[I].damage = 15;
        break;      
    }
}
void print_game(int m){
    erase();

    // update
    move_monster(player.x, player.y);
    update_visibility(player.x, player.y);

    // print massege and health bar
    attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
    attroff(COLOR_PAIR(1));

    mvprintw(1, 1, "%s", Message);
    //strcpy(Message,"");

    attron(COLOR_PAIR(3));
        mvprintw(2, 1, "%s", Message2);
        strcpy(Message2,"");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(7));
        mvprintw(H-2, 1, "\tScore:%d   Gold:%d   Life:%d   Hunger:%d   Floor:%d    Current weapon:%s   Current abilaty:%s (%d steps, %d space)",
                        player.score, player.gold, player.life, player.hunger, Floor, player.current_weapon, player.current_abilaty , player.n_abilaty, player.n_space);
    attroff(COLOR_PAIR(7));

    // print map
    for(int i = 1; i < W; i++){
        for(int j = 2; j < H - 2; j++){
            attron(COLOR_PAIR(map[j][i].color));
            if(map[j][i].visible == 1 && !m){
                mvprintw(j, i, "%s",map[j][i].type);
            }
            else if(m){ // for m keyboard
                mvprintw(j, i, "%s",map[j][i].type);
            }
            else if(map[j][i].visible == 0){
                // for secret door
                if(strcmp(map[j][i].type , s_secret_door) == 0){
                    if((map[j][i+1].visible == 1) || (map[j][i-1].visible == 1)){
                        if((strcmp(map[j][i+1].type, s_wall_o) == 0) || (strcmp(map[j][i-1].type, s_wall_o) == 0)) 
                            mvprintw(j, i, s_wall_o);
                        else 
                            mvprintw(j, i, s_wall_a);
                    }
                }
                // for trap
                else if(strcmp(map[j][i].type , s_trap) == 0){
                    if((map[j][i+1].visible == 1) || (map[j][i-1].visible == 1)){
                        attroff(COLOR_PAIR(map[j][i].color));
                        int I = which_room(i, j);
                        int color = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
                            attron(COLOR_PAIR(color));
                                mvprintw(j, i, s_floor);
                            attroff(COLOR_PAIR(color));
                        attron(COLOR_PAIR(map[j][i].color));
                    }
                }
            }
            attroff(COLOR_PAIR(map[j][i].color));
        }
    }
    // print monsters
    for(int J = 0; J < 6; J++){
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        if(Monsters[J].visibality == 1 && !m && Monsters[J].life > 0){
            mvprintw(Monsters[J].y, Monsters[J].x,"%c",Monsters[J].type);
        }else if(m && Monsters[J].life > 0){// for m keyboard
            mvprintw(Monsters[J].y, Monsters[J].x,"%c",Monsters[J].type);
        }
        attroff(COLOR_PAIR(2));
        attroff(A_BOLD);

    }
        
    // print player
    attron(COLOR_PAIR(current_user.color));
        mvprintw(player.y, player.x, s_hero);
    attroff(COLOR_PAIR(current_user.color));

    refresh();

}
void update_visibility(int x, int y){
    // Update path visibality
    if(strcmp(map[y][x].type,s_path) == 0 || strcmp(map[y][x].type,s_normal_door) == 0){
        int show = 2;
        for(int i = x-show; i <= x+show; i++){
            for(int j = y-show; j <= y+show; j++){
                if(strcmp(map[j][i].type,s_path) == 0)
                    map[j][i].visible = 1;
            }
        }
    }
    // Update room visibality
    int I = which_room(x,y);
    if(I != -1){
        // Update Message
        if(Rooms[I].visibality == 0)
            sprintf(Message, "\t welcome to %s room",Rooms[I].theme);
        Rooms[I].visibality = 1;
        for(int i = Rooms[I].x; i <= Rooms[I].x + Rooms[I].w; i++){
            for(int j = Rooms[I].y; j <= Rooms[I].y + Rooms[I].h; j++){
                if((strcmp(map[j][i].type,s_trap)  != 0) && (strcmp(map[j][i].type,s_secret_door) != 0)){
                        map[j][i].visible = 1;
                }else if((strcmp(map[j][i].type,s_secret_door)  == 0) && j == y && i == x){
                    map[j][i].visible = 1; 
                }
            }
        } 
    }
    // Update trap & gold & treasure visibality
    if((strcmp(map[y][x].type,s_trap)  == 0) && map[y][x].visible == 0){
        player.life -= 10;
        map[y][x].visible = 1;
    }
    else if((strcmp(map[y][x].type,s_black_gold)  == 0)){
        strcpy(Message,"\t You won ten gold coins.");
        player.gold += 10;
        player.score += 10;
        strcpy(map[y][x].type,s_floor);
        int color = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
        map[y][x].color = color;
    }
    else if((strcmp(map[y][x].type,s_normal_gold)  == 0)){
        strcpy(Message,"\t You won five gold coins.");
        player.gold += 5;
        player.score += 5;
        strcpy(map[y][x].type,s_floor);
        int color = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
        map[y][x].color = color;
    }   
    // Update monsters visibality
    Monsters[I].visibality = 1;
}
void collect_backpack(int ch){
    int x = player.x;
    int y = player.y;
    if(ch == 99){ // c (collect)
    int check = 0;
        if(strcmp(map[y][x].type,s_normal_food) == 0)       { player.backpack.normal_food ++; check = 1;}
        else if(strcmp(map[y][x].type,s_exellent_food) == 0){ player.backpack.exellent_food ++; check = 1;}
        else if(strcmp(map[y][x].type,s_magical_food) == 0){ player.backpack.magical_food ++; check = 1;}

        else if(strcmp(map[y][x].type,s_dagger_weapon) == 0){ player.backpack.dagger_weapon += 10; check = 1;}
        else if(strcmp(map[y][x].type,s_magic_wond_weapon) == 0){ player.backpack.magic_wond_weapon += 8; check = 1;}
        else if(strcmp(map[y][x].type,s_normal_arrow_weapon) == 0){ player.backpack.normal_arrow_weapon += 20; check = 1;}
        else if(strcmp(map[y][x].type,s_sword_weapon) == 0){ player.backpack.sword_weapon = 1; check = 1;}

        else if(strcmp(map[y][x].type,s_health_potion) == 0){ player.backpack.health_potion ++; check = 1;}
        else if(strcmp(map[y][x].type,s_speed_potion) == 0){ player.backpack.speed_potion ++; check = 1;}
        else if(strcmp(map[y][x].type,s_damage_potion) == 0){ player.backpack.damage_potion ++; check = 1;}

        if(check){
            strcpy(map[y][x].type,s_floor);
            int I = which_room(x, y);
            map[y][x].color = (strcmp(Rooms[I].theme,"ENCHANT") == 0) ? 5 : (strcmp(Rooms[I].theme,"TREASURE") == 0) ? 4 : 13;
        }
    }
    else if(ch == 98){ // b (backpack)
        Backpack_Menu();
    }
    else if(ch == 32){// space (shooting)
        int check_kill = 0;
        int n = ((strcmp(player.current_abilaty,"2x power") == 0) && player.n_space > 0) ? 2 : 1;
        player.n_space = (player.n_space > 0) ? player.n_space -1 : 0 ;
        // for close-range weapons
        if(strcmp(player.current_weapon,"mace") == 0){
            for(int J = 0; J < 6; J++){
                if((player.x - 1 <= Monsters[J].x && Monsters[J].x <= player.x + 1) && (player.y - 1 <= Monsters[J].y && Monsters[J].y <= player.y + 1)){
                    Monsters[J].life -= (5 * n);
                    if(Monsters[J].life > 0){
                        check_kill = 1;
                        sprintf(Message2,"\t You damaged %c monster (Remaining life: %d)",Monsters[J].type,Monsters[J].life);
                    }
                }
            }
        }
        else if(strcmp(player.current_weapon,"sword") == 0){
            for(int J = 0; J < 6; J++){
                if((player.x - 1 <= Monsters[J].x && Monsters[J].x <= player.x + 1) && (player.y - 1 <= Monsters[J].y && Monsters[J].y <= player.y + 1)){
                    Monsters[J].life -= (10 * n);
                    if(Monsters[J].life > 0){
                        check_kill = 1;
                        sprintf(Message2,"\t You damaged %c monster (Remaining life: %d)",Monsters[J].type,Monsters[J].life);
                    }
                }
            }
        }
        // for long-range weapons
        else{
            int sh = getch();
            if(strcmp(player.current_weapon,"dagger") == 0){
                if(player.backpack.dagger_weapon > 0){
                player.backpack.dagger_weapon--;
                for(int J = 0; J < 6; J++){
                    int check = 0;
                    if      (sh == KEY_UP     && (Monsters[J].x == player.x) && (player.y - 5 <= Monsters[J].y && Monsters[J].y <= player.y)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_DOWN  && (Monsters[J].x == player.x) && (player.y <= Monsters[J].y && Monsters[J].y <= player.y + 5)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_LEFT  && (Monsters[J].y == player.y) && (player.x - 5 <= Monsters[J].x && Monsters[J].x <= player.x)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_RIGHT && (Monsters[J].y == player.y) && (player.x <= Monsters[J].x && Monsters[J].x <= player.x + 5)){ //  damage for 5 distanc
                        check = 1;
                    }
                    if(check){
                        Monsters[J].life -= (12 * n);
                    if(Monsters[J].life > 0){
                        check_kill = 1;
                        sprintf(Message2,"\t You damaged %c monster (Remaining life: %d)",Monsters[J].type,Monsters[J].life);
                    }
                    }
                }
                }else{
                    strcpy(Message, "You don't have enough arrows");
                }
            }
            else if(strcmp(player.current_weapon,"magic wand") == 0){
                if(player.backpack.magic_wond_weapon > 0){
                player.backpack.magic_wond_weapon--;
                int check = 0;
                for(int J = 0; J < 6; J++){
                    
                    if     (sh == KEY_UP     && (Monsters[J].x == player.x) && (player.y - 10 <= Monsters[J].y && Monsters[J].y <= player.y)){ //  damage for 10 distanc
                        check = 1;
                    }else if(sh == KEY_DOWN  && (Monsters[J].x == player.x) && (player.y <= Monsters[J].y && Monsters[J].y <= player.y + 10)){ //  damage for 10 distanc
                        check = 1;
                    }else if(sh == KEY_LEFT  && (Monsters[J].y == player.y) && (player.x - 10 <= Monsters[J].x && Monsters[J].x <= player.x)){ //  damage for 10 distanc
                        check = 1;
                    }else if(sh == KEY_RIGHT && (Monsters[J].y == player.y) && (player.x <= Monsters[J].x && Monsters[J].x <= player.x + 10)){ //  damage for 10 distanc
                        check = 1;
                    }
                    if(check){
                        Monsters[J].life -= (12 * n);
                    if(Monsters[J].life > 0){
                        check_kill = 1;
                        sprintf(Message2,"\t You damaged %c monster (Remaining life: %d)",Monsters[J].type,Monsters[J].life);
                    }
                    }
                }
                }else{
                    strcpy(Message, "You don't have enough arrows");
                }
            }
            else if(strcmp(player.current_weapon,"normal arrow") == 0){
                if(player.backpack.normal_arrow_weapon > 0){
                player.backpack.normal_arrow_weapon--;

                for(int J = 0; J < 6; J++){
                    int check = 0;
                    if     (sh == KEY_UP     && (Monsters[J].x == player.x) && (player.y - 5 <= Monsters[J].y && Monsters[J].y <= player.y)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_DOWN  && (Monsters[J].x == player.x) && (player.y <= Monsters[J].y && Monsters[J].y <= player.y + 5)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_LEFT  && (Monsters[J].y == player.y) && (player.x - 5 <= Monsters[J].x && Monsters[J].x <= player.x)){ //  damage for 5 distanc
                        check = 1;
                    }else if(sh == KEY_RIGHT && (Monsters[J].y == player.y) && (player.x <= Monsters[J].x && Monsters[J].x <= player.x + 5)){ //  damage for 5 distanc
                        check = 1;
                    }
                    if(check){
                        Monsters[J].life -= (5 * n);
                    if(Monsters[J].life > 0){
                        check_kill = 1;
                        sprintf(Message2,"\t You damaged %c monster (Remaining life: %d)",Monsters[J].type,Monsters[J].life);
                    }
                    }
                }
                }else{
                    strcpy(Message, "You don't have enough arrows");
                }
            }
        }
        // update visibality and life monster & message
        int K = which_room(player.x, player.y);
            if(Monsters[K].life <= 0 && Monsters[K].visibality){
                Monsters[K].visibality = 0;
                sprintf(Message,"\t The last monster you killed is %c monster",Monsters[K].type);
            }
        
    }
}
int move_player(int ch){
    int new_x = -1;
    int new_y = -1;
    int step;
    if((strcmp(player.current_abilaty,"2x speed") == 0) && player.n_abilaty > 0){
        step = 2;
    }else{
        step = 1;
    }
    switch(ch){
        case KEY_UP:
            new_x = player.x;
            new_y = player.y - step;
            break;
        case KEY_DOWN:
            new_x = player.x;
            new_y = player.y + step;
            break;
        case KEY_RIGHT:
            new_x = player.x + step;
            new_y = player.y;
            break;
        case KEY_LEFT:
            new_x = player.x - step;
            new_y = player.y;
            break;

        case KEY_HOME:
            new_x = player.x - step;
            new_y = player.y - step;
            break;
        case KEY_PPAGE:
            new_x = player.x + step;
            new_y = player.y - step;
            break;
        case KEY_END:
            new_x = player.x - step;
            new_y = player.y + step;
            break;
        case KEY_NPAGE:
            new_x = player.x + step;
            new_y = player.y + step;
            break;
    }
    if(check_move_player(new_x, new_y)){
        player.x = new_x;
        player.y = new_y;
        return 1;
    }
    return 0;
}
int check_move_player(int x, int y){
    if(x == -1 && y == -1) return 0;
    if(strcmp(map[y][x].type, s_wall_a) != 0 &&
       strcmp(map[y][x].type, s_wall_o) != 0 &&
       strcmp(map[y][x].type, s_pillar) != 0 &&
       strcmp(map[y][x].type, " ")      != 0 &&
       strcmp(map[y][x].type, "")       != 0 &&
       strcmp(map[y][x].type, s_window) != 0 
      ){
        // player update n abilaty
        player.n_abilaty = (player.n_abilaty == 0) ? 0 : player.n_abilaty - 1;

        // player update life and hunger
        int lifeplus;
        if((strcmp(player.current_abilaty,"2x speed") == 0) && player.n_abilaty > 0){
            lifeplus = 4;
        }else{
            lifeplus = 1;
        }
        player.move_count++;
        if(player.move_count == 9){
            if(player.life < 100)
                player.life += lifeplus;
            player.hunger += 2;
            player.move_count = 0;
        }
        if(player.hunger >= 50){
            if(player.hunger < 55)
                strcpy(Message,"\t You are hungry.");
            player.life -= 1;
        }


        return 1; 
    }else{
        return 0;
    }
}
void move_monster(int xP, int yP){
    point p2;// player
    p2.x = xP, p2.y = yP;
    for(int J = 0; J < 6; J++){
        point p1; // monster
        p1.x = Monsters[J].x , p1.y = Monsters[J].y;
        if((p1.x - 2 <= xP && xP <= p1.x + 2) && (p1.y - 2 <= yP && yP <= p1.y + 2)){
            if((Monsters[J].n_move != 0) && Monsters[J].visibality == 1){ // move monster
            // step right
            if((abs((p1.x + 1) - p2.x) < abs(p1.x - p2.x))   && 
                    strcmp(map[p1.y][p1.x + 1].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y][p1.x + 1].type, s_wall_o) != 0){
                Monsters[J].n_move--;
                Monsters[J].x += 1;
            }
            // step left
            else if((abs((p1.x - 1) - p2.x) < abs(p1.x - p2.x))  &&
                    strcmp(map[p1.y][p1.x - 1].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y][p1.x - 1].type, s_wall_o) != 0){
                Monsters[J].n_move--;
                Monsters[J].x -= 1;
            }
            // step up
            else if((abs((p1.y - 1) - p2.y) < abs(p1.y - p2.y))  &&
                    strcmp(map[p1.y - 1][p1.x].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y - 1][p1.x].type, s_wall_o) != 0){
                Monsters[J].n_move--;
                Monsters[J].y -= 1;
            }
            // step down
            else if((abs((p1.y + 1) - p2.y) < abs(p1.y - p2.y))  &&
                    strcmp(map[p1.y + 1][p1.x].type, s_wall_a) != 0    &&
                    strcmp(map[p1.y + 1][p1.x].type, s_wall_o) != 0){
                Monsters[J].n_move--;
                Monsters[J].y += 1;
            }
            }
            if(Monsters[J].visibality == 1 && Monsters[J].life > 0){
                player.life -= Monsters[J].damage;
                sprintf(Message,"\t You were hit by %c monster and -%d life", Monsters[J].type, Monsters[J].damage);
            }
            return;
        }
    }
}
int which_room(int x, int y){
    for(int I = 0; I < 6; I++){
        if((x >= Rooms[I].x && x <= Rooms[I].x + Rooms[I].w) && (y >= Rooms[I].y && y <= Rooms[I].y + Rooms[I].h)){
            return I;
        }
    }
    return -1;
}
void Update_Scoreboard(int life){
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

    int i = 0;
    while(fgets(line,sizeof(line),file)){
        sscanf(line,"username:%[^,],score:%d,gold:%d,games:%d,time:%d",
        users[i].name, &users[i].score, &users[i].gold, &users[i].games, &users[i].time);        
        if(strstr(line,current_user.username)){
            // change current user line
            player.score += (life > 0) ? (100 + player.score) : player.score ; // update score player
            users[i].score += player.score;
            users[i].gold += player.gold;
            users[i].games += 1;
            users[i].time += (life > 0) ? 1 : 0;
        }
        i++;
    }
    fclose(file);

    FILE* outfile = fopen("Scoreboard.txt","w");
    for(int k = 0; k < i; k++){
        fprintf(outfile, "username:%s,score:%d,gold:%d,games:%d,time:%d\n",
                users[k].name, users[k].score, users[k].gold, users[k].games, users[k].time);      
    }
    fclose(outfile);
}
void End_Game_Menu(int life){

    clear();
    attron(COLOR_PAIR(1));
    box(stdscr , 0 , 0);
    mvprintw(1, 1, "\tPress enter to back to Pre-Menu Game");
    attroff(COLOR_PAIR(1));
    char game_over[30][40] = {
"                           ,--. ",
"                          {    }",
"                          K,   }",
"                         /  `Y` ",
"                    _   /   /   ",
"                   {_'-K.__/    ",
"                     `/-.__L._  ",
"                     /  ' /`\\_} ",
"                    /  ' /      ",
"            ____   /  ' /       ",
"     ,-'~~~~    ~~/  ' /_       ",
"   ,'             ``~~~%%',     ",
"  (                     %  Y    ",
" {                      %% I    ",
"{      -                 %  `.  ",
"|       ',                %  )  ",
"|        |   ,..__      __. Y   ",
"|    .,_./  Y ' / ^Y   J   )|   ",
"\\           |' /   |   |   ||   ",
" \\          L_/    . _ (_,.'(   ",
"  \\,   ,      ^^""' / |      )  ",
"    \\_  \\          /,L]     /   ",
"      '-_`-,       ` `   ./`    ",
"         `-(_            )      ",
"             ^^\\..___,.--`      "
    };

    char win_game[20][80] = {
"*******************************************************************************",
"          |                   |                  |                     |       ",
" _________|________________.=\"\"_;=.______________|_____________________|_______",
"|                   |  ,-\"_,=\"\"     `\"=.|                  |                   ",
"|___________________|__\"=._o`\"-._        `\"=.______________|___________________",
"          |                `\"=._o`\"=._      _`\"=._                     |       ",
" _________|_____________________:=._o \"=._.\"_.-=\"'\"=.__________________|_______",
"|                   |    __.--\" , ; `\"=._o.\" ,-\"\"\"-._ \".   |                   ",
"|___________________|_._\"  ,. .` ` `` ,  `\"-._\"-._   \". '__|___________________",
"          |           |o`\"=._` , \"` `; .\". ,  \"-._\"-._; ;              |       ",
" _________|___________| ;`-.o`\"=._; .\" ` '`.\"\\` . \"-._ /_______________|_______",
"|                   | |o;    `\"-.o`\"=._``  '` \" ,__.--o;   |                   ",
"|___________________|_| ;     (#) `-.o `\"=.`_.--\"_o.-; ;___|___________________",
"____/______/______/___|o;._    \"      `\".o|o_.--\"    ;o;____/______/______/____",
"/______/______/______/_\"=._o--._        ; | ;        ; ;/______/______/______/_",
"____/______/______/______/__\"=._o--._   ;o|o;     _._;o;____/______/______/____",
"/______/______/______/______/____\"=._o._; | ;_.--\"o.--\"_/______/______/______/_",
"____/______/______/______/______/_____\"=.o|o_.--\"\"___/______/______/______/____",
"/______/______/______/______/______/______/______/______/______/______/______/_",
"*******************************************************************************"
    };
    if(W > 90 && H > 26){
        if(life > 0){
            for(int i = 0; i < 20 ;i++){
                attron(COLOR_PAIR(7));
                mvprintw(3 + i, 3, "%s",win_game[i]);
                attroff(COLOR_PAIR(7));
            }
            mvprintw(24, W/2,"You win");
            mvprintw(25, W/2,"Total Score: %d",player.score);        
        }else{
            for(int i = 0; i < 25; i++){
                attron(COLOR_PAIR(2));
                mvprintw(3 + i, 3, "%s",game_over[i]);
                attroff(COLOR_PAIR(2));
            }
            mvprintw(19, 40,"Game Over");
            mvprintw(20, 40,"Total Score: %d",player.score);
        }
    }else{
        mvprintw(H/2, W/2, "Your window is small");
    }
    

    int ch = getch();
    while(ch != 10)
        ch = getch();
}
void Save_Game(){
    char path[80];
    sprintf(path,"Maps/%s.bat",current_user.username);
    FILE *file = fopen(path, "wb");

    fwrite(&W, sizeof(W), 1, file);
    fwrite(&H, sizeof(H), 1, file);
    fwrite(map, sizeof(map), 1, file);
    fwrite(&Floor, sizeof(Floor), 1, file);
    fwrite(Rooms, sizeof(Rooms), 1, file);
    fwrite(&Stair, sizeof(Stair), 1, file);
    fwrite(&player, sizeof(player), 1, file);
    fwrite(Monsters, sizeof(Monsters), 1, file);

    fclose(file);
}
void Load_Game(){
    char path[80];
    sprintf(path,"Maps/%s.bat",current_user.username);
    FILE *file = fopen(path, "rb");

    
    fread(&W, sizeof(W), 1, file);
    fread(&H, sizeof(H), 1, file);
    fread(map, sizeof(map), 1, file);
    fread(&Floor, sizeof(Floor), 1, file);
    fread(Rooms, sizeof(Rooms), 1, file);
    fread(&Stair, sizeof(Stair), 1, file);
    fread(&player, sizeof(player), 1, file);
    fread(Monsters, sizeof(Monsters), 1, file);

    fclose(file);
}
void Backpack_Menu(){
    clear();
    strcpy(Message," ");
    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 4;
    const char* back_pack_menu[] = {
    "Foods",
    "Potions",
    "Weapons",
    "Exit"
    };
    while(1) {
        // Draw Box
        erase();
        mvprintw(1, 1, "%s", Message);
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);
        attroff(COLOR_PAIR(1));
        Print_Menu(highlight , back_pack_menu , n_choices);
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
                if      (highlight == 0) {Food_Menu(); break;}
                else if (highlight == 1) {Potion_Menu(); break;}
                else if (highlight == 2) {weapon_Menu(); break;}
                else if (highlight == 3) {return;}
                break;
        }
    }
}
void Food_Menu(){
    clear();
    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 4;

    char normal[50];
    char exellent[50];
    char magical[50];
    sprintf(normal,  "%s     Normal      %d     -10H , +5L       ", s_normal_food, player.backpack.normal_food);
    sprintf(exellent,"%s   Exellent      %d     -10H , +10L , 2xP", s_exellent_food, player.backpack.exellent_food);
    sprintf(magical, "%s    Magical      %d     -10H , +15L , 2xS", s_magical_food, player.backpack.magical_food);
    const char* food_menu[] = {
    normal,
    exellent,
    magical,
    "Exit"
    };
    while(1) {
        // Draw Box
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);

        mvprintw(H/2 + -4, (W - 32)/2, "Logo    Name    numb    Feature ");
        mvprintw(H/2 + -3, (W - 32)/2 ,"________________________________");

        Print_Menu(highlight , food_menu , n_choices);
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
            int h = 10;// number of help hunger
                if(highlight == 0){
                    if(player.backpack.normal_food > 0){
                        player.backpack.normal_food--;

                        int pos = rand() % 100;
                        if(pos > 20){ // for normal food
                            strcpy(Message,"\t You eat a normal food and get -10 hunger, +5 life");
                            player.life = (player.life <= 95) ? player.life + 5 : 100;
                            player.hunger = (player.hunger > h) ? player.hunger - 10 : 0;
                        }else{ // for spoiled food
                            strcpy(Message,"\t You eat a spoiled food and get +10 hunger ,-5 life");
                            player.life -= 5;
                            player.hunger += h;
                        }
                        return;
                    }
                }
                else if (highlight == 1){
                    if(player.backpack.exellent_food > 0){
                        player.backpack.exellent_food--;
                        strcpy(Message,"\t You eat a exellent food and get -10 hunger, + 10 life, 2x power abilaty");
                        strcpy(player.current_abilaty,"2x power");
                        player.n_space = 10;
                        player.life = (player.life <= 90) ? player.life + 10 : 100;
                        player.hunger = (player.hunger > h) ? player.hunger - h : 0;
                        return;
                    }
                }
                else if (highlight == 2){
                    if(player.backpack.magical_food > 0){
                        player.backpack.magical_food--;
                        strcpy(Message,"\t You eat a magical food and get -10 hunger, + 15 life, 2x speed abilaty");
                        strcpy(player.current_abilaty,"2x speed");
                        player.n_abilaty = 10;
                        player.life = (player.life <= 85) ? player.life + 15 : 100;
                        player.hunger = (player.hunger > h) ? player.hunger - h : 0;
                        return;
                    }
                }
                else if (highlight == 3){
                    return;
                }
                break;
        }
    }
}
void Potion_Menu(){
    clear();
    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 4;

    char health[50];
    char speed[50];
    char damage[50];
    sprintf(health, "%s     Health        %d      2x L", s_health_potion, player.backpack.health_potion);
    sprintf(speed,  "%s      Speed       %d      2x S", s_speed_potion, player.backpack.speed_potion);
    sprintf(damage, "%s     Damage       %d      2x P", s_damage_potion, player.backpack.damage_potion);
    const char* potion_menu[] = {
    health,
    speed,
    damage,
    "Exit"
    };
    while(1) {
        // Draw Box
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);

        mvprintw(H/2 + -4, (W - 32)/2, "Logo    Name    numb    Feature ");
        mvprintw(H/2 + -3, (W - 32)/2 ,"________________________________");

        Print_Menu(highlight , potion_menu , n_choices);
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
                    if(player.backpack.health_potion > 0){
                        player.backpack.health_potion--;
                        strcpy(Message,"\t You eat a health potion and get 2x life abilaty");
                        strcpy(player.current_abilaty, "2x life");
                        player.n_abilaty = 10;
                        return;
                    }
                }
                else if (highlight == 1){
                    if(player.backpack.speed_potion > 0){
                        player.backpack.speed_potion--;
                        strcpy(Message,"\t You eat a speed potion and get 2x speed abilaty");
                        strcpy(player.current_abilaty, "2x speed");
                        player.n_abilaty = 10;
                        return;
                    }
                }
                else if (highlight == 2){
                    if(player.backpack.damage_potion > 0){
                        player.backpack.damage_potion--;
                        strcpy(Message,"\t You eat a damage potion and get 2x power abilaty");
                        strcpy(player.current_abilaty, "2x power");
                        player.n_space = 10;
                        return;
                    }
                }
                else if (highlight == 3){
                    return;
                }
                break;
        }
    }
}
void weapon_Menu(){
    clear();
    // Print_Menu inputs
    int highlight = 0;
    int n_choices = 6;

    char mace[50];
    char dagger[50];
    char magic[50];
    char arrow[50];
    char sword[50];
    sprintf(mace,  "%s      Mace      1   5    Short", s_mace_weapon);
    sprintf(sword, "%s     Sword     %d   10   Short", s_sword_weapon, player.backpack.sword_weapon);
    sprintf(dagger,"%s     Dagger     %d   12    5   ", s_dagger_weapon, player.backpack.dagger_weapon);
    sprintf(magic, "%s   Magic Wand   %d   15    10  ", s_magic_wond_weapon, player.backpack.magic_wond_weapon);
    sprintf(arrow, "%s  Normal Arrow  %d   5     5   ", s_normal_arrow_weapon, player.backpack.normal_arrow_weapon);
    const char* weapon_menu[] = {
    mace,
    sword,
    dagger,
    magic,
    arrow,
    "Exit"
    };
    while(1) {
        // Draw Box
        erase();
        attron(COLOR_PAIR(1));
        box(stdscr , 0 , 0);

        mvprintw(H/2 + -4, (W - 32)/2, "Logo    Name   numb   Pow   Dis ");
        mvprintw(H/2 + -3, (W - 32)/2 ,"________________________________");

        Print_Menu(highlight , weapon_menu , n_choices);
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
                if      (highlight == 0){
                    strcpy(Message,"\t Your current weapon changed to mace");
                    strcpy(player.current_weapon, "mace");
                    return;
                }
                else if (highlight == 1){
                    if(player.backpack.sword_weapon > 0){
                    strcpy(Message,"\t Your current weapon changed to sword");
                    strcpy(player.current_weapon, "sword");
                    return;   
                    }
                }
                else if (highlight == 2){
                    if(player.backpack.dagger_weapon > 0){
                    strcpy(Message,"\t Your current weapon changed to dagger");
                    strcpy(player.current_weapon, "dagger");
                    return;   
                    }                    
                }
                else if (highlight == 3){
                    if(player.backpack.magic_wond_weapon > 0){
                    strcpy(Message,"\t Your current weapon changed to magic wand");
                    strcpy(player.current_weapon, "magic wand");
                    return;   
                    }                    
                }
                else if (highlight == 4){
                    if(player.backpack.normal_arrow_weapon > 0){
                    strcpy(Message,"\t Your current weapon changed to normal arrow");
                    strcpy(player.current_weapon, "normal arrow");
                    return;   
                    }                    
                }
                else if (highlight == 5){
                    return;
                }
                break;
        }
    }
}