#ifndef __POSSUM_SCENE_H_
#define __POSSUM_SCENE_H_

#include "SFML/Graphics.hpp"
#include "entity.h"
#include <vector>
#include <memory>

namespace possum {
    class Scene
    {
        public:
            Scene(): entities(), background(sf::Color(0, 0, 0, 0)){};
            Scene(sf::Color bg): entities(), background(bg){}
            Entity& create(int type, float x, float y, float radius, sf::Texture& texture);
            void add(std::shared_ptr<Entity> entity){entities.push_back(entity);};
            std::vector<std::shared_ptr<Entity> > entities;
            sf::Color background;
        protected:
        private:
    };
}
#endif // __POSSUM_SCENE_H_
