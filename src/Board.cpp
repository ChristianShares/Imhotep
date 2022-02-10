//-----------------------------------------------------------------------------
// Board.cpp
//
// Cpp-Class for the game board
//
// Group: 03 (formerly 14.04)
//
// Author:  Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#include "../include/Board.hpp"

int const PLAYER_ID_0 = 0;

enum SiteInfo
{
  TOTAL_AMOUNT_OF_STORAGE_AREAS = 5,
  MINIMUM_START_STONE_COUNT = 2,
  SITE_ID_0 = 0,
  MAX_AMOUNT_OF_MARKET_CARDS = 25,
  MARKET_CARD_UNINITIALIZED = -1,
  FIRST_MARKET_CARD_POSITION = 0,
  MARKET_CARD_SIZE_POSITION = 0,
  INITIAL_MARKET_CARD_SIZE = 0
};


Board::Board(int number_of_players)
{
  // create all_storage_areas_
  for (int site_id = SITE_ID_0;
       site_id <= TOTAL_AMOUNT_OF_STORAGE_AREAS;
       site_id++)
  {
    auto *area = new StorageArea(site_id,
                                 number_of_players);

    this->all_storage_areas_.push_back(area);
  }

  // create all_players_
  for (int player_iterator = PLAYER_ID_0;
       player_iterator < number_of_players;
       player_iterator++)
  {
    auto *player = new Player(player_iterator,
                              player_iterator +
                              MINIMUM_START_STONE_COUNT);

    for (int initializer = FIRST_MARKET_CARD_POSITION;
         initializer < MAX_AMOUNT_OF_MARKET_CARDS;
         initializer++)
    {
      player->market_cards_.push_back(MARKET_CARD_UNINITIALIZED);
    }
    // first element is used for size determination
    player->market_cards_[MARKET_CARD_SIZE_POSITION] = INITIAL_MARKET_CARD_SIZE;
    all_players_.push_back(player);
  }
}
