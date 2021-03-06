#include "stdafx.h"

#include <sstream>

#include "Game.h"
#include "Klingon.h"
#include "Galaxy.h"

namespace StarTrek {

Random Game::generator;

Game::Game() : m_phaser_energy(10000), m_photon_torpedoes(8) {
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
    if (m_phaser_energy >= amount) {
        int distance = enemy->distance();
        if (!phaserHit(distance)) {
            stringstream message;
            message << "Klingon out of range of phasers at " << distance << " sectors...";
            galaxy.writeLine(message.str());
        } else {
            int damage = phaserDamage(amount, distance);
            stringstream message;
            message << "Phasers hit Klingon at " << distance << " sectors with " << damage << " units";
            galaxy.writeLine(message.str());
            std::string damage_msg = dealDamage(damage, enemy);
            galaxy.writeLine(damage_msg);
        }
        m_phaser_energy -= amount;

    } else {
        galaxy.writeLine("Insufficient energy to fire phasers!");
    }
}

void Game::firePhoton(Galaxy& galaxy) {
    Klingon* enemy = (Klingon*)galaxy.variable("target");
    if (m_photon_torpedoes > 0) {
        int distance = enemy->distance();
        if (!torpedoHit(distance)) {
            stringstream message;
            message << "Torpedo missed Klingon at " << distance << " sectors...";
            galaxy.writeLine(message.str());
        } else {
            int damage = torpedoDamage();
            stringstream message;
            message << "Photons hit Klingon at " << distance << " sectors with " << damage << " units";
            galaxy.writeLine(message.str());
            std::string damage_msg = dealDamage(damage, enemy);
            galaxy.writeLine(damage_msg);
        }
        m_photon_torpedoes--;

    } else {
        galaxy.writeLine("No more photon torpedoes!");
    }
}

void Game::fireWeapon(Untouchables::WebGadget* webGadget) {
    Galaxy galaxy(webGadget);
    fireWeapon(galaxy);
}

bool Game::torpedoHit( int distance )
{
    return !((rnd(4) + ((distance / 500) + 1) > 7));
}

bool Game::phaserHit( int distance )
{
    return (distance <= 4000);
}

int Game::phaserDamage( int amount, int distance )
{
    int damage = amount - (((amount /20)* distance /200) + rnd(200));
    if (damage < 1)
        damage = 1;
    return damage;
}

int Game::torpedoDamage()
{
    const int base_damage = 800;

    return base_damage + rnd(50);
}

std::string Game::dealDamage(int damage, Klingon *enemy)
{
    if (damage < enemy->energy()) {
        enemy->energy(enemy->energy() - damage);
        stringstream message;
        message << "Klingon has " << enemy->energy() << " remaining";
        return message.str();
    } else {
        enemy->destroy();
        return "Klingon destroyed!";
    }
}

int Game::energyRemaining(void) {
    return m_phaser_energy;
}

int Game::torpedoes(void) {
    return m_photon_torpedoes;
}

void Game::torpedoes(int value) {
    m_photon_torpedoes = value;
}

}
