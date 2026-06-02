//
// TUI helpers: ANSI colors, box-drawing, raw terminal input
//
#ifndef GAME_TUI_H
#define GAME_TUI_H

#include <iostream>
#include <string>
//#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <emscripten/emscripten.h>
using namespace std;

// ── Raw input ────────────────────────────────────────────────────────────────
/* inline char getch() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c = (char)getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
} */

// ── ANSI codes ────────────────────────────────────────────────────────────────
namespace ansi {
    inline string reset()               { return "\033[0m"; }
    inline string bold()                { return "\033[1m"; }
    inline string fg(int r,int g,int b) { return "\033[38;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m"; }
    inline string bg(int r,int g,int b) { return "\033[48;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m"; }
    inline string moveTo(int row,int col){ return "\033["+to_string(row)+";"+to_string(col)+"H"; }
    inline string clearScreen()        { return "\033[2J\033[H"; }
    inline string hideCursor()         { return "\033[?25l"; }
    inline string showCursor()         { return "\033[?25h"; }
    inline string enterAltScreen()     { return "\033[?1049h"; }
    inline string exitAltScreen()      { return "\033[?1049l"; }
}

// ── Colour palette ────────────────────────────────────────────────────────────
namespace col {
    const string RESET   = ansi::reset();
    const string BOLD    = ansi::bold();
    // Map cells
    const string EMPTY   = ansi::fg(50,50,60);           // dark grey floor
    const string HERO_D  = ansi::bold()+ansi::fg(80,200,255);  // demolition – cyan
    const string HERO_M  = ansi::bold()+ansi::fg(100,255,150); // medic – green
    const string TANK    = ansi::bold()+ansi::fg(255,80,80);   // tank – red
    const string GHOST   = ansi::bold()+ansi::fg(200,100,255); // ghost – purple
    const string POTION  = ansi::fg(255,180,50);               // potion – gold
    const string AMMO    = ansi::fg(200,200,80);               // ammo – yellow
    // UI chrome
    const string BORDER  = ansi::fg(80,120,180);
    const string LABEL   = ansi::fg(160,160,200);
    const string VALUE   = ansi::bold()+ansi::fg(230,230,255);
    const string HP_HIGH = ansi::fg(80,230,80);
    const string HP_MID  = ansi::fg(255,200,0);
    const string HP_LOW  = ansi::bold()+ansi::fg(255,60,60);
    const string HINT    = ansi::fg(100,100,130);
}

// ── Box-drawing helpers ───────────────────────────────────────────────────────
inline void printHLine(int width) {
    cout << col::BORDER;
    for(int i=0;i<width;i++) cout << "─";
    cout << col::RESET;
}

inline void printBox(int rows, int cols, int startRow=1, int startCol=1) {
    // top
    cout << ansi::moveTo(startRow, startCol) << col::BORDER << "╔";
    for(int j=0;j<cols;j++) cout << "══";
    cout << "╗" << col::RESET;
    // sides
    for(int i=1;i<=rows;i++){
        cout << ansi::moveTo(startRow+i, startCol) << col::BORDER << "║" << col::RESET;
        cout << ansi::moveTo(startRow+i, startCol+1+cols*2) << col::BORDER << "║" << col::RESET;
    }
    // bottom
    cout << ansi::moveTo(startRow+rows+1, startCol) << col::BORDER << "╚";
    for(int j=0;j<cols;j++) cout << "══";
    cout << "╝" << col::RESET;
}

// ── Cell rendering ────────────────────────────────────────────────────────────
inline string renderCell(char t, bool ghostVisible=true) {
    switch(t) {
        case 'D': return col::HERO_D + " @" + col::RESET;
        case 'M': return col::HERO_M + " @" + col::RESET;
        case 'T': return col::TANK   + " ▲" + col::RESET;
        case 'G': return ghostVisible ? col::GHOST + " ◆" + col::RESET
                                      : col::EMPTY + " ·" + col::RESET;
        case 'P': return col::POTION + " ♥" + col::RESET;
        case 'A': return col::AMMO   + " ⊕" + col::RESET;
        default:  return col::EMPTY  + " ·" + col::RESET;
    }
}

// ── Confetti win screen ───────────────────────────────────────────────────────
inline void confettiWin() {
    const char* pieces[] = {"★", "✦", "●", "▲", "♦", "✿", "❋", "◆"};
    const string colors[] = {
        ansi::fg(255,80,80),  ansi::fg(80,255,80),  ansi::fg(100,100,255),
        ansi::fg(255,230,0),  ansi::fg(255,80,200), ansi::fg(0,230,230),
        ansi::fg(255,140,0),  ansi::fg(160,255,80)
    };
    const int NUM_COLORS = 8;
    const int NUM_PIECES = 8;

    auto drawBanner = []() {
        cout << ansi::moveTo(9,  8) << ansi::bold() << ansi::fg(255,220,0)
             << " ██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗ ██████╗ ███╗   ██╗██╗" << ansi::reset()
             << ansi::moveTo(10, 8) << ansi::bold() << ansi::fg(255,200,0)
             << " ╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██╔═══██╗████╗  ██║██║" << ansi::reset()
             << ansi::moveTo(11, 8) << ansi::bold() << ansi::fg(255,180,0)
             << "  ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║   ██║██╔██╗ ██║██║" << ansi::reset()
             << ansi::moveTo(12, 8) << ansi::bold() << ansi::fg(255,160,0)
             << "   ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║   ██║██║╚██╗██║╚═╝" << ansi::reset()
             << ansi::moveTo(13, 8) << ansi::bold() << ansi::fg(255,140,0)
             << "    ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝╚██████╔╝██║ ╚████║██╗" << ansi::reset()
             << ansi::moveTo(14, 8) << ansi::bold() << ansi::fg(255,120,0)
             << "    ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═══╝╚═╝" << ansi::reset()
             << ansi::moveTo(16, 20) << ansi::bold() << ansi::fg(200,255,200)
             << "  ✦  All monsters defeated — you are victorious!  ✦" << ansi::reset()
             << ansi::moveTo(18, 30) << ansi::fg(120,120,150)
             << "Press any key to exit..." << ansi::reset();
    };

    // Animate confetti for ~2 seconds (16 frames × 125ms)
    for (int f = 0; f < 16; f++) {
        cout << ansi::clearScreen() << ansi::hideCursor();
        for (int k = 0; k < 60; k++) {
            int row = rand() % 22 + 1;
            int col = rand() % 76 + 1;
            cout << ansi::moveTo(row, col)
                 << colors[rand() % NUM_COLORS]
                 << pieces[rand() % NUM_PIECES]
                 << ansi::reset();
        }
        drawBanner();
        cout.flush();
        //usleep(125000);
        emscripten_sleep(125);
    }

    // Final static frame — confetti stays, wait for key
    //getch();
}

// ── HP bar ────────────────────────────────────────────────────────────────────
inline string hpBar(int hp, int maxHp=100, int barLen=20) {
    int filled = (hp * barLen) / maxHp;
    if(filled < 0) filled = 0;
    if(filled > barLen) filled = barLen;
    string color = (hp > 60) ? col::HP_HIGH : (hp > 30) ? col::HP_MID : col::HP_LOW;
    string bar = color + "[";
    for(int i=0;i<barLen;i++) bar += (i<filled) ? "█" : "░";
    bar += "] " + to_string(hp) + "/" + to_string(maxHp) + col::RESET;
    return bar;
}

#endif //GAME_TUI_H
