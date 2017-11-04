// Copy all possum files to this directory then run
// g++ -std=c++11 possum_test.cpp game.cpp scene.cpp lua.cpp -o possum_test -lsfml-graphics -lsfml-window -lsfml-system

#include "possum.h"
#include "lua_script.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include "SFML/Graphics.hpp"


enum entity_types {
    POSSUM,
    TREE,
    ACORN,
    WIN
};


// possum::Scene rooms[9];
possum::Game game;

/* // Possum callbacks (now in Lua)
void redraw_possum(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void update_possum(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::Time time = *(sf::Time*)(data);
    entity.x += cos(entity.sprite.getRotation()*3.141592653589793238462/180)*time.asSeconds()*60;
    entity.y += sin(entity.sprite.getRotation()*3.141592653589793238462/180)*time.asSeconds()*60;
}

void keypress_possum(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::Event::KeyEvent keyEvent = *(sf::Event::KeyEvent*)(data);
    if (keyEvent.code == sf::Keyboard::Left){
        entity.sprite.setRotation(entity.sprite.getRotation()-10);
    } else if (keyEvent.code == sf::Keyboard::Right){
        entity.sprite.setRotation(entity.sprite.getRotation()+10);
    }
}

void collide_possum(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    possum::Entity& other = *(possum::Entity*)(data);
    if (other.type == TREE){
        float factor = pow(other.radius + entity.radius, 2)/(pow(other.x-entity.x, 2)+pow(other.y-entity.y, 2));
        entity.x = other.x + factor * (entity.x - other.x);
        entity.y = other.y + factor * (entity.y - other.y);
    } else if (other.type == ACORN){
        other.dead = true;
        gameState.set("level", gameState.get("level") + 1);
        game.setScene(&rooms[gameState.get("level")]);
    }
}
*/

void redraw_tree(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void collide_tree(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    possum::Entity& other = *(possum::Entity*)(data);
    if (other.type == TREE){
        entity.x = ((float) rand()) / RAND_MAX * 640;
        entity.y = ((float) rand()) / RAND_MAX * 480;
    }
}

void click_tree(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::Event::MouseButtonEvent buttonEvent = *(sf::Event::MouseButtonEvent*)(data);
    if (buttonEvent.button == sf::Mouse::Left && (pow(buttonEvent.x-entity.x, 2) + pow(buttonEvent.y-entity.y, 2) < pow(entity.radius, 2))){
        entity.dead = true;
        gameState["killed_trees"] = gameState["killed_trees"] + 1;
        std::cout << "Killed " << gameState["killed_trees"] << " trees" << std::endl;
    }
}

void redraw_acorn(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void redraw_win(possum::Entity& entity, possum::Scene& scene, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.sprite.getOrigin());
    window.draw(entity.sprite);
}


int main(int argc, char* argv[]){
    /*sf::Texture tree_texture;
    tree_texture.loadFromFile("tree.png");
    //sf::Texture possum_texture;
    //possum_texture.loadFromFile("possum.png");
    sf::Texture acorn_texture;
    acorn_texture.loadFromFile("acorn.png");*/
    sf::Texture win_texture;
    win_texture.loadFromFile("win.png");
    possum::setup_lua_scripting();
    for (int i = 0; i < 8; i++){
        /*possum::Scene room = game.newScene(sf::Color(10, 32, 16));
        for (int j = 0; j <= i; j++){
            float x = ((float) rand()) / RAND_MAX * 640;
            float y = ((float) rand()) / RAND_MAX * 480;
            possum::Entity& t = room.create(TREE, x, y, 20, tree_texture);
            t.register_event(possum::REDRAW, redraw_tree);
            t.register_event(possum::BUTTON_DOWN, click_tree);
            t.register_event(possum::COLLISION, collide_tree);
        }
        float x = ((float) rand()) / RAND_MAX * 640;
        float y = ((float) rand()) / RAND_MAX * 480;
        possum::Entity& a = room.create(ACORN, x, y, 5, acorn_texture);
        a.register_event(possum::REDRAW, redraw_acorn);*/
        possum::Scene room = load_lua_scene("possum.lua", game);
        room.background = sf::Color(10, 32, 16);
    }
    possum::Scene winroom = *game.newScene();
    winroom.background = sf::Color(10, 32, 16);
    winroom.create(WIN, 0, 0, 0, win_texture).register_event(possum::REDRAW, redraw_win);
    game.create(640, 480, "Possum");
    game.setScene(0);
    game.run();
    return 0;
}
