#include <chrono>
#include <thread>
#include <locale>
#include "Snake.h"

void sleep(unsigned int x){ //sleep x milliseconds
    std::this_thread::sleep_for (std::chrono::milliseconds(x));
}


int main(){

    initscr(); // init the screen
    noecho();  // don't echo key presses
    curs_set(FALSE); // don't display cursor

    setlocale(LC_ALL,"");

    int size_x, size_y;
    int key;

    //getmaxyx(stdscr,size_y,size_x);

    size_y = LINES - 6;
    size_x = COLS - 10;

    WINDOW * window = newwin(size_y+1,size_x+1,3,5);

    keypad(window, TRUE); //allow for special keys
    nodelay(window, TRUE); //constantly read
    box(window,0,0);


    Snake snek{{size_x/2, size_y/2},{size_x,size_y},10};
    Food food{size_x,size_y};
    food.respawn(snek);

    snek.draw(window);
    food.draw(window);

    wrefresh(window);

    while( (key = wgetch(window)) != 10){ //while not enter
        
        snek.turn(key);
        
        if ( snek.body_contains(snek.get_position(),1) )
        {
            break;
        }        

        if( snek.get_position() == food.get_position()){
            snek.grow();
            food.respawn(snek);
        }
        else{
            snek.move();
        }

        sleep(50);
        wclear(window);

        wborder(window,'|', '|', '-', '-', '+', '+', '+', '+');
        snek.draw(window);
        food.draw(window);
        
        wrefresh(window);
    }
    
    nodelay(window, FALSE);

    mvwprintw(window,size_y/2,size_x/2,"You lose! Length: %d\n",snek.get_length());
    mvwprintw(window,(size_y/2)+1,size_x/2,"Press any key to quit");
    wrefresh(window);
    wgetch(window);
    endwin();

    return(0);
}