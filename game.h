#ifndef GAME_H
#define GAME_H

#include<ncurses.h>
#include<curses.h>
#include<locale.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<string.h>
#include<unistd.h>

// Symbols
#define s_hero "𝄞"
#define s_wall_o "─"
#define s_wall_a "│"
#define s_floor "."
#define s_pillar "𐃏"
#define s_stair "⫸"
#define s_window "="
#define s_path "*"
#define s_trap "^"
// Doors
#define s_normal_door "⟰"
#define s_secret_door "?"
// Foods
#define s_normal_food "🌢"
#define s_exellent_food "☘"
#define s_magical_food "𐂂"
//#define s_spoiled_food "𓆛"
// Gold
#define s_normal_gold "⛂"
#define s_black_gold "⛃"
#define s_treasure "🗝"
// Weapon
#define s_mace_weapon "🛠"
#define s_dagger_weapon "🗡"
#define s_magic_wond_weapon "⚕"
#define s_normal_arrow_weapon "➜"
#define s_sword_weapon "⚔"
// Potion
#define s_health_potion "🕊"
#define s_speed_potion "⛸"
#define s_damage_potion "🗲"
// Monster
#define s_deamon_monster "D"
#define s_fire_breathing_monster "F"
#define s_giant_monster "G"
#define s_snake_monster "S"
#define s_undeed_monster "U"


// Structs
typedef struct{
    char type[5];
    int visible;
    int color;
}Cell;

typedef struct{
    int x,y;
}point;

typedef struct{
    int x,y; // Top left corner
    int w,h;
    char theme[20]; // "ENCHANT" , "TREASURE" , "REGULAR"
    point doors[4]; // 0 -> up , 1 -> right , 2 -> down , 3 -> left
    point windows[4]; // 0 -> up , 1 -> right , 2 -> down , 3 -> left
    int visibality;
}Room;

typedef struct{
// Foods
    int normal_food;
    int exellent_food;
    int magical_food;
// Weapon
    int sword_weapon;
    int dagger_weapon;
    int magic_wond_weapon;
    int normal_arrow_weapon;
// Potion
    int health_potion;
    int speed_potion;
    int damage_potion;
} Backpack;

typedef struct{
    int x,y;
    int life;
    int hunger;
    int gold;
    int score;
    Backpack backpack;
    char current_weapon[20]; // "mace" , "dagger" , "magic wand" , "normal arrow" , "sword"
    char current_abilaty[20]; // "2x speed" , "2x power" , 2x life
    int n_abilaty;
    int n_space;
    int move_count;
}Player;

typedef struct {
    int x,y;
    int visibality;
    char type;
    int life;
    int damage;
    int n_move;
} Monster;
// Functions
    void set_color();
    void init_ncurses();
    int play_game();
    void print_game(int m);
    void make_map();
    void update_visibility(int x, int y);
    // Rooms
    void make_room(int x_start, int y_start, int i);
    void add_theme(int number[6], int* size, int list[3], int* stair);
    void add_obj(Room room, int I);
    void save_room(Room room, int I);
    point make_doorXwindow(Room room , int I, int dir);
    void make_path(point p1, point p2, int dir, int curv);
    // Player
    int move_player(int ch);
    void collect_backpack(int ch);
    int check_move_player(int x, int y); // update life & hunger & abilaty 
    int which_room(int x, int y);
    void Backpack_Menu();
        void Food_Menu();
        void Potion_Menu();
        void weapon_Menu();
    // Monsters
    void add_monster(Room room, int I);
    void move_monster(int xP, int yP);
    // End game
    void Save_Game();
    void Load_Game();
    void Update_Scoreboard(int life);
    void End_Game_Menu(int life);


#endif
