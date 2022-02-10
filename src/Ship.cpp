//------------------------------------------------------------------------------
// Ship.cpp
//
// Class for ships
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//------------------------------------------------------------------------------
//

#include "../include/Ship.hpp"

#include <iostream>

enum ShipInfo
{
  ONE_SPACE = 1,
  TWO_SPACES = 2,
  THREE_SPACES = 3,
  FOUR_SPACES = 4,
  SHIP_WITH_ID_0 = 0,
  SHIP_WITH_ID_1 = 1,
  SHIP_WITH_ID_2 = 2,
  SHIP_WITH_ID_3 = 3,
  AMOUNT_OF_SHIPS = 4
};

enum StoneInfo
{
  STONE_ID_0 = 0,
  NO_STONE_ON_PLACE = -1,
  ONE_STONE = 1,
  TWO_STONES = 2,
  THREE_STONES = 3
};

//-----------------------------------------------------------------------------
Ship::Ship(int id, int length)
{
  id_ = id;
  length_ = length;

  for (int iterator = STONE_ID_0;
       iterator < length;
       iterator++)
  {
    spaces_.push_back(NO_STONE_ON_PLACE);
  }

  is_sailed_ = false;
  switch (length)
  {
    case ONE_SPACE:
      minimun_load_ = ONE_STONE;
      break;
    case TWO_SPACES:
      minimun_load_ = ONE_STONE;
      break;
    case THREE_SPACES:
      minimun_load_ = TWO_STONES;
      break;
    case FOUR_SPACES:
      minimun_load_ = THREE_STONES;
      break;
  }
}

void Ship::generateAllShips(std::vector<Ship *> &all_ships, int ship_length_0, int ship_length_1,
                            int ship_length_2, int ship_length_3)
{
  Ship *ship_1 = new Ship(SHIP_WITH_ID_3, ship_length_0);
  Ship *ship_2 = new Ship(SHIP_WITH_ID_2, ship_length_1);
  Ship *ship_3 = new Ship(SHIP_WITH_ID_1, ship_length_2);
  Ship *ship_4 = new Ship(SHIP_WITH_ID_0, ship_length_3);

  // if vector hasn't been constructed yet
  if (all_ships.size() < AMOUNT_OF_SHIPS)
  {
    all_ships.push_back(ship_4);
    all_ships.push_back(ship_3);
    all_ships.push_back(ship_2);
    all_ships.push_back(ship_1);
  }
  else
  {
    for (Ship *ship : all_ships)
    {
      delete ship;
    }
    all_ships[SHIP_WITH_ID_0] = ship_4;
    all_ships[SHIP_WITH_ID_1] = ship_3;
    all_ships[SHIP_WITH_ID_2] = ship_2;
    all_ships[SHIP_WITH_ID_3] = ship_1;
  }


}
