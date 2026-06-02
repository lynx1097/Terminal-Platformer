//
// Created by Abdul Rahman on 12/12/2023.
//
#include <iostream>
#include "Gun.h"
using namespace std;
#ifndef GAME_PISTOL_H
#define GAME_PISTOL_H
// Gun subclasses

class Pistol : public Gun {
public:
    Pistol() : Gun("Pistol", 16, 30, 2) {}

    void useSpecial (){

    };
};
#endif //GAME_PISTOL_H
