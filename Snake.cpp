#include "Snake.h"
#include <chrono>
#include <iterator>

/*
    Game Object
*/

// Initialization of static members

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine GameObject::generator = std::default_random_engine(seed);

GameObject::GameObject(){
    position = std::array<int,2> {0,0};
    max_x = 0;
    max_y = 0;
};

GameObject::GameObject(int x, int y){
    position = std::array<int,2> {x,y};
    max_x = 0;
    max_y = 0;
}

GameObject::GameObject(std::array<int,2> arr){
    position = std::array<int,2>{arr};
}

void GameObject::set_position(int x, int y){
    position = std::array<int,2>{x,y};
}

void GameObject::set_position(std::array<int,2> arr){
    position = std::array<int,2>{arr};
}

void GameObject::set_bounds(int x_bound,int y_bound){
    max_x = x_bound;
    max_y = y_bound;
}

std::array<int,2> GameObject::get_position(){return position;}

/*
    Snake
*/

Snake::Snake(std::array<int,2> arr, std::array<int,2> bounds, int len):
    GameObject{arr}{

    length = len;
    set_bounds(bounds[0],bounds[1]);
    direction = std::array<int,2>{1,0};
    body = std::deque<std::array<int,2>>{};

    int x = position[0];
    int y = position[1];

    for (int i = len-1; i>= 0; i-- ){
        body.push_front(std::array<int,2>{(max_x+ x-i)%max_x,y});
    }
}

void Snake::move(){

    position[0] = (max_x + position[0]+direction[0])%max_x;
    position[1] = (max_y + position[1]+direction[1])%max_y;

    body.pop_back();//remove last element of body
    body.push_front({position[0],position[1]}); //add "head" to body
}

void Snake::grow(){

    position[0] = (max_x + position[0]+direction[0])%max_x;
    position[1] = (max_y + position[1]+direction[1])%max_y;

    body.push_front({position[0],position[1]}); //add "head" to body

    length++;
}

void Snake::draw(WINDOW * win){

    switch (direction[0])
    {
    case 1:
        mvwprintw(win,position[1],position[0],">");
        break;
    case -1:
        mvwprintw(win,position[1],position[0],"<");
        break;
        
    default:
        switch (direction[1])
        {
        case 1:
            mvwprintw(win,position[1],position[0],"V");
            break;
        
        case -1:
            mvwprintw(win,position[1],position[0],"^");
            break;

        default:
            break;
        }
    }

    for(std::deque<std::array<int,2>>::iterator it = ++body.begin(); it != body.end(); ++it){

        int x = (*it)[0];
        int y = (*it)[1];

        mvwprintw(win,y,x,"X");

    } 

}

void Snake::set_direction(int dx, int dy){

    if (dx == -direction[0] || dy == -direction[1]){ // avoid 180 turns
        return;
    }
    direction = {dx,dy};
}

void Snake::turn(int key){
        
    switch (key){

        case KEY_RIGHT:
            set_direction(1,0);
            break;
        
        case KEY_LEFT:
            set_direction(-1,0);
            break;
        
        case KEY_DOWN: //y axis increases downwards
            set_direction(0,1);
            break;
        
        case KEY_UP:
            set_direction(0,-1); 
            break;
    }
}


bool Snake::body_contains(int x, int y, int count_head){

    for (auto it = std::next(body.begin(),count_head); it != body.end(); it++){

        if ( x == (*it)[0] && y == (*it)[1]){
            return true;
        }
    }
    return false;

}

bool Snake::body_contains(std::array<int,2>arr, int count_head){
    return body_contains(arr[0],arr[1],count_head);
}

int Snake::get_length(){return length;}




/* 
    Food
*/

Food::Food(int x_bound, int y_bound){
    set_bounds(x_bound,y_bound);
    dist_x = std::uniform_int_distribution<int> {0,max_x-1};
    dist_y = std::uniform_int_distribution<int> {0,max_y-1};
}

Food::Food(Snake &snake){
    respawn(snake);
}

void Food::respawn(Snake &snake){

    int pos_x, pos_y;

    pos_x = dist_x(generator);
    pos_y = dist_y(generator);

    while( snake.body_contains(pos_x,pos_y) ){
        pos_x = dist_x(generator);
        pos_y = dist_y(generator);
    }

    set_position(pos_x,pos_y);

}
void Food::draw(WINDOW * win){
    mvwprintw(win,position[1],position[0],"O");
}
