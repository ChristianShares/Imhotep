//-----------------------------------------------------------------------------
// Market.cpp
//
// Market-Class that handles all operations in reference to the market
//
// Group: 03 (formerly 14.04)
//
// Author:  Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#include "../include/Market.hpp"

#include <iostream>

// Diverse constants
const int CHISEL_AMOUNT_OF_PLACEMENTS = 2;
const int ZERO_POINTS = 0;

enum Stones
{
  NO_STONES = 0,
  THREE_STONES = 3,
  OBELISK_HEIGHT = 1
};

enum MarkedCards
{
  RED_CARD_ID_0 = 0,
  RED_CARD_ID_1 = 1,
  RED_CARD_ID_2 = 2,
  GREEN_CARD_ID_3 = 3,
  GREEN_CARD_ID_4 = 4,
  GREEN_CARD_ID_5 = 5,
  GREEN_CARD_ID_6 = 6,
  PURPLE_CARD_ID_7 = 7,
  BLUE_CARD_ID_8 = 8,
  BLUE_CARD_ID_9 = 9,
  BLUE_CARD_ID_10 = 10,
  BLUE_CARD_ID_11 = 11
};

enum ActionReturnValues
{
  GET_NEW_STONES_EXECUTED = 1,
  GET_NEW_STONES_ERROR = -1,
  PLACE_STONE_ON_SHIP_EXECUTED = 2,
  PLACE_STONE_ON_SHIP_ERROR = -2,
  SAIL_SHIP_EXECUTED = 3,
  SAIL_SHIP_ERROR = -3,
  TAKE_MARKET_CARD_EXECUTED = 4,
  TAKE_MARKET_CARD_ERROR = -4,
  PLAY_BLUE_CARD_EXECUTED = 5,
  PLAY_BLUE_CARD_ERROR = -5,
  SAIL_UNLOAD_SHIP_ERROR = -6
};

enum InputTypes
{
  NORMAL_ROUND = 0,
  SAIL_SHIP_BLUE_MARKET_CARD = 1,
  PLACE_STONE_BLUE_MARKET_CARD = 2,
  TAKE_MARKET_CARD = 3,
  SAIL_UNLOAD_BLUE_MARKET_CARD = 4
};

enum PlayerInfo
{
  MAX_AMOUNT_OF_PLAYERS = 4,
  PLAYER_ID_0 = 0
};

enum Turn
{
  FIRST_ITERATION = 0,
  FIRST_MOVE = 0
};

//-----------------------------------------------------------------------------
Market::Market()
{
  for (int player_id = PLAYER_ID_0;
       player_id < MAX_AMOUNT_OF_PLAYERS;
       player_id++)
  {
    all_market_card_points_.push_back(ZERO_POINTS);
  }
}

//-----------------------------------------------------------------------------
int getBlueCardsScore(Player *player)
{
  int score_blue_market_card = ZERO_POINTS;
  // first element is size, this makes sure, that it won't be counted
  int iteration_count = FIRST_ITERATION;

  for (int market_card_id : player->market_cards_)
  {
    if ((market_card_id == BLUE_CARD_ID_8 ||
        market_card_id == BLUE_CARD_ID_9 ||
        market_card_id == BLUE_CARD_ID_10 ||
        market_card_id == BLUE_CARD_ID_11) &&
        (iteration_count != FIRST_ITERATION))
    {
      score_blue_market_card++;
    }
    iteration_count++;
  }

  return score_blue_market_card;
}

//-----------------------------------------------------------------------------
int getPurpleCardsScore(Player *player)
{
  int score_purple_market_card = ZERO_POINTS;
  // first element is size, this makes sure, that it won't be counted
  int iteration_count = FIRST_ITERATION;

  for (int market_card_id : player->market_cards_)
  {
    if ((market_card_id == PURPLE_CARD_ID_7) &&
        (iteration_count != FIRST_ITERATION))
    {
      score_purple_market_card++;
    }
    iteration_count++;
  }
  return score_purple_market_card;
}

//-----------------------------------------------------------------------------
int getGreenCardsScore(Player *player)
{
  int score_green_market_card = ZERO_POINTS;
  std::vector<int> market_cards_per_player = player->market_cards_;

  // delete the first element, which just holds the size
  market_cards_per_player.erase(market_cards_per_player.begin());


  int amount_of_stones_pyramid = StorageArea::pyramid_player_stones_cumulated_.size();
  int amount_of_stones_temple = StorageArea::temple_player_stones_cumulated_.size();
  int amount_of_stones_chamber = StorageArea::chamber_player_stones_cumulated_.size();
  int amount_of_stones_obelisk = NO_STONES;

  for (std::vector<int> storage_per_player : StorageArea::obelisk_player_and_height_)
  {
    amount_of_stones_obelisk += storage_per_player[OBELISK_HEIGHT];
  }

  int points_for_pyramid = ZERO_POINTS;
  int points_for_temple = ZERO_POINTS;
  int points_for_chamber = ZERO_POINTS;
  int points_for_obelisk = ZERO_POINTS;


  while (amount_of_stones_pyramid > NO_STONES)
  {
    if (amount_of_stones_pyramid >= THREE_STONES)
    {
      points_for_pyramid++;
    }
    amount_of_stones_pyramid -= THREE_STONES;
  }

  while (amount_of_stones_temple > NO_STONES)
  {
    if (amount_of_stones_temple >= THREE_STONES)
    {
      points_for_temple++;
    }
    amount_of_stones_temple -= THREE_STONES;
  }

  while (amount_of_stones_chamber > NO_STONES)
  {
    if (amount_of_stones_chamber >= THREE_STONES)
    {
      points_for_chamber++;
    }
    amount_of_stones_chamber -= THREE_STONES;
  }

  while (amount_of_stones_obelisk > NO_STONES)
  {
    if (amount_of_stones_obelisk >= THREE_STONES)
    {
      points_for_obelisk++;
    }
    amount_of_stones_obelisk -= THREE_STONES;
  }



  for (int market_card_id : market_cards_per_player)
  {
      switch (market_card_id)
      {
        case GREEN_CARD_ID_3:
          score_green_market_card += points_for_pyramid;
          break;

        case GREEN_CARD_ID_4:
          score_green_market_card += points_for_temple;
          break;

        case GREEN_CARD_ID_5:
          score_green_market_card += points_for_chamber;
          break;

        case GREEN_CARD_ID_6:
          score_green_market_card += points_for_obelisk;
          break;
      }
  }

  return score_green_market_card;
}

//-----------------------------------------------------------------------------
void Market::addMarketPoints(int player_id,
                             std::vector<Player *> &all_players)
{
  int end_game_market_score = ZERO_POINTS;

  if (player_id >= (int) all_players.size())
  {
    return;
  }

  Player *player = all_players[player_id];


  end_game_market_score = end_game_market_score +
                          getPurpleCardsScore(player) +
                          getGreenCardsScore(player) +
                          getBlueCardsScore(player);

  Action::increaseGrantedPointsTotal(end_game_market_score, player_id);
  all_market_card_points_[player_id] = end_game_market_score;
}

//-----------------------------------------------------------------------------
std::vector<int> Market::getAllMarketPoints()
{
  return all_market_card_points_;
}

//-----------------------------------------------------------------------------
void Market::handleChisel(net::Server *server,
                          Board &board,
                          std::vector<int> &respective_player,
                          int &player_id,
                          int &new_total_amount,
                          int &stone_count_difference,
                          int &player_stones_at_start,
                          bool &ship_was_sailed,
                          net::Server::Message *input)
{
  Handler *chisel = nullptr;

  for (int two_moves_for_player = FIRST_MOVE;
       two_moves_for_player < CHISEL_AMOUNT_OF_PLACEMENTS;
       two_moves_for_player++)
  {
    ServerMessage::sendRequestInputMarket(server,
                                          input->player_id,
                                          PLACE_STONE_BLUE_MARKET_CARD,
                                          board.all_market_cards_);

    net::Server::Message *place_stone_input = chisel->waitForServerInput(server);

    Action *place_stone_action;
    PlayerMessage place_stone_message = PlayerMessage();

    bool place_stone_message_malformed = Handler::handleMalformedInput(server,
                                                                       place_stone_input,
                                                                       place_stone_message,
                                                                       place_stone_input->player_id);


    if (!place_stone_message_malformed)
    {
      place_stone_action = place_stone_message.executeAction();


      if (place_stone_action->getActionReturnValue() == PLACE_STONE_ON_SHIP_EXECUTED)
      {
        ServerMessage::sendPlaceStoneOnShip(server,
                                            board.all_players_,
                                            place_stone_input,
                                            stone_count_difference,
                                            new_total_amount);
        ServerMessage::sendUpdateShip(server, place_stone_action, board.all_ships_);
      }
      else
      {
        ServerMessage::sendInexecutableInput(server, place_stone_input->player_id);
        handleChisel(server,
                     board,
                     respective_player,
                     player_id,
                     new_total_amount,
                     stone_count_difference,
                     player_stones_at_start,
                     ship_was_sailed,
                     input);
      }
    }

    delete place_stone_input;
    delete place_stone_action;
  }
  delete chisel;
}


//-----------------------------------------------------------------------------
void Market::handleLever(net::Server *server,
                         Board &board,
                         std::vector<int> &respective_player,
                         int &player_id,
                         int &new_total_amount,
                         int &stone_count_difference,
                         int &player_stones_at_start,
                         bool &ship_was_sailed)
{
  Handler *lever = nullptr;

  ServerMessage::sendRequestInput(server,
                                  respective_player,
                                  player_id,
                                  SAIL_UNLOAD_BLUE_MARKET_CARD,
                                  board.all_market_cards_);


  net::Server::Message *sail_unload_ship_input = lever->waitForServerInput(server);

  Action *sail_unload_ship_action;
  PlayerMessage sail_unload_ship_message = PlayerMessage();

  bool place_stone_message_malformed = Handler::handleMalformedInput(server,
                                                                     sail_unload_ship_input,
                                                                     sail_unload_ship_message,
                                                                     sail_unload_ship_input->player_id);

  if (!place_stone_message_malformed)
  {
    sail_unload_ship_action = sail_unload_ship_message.executeAction();

    if (sail_unload_ship_action->getActionReturnValue() == SAIL_SHIP_EXECUTED)
    {
      ship_was_sailed = true;

      ServerMessage::sendSailedShip(server,
                                    sail_unload_ship_action);

      lever->handleSailShipExecuted(server,
                                    sail_unload_ship_action,
                                    sail_unload_ship_input,
                                    board);
    }
    else
    {
      ServerMessage::sendInexecutableInput(server,
                                           sail_unload_ship_input->player_id);

      handleLever(server,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed);
    }
  }

  delete sail_unload_ship_input;
  delete lever;
  delete sail_unload_ship_action;
}

//-----------------------------------------------------------------------------
void Market::handleHammer(net::Server *server,
                          Action *action,
                          Board &board,
                          std::vector<int> &respective_player,
                          int &player_id,
                          int &new_total_amount,
                          int &stone_count_difference,
                          int &player_stones_at_start,
                          bool &ship_was_sailed,
                          net::Server::Message *input)
{
  Handler *hammer = nullptr;

  ServerMessage::sendGetNewStones(server,
                                  board.all_players_,
                                  input,
                                  new_total_amount,
                                  stone_count_difference,
                                  player_stones_at_start);

  ServerMessage::sendRequestInput(server,
                                  respective_player,
                                  player_id,
                                  PLACE_STONE_BLUE_MARKET_CARD,
                                  board.all_market_cards_);

  net::Server::Message *place_stone_input = hammer->waitForServerInput(server);

  Action *place_stone_action;
  PlayerMessage place_stone_message = PlayerMessage();

  bool place_stone_message_malformed = Handler::handleMalformedInput(server,
                                                                     place_stone_input,
                                                                     place_stone_message,
                                                                     place_stone_input->player_id);


  if (!place_stone_message_malformed)
  {
    place_stone_action = place_stone_message.executeAction();

    if (place_stone_action->getActionReturnValue() == PLACE_STONE_ON_SHIP_EXECUTED)
    {
      ServerMessage::sendPlaceStoneOnShip(server,
                                          board.all_players_,
                                          place_stone_input,
                                          stone_count_difference,
                                          new_total_amount);

      ServerMessage::sendUpdateShip(server,
                                    place_stone_action,
                                    board.all_ships_);
    }
    else
    {
      ServerMessage::sendInexecutableInput(server, place_stone_input->player_id);
      handleHammer(server,
                   place_stone_action,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed,
                   input);
    }
  }

  delete place_stone_input;
  delete hammer;
  delete place_stone_action;
}

//-----------------------------------------------------------------------------
void Market::handleSails(net::Server *server,
                         Board &board,
                         std::vector<int> &respective_player,
                         int &player_id,
                         int &new_total_amount,
                         int &stone_count_difference,
                         int &player_stones_at_start,
                         bool &ship_was_sailed,
                         net::Server::Message *input)
{
  Handler *sails = nullptr;

  ServerMessage::sendRequestInput(server,
                                  respective_player,
                                  player_id,
                                  PLACE_STONE_BLUE_MARKET_CARD,
                                  board.all_market_cards_);

  net::Server::Message *place_stone_input = sails->waitForServerInput(server);

  Action *place_stone_action;
  PlayerMessage place_stone_message = PlayerMessage();

  bool place_stone_message_malformed = Handler::handleMalformedInput(server,
                                                                     place_stone_input,
                                                                     place_stone_message,
                                                                     place_stone_input->player_id);


  if (!place_stone_message_malformed)
  {
    place_stone_action = place_stone_message.executeAction();

    if (place_stone_action->getActionReturnValue() == PLACE_STONE_ON_SHIP_EXECUTED)
    {
      ServerMessage::sendPlaceStoneOnShip(server,
                                          board.all_players_,
                                          place_stone_input,
                                          stone_count_difference,
                                          new_total_amount);
      ServerMessage::sendUpdateShip(server, place_stone_action, board.all_ships_);
    }
    else
    {
      ServerMessage::sendInexecutableInput(server, place_stone_input->player_id);
      handleSails(server,
                  board,
                  respective_player,
                  player_id,
                  new_total_amount,
                  stone_count_difference,
                  player_stones_at_start,
                  ship_was_sailed,
                  input);
    }
  }

  delete place_stone_input;
  delete place_stone_action;

  ServerMessage::sendRequestInput(server,
                                  respective_player,
                                  player_id,
                                  SAIL_SHIP_BLUE_MARKET_CARD,
                                  board.all_market_cards_);

  net::Server::Message *sail_ship_input = sails->waitForServerInput(server);

  Action *sail_ship_action;
  PlayerMessage sail_ship_message = PlayerMessage();

  bool sail_ship_message_malformed = Handler::handleMalformedInput(server,
                                                                   sail_ship_input,
                                                                   sail_ship_message,
                                                                   sail_ship_input->player_id);

  if (!sail_ship_message_malformed)
  {
    sail_ship_action = sail_ship_message.executeAction();

    if (sail_ship_action->getActionReturnValue() == SAIL_SHIP_EXECUTED)
    {
      ship_was_sailed = true;

      ServerMessage::sendSailedShip(server,
                                    sail_ship_action);

      sails->handleSailShipExecuted(server,
                                    sail_ship_action,
                                    sail_ship_input,
                                    board);
    }
    else
    {
      ServerMessage::sendInexecutableInput(server,
                                           sail_ship_input->player_id);

      handleSails(server,
                  board,
                  respective_player,
                  player_id,
                  new_total_amount,
                  stone_count_difference,
                  player_stones_at_start,
                  ship_was_sailed,
                  sail_ship_input);
    }
  }
  delete sail_ship_input;
  delete sail_ship_action;
  delete sails;
}
