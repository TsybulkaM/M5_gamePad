#ifndef GAMEHUB_HPP
#define GAMEHUB_HPP

#include "Taxi.hpp"
#include "Ball.hpp"
#include "Pong.hpp"
//#include "Mario.hpp"

struct GameElement {
    const char* name;
    ArcadeGame* engine;
    
    GameElement(const char* n, ArcadeGame* e) : engine(e) {
        name = strdup(n);
    }

    ~GameElement() {
        free((void*)name);
        delete engine;
    }
};

GameElement* GameHub[] = { 
  new GameElement("Pong", new Pong()),
  new GameElement("Ball", new Ball()), 
  new GameElement("Taxi", new Taxi()), 
  //new GameElement("Mario", new Mario())
};
const int GameHub_size = sizeof(GameHub) / sizeof(GameHub[0]);

#endif // GAMEHUB_HPP
