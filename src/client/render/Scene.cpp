#include <utility>

//
// Created by paul on 22/10/18.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "Scene.h"
#include "CustomException.h"
#include "render.h"
#include <iostream>
#include <string>
using namespace std;
using namespace render;

Scene::Scene(shared_ptr<state::State> state1,string tileSet) {

    //good dimensions : 620 x 620
    sf::RenderWindow window(sf::VideoMode(620,620),"test window");
    vector<shared_ptr<LayerRender>> layerVec;
    this->state = std::move(state1);
    if(state->getPlayers()->size() <=0){
        throw new runtime_error("cannot render a state with no players");
    }
    if(!state->getMap())
    {
        throw new runtime_error("cannot render a state with no map");
    }
    uint tileWidth = this->state->getMap()->getTileWidth();
    uint tileHeight = this->state->getMap()->getTileHeight();

    float x = state->center.x*tileWidth;
    float y = state->center.y*tileHeight;
    for(auto layer: *(this->state->getMap()->getLayers()))
    {
        shared_ptr<LayerRender> layerRend;
        layerRend.reset(new LayerRender());

        if(!(layerRend->load(tileSet,sf::Vector2u(tileWidth,tileHeight),
                layer.getData(),layer.getWidth(),layer.getHeight()))) throw  runtime_error("bad layer rendering");
        layerVec.push_back(layerRend);
    }
    string tileset2 = "res/src/tilestPokemon.png";
    vector<shared_ptr<PokeRender>> layerPoke;
    for( auto player : *(this->state->getPlayers()))
    {
        shared_ptr<PokeRender> pokeRender;
        pokeRender.reset(new PokeRender());
        if(player->getPokemon()){
            state::Pokemon pokemon = *(player->getPokemon());
            if(!(pokeRender->load(tileset2,sf::Vector2u(tileWidth,tileHeight),pokemon))) throw runtime_error("bad pokemon rendering");
            layerPoke.push_back(pokeRender);
        }

    }

    while(window.isOpen()){
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(auto layerRend : layerVec)
        {
            window.draw(*layerRend);
        }
        for(auto pokeRend: layerPoke)
        {

            window.draw(*pokeRend);
        }

        sf::Font font;
        font.loadFromFile("res/src/arial.ttf");

        sf::Text text;
        text.setFont(font);
        text.setString("close the window to \n display an other state");
        // choix de la taille des caractères
        text.setCharacterSize(10); // exprimée en pixels, pas en points !

// choix de la couleur du texte
        text.setColor(sf::Color::Black);

// choix du style du texte
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        text.setPosition(sf::Vector2f(x-5, y));

        sf::View view2(sf::Vector2f(x, y), sf::Vector2f(200.f, 200.f));
        window.draw(text);

        window.setView(view2);
        window.display();
    }
}