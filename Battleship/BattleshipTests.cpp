#include "gtest/gtest.h"

#include "Ship.h"
#include "Grid.h"
#include "Game.h"

TEST(ShipTests, IsNotSunk)
{
    Ship ship(2);
    EXPECT_FALSE(ship.IsSunk());
}

TEST(ShipTests, Sink)
{
    Ship ship(1);
    ship.Hit();
    EXPECT_TRUE(ship.IsSunk());
}

TEST(ShipTests, ShipSize)
{
    Ship ship(2);
    EXPECT_EQ(2, ship.Size());
}

TEST(GridTests, MissesWhenNoShipInCell)
{
    Grid grid;
    EXPECT_FALSE( grid.Hit( 0, 0 ) );
}

TEST(GridTests, MissesOutOfBounds)
{
    Grid grid;
    EXPECT_THROW(grid.Hit(-1, -1), OutOfBounds);
    EXPECT_THROW(grid.Hit(10, 10), OutOfBounds);
}

TEST(GridTests, CanHitShip)
{
    Grid grid;
    Ship ship(1);

    grid.PlaceShip( &ship, 0, 0 );
    EXPECT_TRUE( grid.Hit( 0, 0 ) );
    EXPECT_TRUE( ship.IsSunk() );
}

TEST(GridTests, PlaceShipOutOfBounds)
{
    Grid grid;
    Ship ship(1);
    EXPECT_THROW(grid.PlaceShip( &ship, 10, 10 ), OutOfBounds);
}

TEST(GridTests, PlaceShipHorizontal)
{
    Grid grid;
    Ship ship(2);
    grid.PlaceShip( &ship, 0, 0, HORIZONTAL );
    EXPECT_TRUE( grid.Hit( 0, 0 ) );
    EXPECT_TRUE( grid.Hit( 1, 0 ) );
    EXPECT_FALSE( grid.Hit( 2, 0 ) );
}

TEST(GridTests, PlaceShipVertical)
{
    Grid grid;
    Ship ship(2);
    grid.PlaceShip( &ship, 0, 0, VERTICAL );
    EXPECT_TRUE( grid.Hit( 0, 0 ) );
    EXPECT_TRUE( grid.Hit( 0, 1 ) );
    EXPECT_FALSE( grid.Hit( 0, 2 ) );
}

TEST(GridTests, HitShipSameSpot)
{
    Grid grid;
    Ship ship(2);

    grid.PlaceShip( &ship, 0, 0 );
    grid.Hit(0, 0);
    grid.Hit(0, 0);
    EXPECT_FALSE(ship.IsSunk());
}

TEST(GridTests, NotAllShipsSunk)
{
    Grid grid;
    Ship ship(1);

    grid.PlaceShip( &ship, 5, 5 );
    EXPECT_TRUE( grid.HasUnsunkShip() );
}

TEST(GridTests, AllShipsSunk)
{
    Grid grid;
    Ship ship(1);

    grid.PlaceShip( &ship, 5, 5 );
    grid.Hit( 5, 5 );
    EXPECT_FALSE( grid.HasUnsunkShip() );
}

TEST(GridTests, CannotPlaceBigShipOutOfBounds)
{
    Grid grid;
    Ship ship(5);
    EXPECT_THROW(grid.PlaceShip( &ship, 8, 8, HORIZONTAL ), OutOfBounds);
    EXPECT_FALSE(grid.Hit( 8, 8 ));
}

TEST(GridTests, CannotOverlapShips)
{
    Grid grid;

    Ship ship1(2);
    grid.PlaceShip(&ship1, 0, 0, HORIZONTAL);

    Ship ship2(2);
    EXPECT_THROW(grid.PlaceShip(&ship2, 1, 0, HORIZONTAL), OverlappingShips);
}

TEST(GameTests, PlayerOneSinksPlayerTwo)
{
    Game game;

    Ship player1Ship(1);
    game.PlaceShip(0, &player1Ship, 0, 0, HORIZONTAL);

    Ship player2Ship(1);
    game.PlaceShip(1, &player2Ship, 0, 0, HORIZONTAL);

    game.Hit(0, 0);
    EXPECT_TRUE(player2Ship.IsSunk());
    EXPECT_FALSE(player1Ship.IsSunk());
}

TEST(GameTests, PlayerTwoSinksPlayerOne)
{
    Game game;

    Ship player1Ship(1);
    game.PlaceShip(0, &player1Ship, 0, 0, HORIZONTAL);

    Ship player2Ship(1);
    game.PlaceShip(1, &player2Ship, 0, 0, HORIZONTAL);

    game.Hit(5, 5);
    game.Hit(0, 0);

    EXPECT_TRUE(player1Ship.IsSunk());
    EXPECT_FALSE(player2Ship.IsSunk());
}

TEST(GameTests, Player1Wins)
{
    Game game;

    Ship player1Ship(1);
    game.PlaceShip(0, &player1Ship, 0, 0, HORIZONTAL);

    Ship player2Ship(1);
    game.PlaceShip(1, &player2Ship, 0, 0, HORIZONTAL);

    game.Hit(0, 0);
    EXPECT_EQ(0, game.Winner());
}

TEST(GameTests, Player2Wins)
{
    Game game;

    Ship player1Ship(1);
    game.PlaceShip(0, &player1Ship, 0, 0, HORIZONTAL);

    Ship player2Ship(1);
    game.PlaceShip(1, &player2Ship, 0, 0, HORIZONTAL);

    game.Hit(5, 5);
    game.Hit(0, 0);
    EXPECT_EQ(1, game.Winner());
}

TEST(GameTests, NoWinner)
{
    Game game;

    Ship player1Ship(1);
    game.PlaceShip(0, &player1Ship, 0, 0, HORIZONTAL);

    Ship player2Ship(1);
    game.PlaceShip(1, &player2Ship, 0, 0, HORIZONTAL);

    EXPECT_EQ(-1, game.Winner());
}

TEST(GameTests, ComplicatedExample)
{
    Game game;

    Ship player1Ship1(2);
    Ship player1Ship2(4);

    Ship player2Ship1(3);
    Ship player2Ship2(1);

    game.PlaceShip(0, &player1Ship1, 5, 7, HORIZONTAL);
    game.PlaceShip(0, &player1Ship2, 2, 5, VERTICAL);

    game.PlaceShip(1, &player2Ship1, 2, 3, VERTICAL);
    game.PlaceShip(1, &player2Ship2, 7, 1, VERTICAL);

    EXPECT_FALSE(game.Hit(0, 0)); // player 1
    EXPECT_FALSE(game.Hit(1, 0)); // player 2
    EXPECT_TRUE(game.Hit(2, 4)); // player 1
    EXPECT_TRUE(game.Hit(6, 7)); // player 2
    EXPECT_TRUE(game.Hit(2, 5)); // player 1
    EXPECT_FALSE(game.Hit(0, 0)); // player 2
    EXPECT_TRUE(game.Hit(2, 3)); // player 1

    EXPECT_TRUE(player2Ship1.IsSunk());
    EXPECT_EQ(-1, game.Winner());

    EXPECT_FALSE(game.Hit(1, 1)); // player 2
    EXPECT_TRUE(game.Hit(7, 1)); // player 1
    EXPECT_EQ(0, game.Winner());
}

int main(int argc, char** argv)
{
    // run all tests
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
