//-----------------------------------------------------------------------------
// ServerMessage.cpp
//
// Cpp-Class for all server messages.
//
// Group: 03 (formerly 14.04)
//
// Author:  Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#include "../include/ServerMessages.hpp"

#include <algorithm>

// Server Messages
const std::string START_GAME = "StartGame ";
const std::string START_ROUND = "StartRound ";
const std::string GRANT_POINTS = "GrantPoints ";
const std::string END_GAME = "EndGame ";
const std::string START_TURN = "StartTurn ";
const std::string END_TURN = "EndTurn ";
const std::string REQUEST_INPUT = "RequestInput ";
const std::string WRONG_PLAYER = "WrongPlayer ";
const std::string UPDATE_STONES = "UpdateStones ";
const std::string INEXECUTABLE_INPUT = "InexecutableInput ";
const std::string UPDATE_CONSTRUCTION_SITE = "UpdateConstructionSite ";
const std::string UPDATE_SHIP = "UpdateShip ";
const std::string SAILED_SHIP = "SailedShip ";
const std::string MALFORMED_INPUT = "MalformedInput ";
const std::string END_ROUND = "EndRound\n";
const std::string SPACE = " ";
const std::string NEW_LINE = "\n";
const std::string NORMAL_ROUND = " 0\n";
const std::string PLAYED_MARKET_CARD = "PlayedMarketCard ";
const std::string GRANT_MARKET_CARD = "GrantMarketCard ";

// Diverse IDs and positioning
const int PLAYER_ID_0 = 0;
const int HIGHEST_PLAYER_ID = 3;
const int NEXT_PLAYER = 1;
const int ONE_POINT = 1;

// Stone amount calculation
const int ONE_STONE_TAKEN = -1;

const int PLACE_STONE = 2;
const int ZERO_POINTS = 0;
const int NO_STONE_ON_SHIP = -1;

enum MarketCard
{
   MARKET_CARD_ID_0 = 0,
   MARKET_CARD_ID_1 = 1,
   MARKET_CARD_ID_2 = 2,
   MARKET_CARD_ID_3 = 3,
   TAKE_MARKET_CARD = 3,
   MARKET_CARD_TAKEN = -1,
   FIRST_MARKET_CARD_POSITION = 0,
   LAST_MARKET_CARD_POSITION = 4,
   HIGHEST_SCORE = 0,
   HIGHEST_MARKET_CARD_POSITION = 24,
   FIRST_ACTUAL_MARKET_CARD = 1,
   MARKET_CARD_SIZE = 0
};

//-----------------------------------------------------------------------------
void ServerMessage::sendStartGameMessage(net::Server *server,
                                                int &number_of_players,
                                                PlayerMessage *first_message,
                                                std::vector<Player *> &all_players)
{
  string start_game_message = START_GAME;
  start_game_message.append(std::to_string(number_of_players));

  for (int player_iterator = PLAYER_ID_0;
       player_iterator < number_of_players;
       player_iterator++)
  {
      start_game_message.append(SPACE +
                              std::to_string(all_players[player_iterator]->stones_));
  }

  start_game_message.append(SPACE +
                            std::to_string(number_of_players));

  for (int player_iterator = PLAYER_ID_0;
       player_iterator < number_of_players;
       player_iterator++)
  {
    start_game_message.append(SPACE +
                              first_message->getArguments()[player_iterator +
                                                            NEXT_PLAYER]);
  }

  start_game_message.append(NEW_LINE);

  server->send(start_game_message);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendStartRoundMessage(net::Server *server,
                                                 std::vector<int> &all_market_cards,
                                                 std::vector<Ship *> &all_ships)
{
  string start_round_message;

  start_round_message.append(START_ROUND);
  start_round_message.append(std::to_string(all_ships.size()));

  for (Ship *ship : all_ships)
  {
      start_round_message.append(SPACE +
                                 std::to_string(ship->length_));
  }

  start_round_message.append(SPACE +
                             std::to_string(all_market_cards.size()));

  for (int market_card_position = FIRST_MARKET_CARD_POSITION;
       market_card_position < LAST_MARKET_CARD_POSITION;
       market_card_position++)
  {
      start_round_message.append(SPACE +
                               std::to_string(all_market_cards[market_card_position]));
  }

  start_round_message.append(NEW_LINE);

  server->send(start_round_message);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendGrandPointsMessage(net::Server *server,
                                           int player_id,
                                           std::vector<int> new_granted_points,
                                           std::vector<int> total_granted_points)
{
  if (new_granted_points[player_id] == ZERO_POINTS)
  {
    return;
  }

  server->send(GRANT_POINTS +
               std::to_string(player_id) +
               SPACE +
               std::to_string(new_granted_points[player_id]) +
               SPACE +
               std::to_string(total_granted_points[player_id]) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendEndGameMessage(net::Server *server,
                                       Action *action,
                                       int number_of_players)
{
  std::vector<int> score = action->getTotalGrantedPoints();
  std::vector<int> sort_score = action->getTotalGrantedPoints();
  std::vector<int> score_with_ids;

  for (int player_id = HIGHEST_PLAYER_ID;
       player_id >= PLAYER_ID_0;
       player_id--)
  {
    if (score[player_id] == ZERO_POINTS)
    {
      score.erase(score.begin() +
                  player_id);
    }
    if (sort_score[player_id] == ZERO_POINTS)
    {
      sort_score.erase(sort_score.begin() +
                       player_id);
    }
  }

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    score_with_ids.push_back(NO_STONE_ON_SHIP);
  }

  int player_id_for_sorting = PLAYER_ID_0;

  // sort vector "sort_score" from highest value to smallest
  std::sort(sort_score.begin(), sort_score.end());
  int highest_score = sort_score[number_of_players
                                 - ONE_POINT];
  while (highest_score != sort_score[HIGHEST_SCORE])
  {
    std::rotate(sort_score.begin(),
                sort_score.begin()
                + NEXT_PLAYER,
                sort_score.end());
  }

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    do
    {
      if (score[player_id] == sort_score[player_id_for_sorting])
      {
        score_with_ids[player_id_for_sorting] = player_id;
        player_id_for_sorting = PLAYER_ID_0;
        break;
      }
      player_id_for_sorting++;
    } while (score[player_id] != sort_score[player_id_for_sorting -
                                            NEXT_PLAYER]);
  }



  string end_game_message = END_GAME +
                            std::to_string(number_of_players);

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    end_game_message.append(SPACE +
                            std::to_string(score_with_ids[player_id]));
  }
  server->send(end_game_message +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendStartTurnMessage(net::Server *server,
                                         std::vector<int> &respective_player,
                                         int player_id)
{
  server->send(START_TURN +
               std::to_string(respective_player[player_id]) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendEndTurnMessage(net::Server *server, std::vector<int> &respective_player, int player_id)
{
  server->send(END_TURN +
               std::to_string(respective_player[player_id]) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendRequestInput(net::Server *server,
                                     std::vector<int> &respective_player,
                                     int player_id,
                                     int input_type,
                                     std::vector<int> &all_market_cards)
{
  server->send(REQUEST_INPUT +
               std::to_string(respective_player[player_id]) +
               SPACE +
               std::to_string(input_type) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendRequestInputMarket(net::Server *server,
                                           int player_id,
                                           int input_type,
                                           std::vector<int> &all_market_cards)
{
  if (input_type == TAKE_MARKET_CARD)
  {
    server->send(REQUEST_INPUT +
                 std::to_string(player_id) +
                 SPACE +
                 std::to_string(input_type) +
                 SPACE +
                 std::to_string(all_market_cards.size()) +
                 SPACE +
                 std::to_string(all_market_cards[MARKET_CARD_ID_0]) +
                 SPACE +
                 std::to_string(all_market_cards[MARKET_CARD_ID_1]) +
                 SPACE +
                 std::to_string(all_market_cards[MARKET_CARD_ID_2]) +
                 SPACE +
                 std::to_string(all_market_cards[MARKET_CARD_ID_3]) +
                 NEW_LINE);
  }
  if (input_type == PLACE_STONE)
  {
    server->send(REQUEST_INPUT +
                 std::to_string(player_id) +
                 SPACE +
                 std::to_string(input_type) +
                 NEW_LINE);
  }
}


//-----------------------------------------------------------------------------
void ServerMessage::sendWrongPlayer(net::Server *server, net::Server::Message *input)
{
  server->send(WRONG_PLAYER +
               std::to_string(input->player_id) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendGetNewStones(net::Server *server,
                                     std::vector<Player *> all_players,
                                     net::Server::Message *input,
                                     int &new_total_amount,
                                     int &stone_count_difference,
                                     int &player_stones_at_start)
{
  new_total_amount = all_players[input->player_id]->stones_;
  stone_count_difference = new_total_amount - player_stones_at_start;

  server->send(UPDATE_STONES +
               std::to_string(input->player_id) +
               SPACE +
               std::to_string(stone_count_difference) +
               SPACE +
               std::to_string(new_total_amount) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendInexecutableInput(net::Server *server,
                                          int &player_id)
{
  server->send(INEXECUTABLE_INPUT +
               std::to_string(player_id) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendUpdateShip(net::Server *server,
                                   Action *action,
                                   std::vector<Ship *> &all_ships)
{
  int id_of_sailed_ship = action->getShipId();

  std::string stones_on_ship_updated = UPDATE_SHIP +
                                       std::to_string(id_of_sailed_ship) +
                                       SPACE +
                                       std::to_string(all_ships[id_of_sailed_ship]->spaces_.size());

  for (auto const &value : all_ships[id_of_sailed_ship]->spaces_)
  {
    stones_on_ship_updated.append(SPACE +
                                  std::to_string(value));
  }
  stones_on_ship_updated.append(NEW_LINE);
  server->send(stones_on_ship_updated);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendUpdateConstructionSite(net::Server *server,
                                               int site_id,
                                               std::string site_id_arguments)
{
  std::string server_message = UPDATE_CONSTRUCTION_SITE +
                               std::to_string(site_id) +
                               SPACE +
                               site_id_arguments +
                               NEW_LINE;
  server->send(server_message);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendUpdateMarket(net::Server *server,
                                     int site_id,
                                     int market_card_id,
                                     std::vector<int> market_cards)
{
  std::string updated_market_cards;

  market_cards[market_card_id] = MARKET_CARD_TAKEN;

  updated_market_cards = std::to_string(market_cards[MARKET_CARD_ID_0]) +
                         SPACE +
                         std::to_string(market_cards[MARKET_CARD_ID_1]) +
                         SPACE +
                         std::to_string(market_cards[MARKET_CARD_ID_2]) +
                         SPACE +
                         std::to_string(market_cards[MARKET_CARD_ID_3]);

  std::string server_message = UPDATE_CONSTRUCTION_SITE +
                               std::to_string(site_id) +
                               SPACE +
                               std::to_string(market_cards.size()) +
                               SPACE +
                               updated_market_cards +
                               NEW_LINE;
  server->send(server_message);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendPlaceStoneOnShip(net::Server *server,
                                         std::vector<Player *> all_players,
                                         net::Server::Message *input,
                                         int &stone_count_difference,
                                         int &new_total_amount)
{
  stone_count_difference = ONE_STONE_TAKEN;
  new_total_amount = all_players[input->player_id]->stones_;

  server->send(UPDATE_STONES +
               std::to_string(input->player_id) +
               SPACE +
               std::to_string(stone_count_difference) +
               SPACE +
               std::to_string(new_total_amount) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendSailedShip(net::Server *server,
                                   Action *action)
{
  server->send(SAILED_SHIP +
               std::to_string(action->getShipId()) +
               SPACE +
               std::to_string(action->getSiteId()) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendEndRound(net::Server *server)
{
  server->send(END_ROUND);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendPlayedMarketCard(net::Server *server,
                                         int player_id,
                                         int market_card_id)
{
  server->send(PLAYED_MARKET_CARD +
               std::to_string(player_id) +
               SPACE +
               std::to_string(market_card_id) +
               NEW_LINE);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendGrantMarketCard(net::Server *server,
                                        std::vector<int> market_cards,
                                        int player_id,
                                        int card_type)
{
  for (int market_card_position = HIGHEST_MARKET_CARD_POSITION;
       market_card_position >= FIRST_ACTUAL_MARKET_CARD;
       market_card_position--)
  {
    if (market_cards[market_card_position] == MARKET_CARD_TAKEN)
    {
      market_cards.erase(market_cards.begin() +
                         market_card_position);
    }
  }

  std::string server_message;

  server_message = GRANT_MARKET_CARD +
                   std::to_string(player_id) +
                   SPACE +
                   std::to_string(card_type) +
                   SPACE +
                   std::to_string(market_cards[MARKET_CARD_SIZE]) +
                   SPACE;

  for (int card_position = FIRST_ACTUAL_MARKET_CARD;
       card_position < (int) market_cards.size() - 1
       ; card_position++)
  {
    server_message += std::to_string(market_cards[card_position]) +
                      SPACE;
  }

  server_message += std::to_string(market_cards[market_cards.size() -
                                                FIRST_ACTUAL_MARKET_CARD]) +
                    NEW_LINE;

  server->send(server_message);
}

//-----------------------------------------------------------------------------
void ServerMessage::sendGrantPointsPyramid(net::Server *server)
{
    std::vector<int> new_granted_points = Action::getGrantedPointsPyramid();
    std::vector<int> total_points = Action::getTotalGrantedPoints();
    std::string server_message;


    for (int player_id = PLAYER_ID_0;
         player_id < (int) new_granted_points.size();
         player_id++)
    {
        if (new_granted_points[player_id] != ZERO_POINTS)
        {
            server_message = GRANT_POINTS +
                             std::to_string(player_id) +
                             SPACE +
                             std::to_string(new_granted_points[player_id]) +
                             SPACE +
                             std::to_string(total_points[player_id]) +
                             NEW_LINE;
            server->send(server_message);
        }
    }
}

//-----------------------------------------------------------------------------
void ServerMessage::sendMalformedInput(net::Server *server,
                                       int player_id)
{
    server->send(MALFORMED_INPUT +
               std::to_string(player_id) +
               NEW_LINE);
}
