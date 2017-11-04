#include "game.h"
#include "entity.h"
#include <math.h>
#include <iostream>

namespace possum {
    Game::Game()
    {
        gameState["scene"] = 0;
    }

    Game::~Game()
    {
        //dtor
    }

    void Game::create(int width, int height, std::string title){
        window.create(sf::VideoMode(width, height), title);
        gameState["width"] = width;
        gameState["height"] = height;
    }

    void Game::run(){
        sf::Clock clock;
        while (window.isOpen()){
            if (gameState["scene"] != -1){
                currentScene = *scenes[gameState["scene"]];
                gameState["scene"] = -1;
            }
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                } else if (event.type == sf::Event::KeyPressed){
                    for (const auto& i : currentScene.entities){
                        i->handle_event(KEY_DOWN, currentScene, gameState, &event.key);
                    }
                } else if (event.type == sf::Event::KeyReleased){
                    for (const auto& i : currentScene.entities){
                        i->handle_event(KEY_UP, currentScene, gameState, &event.key);
                    }
                } else if (event.type == sf::Event::MouseButtonPressed){
                    for (const auto& i : currentScene.entities){
                        i->handle_event(BUTTON_DOWN, currentScene, gameState, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseButtonReleased){
                    for (const auto& i : currentScene.entities){
                        i->handle_event(BUTTON_UP, currentScene, gameState, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseMoved){
                    for (const auto& i : currentScene.entities){
                        i->handle_event(POINTER_MOVE, currentScene, gameState, &event.mouseMove);
                    }
                } else if (event.type == sf::Event::Resized){
                    std::cout << "Resized to " << event.size.width << "x" << event.size.height << std::endl;
                    gameState["width"] = event.size.width;
                    gameState["height"] = event.size.height;
                }
            }
            sf::Time time = clock.getElapsedTime() + clock.restart();
            for (const auto& i : currentScene.entities){
                i->handle_event(UPDATE, currentScene, gameState, &time);
            }
            for (std::vector<std::shared_ptr<Entity> >::iterator ep1 = currentScene.entities.begin(); ep1 < currentScene.entities.end(); ++ep1){
                std::shared_ptr<Entity> e1 = *ep1;
                for (std::vector<std::shared_ptr<Entity> >::iterator ep2 = currentScene.entities.begin(); ep2 < ep1; ++ep2){
                    std::shared_ptr<Entity> e2 = *ep2;
                    if (pow(e1->x-e2->x, 2)+pow(e1->y-e2->y, 2) < pow(e1->radius+e2->radius, 2)){
                        e1->handle_event(COLLISION, currentScene, gameState, &*e2);
                        e2->handle_event(COLLISION, currentScene, gameState, &*e1);
                    }
                }
            }
            sf::View view(sf::FloatRect(gameState["x"], gameState["y"], gameState["width"], gameState["height"]));
            window.setView(view);
            window.clear(currentScene.background);
            currentScene.entities.erase(std::remove_if(currentScene.entities.begin(), currentScene.entities.end(), [](std::shared_ptr<Entity> const &e){return e->dead;}), currentScene.entities.end());
            for (const auto& i : currentScene.entities){
                /* if (i->callbacks[REDRAW] == 0){
                    std::cout << "Not ";
                }
                std::cout << "Redrawing object" << i->type << "!" << std::endl;*/
                i->handle_event(REDRAW, currentScene, gameState, &window);
            }
            window.display();
        }
    }

    sf::Texture& Game::loadTexture(std::string filename){
        if (textures.find(filename) != textures.end()){
            return textures.find(filename)->second;
        }
        textures[filename] = sf::Texture();
        textures[filename].loadFromFile(filename);
        return textures[filename];
    }
}
