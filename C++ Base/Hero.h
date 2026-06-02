//
// Created by Abdul Rahman on 12/12/2023.
//
#ifndef GAME_HERO_H
#define GAME_HERO_H
#include <iostream>
#include "Gun.h"
#include "Character.h"
#include "Ammo.h"
#include "Potion.h"
#include "Monster.h"
#include "TankMonster.h"

using namespace std;

// Hero class
class Hero : public Character {

protected:
    Gun* current;
    Gun* weapon;

public:
    Hero(char t, int x, int y, int hp) : Character(t, x, y, hp) {
        current = nullptr;
        weapon = nullptr;
    }
    friend ostream& operator<< (ostream& os , Hero& h){
        return h.getInfo(os);
    };

    Gun* getWeapon()  {
        return weapon;
    }
    Gun* getCurrentWeapon(){
        return current;
    }
    void setWeapon(Gun* g){
        weapon = g;
        current =g;
    }

    friend void operator+ ( Hero& h , const Ammo& a){
        h.current->setBullets(h.current->getBullets()+a.getBullets());
    }
    friend void operator+ (  Hero& h ,const  Potion& p){
        h.setHealthPoints(h.getHealthPoints()+p.getHealingAmount());
    }
    friend void operator- ( Hero& h , Monster& m){
        // Hero takes 10 melee damage from any monster
        h.setHealthPoints(h.getHealthPoints()-10);
        // Weapon damage reduced by TankMonster shield
        int dmg = (h.current != nullptr) ? h.current->getDamage() : 20;
        if (m.getType() == 'T') {
            TankMonster& tank = (TankMonster&)m;
            int absorbed = min(dmg, tank.getShield());
            tank.setShield(tank.getShield() - absorbed);
            dmg -= absorbed;
        }
        m.setHealthPoints(m.getHealthPoints()-dmg);
    }

    virtual ostream& getInfo( ostream& os  ){return os;};

    virtual ~Hero(){
        if(weapon != nullptr)
            delete weapon;
        if(current != nullptr && current != weapon)
            delete current;
    }





    virtual void setWeaponTwo(Gun* g){};

    virtual void useSpecial (){};
};


#endif //GAME_HERO_H
