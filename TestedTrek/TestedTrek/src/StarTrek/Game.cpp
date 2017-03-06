#include "stdafx.h"

#include <sstream>

#include "Game.h"
#include "Klingon.h"
#include "Galaxy.h"

namespace StarTrek {

Random Game::generator;

Game::Game() : e_(10000), t_(8) {
    Game::generator = &rand;
}

void Game::fireWeapon(Galaxy& galaxy) {
	if (galaxy.parameter("command") == "phaser") {
        firePhaser(galaxy);
	} else if (galaxy.parameter("command") == "photon") {
        firePhoton(galaxy);
	}
}

void Game::firePhaser(Galaxy& galaxy) {
		int amount = atoi(galaxy.parameter("amount").c_str());
		Klingon* enemy = (Klingon*)galaxy.variable("target");
		if (e_ >= amount) {
			int distance = enemy->distance();
			if (distance > 4000) {
				stringstream message;
				message << "Klingon out of range of phasers at " << distance << " sectors...";
				galaxy.writeLine(message.str());
			} else {
				int damage = amount - (((amount /20)* distance /200) + rnd(200));
				if (damage < 1)
					damage = 1;
				stringstream message;
				message << "Phasers hit Klingon at " << distance << " sectors with " << damage << " units";
				galaxy.writeLine(message.str());
				if (damage < enemy->energy()) {
					enemy->energy(enemy->energy() - damage);
					stringstream message;
					message << "Klingon has " << enemy->energy() << " remaining";
					galaxy.writeLine(message.str());
				} else {
					galaxy.writeLine("Klingon destroyed!");
					enemy->destroy();
				}
			}
			e_ -= amount;

		} else {
			galaxy.writeLine("Insufficient energy to fire phasers!");
		}
}

void Game::firePhoton(Galaxy& galaxy) {
    Klingon* enemy = (Klingon*)galaxy.variable("target");
    if (t_ > 0) {
        int distance = enemy->distance();
        if ((rnd(4) + ((distance / 500) + 1) > 7)) {
            stringstream message;
            message << "Torpedo missed Klingon at " << distance << " sectors...";
            galaxy.writeLine(message.str());
        } else {
            int damage = 800 + rnd(50);
            stringstream message;
            message << "Photons hit Klingon at " << distance << " sectors with " << damage << " units";
            galaxy.writeLine(message.str());

            if (damage < enemy->energy()) {
                enemy->energy(enemy->energy() - damage);
                stringstream message;
                message << "Klingon has " << enemy->energy() << " remaining";
                galaxy.writeLine(message.str());
            } else {
                galaxy.writeLine("Klingon destroyed!");
                enemy->destroy();
            }
        }
        t_--;

    } else {
        galaxy.writeLine("No more photon torpedoes!");
    }
}

void Game::fireWeapon(Untouchables::WebGadget* webGadget) {
    Galaxy galaxy(webGadget);
    fireWeapon(galaxy);
}

int Game::energyRemaining(void) {
    return e_;
}

int Game::torpedoes(void) {
    return t_;
}

void Game::torpedoes(int value) {
    t_ = value;
}

}
