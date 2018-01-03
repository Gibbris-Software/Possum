#ifndef POSSUM_ENTITY_H
#define POSSUM_ENTITY_H

#include "gamestate.h"
#include "sfml.h"

namespace possum {
    class Entity;
    class Scene;
    class Game;

    typedef void (*callback)(Entity&, Scene&, Game&, void*);

    enum event {
        UPDATE,
        REDRAW,
        COLLISION,
        BUTTON_UP,
        BUTTON_DOWN,
        POINTER_MOVE,
        KEY_UP,
        KEY_DOWN,
        USER_EVENT_1,
        USER_EVENT_2,
        USER_EVENT_3,
        USER_EVENT_4,
        USER_EVENT_5,
        USER_EVENT_6,
        USER_EVENT_7,
        USER_EVENT_8,
    };

    class Entity
    {
        public:
            void handle_event(int event, Scene& scene, Game& game, void* data) {if (callbacks[event] != 0) {callbacks[event](*this, scene, game, data);}}
            Entity& register_event(int event, callback handler) {callbacks[event] = handler; return *this;}
            Entity(int type, float x, float y, float radius, sf::Texture& texture):
             type(type), x(x), y(y), radius(radius), texture(texture), sprite(texture),
             callbacks{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, dead(false), state() {}
            int type;
            float x;
            float y;
            float radius;
            sf::Texture& texture;
            sf::Sprite sprite;
            callback callbacks[16];
            bool dead;
            State state;
    };
}

#endif // POSSUM_ENTITY_H
