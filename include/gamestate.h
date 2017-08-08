#ifndef POSSUM_GAMESTATE_H
#define POSSUM_GAMESTATE_H

namespace possum {
    class State
    {
        public:
            State(){};
            int get(std::string name){return data[name];};
            void set(std::string name, int value){data[name]=value;};
        protected:
        private:
            std::map<std::string, int> data;
    };
}

#endif // GAMESTATE_H
