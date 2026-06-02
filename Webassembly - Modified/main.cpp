#include "Game.h"
#include <emscripten.h>

// Define the static member variable here so the linker can find it
int Game::round = 0;

Game game;

extern "C" {
    // JavaScript will call this function and pass the ASCII code of the pressed key
    EMSCRIPTEN_KEEPALIVE
    void sendInputToGame(int asciiChar) {
        game.handleInput((char)asciiChar);
    }
}

int main() {
    game.initMenu();
    // main() finishes immediately, but Emscripten keeps the WASM module alive in the browser.
    return 0; 
}