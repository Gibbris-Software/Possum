#include "game.h"
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


possum::Scene rooms[9];
possum::Game game;

void redraw_possum(possum::Entity& entity, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void update_possum(possum::Entity& entity, possum::State& gameState, void* data){
    sf::Time time = *(sf::Time*)(data);
    entity.x += cos(entity.sprite.getRotation()*3.141592653589793238462/180)*time.asSeconds()*60;
    entity.y += sin(entity.sprite.getRotation()*3.141592653589793238462/180)*time.asSeconds()*60;
}

void keypress_possum(possum::Entity& entity, possum::State& gameState, void* data){
    sf::Event::KeyEvent keyEvent = *(sf::Event::KeyEvent*)(data);
    if (keyEvent.code == sf::Keyboard::Left){
        entity.sprite.setRotation(entity.sprite.getRotation()-10);
    } else if (keyEvent.code == sf::Keyboard::Right){
        entity.sprite.setRotation(entity.sprite.getRotation()+10);
    }
}

void collide_possum(possum::Entity& entity, possum::State& gameState, void* data){
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

void redraw_tree(possum::Entity& entity, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void collide_tree(possum::Entity& entity, possum::State& gameState, void* data){
    possum::Entity& other = *(possum::Entity*)(data);
    if (other.type == TREE){
        entity.x = ((float) rand()) / RAND_MAX * 640;
        entity.y = ((float) rand()) / RAND_MAX * 480;
    }
}

void click_tree(possum::Entity& entity, possum::State& gameState, void* data){
    sf::Event::MouseButtonEvent buttonEvent = *(sf::Event::MouseButtonEvent*)(data);
    if (buttonEvent.button == sf::Mouse::Left && (pow(buttonEvent.x-entity.x, 2) + pow(buttonEvent.y-entity.y, 2) < pow(entity.radius, 2))){
        entity.dead = true;
        gameState.set("killed_trees", gameState.get("killed_trees") + 1);
        std::cout << "Killed " << gameState.get("killed_trees") << " trees" << std::endl;
    }
}

void redraw_acorn(possum::Entity& entity, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.x, entity.y);
    window.draw(entity.sprite);
}

void redraw_win(possum::Entity& entity, possum::State& gameState, void* data){
    sf::RenderWindow& window = *(sf::RenderWindow*)(data);
    entity.sprite.setPosition(entity.sprite.getOrigin());
    window.draw(entity.sprite);
}


int main(int argc, char* argv[]){
    sf::Texture tree_texture;
    tree_texture.loadFromFile("/home/john/Documents/Python/Possum/tree.png");
    sf::Texture possum_texture;
    possum_texture.loadFromFile("/home/john/Documents/Python/Possum/possum.png");
    sf::Texture acorn_texture;
    acorn_texture.loadFromFile("/home/john/Documents/Python/Possum/acorn.png");
    sf::Texture win_texture;
    win_texture.loadFromFile("/home/john/Documents/Python/Possum/win.png");
    for (int i = 0; i < 8; i++){
        possum::Scene room(sf::Color(10, 32, 16));
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
        a.register_event(possum::REDRAW, redraw_acorn);
        possum::Entity& p = room.create(POSSUM, 320, 240, 10, possum_texture);
        p.register_event(possum::UPDATE, update_possum);
        p.register_event(possum::REDRAW, redraw_possum);
        p.register_event(possum::COLLISION, collide_possum);
        p.register_event(possum::KEY_DOWN, keypress_possum);
        rooms[i] = room;
    }
    possum::Scene winroom(sf::Color(10, 32, 16));
    winroom.create(WIN, 0, 0, 0, win_texture).register_event(possum::REDRAW, redraw_win);
    rooms[8] = winroom;
    game.create(640, 480, "Possum");
    game.setScene(&rooms[0]);
    game.run();
    return 0;
}
