#include "game.h"
#include "entity.h"
#include <math.h>
#include <iostream>

namespace possum {
    Game::Game()
    {
        state["scene"] = 0;
    }

    Game::~Game()
    {
        //dtor
    }

    void Game::create(int width, int height, std::string title){
        window.create(sf::VideoMode(width, height), title);
        state["width"] = width;
        state["height"] = height;
    }

    void Game::run(){
        sf::Clock clock;
        while (window.isOpen()){
            if (state["scene"] != -1){
                currentScene = *scenes[state["scene"]];
                state["scene"] = -1;
            }
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                } else if (event.type == sf::Event::KeyPressed){
                    for (int i = 0; i < currentScene.entities.size(); i++){
                        currentScene.entities[i]->handle_event(KEY_DOWN, currentScene, *this, &event.key);
                    }
                } else if (event.type == sf::Event::KeyReleased){
                    for (int i = 0; i < currentScene.entities.size(); i++){
                        currentScene.entities[i]->handle_event(KEY_UP, currentScene, *this, &event.key);
                    }
                } else if (event.type == sf::Event::MouseButtonPressed){
                    for (int i = 0; i < currentScene.entities.size(); i++){
                        currentScene.entities[i]->handle_event(BUTTON_DOWN, currentScene, *this, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseButtonReleased){
                    for (int i = 0; i < currentScene.entities.size(); i++){
                        currentScene.entities[i]->handle_event(BUTTON_UP, currentScene, *this, &event.mouseButton);
                    }
                } else if (event.type == sf::Event::MouseMoved){
                    for (int i = 0; i < currentScene.entities.size(); i++){
                        currentScene.entities[i]->handle_event(POINTER_MOVE, currentScene, *this, &event.mouseMove);
                    }
                } else if (event.type == sf::Event::Resized){
                    std::cout << "Resized to " << event.size.width << "x" << event.size.height << std::endl;
                    state["width"] = event.size.width;
                    state["height"] = event.size.height;
                }
            }
            sf::Time time = clock.getElapsedTime() + clock.restart();
            for (int i = 0; i < currentScene.entities.size(); i++){
                currentScene.entities[i]->handle_event(UPDATE, currentScene, *this, &time);
            }
            for (int i = 0; i < currentScene.entities.size()-1; i++){
                std::shared_ptr<Entity> e1 = currentScene.entities[i];
                for (int j = i+1; j < currentScene.entities.size(); j++){
                    std::shared_ptr<Entity> e2 = currentScene.entities[j];
                    if (pow(e1->x-e2->x, 2)+pow(e1->y-e2->y, 2) < pow(e1->radius+e2->radius, 2)){
                        e1->handle_event(COLLISION, currentScene, *this, &*e2);
                        e2->handle_event(COLLISION, currentScene, *this, &*e1);
                    }
                }
            }
            sf::View view(sf::FloatRect(state["x"], state["y"], state["width"], state["height"]));
            window.setView(view);
            window.clear(currentScene.background);
            currentScene.entities.erase(std::remove_if(currentScene.entities.begin(), currentScene.entities.end(), [](std::shared_ptr<Entity> const &e){return e->dead;}), currentScene.entities.end());
            for (int i = 0; i < currentScene.entities.size(); i++){

                /* if (i->callbacks[REDRAW] == 0){
                    std::cout << "Not ";
                }
                std::cout << "Redrawing object" << i->type << "!" << std::endl;*/
                currentScene.entities[i]->handle_event(REDRAW, currentScene, *this, &window);
            }
            window.display();
        }
    }

    sf::Texture& Game::getTexture(std::string filename){
        if (textures.find(filename) != textures.end()){
            return textures.find(filename)->second;
        }
        textures[filename] = sf::Texture();
        textures[filename].loadFromFile(filename);
        return textures[filename];
    }

    sf::Font& Game::getFont(std::string filename){
        if (fonts.find(filename) != fonts.end()){
            return fonts.find(filename)->second;
        }
        fonts[filename] = sf::Font();
        fonts[filename].loadFromFile(filename);
        return fonts[filename];
    }
}
