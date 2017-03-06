#ifndef GAME_HEADER 
#define GAME_HEADER 

#include <cstdlib>
#include <Untouchables/WebGadget.h>


namespace StarTrek {

class Galaxy;

typedef int(*Random)(void);


class Game {
private:
	int e_;
	int t_;

	static int rnd(int maximum) {
		return generator() % maximum; 
	}

public:
    Game();
    void fireWeapon(Untouchables::WebGadget* wg);
    void fireWeapon(Galaxy& galaxy);
    void firePhoton(Galaxy& galaxy);
    void firePhaser(Galaxy& galaxy);
    int energyRemaining(void);
    void torpedoes(int value);
    int torpedoes(void);

    static Random generator;
};

}

#endif
