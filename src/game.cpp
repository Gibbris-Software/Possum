#include "game.h"
#include "entity.h"
#include <math.h>
#include <iostream>

namespace possum {
    Game::Game(): pendingScene(0), currentScene(0)
    {
        //ctor
    }

    Game::~Game()
    {
        //dtor
    }

    void Game::create(int width, int height, std::string title){
        window.create(sf::VideoMode(width, height), title);
        gameState.set("width", width);
        gameState.set("height", height);
    }

    void Game::run(){
        sf::Clock clock;
        while (window.isOpen()){
            if (pendingScene != 0){
                currentScene = pendingScene;
                pendingScene = 0;
            }
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                } else if (event.type == sf::Event::KeyPressed){
                    for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                        i->handle_event(KEY_DOWN, gameState, &event.key);
                    }
                } else if (event.type == sf::Event::KeyReleased){
                    for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                        i->handle_event(KEY_UP, gameState, &event.key);
                    }
                } else if (event.type == sf::Event::MouseButtonPressed){
                    for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                        i->handle_event(BUTTON_DOWN, gameState, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseButtonReleased){
                    for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                        i->handle_event(BUTTON_UP, gameState, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseMoved){
                    for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                        i->handle_event(POINTER_MOVE, gameState, &event.mouseMove);
                    }
                } else if (event.type == sf::Event::Resized){
                    gameState.set("width", event.size.width);
                    gameState.set("height", event.size.height);
                }
            }
            sf::Time time = clock.getElapsedTime() + clock.restart();
            for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                i->handle_event(UPDATE, gameState, &time);
            }
            for (std::vector<Entity>::iterator e1 = currentScene->entities.begin(); e1 < currentScene->entities.end(); ++e1){
                for (std::vector<Entity>::iterator e2 = currentScene->entities.begin(); e2 < e1; ++e2){
                    if (pow(e1->x-e2->x, 2)+pow(e1->y-e2->y, 2) < pow(e1->radius+e2->radius, 2)){
                        e1->handle_event(COLLISION, gameState, &(*e2));
                        e2->handle_event(COLLISION, gameState, &(*e1));
                    }
                }
            }
            window.clear(currentScene->background);
            currentScene->entities.erase(std::remove_if(currentScene->entities.begin(), currentScene->entities.end(), Entity::isDead), currentScene->entities.end());
            for (std::vector<Entity>::iterator i = currentScene->entities.begin(); i < currentScene->entities.end(); ++i){
                /* if (i->callbacks[REDRAW] == 0){
                    std::cout << "Not ";
                }
                std::cout << "Redrawing object" << i->type << "!" << std::endl;*/
                i->handle_event(REDRAW, gameState, &window);
            }
            window.display();
        }
    }
}
