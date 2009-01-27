#include "spaceship.h"

int main() {
    scifi::SpaceShip* ship = new scifi::SpaceShip("Beagle");

    ship->stabliseIonFluxers();
    ship->initiateHyperwarp();

    delete ship;

    return 0;
}

