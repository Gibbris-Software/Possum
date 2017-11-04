#include "lua_script.h"
#include "sfml.h"
#include "gamestate.h"

#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace possum {
    //*/ For debugging
    void stackdump_g(lua_State* l)
    {
        int i;
        int top = lua_gettop(l);
     
        printf("total in stack %d\n",top);
     
        for (i = 1; i <= top; i++)
        {  //* repeat for each level 
            int t = lua_type(l, i);
            switch (t) {
                case LUA_TSTRING:  //* strings /
                    printf("string: '%s'\n", lua_tostring(l, i));
                    break;
                case LUA_TBOOLEAN:  //* booleans /
                    printf("boolean %s\n",lua_toboolean(l, i) ? "true" : "false");
                    break;
                case LUA_TNUMBER:  //* numbers /
                    printf("number: %g\n", lua_tonumber(l, i));
                    break;
                default:  //* other values /
                    printf("%s\n", lua_typename(l, t));
                    break;
            }
            printf("  ");  //* put a separator /
        }
        printf("\n");  //* end the listing /
    }//*/


    class LuaEntity: public Entity {
      public:
        lua_State* data;
        LuaEntity(int type, float x, float y, float radius, sf::Texture& texture, lua_State* data): Entity(type, x, y, radius, texture), data(data){};
    };

    luaL_Reg scene_lib[2];

    void redraw(Entity& e, Scene& scene, State& state, void* data){
        sf::RenderWindow& window = *(sf::RenderWindow*)(data);
        e.sprite.setPosition(e.x, e.y);
        window.draw(e.sprite);
    }

    void update(Entity& e, Scene& scene, State& state, void* data){
        sf::Time time = *(sf::Time*) data;
        LuaEntity* entity = (LuaEntity*) &e;
        //stackdump_g(entity->data);
        //::cout << "Start: " << lua_gettop(entity->data) << std::endl;
        lua_pushlightuserdata(entity->data, entity);
        lua_gettable(entity->data, LUA_REGISTRYINDEX);
        //std::cout << "With Entity: " << lua_gettop(entity->data) << std::endl;
        lua_getfield(entity->data, -1, "type");
        //stackdump_g(entity->data);
        lua_pop(entity->data, 1);
        //stackdump_g(entity->data);
        lua_getfield(entity->data, -1, "callbacks");
        //stackdump_g(entity->data);
        lua_getfield(entity->data, -1, "update");
        lua_pushlightuserdata(entity->data, entity);
        lua_gettable(entity->data, LUA_REGISTRYINDEX);
        lua_getglobal(entity->data, "scene");
        lua_newtable(entity->data); // Game state
        for (auto item : state){
            lua_pushinteger(entity->data, item.second);
            lua_pushstring(entity->data, item.first.c_str());
            lua_settable(entity->data, -3);
        }
        lua_pushnumber(entity->data, time.asSeconds());
        //stackdump_g(entity->data);
        lua_call(entity->data, 4, 0);
        //stackdump_g(entity->data);
        //std::cout << "When I need the entity again: " << lua_gettop(entity->data) << std::endl;
        lua_pop(entity->data, 1);
        lua_getfield(entity->data, 1, "dead");
        entity->dead = lua_toboolean(entity->data, 2);
        lua_pop(entity->data, 1);
        lua_getfield(entity->data, 1, "rotation");
        entity->sprite.setRotation(lua_tonumber(entity->data, 2));
        lua_pop(entity->data, 1);
        lua_getfield(entity->data, 1, "x");
        entity->x = lua_tonumber(entity->data, 2);
        lua_pop(entity->data, 1);
        lua_getfield(entity->data, 1, "y");
        entity->y = lua_tonumber(entity->data, 2);
        lua_pop(entity->data, 1);
        lua_getfield(entity->data, 1, "radius");
        entity->radius = lua_tonumber(entity->data, 2);
        lua_pop(entity->data, 2);
        //std::cout << "End: " << lua_gettop(entity->data) << std::endl;
        //std::cout << entity->x << ", " << entity->y << std::endl;
    }

    void keypress(Entity& e, Scene& scene, State& state, void* data){
        sf::Event::KeyEvent key_event = *(sf::Event::KeyEvent*) data;
        LuaEntity* entity = (LuaEntity*) &e;
        //stackdump_g(entity->data);
        //std::cout << "Start: " << lua_gettop(entity->data) << std::endl;
        lua_pushlightuserdata(entity->data, entity);
        lua_gettable(entity->data, LUA_REGISTRYINDEX);
        //std::cout << "With Entity: " << lua_gettop(entity->data) << std::endl;
        lua_getfield(entity->data, -1, "type");
        //stackdump_g(entity->data);
        lua_pop(entity->data, 1);
        //stackdump_g(entity->data);
        lua_getfield(entity->data, -1, "callbacks");
        //stackdump_g(entity->data);
        lua_getfield(entity->data, -1, "keypress");
        lua_pushlightuserdata(entity->data, entity);
        lua_gettable(entity->data, LUA_REGISTRYINDEX);
        lua_getglobal(entity->data, "scene");
        lua_newtable(entity->data); // Game state
        for (auto item : state){
            lua_pushinteger(entity->data, item.second);
            lua_pushstring(entity->data, item.first.c_str());
            lua_settable(entity->data, -3);
        }
        lua_pushinteger(entity->data, key_event.code);
        //stackdump_g(entity->data);
        lua_call(entity->data, 4, 0);
        lua_pop(entity->data, 2);
    }

    // Scene functions
    int create_lua(lua_State* state){
        int n = lua_gettop(state);
        if (n != 1){
            lua_pushliteral(state, "Can only create entity from a single table");
            lua_error(state);
            return 0;
        }
        //std::cout << "Creating Entity!" << std::endl;
        //stackdump_g(state);
        lua_getfield(state, 1, "type");
        int type = lua_tointeger(state, 2);
        lua_pop(state, 1);
        lua_getfield(state, 1, "x");
        float x = lua_tonumber(state, 2);
        lua_pop(state, 1);
        lua_getfield(state, 1, "y");
        float y = lua_tonumber(state, 2);
        lua_pop(state, 1);
        lua_getfield(state, 1, "radius");
        float radius = lua_tonumber(state, 2);
        lua_pop(state, 1);
        lua_getfield(state, 1, "texture");
        std::string texture = lua_tostring(state, 2);
        lua_pop(state, 1);
        Scene* scene = (Scene*) lua_touserdata(state, lua_upvalueindex(1));
        Game* game = (Game*) lua_touserdata(state, lua_upvalueindex(2));
        std::shared_ptr<Entity> entity(new LuaEntity(type, x, y, radius, game->loadTexture(texture), state));
        scene->add(entity);
        lua_getfield(state, 1, "visible");
        if (lua_toboolean(state, -1) == 1){
            entity->register_event(REDRAW, redraw);
        }
        lua_pop(state, 1);
        if (lua_getfield(state, 1, "callbacks") == LUA_TTABLE){
            lua_pushnil(state);
            while (lua_next(state, 2) != 0){
                lua_pop(state, 1);
                if (lua_type(state, -1) == LUA_TSTRING){
                    std::string name = lua_tostring(state, -1);
                    if (name == "update"){
                        entity->register_event(UPDATE, update);
                    } else if (name == "keypress"){
                        entity->register_event(KEY_DOWN, keypress);
                    /*
                            break;
                        case "keyrelease":
                            //entity->register_event(KEY_UP, keyrelease);
                            break;
                        case "buttonpress":
                            //entity->register_event(BUTTON_DOWN, buttonpress);
                            break;
                        case "buttonrelease":
                            //entity->register_event(BUTTON_UP, buttonrelease);
                            break;
                        case "pointermotion":
                            //entity->register_event(POINTER_MOVE, pointer);
                            break;
                        case "collision":
                            //entity->register_event(COLLISION, keypress);
                            break;
                    */}
                }
            }
        }
        lua_pop(state, 1);
        //stackdump_g(state);
        lua_pushlightuserdata(state, entity.get());
        lua_rotate(state, -2, 1);
        lua_settable(state, LUA_REGISTRYINDEX);
        return 0;
    }

    void open_keyboard(lua_State* state){
        lua_newtable(state);
        lua_pushinteger(state, sf::Keyboard::Left);
        lua_setfield(state, -2, "Left");
        lua_pushinteger(state, sf::Keyboard::Right);
        lua_setfield(state, -2, "Right");
        lua_setglobal(state, "keyboard");
    }

    // Public scene loader
    Scene& load_lua_scene(std::string filename, Game& game){
        lua_State* data = luaL_newstate();
        //lua_pushcfunction(data, luaopen_base);
        //lua_pushliteral(data, "");
        //lua_call(data, 1, 0);
        Scene* scene = game.newScene().get();
        luaL_requiref(data, LUA_MATHLIBNAME, luaopen_math, 1);
        lua_pop(data, 1);
        luaL_newlibtable(data, scene_lib); // Scene
        lua_pushlightuserdata(data, scene);
        lua_pushlightuserdata(data, &game);
        luaL_setfuncs(data, scene_lib, 2);
        lua_setglobal(data, "scene");
        lua_newtable(data); // Game state
        for (auto item : game.gameState){
            lua_pushstring(data, item.first.c_str());
            lua_pushinteger(data, item.second);
            lua_settable(data, -3);
        }
        lua_setglobal(data, "state");
        open_keyboard(data);
        if (luaL_loadfile(data, filename.c_str()) != LUA_OK){
            std::cout << lua_tostring(data, -1) << std::endl;
            return *scene;
        }
        lua_call(data, 0, 0);
        //stackdump_g(data);
        return *scene;
    }

    void setup_lua_scripting(){
        scene_lib[0] = {"create", create_lua};
        scene_lib[1] = {NULL, NULL};
    }

}
