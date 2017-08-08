#include "scene.h"

namespace possum {
    Entity& Scene::create(int type, float x, float y, float radius, sf::Texture& texture){
        Entity e;
        e.type = type;
        e.x = x;
        e.y = y;
        e.radius = radius;
        e.dead = false;
        e.texture = texture;
        e.sprite.setTexture(texture);
        e.sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
        for (int i = 0; i < 16; i++){
            e.callbacks[i] = 0;
        }
        entities.push_back(e);
        return entities.at(entities.size()-1);
    }

}
