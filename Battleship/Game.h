#pragma once

#include "Ship.h"
#include "Grid.h"

const int NUM_PLAYERS = 2;

class Game
{
public:
    void PlaceShip( int player, Ship* ship, int x, int y, Direction direction )
    {
        m_player_grids[player].PlaceShip(ship, x, y, direction);
    }

    void SwitchPlayer()
    {
        m_current_player = (m_current_player + 1) % NUM_PLAYERS;
    }

    bool Hit( int x, int y )
    {
        bool hit = OtherPlayerGrid().Hit( x, y );
        SwitchPlayer();
        return hit;
    }

    int Winner() const
    {
        for ( int i = 0; i < NUM_PLAYERS; ++i )
        {
            if ( !m_player_grids[i].HasUnsunkShip() )
            {
                return OtherPlayerNumber(i);
            }
        }
        return -1;
    }

private:
    int OtherPlayerNumber( int player ) const
    {
        return (player + 1) % NUM_PLAYERS;
    }

    Grid& OtherPlayerGrid()
    {
        return m_player_grids[(m_current_player + 1) % NUM_PLAYERS];
    }

    Grid m_player_grids[NUM_PLAYERS];
    int m_current_player = 0;
};
