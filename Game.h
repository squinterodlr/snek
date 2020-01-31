#include "Snake.h"

enum class GameState { started, running, paused, failed, quit, restart };

class Game{

    public:
        Game();
        void run();


    private:
        int score;
        Snake snake;
        Food food;
        GameState state;

        void start();
        void loop();
        void quit();
        void process_key(int key);
        void sleep(unsigned int ms);

};