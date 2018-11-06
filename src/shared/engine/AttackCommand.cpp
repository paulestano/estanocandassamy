//
// Created by paul on 28/10/18.
//
#include "AttackCommand.h"
#include "state.h"
#include <memory>
#include <iostream>
using namespace engine;
using namespace std;
using namespace state;



void AttackCommand::execute(state::State &state) {
    State& state_ref = state;
    shared_ptr<Player> player_ptr = nullptr;
//    unique_ptr<Pokemon> poke_ptr;
    for( auto player : state_ref.getPlayers())
    {
        if(player.second->getID()== idPlayer)
        {
            player_ptr = player.second;
//            cout << "player found" <<endl;
        }
    }


    if(!player_ptr)
    {
        cout <<"no player"<<endl;
        throw new runtime_error("move command with bad player");
    }
    if(!(player_ptr->getPokemon().get()))
    {
        cout << "no pokemon" <<endl;
        throw new runtime_error("move command with bad pokemon");
    }

    Position p(player_ptr->getPokemon()->getPosition());
    unsigned int id_attacked = this->idPlayer;
    switch(player_ptr->getPokemon()->getOrientation())
    {
        case EST:

            p.x++;
           id_attacked = this->findPokemon(p,state_ref);
            if(id_attacked != this->idPlayer)
            {
                unsigned int lifeAttacked = state_ref.getPlayers().at(id_attacked)->getPokemon()->getCurrentLife();
                unsigned int newLife = 0;
                if(lifeAttacked > 50)
                {
                    newLife = lifeAttacked - 50;
                }
                state_ref.getPlayers().at(id_attacked)->getPokemon()->setCurrentLife(newLife);
                StateEvent e(StateEventId::ALL_CHANGED);
                state_ref.notifyObservers(e);

            }
            break;

        case SOUTH:
            p.y++;
             id_attacked = this->findPokemon(p,state_ref);
            if(id_attacked != this->idPlayer)
            {
                unsigned int lifeAttacked = state_ref.getPlayers().at(id_attacked)->getPokemon()->getCurrentLife();
                unsigned int newLife = 0;
                if(lifeAttacked > 50)
                {
                    newLife = lifeAttacked - 50;
                }
                state_ref.getPlayers().at(id_attacked)->getPokemon()->setCurrentLife(newLife);


            }
            break;
        case NORTH:
            p.y--;
            id_attacked = this->findPokemon(p,state_ref);
            if(id_attacked != this->idPlayer)
            {
                unsigned int lifeAttacked = state_ref.getPlayers().at(id_attacked)->getPokemon()->getCurrentLife();
                unsigned int newLife = 0;
                if(lifeAttacked > 50)
                {
                    newLife = lifeAttacked - 50;
                }
                state_ref.getPlayers().at(id_attacked)->getPokemon()->setCurrentLife(newLife);
                StateEvent e(StateEventId::ALL_CHANGED);
                state_ref.notifyObservers(e);

            }
            break;
        case WEST:
            p.x--;
            id_attacked = this->findPokemon(p,state_ref);
            if(id_attacked != this->idPlayer)
            {
                unsigned int lifeAttacked = state_ref.getPlayers().at(id_attacked)->getPokemon()->getCurrentLife();
                unsigned int newLife = 0;
                if(lifeAttacked > 50)
                {
                    newLife = lifeAttacked - 50;
                }
                state_ref.getPlayers().at(id_attacked)->getPokemon()->setCurrentLife(newLife);
                StateEvent e(StateEventId::ALL_CHANGED);
                state_ref.notifyObservers(e);

            }
            break;


    }


}

AttackCommand::AttackCommand(unsigned int idPlayer) : idPlayer(idPlayer){
}


/**
 * @param p
 * @param state_ref
 * @return idPlayer if can't find a pokemon at p. PokeId at location p
 */
unsigned int AttackCommand::findPokemon(state::Position &p,State& state_ref) {

    for(auto entry: state_ref.getPlayers())
    {
        if(entry.second->getID() != this->idPlayer)
        {
            if(entry.second->getPokemon()->getPosition().x == p.x && entry.second->getPokemon()->getPosition().y == p.y)
            {
                return entry.second->getID();
            }
        }
    }
    return this->idPlayer;
}

