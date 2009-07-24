#ifndef SPACESHIP_H 
#define SPACESHIP_H

//#pragma GCC visibility push(hidden)

#include <string>
#include <vector>


namespace scifi {
class SpaceShip {
public:
    SpaceShip(std::string const& name);
    ~SpaceShip();
    void stabliseIonFluxers();
    void initiateHyperwarp();
private:
    SpaceShip(const SpaceShip&);
    SpaceShip& operator= (const SpaceShip&);

    typedef unsigned int FluxLevel;
    typedef std::vector<FluxLevel> FluxLevels;

    void doSomethingInternal();
    FluxLevel checkFluxLevel(size_t ionFluxerIdx);

    std::string m_name;
    FluxLevels m_fluxLevels;
};

}

//#pragma GCC visibility pop

#endif

