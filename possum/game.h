#ifndef POSSUM_GAME_H
#define POSSUM_GAME_H

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "scene.h"
#include "gamestate.h"

#include <vector>

namespace possum {
    class Game
    {
        public:
            Game();
            void create(int width, int height, std::string title);
            virtual ~Game();
            void run();
            void setScene(Scene* nextScene) { pendingScene = nextScene; };
        protected:
        private:
            sf::RenderWindow window;
            Scene* pendingScene;
            Scene* currentScene;
            State gameState;
    };
}
#endif // POSSUM_GAME_H
