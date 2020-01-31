#include <ncurses.h>
#include <array>
#include <deque>
#include <random>

class GameObject{

    protected:
        static std::default_random_engine generator;
        int max_x, max_y;
        std::array<int,2> position;

    public:

        GameObject();
        GameObject(int x, int y);
        GameObject(std::array<int,2> arr);

        void set_position(int x, int y);
        void set_position(std::array<int,2> arr);
        void set_bounds(int x_bound, int y_bound);
        
        std::array<int,2> get_position();


};


class Snake: public GameObject{

    private:
        std::array<int, 2> direction;
        std::deque<std::array<int, 2>> body;
        int length;

    public:
        Snake(std::array<int,2> arr={0,0}, std::array<int,2> bounds = {10,10}, int len = 10);
        void move();
        void grow();
        void draw(WINDOW * win = stdscr);
        void set_direction(int dx, int dy);
        void turn(int key);


        bool body_contains(int x, int y, int count_head = 0);
        bool body_contains(std::array<int,2> arr, int count_head = 0);

        int get_length();
};

class Food: public GameObject{

    private:
        std::uniform_int_distribution<int> dist_x, dist_y;
    public:
        Food(int x_bound = 0,int y_bound = 0);
        Food(Snake &snake);
        void respawn(Snake &snake);
        void draw(WINDOW * win = stdscr);

};