//-----------------------------------------------------------------------------
// GameHandler.cpp
//
// Cpp-Class for the game handling. This is executed right after the input ...
// ... checks.
//
// Group: 03 (formerly 14.04)
//
// Author:  Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#include "../include/GameHandler.hpp"

#include <iostream>
#include <regex>
#include <string>

// Server
const std::string IP = "127.0.0.1";
const std::string IP_SERVER = "0.0.0.0";
const std::string SPACE = " ";

// Console Outputs
const std::string INVALID_ARGUMENT_NUMBER = "Invalid number of arguments: [Players] [Port] expected\n";
const std::string RECEIVED_FROM_PLAYER = "Received from player ";
const std::string WAIT_FOR = "Waiting for ";
const std::string PLAYERS = " player(s) to connect...";
const std::string SEMICOLON = ": ";
const std::string ALL_PLAYERS_CONNECTED = "All players connected: ";

// Regex
const std::string NON_DIGIT = "\\D";

// Diverse Constants
const int THREE_ARGUMENTS = 3;
const int VALID_INPUT = 0;
const int NO_SHIP_SAILED = 0;
const int ALL_SHIPS = 4;

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
  PLAY_BLUE_CARD_ERROR = -5
};

enum InputTypes
{
  NORMAL_ROUND = 0,
  SAIL_SHIP_BLUE_MARKET_CARD = 1,
  PLACE_STONE_BLUE_MARKET_CARD = 2,
  TAKE_MARKET_CARD = 3,
  SAIL_UNLOAD_BLUE_MARKET_CARD = 4
};

enum StorageAreas
{
  MARKET = 0,
  PYRAMID = 1,
  TEMPLE = 2,
  CHAMBER = 3,
  OBELISK = 4,
  TOTAL_AMOUNT_OF_STORAGE_AREAS = 5,
  OBELISK_HEIGHT = 1
};

enum MarkedCards
{
  FIRST_MARKET_CARD_POSITION = 0,
  USED_MARKET_CARD = -1,
  MARKET_CARD_SIZE = 0,
  DECREASE_SIZE_BY_ONE = 1,
  INCREASE_SIZE_BY_ONE = 1,
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

enum PlayerInfo
{
  AMOUNT_OF_PLAYERS = 1,
  MINIMUM_AMOUNT_OF_PLAYERS = 1,
  MAXIMUM_AMOUNT_OF_PLAYERS = 4,
  PLAYER_ID_0 = 0,
  FIRST_PLAYER_THIS_ROUND = 0,
  NEXT_PLAYER = 1,
  ZERO_POINTS = 0
};

enum RoundInfo
{
  FIRST_ROUND = 0,
  AMOUNT_OF_TOTAL_ROUNDS = 6
};

enum PortInfo
{
  PORT = 2,
  MINIMUM_PORT_NUMBER = 1,
  MAXIMUM_PORT_NUMBER = 65535
};

enum ReturnValues
{
  ALL_WENT_WELL = 0,
  WRONG_NUMBER_OF_ARGUMENTS = 1,
  ARGUMENTS_ARE_WRONG = 2,
  INTERNAL_ERROR = -1
};


using std::string;

//-----------------------------------------------------------------------------
int Handler::executeGame(int number_of_players,
                         unsigned short port)
{
  // start server and wait
  net::Server *server = startServer(number_of_players,
                                    port);

  net::Server::Message *all_players_connected = waitForServerInput(server);

  Board board(number_of_players);

  PlayerMessage *first_message = PlayerMessage::processFirstPlayerMessage(all_players_connected,
                                                                          board.all_players_,
                                                                          board.all_storage_areas_,
                                                                          board.all_ships_);

  std::cout << ALL_PLAYERS_CONNECTED << all_players_connected->message << std::endl;

  ServerMessage::sendStartGameMessage(server,
                                      number_of_players,
                                      first_message,
                                      board.all_players_);

  // vector to determine order of player moves
  std::vector<int> respective_player;

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    respective_player.push_back(player_id);
  }

  Action *action;
  std::vector<int> granted_points;

  // Start rounds
  for (int i = FIRST_ROUND;
       i < AMOUNT_OF_TOTAL_ROUNDS;
       i++)
  {
    Ship::generateAllShips(board.all_ships_,
                           Random::getInstance().getNextShipSize(),
                           Random::getInstance().getNextShipSize(),
                           Random::getInstance().getNextShipSize(),
                           Random::getInstance().getNextShipSize());

    StorageArea::generateMarketCardVector(board.all_market_cards_,
                                          Random::getInstance().getNextMarketCard(),
                                          Random::getInstance().getNextMarketCard(),
                                          Random::getInstance().getNextMarketCard(),
                                          Random::getInstance().getNextMarketCard());

    PlayerMessage::setAllShips(board.all_ships_);

    action = executeRoundsAndTurns(server,
                                   board,
                                   board.all_market_cards_,
                                   respective_player,
                                   number_of_players,
                                   action);

  }
  StorageArea::grantPointsObelisk(board.all_players_.size());
  StorageArea::grantPointsChamber();

  Market market_points;

  std::vector<int> granted_points_obelisk = action->getGrantedPointsObelisk();

  for (int player_id = PLAYER_ID_0;
       player_id < MAXIMUM_AMOUNT_OF_PLAYERS;
       player_id++)
  {
    action->increaseGrantedPointsTotal(granted_points_obelisk[player_id],
                                       player_id);

    market_points.addMarketPoints(player_id,
                                  board.all_players_);
  }

  std::vector<int> game_end_granted_points = calculateTotalPoints(action->getGrantedPointsChamber(),
                                                                  granted_points_obelisk);

  game_end_granted_points = calculateTotalPoints(game_end_granted_points,
                                                 market_points.getAllMarketPoints());

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    ServerMessage::sendGrandPointsMessage(server,
                                          player_id,
                                          game_end_granted_points,
                                          action->getTotalGrantedPoints());
  }

  ServerMessage::sendEndGameMessage(server,
                                    action,
                                    number_of_players);

  freeMemory(server,
             all_players_connected,
             first_message,
             board);
  return 0;
}

//-----------------------------------------------------------------------------
Action *Handler::executeRoundsAndTurns(net::Server *server,
                                       Board &board,
                                       std::vector<int> &all_market_cards,
                                       std::vector<int> &respective_player,
                                       int number_of_players,
                                       Action *action)
{
  ServerMessage::sendStartRoundMessage(server,
                                       all_market_cards,
                                       board.all_ships_);

  int new_total_amount;
  int stone_count_difference;
  int player_stones_at_start;

  std::vector<int> granted_points;
  int amount_of_sailed_ships = NO_SHIP_SAILED;

  // will be needed to determine the player id for the new round
  int player_id_last_move;

  bool ship_was_sailed = false;
  //Start turns of the players
  while (amount_of_sailed_ships < ALL_SHIPS)
  {
    for (int player_id = PLAYER_ID_0;
         player_id < number_of_players;
         player_id++)
    {
      ServerMessage::sendStartTurnMessage(server,
                                          respective_player,
                                          player_id);

      executeTurns(server,
                   action,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed);

      ServerMessage::sendEndTurnMessage(server,
                                        respective_player,
                                        player_id);

      if (ship_was_sailed)
      {
        amount_of_sailed_ships++;
        ship_was_sailed = false;
      }

      if (amount_of_sailed_ships == ALL_SHIPS)
      {
        // first element of respective player will contain the player_id of the last player who made a move
        player_id_last_move = respective_player[player_id];
        while (respective_player[FIRST_PLAYER_THIS_ROUND] != player_id_last_move)
        {
          std::rotate(respective_player.begin(),
                      respective_player.begin() + NEXT_PLAYER,
                      respective_player.end());
        }
        break;
      }
    }
  }
  // now let the player start the new round who follows next to the last player who made a turn in the last round
  std::rotate(respective_player.begin(),
              respective_player.begin() + NEXT_PLAYER,
              respective_player.end());

  // TEMPLE: won't grant points if no ship sailed there
  StorageArea::grantPointsTemple(board.all_players_.size());

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    ServerMessage::sendGrandPointsMessage(server,
                                          player_id,
                                          action->getGrantedPointsTemple(),
                                          action->getTotalGrantedPoints());
  }

  Action::resetTemple();

  ServerMessage::sendEndRound(server);

  return action;
}

//-----------------------------------------------------------------------------
Action *Handler::executeTurns(net::Server *server,
                              Action *action,
                              Board &board,
                              std::vector<int> &respective_player,
                              int &player_id,
                              int &new_total_amount,
                              int &stone_count_difference,
                              int &player_stones_at_start,
                              bool &ship_was_sailed)
{
  static bool is_right_player = true;

  player_stones_at_start = board.all_players_[player_id]->stones_;

  // request input
  if (is_right_player)
  {
    ServerMessage::sendRequestInput(server,
                                    respective_player,
                                    player_id,
                                    NORMAL_ROUND,
                                    board.all_market_cards_);
  }
  else
  {
    is_right_player = true;
  }

  net::Server::Message *input = waitForServerInput(server);

  // if right player makes move
  if (respective_player[player_id] == input->player_id)
  {
    is_right_player = true;
    PlayerMessage new_message = PlayerMessage();
    bool message_is_malformed_input = handleMalformedInput(server,
                                                           input,
                                                           new_message,
                                                           player_id);
    if (!message_is_malformed_input)
    {
      makeActualMove(server,
                     action,
                     board,
                     respective_player,
                     player_id,
                     new_total_amount,
                     stone_count_difference,
                     player_stones_at_start,
                     ship_was_sailed,
                     input,
                     new_message);
    }
  }
    // if wrong player makes move
  else
  {
    ServerMessage::sendWrongPlayer(server,
                                   input);

    // make sure that no request_input message will be sent
    is_right_player = false;

    executeTurns(server,
                 action,
                 board,
                 respective_player,
                 player_id,
                 new_total_amount,
                 stone_count_difference,
                 player_stones_at_start,
                 ship_was_sailed);
  }
  delete input;
  return action;
}

//-----------------------------------------------------------------------------
void Handler::makeActualMove(net::Server *server,
                             Action *action,
                             Board &board,
                             std::vector<int> &respective_player,
                             int &player_id,
                             int &new_total_amount,
                             int &stone_count_difference,
                             int &player_stones_at_start,
                             bool &ship_was_sailed,
                             net::Server::Message *input,
                             PlayerMessage &new_message)
{

  // Check return value of new_message and handle malformed and inexecutable input
  std::cout << RECEIVED_FROM_PLAYER << std::to_string(input->player_id) << SEMICOLON << input->message << std::endl;
  player_stones_at_start = board.all_players_[input->player_id]->stones_;

  action = new_message.executeAction();

  switch (action->getActionReturnValue())
  {
    case GET_NEW_STONES_EXECUTED:
      ServerMessage::sendGetNewStones(server,
                                      board.all_players_,
                                      input,
                                      new_total_amount,
                                      stone_count_difference,
                                      player_stones_at_start);
      break;
    case GET_NEW_STONES_ERROR:
      ServerMessage::sendInexecutableInput(server,
                                           input->player_id);

      executeTurns(server,
                   action,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed);
      break;
    case PLACE_STONE_ON_SHIP_EXECUTED:
      ServerMessage::sendPlaceStoneOnShip(server,
                                          board.all_players_,
                                          input,
                                          stone_count_difference,
                                          new_total_amount);
      ServerMessage::sendUpdateShip(server, action, board.all_ships_);
      break;
    case PLACE_STONE_ON_SHIP_ERROR:
      ServerMessage::sendInexecutableInput(server,
                                           input->player_id);

      executeTurns(server,
                   action,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed);
      break;
    case SAIL_SHIP_EXECUTED:
      ship_was_sailed = true;
      ServerMessage::sendSailedShip(server,
                                    action);
      handleSailShipExecuted(server,
                             action,
                             input,
                             board);
      break;
    case SAIL_SHIP_ERROR:
      ServerMessage::sendInexecutableInput(server,
                                           input->player_id);
      executeTurns(server,
                   action,
                   board,
                   respective_player,
                   player_id,
                   new_total_amount,
                   stone_count_difference,
                   player_stones_at_start,
                   ship_was_sailed);
      break;
    case PLAY_BLUE_CARD_EXECUTED:

      handlePlayBlueCard(server,
                         action,
                         board,
                         respective_player,
                         player_id,
                         new_total_amount,
                         stone_count_difference,
                         player_stones_at_start,
                         ship_was_sailed,
                         input);
      break;

    default:
      ship_was_sailed = false;
  }
  delete action;
}

//-----------------------------------------------------------------------------
void Handler::handlePlayBlueCard(net::Server *server,
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
  Market blue_market_cards;

  std::vector<int> market_cards_of_player = board.all_players_[input->player_id]->market_cards_;
  int needed_card_id = std::stoi(action->getSiteIdArguments());
  int position_of_market_card = FIRST_MARKET_CARD_POSITION;

  // get position of blue market card in all market cards of player
  for (int marked_card_id : market_cards_of_player)
  {
    if (marked_card_id == needed_card_id)
    {
      break;
    }
    position_of_market_card++;
  }

  ServerMessage::sendPlayedMarketCard(server,
                                      input->player_id,
                                      market_cards_of_player[position_of_market_card]);

  board.all_players_[input->player_id]->market_cards_[position_of_market_card] = USED_MARKET_CARD;
  board.all_players_[input->player_id]->market_cards_[MARKET_CARD_SIZE] -= DECREASE_SIZE_BY_ONE;

  switch (needed_card_id)
  {
    case BLUE_CARD_ID_8:
      blue_market_cards.handleLever(server,
                                    board,
                                    respective_player,
                                    player_id,
                                    new_total_amount,
                                    stone_count_difference,
                                    player_stones_at_start,
                                    ship_was_sailed);
      break;

    case BLUE_CARD_ID_9:
      blue_market_cards.handleHammer(server,
                                     action,
                                     board,
                                     respective_player,
                                     player_id,
                                     new_total_amount,
                                     stone_count_difference,
                                     player_stones_at_start,
                                     ship_was_sailed,
                                     input);
      break;

    case BLUE_CARD_ID_10:
      blue_market_cards.handleSails(server,
                                    board,
                                    respective_player,
                                    player_id,
                                    new_total_amount,
                                    stone_count_difference,
                                    player_stones_at_start,
                                    ship_was_sailed,
                                    input);
      break;

    case BLUE_CARD_ID_11:
      blue_market_cards.handleChisel(server,
                                     board,
                                     respective_player,
                                     player_id,
                                     new_total_amount,
                                     stone_count_difference,
                                     player_stones_at_start,
                                     ship_was_sailed,
                                     input);
      break;

  }
}

//-----------------------------------------------------------------------------
int Handler::checkInputValidity(int argc,
                                const char **argv)
{
  int number_of_players;
  unsigned short port;

  if (argc != THREE_ARGUMENTS)
  {
    std::cerr << INVALID_ARGUMENT_NUMBER;
    return WRONG_NUMBER_OF_ARGUMENTS;
  }

  std::string ip {IP};

  if (std::regex_match(argv[AMOUNT_OF_PLAYERS],
                       std::regex(NON_DIGIT)))
  {
    return ARGUMENTS_ARE_WRONG;
  }
  else
  {
    number_of_players = std::stoi(argv[AMOUNT_OF_PLAYERS]);
  }

  if (number_of_players < MINIMUM_AMOUNT_OF_PLAYERS ||
      number_of_players > MAXIMUM_AMOUNT_OF_PLAYERS)
  {
    return ARGUMENTS_ARE_WRONG;
  }

  if (std::regex_match(argv[PORT],
                       std::regex(NON_DIGIT)))
  {
    return ARGUMENTS_ARE_WRONG;
  }
  else
  {
    port = static_cast<unsigned short>(std::stoi(argv[PORT]));
  }

  if (port < MINIMUM_PORT_NUMBER || port > MAXIMUM_PORT_NUMBER)
  {
    return ARGUMENTS_ARE_WRONG;
  }
  return ALL_WENT_WELL;
}

//-----------------------------------------------------------------------------
void Handler::setPortAndPlayers(const char **argv,
                                int &number_of_players,
                                unsigned short &port)
{
  number_of_players = std::stoi(argv[AMOUNT_OF_PLAYERS]);
  port = static_cast<unsigned short>(std::stoi(argv[PORT]));
}

//-----------------------------------------------------------------------------
std::vector<int> Handler::calculateTotalPoints(std::vector<int> immediately_granted_points,
                                               std::vector<int> additional_game_end_points)
{
  std::vector<int> total_points_game_end;

  total_points_game_end.reserve(MAXIMUM_AMOUNT_OF_PLAYERS);
  for (int player_id = PLAYER_ID_0;
       player_id < MAXIMUM_AMOUNT_OF_PLAYERS;
       player_id++)
  {
    total_points_game_end.push_back(immediately_granted_points[player_id] +
                                    additional_game_end_points[player_id]);
  }
  return total_points_game_end;
}


//-----------------------------------------------------------------------------
void Handler::freeMemory(net::Server *server,
                         net::Server::Message *all_players_connected,
                         PlayerMessage *player_message,
                         Board &board)
{
  delete server;
  delete all_players_connected;
  delete player_message;

  for (StorageArea *storageArea : board.all_storage_areas_)
  {
    delete storageArea;
  }
  for (Ship *ship : board.all_ships_)
  {
    delete ship;
  }
  for (Player *player : board.all_players_)
  {
    delete player;
  }
}

//-----------------------------------------------------------------------------
net::Server *Handler::startServer(int number_of_players,
                                  unsigned short port)
{
  try
  {
    std::cout << WAIT_FOR << number_of_players << PLAYERS << std::endl;
    auto *server = new net::Server(port,
                                   IP_SERVER,
                                   number_of_players);
    return server;
  }
  catch (const std::exception &)
  {
    exit(INTERNAL_ERROR);
  }
}

//-----------------------------------------------------------------------------
net::Server::Message *Handler::waitForServerInput(net::Server *server)
{
  try
  {
    net::Server::Message *all_players_connected = new net::Server::Message(server->waitForNewInput());
    return all_players_connected;
  }
  catch (const std::exception &)
  {
    delete server;
    exit(INTERNAL_ERROR);
  }
}

//-----------------------------------------------------------------------------
bool Handler::handleMalformedInput(net::Server *server,
                                   net::Server::Message *input,
                                   PlayerMessage &new_message,
                                   int &player_id)
{
  new_message.setPlayerMessage(input->player_id, input->message);
  bool message_is_malformed_input = new_message.messageIsMalformedInput();
  if (message_is_malformed_input)
  {
    ServerMessage::sendMalformedInput(server,
                                      input->player_id);
  }

  return message_is_malformed_input;
}

//-----------------------------------------------------------------------------
void Handler::handleSailShipExecuted(net::Server *server,
                                     Action *action,
                                     net::Server::Message *input,
                                     Board &board)
{
  // market
  if(action->getSiteId() == MARKET)
  {
    std::vector<int> stones_on_ship = action->getStonesOnShip();

    for (int player_id_market : stones_on_ship)
    {
      handleSailedToMarket(server,
                           input,
                           board,
                           player_id_market);
    }
    return;
  }



  // temple and chamber
  ServerMessage::sendUpdateConstructionSite(server,
                                            action->getSiteId(),
                                            action->getSiteIdArguments());



  // points are granted immediately for this constructionSite
  if (action->getSiteId() == PYRAMID)
  {
    std::vector<int> stones_on_ship = action->getStonesOnShip();
    StorageArea::grantPointsPyramid(stones_on_ship);
    ServerMessage::sendGrantPointsPyramid(server);
    Action::resetPyramid();
  }
}

//-----------------------------------------------------------------------------
void Handler::handleSailedToMarket(net::Server *server,
                                   net::Server::Message *input,
                                   Board &board,
                                   int &player_id_market)
{
  Action *take_market_card;
  ServerMessage::sendRequestInputMarket(server,
                                        player_id_market,
                                        TAKE_MARKET_CARD,
                                        board.all_market_cards_);
  input = waitForServerInput(server);

  PlayerMessage new_message = PlayerMessage();

  bool message_is_malformed_input = handleMalformedInput(server,
                                                         input,
                                                         new_message,
                                                         player_id_market);

  if (!message_is_malformed_input)
  {
    take_market_card = new_message.executeAction();

    if (take_market_card->getActionReturnValue() == TAKE_MARKET_CARD_EXECUTED)
    {
      int market_card_id = std::stoi(take_market_card->getSiteIdArguments());
      ServerMessage::sendUpdateMarket(server,
                                      MARKET,
                                      market_card_id,
                                      board.all_market_cards_);

      int market_id_position = FIRST_MARKET_CARD_POSITION;

      // first field is amount of taken market cards -> increase by one
      board.all_players_[player_id_market]->market_cards_[MARKET_CARD_SIZE] += INCREASE_SIZE_BY_ONE;


      // locate card after all the other market cards, that were already used
      while(board.all_players_[player_id_market]->market_cards_[market_id_position] != USED_MARKET_CARD)
      {
        market_id_position++;
      }

      board.all_players_[player_id_market]->market_cards_[market_id_position] = board.all_market_cards_[market_card_id];


      // player takes green market card
      if (board.all_market_cards_[market_card_id] == GREEN_CARD_ID_3 ||
          board.all_market_cards_[market_card_id] == GREEN_CARD_ID_4 ||
          board.all_market_cards_[market_card_id] == GREEN_CARD_ID_5 ||
          board.all_market_cards_[market_card_id] == GREEN_CARD_ID_6 ||
          board.all_market_cards_[market_card_id] == PURPLE_CARD_ID_7)
      {
        ServerMessage::sendGrantMarketCard(server,
                                           board.all_players_[player_id_market]->market_cards_,
                                           player_id_market,
                                           board.all_market_cards_[market_card_id]);
      }
      if (board.all_market_cards_[market_card_id] == RED_CARD_ID_0)
      {
        ServerMessage::sendPlayedMarketCard(server,
                                            player_id_market,
                                            board.all_market_cards_[market_card_id]);

        StorageArea::pyramid_player_stones_cumulated_.push_back(player_id_market);

        std::string pyramid_arguments = std::to_string(StorageArea::pyramid_player_stones_cumulated_.size()) +
                                        SPACE;

        for (int stone_on_pyramid : StorageArea::pyramid_player_stones_cumulated_)
        {
          pyramid_arguments += std::to_string(stone_on_pyramid) +
                               SPACE;
        }

        pyramid_arguments.erase(pyramid_arguments.size() -
                                DECREASE_SIZE_BY_ONE);

        ServerMessage::sendUpdateConstructionSite(server,
                                                  PYRAMID,
                                                  pyramid_arguments);

        std::vector<int> stones_on_ship;
        stones_on_ship.push_back(player_id_market);
        StorageArea::grantPointsPyramid(stones_on_ship);
        ServerMessage::sendGrantPointsPyramid(server);
        Action::resetPyramid();
      }

      if (board.all_market_cards_[market_card_id] == RED_CARD_ID_1)
      {
        ServerMessage::sendPlayedMarketCard(server,
                                            player_id_market,
                                            board.all_market_cards_[market_card_id]);

        StorageArea::chamber_player_stones_cumulated_.push_back(player_id_market);


        std::string chamber_arguments = std::to_string(StorageArea::chamber_player_stones_cumulated_.size()) +
                                        SPACE;

        for (int stone_on_chamber : StorageArea::chamber_player_stones_cumulated_)
        {
          chamber_arguments += std::to_string(stone_on_chamber) +
                               SPACE;
        }

        chamber_arguments.erase(chamber_arguments.size() -
                                DECREASE_SIZE_BY_ONE);


        ServerMessage::sendUpdateConstructionSite(server,
                                                  CHAMBER,
                                                  chamber_arguments);
      }

      if (board.all_market_cards_[market_card_id] == RED_CARD_ID_2)
      {
        int total_number_of_players = StorageArea::obelisk_player_and_height_.size();

        ServerMessage::sendPlayedMarketCard(server,
                                            player_id_market,
                                            board.all_market_cards_[market_card_id]);

        StorageArea::obelisk_player_and_height_[player_id_market][OBELISK_HEIGHT] += INCREASE_SIZE_BY_ONE;

        std::string obelisk_arguments = std::to_string(total_number_of_players) +
                                        SPACE;

        for (std::vector<int> obelisk_per_player : StorageArea::obelisk_player_and_height_)
        {
          obelisk_arguments += std::to_string(obelisk_per_player[OBELISK_HEIGHT]) +
                               SPACE;
        }

        obelisk_arguments.erase(obelisk_arguments.size() -
                                DECREASE_SIZE_BY_ONE);


        ServerMessage::sendUpdateConstructionSite(server,
                                                  OBELISK,
                                                  obelisk_arguments);
      }

      if (board.all_market_cards_[market_card_id] == BLUE_CARD_ID_8)
      {


        ServerMessage::sendGrantMarketCard(server,
                                           board.all_players_[player_id_market]->market_cards_,
                                           player_id_market,
                                           board.all_market_cards_[market_card_id]);
      }

      if (board.all_market_cards_[market_card_id] == BLUE_CARD_ID_9)
      {
        ServerMessage::sendGrantMarketCard(server,
                                           board.all_players_[player_id_market]->market_cards_,
                                           player_id_market,
                                           board.all_market_cards_[market_card_id]);
      }

      if (board.all_market_cards_[market_card_id] == BLUE_CARD_ID_10)
      {
        ServerMessage::sendGrantMarketCard(server,
                                           board.all_players_[player_id_market]->market_cards_,
                                           player_id_market,
                                           board.all_market_cards_[market_card_id]);
      }

      if (board.all_market_cards_[market_card_id] == BLUE_CARD_ID_11)
      {
        ServerMessage::sendGrantMarketCard(server,
                                           board.all_players_[player_id_market]->market_cards_,
                                           player_id_market,
                                           board.all_market_cards_[market_card_id]);
      }



      board.all_market_cards_[market_card_id] = USED_MARKET_CARD;
    }
    else
    {
      ServerMessage::sendInexecutableInput(server,
                                           input->player_id);
      Handler::handleSailedToMarket(server,
                                    input,
                                    board,
                                    player_id_market);
    }


  }
  delete input;
  delete take_market_card;
}