#ifndef WEAPON_HEADER 
#define WEAPON_HEADER 

#include <cstdlib>


namespace StarTrek {


class Weapon {
private:
    std::string dealDamage(int damage, Klingon *enemy);

public:
    Weapon();

    virtual void Fire(Galaxy& galaxy) = 0;
};

class Phaser : public Weapon {
public:
    Phaser();

    virtual void Fire(Galaxy& galaxy);
};

}

#endif

