#ifndef __POSSUM_SCENE_H_
#define __POSSUM_SCENE_H_

#include "SFML/Graphics.hpp"
#include "entity.h"
#include <vector>

namespace possum {
    class Scene
    {
        public:
            Scene(): background(sf::Color(0, 0, 0, 0)){};
            Scene(sf::Color bg): background(bg){}
            Entity& create(int type, float x, float y, float radius, sf::Texture& texture);
            std::vector<Entity> entities;
            sf::Color background;
        protected:
        private:
    };
}
#endif // __POSSUM_SCENE_H_
