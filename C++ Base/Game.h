//
// Created by Abdul Rahman on 12/12/2023.
//
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "TUI.h"
#include "Object.h"
#include "Potion.h"
#include "Ammo.h"
#include "TankMonster.h"
#include "GhostMonster.h"
#include "Pistol.h"
#include "DemolitionHero.h"
#include "Bazooka.h"
#include "Rifle.h"
#include "MedicHero.h"

using namespace std;

#ifndef GAME_GAME_H
#define GAME_GAME_H

struct Location {
    int posX, posY;
    Location() : posX(0), posY(0) {}
    Location(int x, int y) : posX(x), posY(y) {}
};

class Game {
private:
    static const int rows = 15;
    static const int cols = 15;
    Hero* hero;
    string statusMsg;

public:
    static int round;
    Object* grid[rows][cols];

    Game() {
        hero = nullptr;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                grid[i][j] = new Object('.');
    }

    ~Game() {
        if (hero != nullptr)
            grid[hero->getPositionX()][hero->getPositionY()] = nullptr;
        delete hero;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                delete grid[i][j];
    }

    // ── Core logic ──────────────────────────────────────────────────────────

    Location checkLocation(Location oldLoc, Location newLoc) {
        int x = newLoc.posX, y = newLoc.posY;
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            char t = grid[x][y]->getType();
            if (t == '.') return newLoc;
            if (t == 'T' || t == 'G') {
                *hero - (*(Monster*)grid[x][y]);
                statusMsg = "Combat! You hit the monster.";
                return oldLoc;
            }
            if (t == 'P') { *hero + (*(Potion*)grid[x][y]);  statusMsg = "Picked up a Potion!"; }
            if (t == 'A') { *hero + (*(Ammo*)grid[x][y]);    statusMsg = "Picked up Ammo!"; }
            return newLoc;
        }
        statusMsg = "Out of bounds!";
        return oldLoc;
    }

    void move(Hero& h, char direction) {
        Location heroLoc(h.getPositionX(), h.getPositionY());
        Location moveLoc = heroLoc;
        switch (direction) {
            case 'w': case 'W': moveLoc = checkLocation(heroLoc, Location(heroLoc.posX-1, heroLoc.posY)); break;
            case 'a': case 'A': moveLoc = checkLocation(heroLoc, Location(heroLoc.posX, heroLoc.posY-1)); break;
            case 's': case 'S': moveLoc = checkLocation(heroLoc, Location(heroLoc.posX+1, heroLoc.posY)); break;
            case 'd': case 'D': moveLoc = checkLocation(heroLoc, Location(heroLoc.posX, heroLoc.posY+1)); break;
            default: break;
        }
        if (moveLoc.posX == heroLoc.posX && moveLoc.posY == heroLoc.posY) return;
        delete grid[moveLoc.posX][moveLoc.posY];
        grid[heroLoc.posX][heroLoc.posY] = new Object('.');
        grid[moveLoc.posX][moveLoc.posY] = &h;
        h.setPosition(moveLoc.posX, moveLoc.posY);
    }

    void generateMap() {
        srand(time(0));
        for (int i = 0; i < 5; i++) {
            int ha = rand() % 41 + 10;
            int rx = rand() % rows, ry = rand() % cols;
            delete grid[rx][ry]; grid[rx][ry] = new Potion(ha);
        }
        for (int i = 0; i < 10; i++) {
            int b = rand() % 16 + 5;
            int rx = rand() % rows, ry = rand() % cols;
            delete grid[rx][ry]; grid[rx][ry] = new Ammo(b);
        }
        for (int j = 0; j < 5; j++) {
            int hp = (rand() % 4 + 1) * 25;          // 25–100 HP
            int shield = hp / 2;                       // shield = half HP
            int rx = rand() % rows, ry = rand() % cols;
            delete grid[rx][ry]; grid[rx][ry] = new TankMonster('T', 0, 0, hp, shield);
        }
        for (int j = 0; j < 5; j++) {
            int hp = (rand() % 4 + 1) * 25;          // 25–100 HP
            int rx = rand() % rows, ry = rand() % cols;
            delete grid[rx][ry]; grid[rx][ry] = new GhostMonster('G', 0, 0, hp);
        }
    }

    int checkGameOver() {
        if (hero->getHealthPoints() <= 0) return 0;
        int monsters = 0;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                char t = grid[i][j]->getType();
                if ((t == 'T' || t == 'G') && ((Monster*)grid[i][j])->getHealthPoints() > 0)
                    monsters++;
            }
        if (monsters == 0) return 1;
        return -1;
    }

    virtual void fireGun(const char direction) {
        Gun* gun = hero->getCurrentWeapon();
        if (!gun || gun->getBullets() <= 0) {
            statusMsg = "Out of ammo!";
            return;
        }
        // Spend one bullet per shot (not per cell traversed)
        gun->setBullets(gun->getBullets() - 1);
        int i = hero->getPositionX(), j = hero->getPositionY();
        int range = gun->getRange();
        bool hit = false;
        switch (direction) {
            case 'w': case 'W': for(i--;i>=0  && range>0;i--,range--) { if(gun->fireGun(grid[i][j])){hit=true;break;} } break;
            case 's': case 'S': for(i++;i<15  && range>0;i++,range--) { if(gun->fireGun(grid[i][j])){hit=true;break;} } break;
            case 'a': case 'A': for(j--;j>=0  && range>0;j--,range--) { if(gun->fireGun(grid[i][j])){hit=true;break;} } break;
            case 'd': case 'D': for(j++;j<15  && range>0;j++,range--) { if(gun->fireGun(grid[i][j])){hit=true;break;} } break;
            default: break;
        }
        statusMsg = hit ? "Direct hit!" : "Fired — no target in range.";
    }

    void useGunSpecial() {
        string curr = hero->getCurrentWeapon()->getName();
        int b = hero->getCurrentWeapon()->getBullets();
        if ((curr == "Pistol" && b < 4) || (curr == "Bazooka" && b < 3)) {
            statusMsg = "Insufficient ammo for special!";
            return;
        }
        hero->getCurrentWeapon()->useSpecial(*hero, grid);
        statusMsg = "Gun special activated!";
    }

    // ── TUI rendering ────────────────────────────────────────────────────────

    void renderMap() {
        // Update ghost visibility and cull dead monsters
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char t = grid[i][j]->getType();
                if ((t == 'T' || t == 'G') && ((Monster*)grid[i][j])->getHealthPoints() <= 0) {
                    delete grid[i][j];
                    grid[i][j] = new Object('.');
                    t = '.';
                }
                if (t == 'G') {
                    GhostMonster* gm = (GhostMonster*)grid[i][j];
                    gm->setVisible(round % 3 != 0);
                }
            }
        }

        // ── title bar ──
        cout << col::BORDER << col::BOLD;
        cout << "  ╔";
        for(int j=0;j<cols;j++) cout << "═══";
        cout << "╗" << col::RESET << "\n";

        // ── map rows ──
        for (int i = 0; i < rows; i++) {
            cout << col::BORDER << "  ║" << col::RESET;
            for (int j = 0; j < cols; j++) {
                char t = grid[i][j]->getType();
                bool vis = true;
                if (t == 'G') vis = ((GhostMonster*)grid[i][j])->isVisibleOnMap();
                cout << renderCell(t, vis);
            }
            cout << col::BORDER << "║" << col::RESET << "\n";
        }

        // ── bottom border ──
        cout << col::BORDER << col::BOLD;
        cout << "  ╚";
        for(int j=0;j<cols;j++) cout << "═══";
        cout << "╝" << col::RESET << "\n\n";
    }

    void renderStatus() {
        Gun* w = hero->getCurrentWeapon();
        string heroType = (hero->getType() == 'D') ? "Demolition" : "Medic";
        int hp = hero->getHealthPoints();

        cout << col::LABEL << "  Hero  " << col::VALUE << heroType
             << col::LABEL << "   Pos " << col::VALUE
             << "(" << hero->getPositionX() << "," << hero->getPositionY() << ")"
             << col::LABEL << "   Round " << col::VALUE << round << col::RESET << "\n";

        cout << "  " << col::LABEL << "HP   " << hpBar(hp) << "\n";

        if (w) {
            int b = w->getBullets();
            string ammoCol = (b > 5) ? col::VALUE : (b > 0) ? col::HP_MID : col::HP_LOW;
            cout << "  " << col::LABEL << "Gun  " << col::VALUE << w->getName()
                 << col::LABEL << "  dmg " << col::VALUE << w->getDamage()
                 << col::LABEL << "  range " << col::VALUE << w->getRange()
                 << col::LABEL << "  ammo " << ammoCol << b << col::RESET << "\n";
        }

        if (!statusMsg.empty()) {
            cout << "\n  " << col::HINT << "» " << statusMsg << col::RESET << "\n";
            statusMsg.clear();
        }

        cout << "\n  " << col::HINT
             << "[WASD] Move  [F+WASD] Fire  [X] Special  [G] Gun Special  [Q] Quit"
             << col::RESET << "\n\n";
    }

    void updateMap() {
        static int stepCount = 0;
        bool newRound = (++stepCount % 5 == 0);
        if (newRound) round++;
        cout << ansi::clearScreen() << ansi::hideCursor();

        int state = checkGameOver();
        if (state == 0) {
            cout << "\n\n  " << col::HP_LOW << col::BOLD
                 << "  ██████╗  █████╗ ███╗   ███╗███████╗ ██████╗ ██╗   ██╗███████╗██████╗ \n"
                 << "  ██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔═══██╗██║   ██║██╔════╝██╔══██╗\n"
                 << "  ██║  ███╗███████║██╔████╔██║█████╗  ██║   ██║██║   ██║█████╗  ██████╔╝\n"
                 << "  ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n"
                 << "  ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n"
                 << "   ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\n"
                 << col::RESET << "\n  Your hero has fallen. Better luck next time.\n\n"
                 << ansi::fg(120,120,150) << "  Press any key to exit..." << ansi::reset();
            cout.flush();
            getch();
            cout << ansi::showCursor() << ansi::exitAltScreen();
            exit(0);
        }
        if (state == 1) {
            confettiWin();
            cout << ansi::showCursor() << ansi::exitAltScreen();
            exit(0);
        }

        // Spawn new monsters once at the start of every 20th round
        if (newRound && round > 0 && round % 20 == 0) {
            int hp  = (rand() % 4 + 1) * 25;         // 25–100 HP
            int hp2 = (rand() % 4 + 1) * 25;
            int x, y;
            do { x = rand()%rows; y = rand()%cols; } while(grid[x][y]->getType()!='.');
            delete grid[x][y]; grid[x][y] = new TankMonster('T',0,0,hp,hp/2);
            do { x = rand()%rows; y = rand()%cols; } while(grid[x][y]->getType()!='.');
            delete grid[x][y]; grid[x][y] = new GhostMonster('G',0,0,hp2);
        }

        renderMap();
        renderStatus();
    }

    // ── Start screen ─────────────────────────────────────────────────────────

    void startGame() {
        cout << ansi::enterAltScreen() << ansi::clearScreen() << ansi::hideCursor();
        cout << col::BORDER << col::BOLD
             << "\n  ╔══════════════════════════════════╗\n"
             << "  ║   D E M O L I T I O N  R U N    ║\n"
             << "  ╚══════════════════════════════════╝\n"
             << col::RESET << "\n";

        cout << ansi::showCursor();

        // Hero selection
        int heroType = 0;
        while (heroType != 1 && heroType != 2) {
            cout << col::LABEL << "  Choose hero  " << col::VALUE << "[1] Demolition  [2] Medic" << col::RESET << "  > ";
            cin >> heroType;
        }

        int gunType = 0;
        while (gunType < 1 || gunType > 3) {
            cout << col::LABEL << "  Choose weapon " << col::VALUE << "[1] Pistol  [2] Bazooka  [3] Rifle" << col::RESET << "  > ";
            cin >> gunType;
        }

        Gun* gun1 = nullptr;
        Gun* gun2 = nullptr;

        if (heroType == 1) {
            hero = new DemolitionHero('D', 0, 0, 100);
            int gun2type = 0;
            while (gun2type < 1 || gun2type > 3) {
                cout << col::LABEL << "  Second weapon " << col::VALUE << "[1] Pistol  [2] Bazooka  [3] Rifle" << col::RESET << "  > ";
                cin >> gun2type;
            }
            if (gun2type == 1) gun2 = new Pistol();
            else if (gun2type == 2) gun2 = new Bazooka();
            else gun2 = new Rifle();
        } else {
            hero = new MedicHero('M', 0, 0, 100, 50);
        }

        if (gunType == 1) gun1 = new Pistol();
        else if (gunType == 2) gun1 = new Bazooka();
        else gun1 = new Rifle();

        hero->setWeapon(gun1);
        hero->setWeaponTwo(gun2);

        generateMap();
        grid[0][0] = hero;
        updateMap();

        // ── Main game loop (no Enter needed) ──
        bool firingMode = false;
        while (checkGameOver() == -1) {
            char input = getch();
            if (firingMode) {
                firingMode = false;
                switch(input) {
                    case 'w': case 'W': case 'a': case 'A':
                    case 's': case 'S': case 'd': case 'D':
                        fireGun(input); updateMap(); break;
                    default:
                        statusMsg = "Firing cancelled."; updateMap(); break;
                }
                continue;
            }
            switch (input) {
                case 'w': case 'W': case 'a': case 'A':
                case 's': case 'S': case 'd': case 'D':
                    move(*hero, input); updateMap(); break;
                case 'q': case 'Q':
                    cout << ansi::showCursor() << ansi::exitAltScreen(); exit(0);
                case 'f': case 'F':
                    firingMode = true;
                    statusMsg = "FIRE > press WASD to aim...";
                    updateMap();
                    break;
                case 'x': case 'X':
                    hero->useSpecial(); updateMap(); break;
                case 'g': case 'G':
                    useGunSpecial(); updateMap(); break;
                default:
                    break;
            }
        }
        cout << ansi::showCursor();
    }
};

#endif //GAME_GAME_H
