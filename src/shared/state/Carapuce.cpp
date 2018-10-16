//
// Created by paul on 10/10/18.
//

#include "Carapuce.h"

using namespace state;
using namespace std;

Carapuce::Carapuce(int& pokeId, state::Position position, bool alive) {
    this->currentLife = currentLife;
    this->position = position;
    this->alive = alive;
    this->name = "carapuce";
    this->pokeId = pokeId;
    fullLife = 200;
}



void Carapuce::heal() {
    if (currentLife <= fullLife-5)
    {
        currentLife += 5;
    }
}