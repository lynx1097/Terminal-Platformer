//
// Created by Abdul Rahman on 12/12/2023.
//
#include <iostream>
#include "Gun.h"
using namespace std;

#ifndef GAME_RIFLE_H
#define GAME_RIFLE_H
class Rifle : public Gun {
public:
    Rifle() : Gun("Rifle", 30, 20, 4) {}
    void useSpecial ( Hero & h,Object* map[15][15])override{


    };


};
#endif //GAME_RIFLE_H
