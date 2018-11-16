#include <utility>

//
// Created by paul on 13/11/18.
//
#include <memory>
#include "AstarComputer.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <iostream>
#include <stdlib.h>
using namespace std;
using namespace state;
using namespace ai;


shared_ptr<Node> AstarComputer::compute() {
    openList.push_back(source);
    int k = 0;

    while(!openList.empty())
    {
        shared_ptr<Node> n= openList.at(0);
//        shared_ptr<Node> n= nW.lock();
        closedList.push_back(n);
        openList.erase(openList.cbegin());
        openList.shrink_to_fit();
        cout << k << " iterations" << endl;

        cout << "position :"<<n->getPosition().x << ","<< n->getPosition().y << endl;
        if(n->getPosition().x == objectif->getPosition().x && n->getPosition().y == objectif->getPosition().y)
            return n;
        else {
            for( auto v : n->getAvailableNeigbors(35,map.get())){
                bool passInFor = true;
                v->heuristic = static_cast<unsigned int>(
                        abs((int)(objectif->getPosition().x) - (int)(v->getPosition().x)) +
                        abs((int)(objectif->getPosition().y)-(int)(v->getPosition().y)));
                //looking for v in closedList with heuristique < current heuristique
                for(auto ni:closedList)
                {
                    // if found don't insert in openList ...
                    if(ni->getPosition().x == v->getPosition().x && ni->getPosition().y == v->getPosition().y)
                    {
                        if(ni->heuristic>v->heuristic)
                        {
                            openList.push_back(v);
                            closedList.erase(std::find(closedList.begin(),closedList.end(),ni));
                            break;
                        }
                        passInFor = false;
                    }
                }
                // don't need to check if v is in openlist if v is already found in closedList with heuristique < current heuristique
                if(passInFor)
                {
                    // same than for closedList
                    for(auto ni: openList)
                    {
                        if(ni->getPosition().x == v->getPosition().x && ni->getPosition().y == v->getPosition().y)

                        {
                            if(ni->heuristic>v->heuristic)
                            {
                                ni->previousNode = n;
                                ni->heuristic = v->heuristic;
                                break;
                            }
                            passInFor = false;
                        }
                    }
                }
                if(passInFor) {
                    openList.push_back(v);
                    std::sort(openList.begin(),openList.end(),HCompare());
                }
                std::sort(openList.begin(),openList.end(),HCompare());
            }
            
        }
        k = k+1;


    }

    return nullptr;
}

AstarComputer::AstarComputer(shared_ptr<state::Map> map, Node &objectif, Node &source) {
    this->objectif = make_shared<Node>(objectif);
    this->source = make_shared<Node>(source);
    this->map = std::move(map);
}

