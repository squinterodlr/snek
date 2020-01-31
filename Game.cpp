#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>

Game::Game(){

    std::cout<<"Starting game..."<<std::endl;
    initscr(); // init the screen
    noecho();  // don't echo key presses
    curs_set(FALSE); // don't display cursor
    keypad(stdscr, TRUE); //allow for special keys
    setlocale(LC_ALL,"");

}

void Game::run(){

    start();

    loop();

    quit();
}

void Game::start(){

    nodelay(stdscr, FALSE);
    int max_x = COLS;
    int max_y = LINES-3;

    snake = Snake {{max_x/2, max_y/2},{max_x,max_y},10};
    food = Food {max_x,max_y};
    food.respawn(snake);


    score = 0;

    mvprintw(LINES-2,0,"Score: %d",score);
    mvprintw(LINES-1,0,"p: pause\t q: quit");

    mvprintw(LINES/2,(COLS/2)-11,"Press any key to start");

    refresh();

    if( getch() != 'q'){
        state = GameState::started;
    }

}

void Game::loop(){

    if (state == GameState::started){
        
        nodelay(stdscr, TRUE); //constantly read
        snake.draw();
        food.draw();
        refresh();
        state = GameState::running;

    }


    while(state == GameState::running){//while( (key = getch()) != 10){ //while not enter
        
        process_key(getch());
        if(state == GameState::quit){return;}
        if(state == GameState::restart){break;}
        sleep(50);
        clear();
        
        snake.draw();
        food.draw();

        mvprintw(LINES-2,0,"Score: %d",score);
        mvprintw(LINES-1,0,"p: pause\t q: quit\t r:restart");

        refresh();
    } 

    switch (state)
    {

    case GameState::failed:

        nodelay(stdscr, FALSE);
        mvprintw(LINES/2    ,(COLS/2)-5,"You lose!");
        mvprintw(LINES/2 + 1,(COLS/2)-5,"Score: %d",score);
        mvprintw(LINES/2 + 3,(COLS/2)-5,"r: restart");
        refresh();
        process_key(getch());
        if (state != GameState::restart)
        {
            return;
        }


    case GameState::restart:
        clear();
        start();
        loop();
        break;


    default:
        break;
    }

}

void Game::quit(){

    endwin();

}

void Game::process_key(int key){

    switch (key)
    {
    case 'p': // pause - unpause

        switch (state)
        {
        case GameState::paused:
            state = GameState::running;
            nodelay(stdscr, TRUE);
            break;
        
        case GameState::running:
            state = GameState::paused;
            nodelay(stdscr, FALSE);
            mvprintw(LINES-2,0,"Score: %d",score);
            mvprintw(LINES-1,0,"p: unpause\t q: quit\t r: restart");
            refresh();

        default:
            process_key(getch());
            break;
        }
        break;

    case 'q': // always quit
        state = GameState::quit;
        break;

    case 'r': // always restart
        state = GameState::restart;
        /*
        switch (state)
        {
        case GameState::running:
            state = GameState::restart;
            break;
        
        default:
            break;
        }
        */
       break;

    default:

        switch (state)
        {
        case GameState::running:
            snake.turn(key);
            
            if ( snake.body_contains(snake.get_position(),1))
            {
                state = GameState::failed;
                break;
            }

            if( snake.get_position() == food.get_position()){
                snake.grow();
                food.respawn(snake);
                score++;
            }
            else{
                snake.move();
            }
            break;

        default:
            process_key(getch());
            break;
        }

        
        break;
    }



}


void Game::sleep(unsigned int ms){ //sleep x milliseconds
    std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}

