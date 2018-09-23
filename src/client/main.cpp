#include <iostream>
#include <string.h>
// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>
#include <PixelsLoader.h>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"

using namespace std;
using namespace state;

int main(int argc,char* argv[]) 
{

    PixelsLoader *pixelsLoader = new PixelsLoader("res/map.json", "res/timemap.json","res/tilemap.png");
    if(!pixelsLoader->init()) {
        cout << "cannot parse metadata" << endl;
    }
    std::shared_ptr<std::vector<sf::Texture>> cachePtr = pixelsLoader->textureCache;
    delete pixelsLoader;
    // check the number of arguments
    if(argc != 1) {
        // check the 1st argument
        if(strcmp(argv[1],"hello")== 0) {
            cout << "bonjour les potes" << endl;
        }
        // display a warning when a wrong number of arguments is given
        else{
            cout << "Je ne comprends pas!" << endl;
        }
    }

    sf::RenderWindow window(sf::VideoMode(526,601),"test window");



    while (window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                cachePtr.reset();
                window.close();
                return 0;
            }
            window.clear();
            int x= 0;
            int y = 0;
            for (int i = 0; i<504;i++)
            {
                auto *sprite = new sf::Sprite();
                sprite->setTexture(cachePtr.get()->at(i));
                sprite->setPosition(x*24,y*24);
                window.draw(*sprite);
                x++;
                if (x > 20)
                {
                    y++;
                    x = 0;
                }
                delete sprite;

            }
            window.display();
        }
    }

    return 0;
}
