#include <utility>

#include <utility>


//
// Created by paul on 22/10/18.
//

#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "render.h"
#include <iostream>
#include <string>
#include <state/TabEvent.h>
#include "state.h"
#include "ai.h"
#include "engine.h"

using namespace std;
using namespace render;
using namespace state;
using namespace engine;
using namespace ai;
Scene::Scene(shared_ptr<engine::Engine> engine,string tileSet,unsigned int  pokeTarId) : engine(
        std::move(engine)),pokeTarId(pokeTarId) {


    this->tileset= std::move(tileSet);
    string tileset2 = "res/src/tilestPokemon.png";
    this->pokeTileSet.reset(new sf::Texture());
    this->pokeTileSet->loadFromFile("res/src/tilestPokemon.png");
    uint tileWidth = this->engine ->getState().getMap()->getTileWidth();
    uint tileHeight = this->engine->getState().getMap()->getTileHeight();
    tileSize = sf::Vector2u(tileWidth,tileHeight);

    updateMap();
    updatePlayers();


}

void Scene::draw(sf::RenderWindow& window) {

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch(event.type) {
            default:
                break;
            case sf::Event::Closed :
                window.close();
                break;
            case sf::Event::KeyPressed:
                sf::Keyboard::Key k = event.key.code;
                switch(k){
                    case sf::Keyboard::Key::Right  :
                        engine->addCommand(new MoveCommand(EST,0),0);
                        break;
                    case sf::Keyboard::Key::Left :
                        engine->addCommand(new MoveCommand(WEST,0),0);
                        break;
                    case sf::Keyboard::Key::Up:
                        engine->addCommand(new MoveCommand(NORTH,0),0);
                        break;
                    case sf::Keyboard::Key::Down:
                        engine->addCommand(new MoveCommand(SOUTH,0),0);
                        break;
                    case sf::Keyboard::Key::A :
                        engine->addCommand(new AttackCommand(0),0);
                        break;
                    case sf::Keyboard::Key::H:
                        engine->addCommand(new HealCommand(0),0);
                        break;
                    default:
                        break;
                }


        }
        if(event.type == sf::Event::Closed)
            window.close();

    }



    window.clear();
    for(const auto &layerRend : layerVec)
    {
        window.draw(*layerRend);
    }
    for(auto pokeRend: pokeVec)
    {

        window.draw(*(pokeRend.second));
    }

    sf::Font f;
    if(!f.loadFromFile("res/src/arial.ttf")){
        throw new runtime_error("cannot load font");
    }


    sf::View view2(sf::Vector2f(this->xCenter, this->yCenter), sf::Vector2f(200.f, 200.f));

    infoPlayers = "";
    int c = 0;
    for(auto player : engine->getState().getPlayers())
    {
        infoPlayers += "pokemon "+to_string(c)+" current life : "+
                       to_string(player.second->getPokemon()->getCurrentLife()) + "/"+ to_string(player.second->getPokemon()->getFullLife())+"\n";
        c++;
    }

    sf::Text text;
    text.setString(infoPlayers);
    text.setPosition(xCenter-90,yCenter-90);
    text.setFont(f);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setScale(sf::Vector2f(0.27,0.27));
    text.setStyle(sf::Text::Regular);


    sf::Text t2;
    t2.setString("Use: 'L' to change the level\n'A' to attack\n'Arrow Keys' to move\n'H' to heal");
    t2.setPosition(xCenter,yCenter+40);
    t2.setFont(f);
    t2.setFillColor(sf::Color::Red);
    t2.setCharacterSize(25);
    t2.setScale(sf::Vector2f(0.27,0.27));
    t2.setStyle(sf::Text::Bold);

    window.draw(text);
    window.draw(t2);
//        count--;
    window.setView(view2);
    window.display();
}

void Scene::updateMap() {
    layerVec.clear();

    if(!engine->getState().getMap())
    {
        throw new runtime_error("cannot render a state with no map");
    }
    uint tileWidth = engine ->getState().getMap()->getTileWidth();
    uint tileHeight = engine->getState().getMap()->getTileHeight();

//    this->xCenter = engine->getState().getPlayers().at(0)->getPokemon()->getPosition().x*tileWidth;
//    this->yCenter = engine->getState().getPlayers().at(0)->getPokemon()->getPosition().y*tileHeight;

    for(auto layer: *(engine->getState().getMap()->getLayers()))
    {
        shared_ptr<LayerRender> layerRend;
        layerRend.reset(new LayerRender());

        if(!(layerRend->load(this->tileset,sf::Vector2u(tileWidth,tileHeight),
                             layer.getData(),layer.getWidth(),layer.getHeight()))) throw  runtime_error("bad layer rendering");
        layerVec.push_back(layerRend);
    }
    string tileset2 = "res/src/tilestPokemon.png";
}

void movePokeRender(unsigned int pokeId)
{

}


void Scene::stateChanged(const state::Event& e) {
    const Event& event1 = e;

    if(e.getEventType()==TAB_EVENT) {
        cout << "tab event" << endl;
        state::TabEvent eventTab = *(TabEvent*)(event1.clone());
        unsigned int pokeId = engine->getState().getPlayers().at(eventTab.playerId)->getPokemon()->getID();
        switch(eventTab.tabEventId) {
            case MOVE:
                this->pokeVec[pokeId]->setPosition(engine->getState().getPlayers().at(eventTab.playerId)->getPokemon()->
                        getPosition().x*24,engine->getState().getPlayers().at(eventTab.playerId)->getPokemon()->getPosition().y*24);
                this->xCenter = engine->getState().getPlayers().at(pokeTarId)->getPokemon()->getPosition().x*tileSize.x;
                this->yCenter = engine->getState().getPlayers().at(pokeTarId)->getPokemon()->getPosition().y*tileSize.y;
                break;
            case DEATH:
                pokeVec.erase(pokeId);
                updatePlayers();
                break;
            case ORIENT:
                updatePlayers();
                break;
        }
    }
    else
    { StateEvent event = *(StateEvent*)(e.clone());
        switch(event.stateEvent)
        {
            case ATTACK:
                break;
            case StateEventId::LEVEL_CHANGE :
                updateMap();
                updatePlayers();
                break;
            case ALL_CHANGED:
                updateMap();
                updatePlayers();
                break;
            default:
                break;
        }
    }

}


void Scene::updatePlayers() {
    this->pokeVec.clear();
    if(engine->getState().getPlayers().size() <1){
        throw new runtime_error("cannot render a state with no players");
    }
    this->xCenter = engine->getState().getPlayers().at(pokeTarId)->getPokemon()->getPosition().x*tileSize.x;
    this->yCenter = engine->getState().getPlayers().at(pokeTarId)->getPokemon()->getPosition().y*tileSize.y;
    for( auto player :engine->getState().getPlayers())
    {
        shared_ptr<sf::Sprite> pokeRender;

        if(player.second->getPokemon()->getAlive()){
            pokeRender.reset(new sf::Sprite());
            pokeRender->setTexture(*(pokeTileSet.get()));
            std::shared_ptr<state::Pokemon> pokemon = player.second->getPokemon();
            unsigned int tileNumber = 0;
            switch (pokemon->getType()) {
                case PokeType ::BULBIZARRE://bulbizarre
                    tileNumber = 12;
                    break;
                case PokeType ::SALAMECHE://salameche
                    tileNumber = 33;
                    break;
                case CARAPUCE://carapuce
                    tileNumber = 54;
                    break;
                default:
                    throw runtime_error("pokemon rendering error");

            }
            Position position = pokemon->getPosition();
            tileNumber += pokemon->orientation;

            int tu = tileNumber % (pokeTileSet->getSize().x / tileSize.x);
            int tv = tileNumber / (pokeTileSet->getSize().x / tileSize.x);
            pokeRender->setTextureRect(sf::IntRect(tu * (tileSize.x+1)+1,tv * (tileSize.y+1)+1,tileSize.x,tileSize.y));
            pokeRender->setPosition(position.x* tileSize.x,position.y*tileSize.y);
            pokeVec.insert(make_pair(player.second->getPokemon()->getID(),pokeRender));
        }

    }
}
