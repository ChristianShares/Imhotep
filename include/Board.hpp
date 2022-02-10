//-----------------------------------------------------------------------------
// Board.hpp
//
// Header-Class for the Game Board
//
// Group: 03 (formerly 14.04)
//
// Author: Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_BOARD_HPP
#define IMHOTEP_SERVER_BOARD_HPP

#include "Player.hpp"
#include "PlayerMessage.hpp"
#include "Server.hpp"
#include "Ship.hpp"
#include "StorageArea.hpp"
#include "ServerMessages.hpp"

class Board
{
private:

public:
  std::vector<Player *> all_players_;
  std::vector<Ship *> all_ships_;
  std::vector<StorageArea *> all_storage_areas_;
  std::vector<int> all_market_cards_;

  //-------------------------------------------------------------------------
  ///
  /// Constructor
  ///
  /// @param number_players the amount of players
  Board(int number_of_players);

};

#endif
