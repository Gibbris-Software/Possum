#ifndef POSSUM_GAME_H
#define POSSUM_GAME_H

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "scene.h"
#include "gamestate.h"

#include <vector>
#include <memory>

namespace possum {
    class Game
    {
        public:
            Game();
            void create(int width, int height, std::string title);
            virtual ~Game();
            void run();
            void setScene(int nextScene) { gameState["scene"] = nextScene; };
            std::shared_ptr<Scene> newScene() { scenes.push_back(std::shared_ptr<Scene>(new Scene())); return scenes.back();};
            sf::Texture& loadTexture(std::string filename);
            State gameState;
        protected:
        private:
            sf::RenderWindow window;
            Scene currentScene;
            std::vector<std::shared_ptr<Scene>> scenes;
            std::map<std::string, sf::Texture> textures;
    };
}
#endif // POSSUM_GAME_H
