#include"menu.h"
#include"game.h"
// Game state
typedef enum {
    MENU,
    GAME,
    EXIT,
} game_states;

int main(){

    // Start curses mode
    init_ncurses();

    // Init current state
    game_states current_state;
    current_state = MENU;
    int result;

    while(1) {
        switch (current_state) {
            case MENU:
                result = show_menu();
                if (result == 0) 
                    current_state = EXIT;
                else if (result == 1)
                    current_state = GAME;
                
                break;

            case GAME:
                result = play_game();
                if (result == 0)
                    current_state = EXIT;
                else if (result == 1)
                    current_state = MENU; 
                                  
                break;
            case EXIT:
                endwin();
                return 0;
        }
    }
}