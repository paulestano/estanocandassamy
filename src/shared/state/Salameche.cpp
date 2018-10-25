//
// Created by paul on 10/10/18.
//

#include "Salameche.h"

#include <memory>

using namespace state;
using namespace std;

Salameche::Salameche(uint &pokeId, string orientation,uint currentLife, state::Position position, bool alive) {
    this->currentLife = currentLife;
    this->position = position;
    this->pokeId = pokeId;
    this->alive = alive;
    this->fullLife = 200;
    this->type = "Salameche";
//    this->orientation = orientation;
    if(orientation == "south" || orientation == "north" || orientation == "west" || orientation == "est")
    {
        this->orientation= orientation;
    }

}
