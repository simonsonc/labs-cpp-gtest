#pragma

#include "Grid.h"

class AiPlayer
{
public:
    void PlaceShips(Grid& grid, const std::vector<Ship*>& ships)
    {
        auto pos = CalculateShipPosition(grid, ships[0]);
        grid.PlaceShip(ships[0], pos.first, pos.second, HORIZONTAL );
    }

    virtual Coord CalculateShipPosition(Grid& grid, Ship* ship)
    {
        return {1, 1};
    }
};
