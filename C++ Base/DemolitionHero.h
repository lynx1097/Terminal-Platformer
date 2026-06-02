//
// Created by Abdul Rahman on 12/12/2023.
//
#include <iostream>
#include "Hero.h"
#include "Gun.h"
using namespace std;


#ifndef GAME_DEMOLITIONHERO_H
#define GAME_DEMOLITIONHERO_H
class DemolitionHero : public Hero {
private :
    Gun* weapon2  ;

public:
    DemolitionHero(char t, int x, int y, int hp) : Hero(t, x, y, hp), weapon2(nullptr) {};
    void setWeaponTwo(Gun* g) override{
        if (getWeapon()== nullptr)
            Hero::setWeapon(g);
        else
            weapon2 = g;
    }

    friend ostream& operator<< (ostream& , const DemolitionHero&);


    ostream& getInfo ( ostream& os) override{
        os << "Hero type : "<< type << endl << "Location : [ X : " << positionX << " , Y : " << positionY << " ]\nHP : " << healthPoints << endl << "Gun(s) : "<< endl << "   1] " << "(current) " <<  endl << *weapon;
        if (weapon2 != nullptr)
            os << "   2]" << *weapon2;
        os << endl;
        return os;
    }

    void useSpecial() override{
        if (current == getWeapon())
            current = weapon2;
        else
            current = getWeapon();
    }
    ~DemolitionHero(){
        if(weapon2!= nullptr)
            delete weapon2;
    }

};

ostream& operator<< (ostream& os , DemolitionHero& h){
    return h.getInfo(os);
};

#endif //GAME_DEMOLITIONHERO_H
