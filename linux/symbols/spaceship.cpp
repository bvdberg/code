#include "spaceship.h"

using namespace scifi;
using namespace std;

SpaceShip::SpaceShip(string const& name)
    : m_name(name)
{}

SpaceShip::~SpaceShip() {}

void SpaceShip::stabliseIonFluxers() {}

void SpaceShip::initiateHyperwarp() {}

void SpaceShip::doSomethingInternal() {}

SpaceShip::FluxLevel SpaceShip::checkFluxLevel(size_t ionFluxerIdx) {
    return 0;
}

