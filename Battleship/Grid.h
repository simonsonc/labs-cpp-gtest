#pragma once

#include <vector>

#include "Ship.h"

using namespace std;

const int GRID_SIZE = 10;

class OutOfBounds {};
class OverlappingShips {};


typedef struct Cell
{
    Ship *m_ship = nullptr;
    bool m_hit = false;
} Cell;

enum Direction
{
    HORIZONTAL,
    VERTICAL
};

class Grid
{
public:
    bool Hit( int x, int y )
    {
        CheckBounds( x, y );
        Cell& cell = m_grid[x][y];
        if ( cell.m_ship != nullptr && !cell.m_hit )
        {
            cell.m_ship->Hit();
            cell.m_hit = true;
        }
        return cell.m_ship != nullptr;
    }

    void PlaceShip( Ship *ship, int initial_x, int initial_y, Direction direction = HORIZONTAL )
    {
        CheckBounds( initial_x, initial_y );
        auto cells = GetCells( ship->Size(), initial_x, initial_y, direction );
        for ( auto coords : cells )
        {
            CheckBounds( coords.first, coords.second );
            if ( m_grid[coords.first][coords.second].m_ship )
            {
                throw OverlappingShips();
            }
        }
        for ( auto coords : cells )
        {
            m_grid[coords.first][coords.second].m_ship = ship;
        }
    }

    bool HasUnsunkShip() const
    {
        for ( int x = 0; x < GRID_SIZE; ++x )
        {
            for ( int y = 0; y < GRID_SIZE; ++y )
            {
                if ( m_grid[x][y].m_ship )
                {
                    if ( !m_grid[x][y].m_ship->IsSunk() )
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

private:
    vector<pair<int, int>> GetCells( int size, int initial_x, int initial_y, Direction direction )
    {
        vector<pair<int, int>> ret;
        if ( direction == HORIZONTAL )
        {
            for ( int x = initial_x; x < initial_x + size; ++x )
            {
                ret.push_back({x, initial_y});
            }
        }
        else
        {
            for ( int y = initial_y; y < initial_y + size; ++y )
            {
                ret.push_back({initial_x, y});
            }
        }
        return ret;
    }

    void CheckBounds( int x, int y )
    {
        if ( x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE )
        {
            throw OutOfBounds();
        }
    }

    Cell m_grid[GRID_SIZE][GRID_SIZE] = {};
};
