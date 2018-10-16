//
// Created by paul on 10/10/18.
//

#include "Salameche.h"



using namespace state;
using namespace std;

Salameche::Salameche(int& pokeId, uint currentLife, state::Position position, bool alive) {
    this->name = "Salameche";
    this->currentLife = currentLife;
    this->position = position;
    this->pokeId = pokeId;
    this->alive = alive;
    this->fullLife = 200;
}

void Salameche::heal() {
    if (currentLife<=fullLife-10)
    {
        currentLife += 10;
    }
}