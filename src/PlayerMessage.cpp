//------------------------------------------------------------------------------
// PlayerMessage.cpp
//
// Class for player messages
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//------------------------------------------------------------------------------
//

#include "../include/PlayerMessage.hpp"
#include <iostream>
#include <locale>
#include <regex>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

const std::string ALL_PLAYERS_CONNECTED = "allplayersconnected";
const std::string GET_NEW_STONES = "getnewstones";
const std::string PLACE_STONE_ON_SHIP = "placestoneonship";
const std::string SAIL_SHIP = "sailship";
const std::string PLAY_BLUE_MARKET_CARD = "playbluemarketcard";
const std::string TAKE_MARKET_CARD = "takemarketcard";
const std::string SAIL_AND_UNLOAD_SHIP = "sailandunloadship";

// Regex
const std::string NON_DIGIT = "\\D";
const std::string WHITE_SPACE = "\\s";

const int MAX_STONES_OF_PLAYERS = 5;
const int INTERNAL_ERROR = -1;

// Diverse Indexing
const int CARD_ID = 0;
const int CARD_INDEX = 0;
const int LOWEST_CARD_INDEX = 0;
const int HIGHEST_CARD_INDEX = 3;
const int FIRST_UNLOAD_ORDER_POSITION = 3;

enum RegexInfo
{
  VALUE_WAS_SET = 1,
  LOWERCASE_INPUTS = 0,
  PLAYER_NAME_AMOUNT = 0,
  ONE_INPUT_ARGUMENT = 1,
  TWO_INPUT_ARGUMENTS = 2,
  FOUR_INPUT_ARGUMENTS = 4,
  TWO_MESSAGE_ARGUMENTS = 2
};

enum PlayerActions
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

enum ConstructionSites
{
  MARKET = 0,
  PYRAMID = 1,
  TEMPLE = 2,
  CHAMBER = 3,
  OBELISK = 4
};

enum ShipInfo
{
  ZERO_LOADED_SPACES = 0,
  SHIP_ID = 0,
  POSITION_ON_SHIP = 1,
  NO_STONE_ON_PLACE = -1,
  ONE_PLAYER_STONE = 1,
  THREE_PLAYER_STONES = 3
};

enum StorageAreaInfo
{
  SITE_ID = 1,
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



std::vector<Player *> PlayerMessage::all_players_;
std::vector<Ship *> PlayerMessage::all_ships_;
std::vector<StorageArea *> PlayerMessage::all_storage_areas_;

//-----------------------------------------------------------------------------
PlayerMessage::PlayerMessage()
{
}

//-----------------------------------------------------------------------------
PlayerMessage::PlayerMessage(std::vector<Player *> players,
                             std::vector<StorageArea *> storage_areas,
                             std::vector<Ship *> ships)
{
  all_players_ = players;
  all_ships_ = ships;
  all_storage_areas_ = storage_areas;
}

//-----------------------------------------------------------------------------
void PlayerMessage::setPlayerMessage(int player_id, std::string message)
{
  this->player_id_ = player_id;
  splitMessage(this, message);
}

//-----------------------------------------------------------------------------
void PlayerMessage::splitMessage(PlayerMessage *message, std::string text)
{
  std::vector<std::string> splitted_text;
  std::istringstream string_stream(text);
  for (std::string string; string_stream >> string;)
  {
    splitted_text.push_back(string);
  }
  int iterator = 0;
  int message_type = 0;

  for (std::string value : splitted_text)
  {
    if (iterator == LOWERCASE_INPUTS)
    {
      for (auto &c : value)
        c = std::tolower(c);

      if (value == ALL_PLAYERS_CONNECTED ||
          value == GET_NEW_STONES ||
          value == PLACE_STONE_ON_SHIP ||
          value == SAIL_SHIP ||
          value == PLAY_BLUE_MARKET_CARD ||
          value == TAKE_MARKET_CARD ||
          value == SAIL_AND_UNLOAD_SHIP)
      {
        message->type_ = value;
        message_type = VALUE_WAS_SET;
      }
    }
    if (iterator > LOWERCASE_INPUTS)
    {
      if (message_type == VALUE_WAS_SET)
        message->arguments_.push_back(value);
    }
    iterator++;
  }
}

//-----------------------------------------------------------------------------
bool PlayerMessage::messageIsMalformedInput()
{
  if (this->type_ == ALL_PLAYERS_CONNECTED)
  {
    if (!this->arguments_.empty())
    {
      int iterator = 0;
      int amount_player = 0;
      int player_name_count = 0;

      for (std::string argument : this->arguments_)
      {
        if (std::regex_match(argument, std::regex(WHITE_SPACE)))
        {
          return true;
        }
        if (iterator == PLAYER_NAME_AMOUNT)
        {
          if (std::regex_match(argument, std::regex(NON_DIGIT)))
          {
            return true;
          }
          amount_player = std::stoi(argument);
        }
        if (iterator > PLAYER_NAME_AMOUNT)
        {
          player_name_count++;
        }
        iterator++;
      }
      if (amount_player != player_name_count)
      {
        return true;
      }
    }
    else
    {
      return true;
    }
  }
  else if (this->type_ == GET_NEW_STONES)
  {
    if (!this->arguments_.empty())
    {
      return true;
    }
  }
  else if (this->type_ == PLACE_STONE_ON_SHIP)
  {
    if (this->arguments_.size() != TWO_INPUT_ARGUMENTS)
    {
      return true;
    }
    else
    {
      if (std::regex_match(arguments_[SHIP_ID],
                           std::regex(NON_DIGIT)) ||
          std::regex_match(arguments_[POSITION_ON_SHIP],
                           std::regex(NON_DIGIT)))
      {
        return true;
      }
    }
  }
  else if (this->type_ == SAIL_SHIP)
  {
    if (this->arguments_.size() != TWO_INPUT_ARGUMENTS)
    {
      return true;
    }
    else
    {
      if (std::regex_match(arguments_[SHIP_ID],
                           std::regex(NON_DIGIT)) ||
          std::regex_match(arguments_[SITE_ID],
                           std::regex(NON_DIGIT)))
      {
        return true;
      }
    }
  }
  else if (this->type_ == PLAY_BLUE_MARKET_CARD)
  {
    if (this->arguments_.size() != ONE_INPUT_ARGUMENT)
    {
      return true;
    }
    else
    {
      if (std::regex_match(arguments_[CARD_ID],
                           std::regex(NON_DIGIT)))
      {
        return true;
      }
    }
  }
  else if (this->type_ == TAKE_MARKET_CARD)
  {
    if (this->arguments_.size() != ONE_INPUT_ARGUMENT)
    {
      return true;
    }
    else
    {
      if (std::regex_match(arguments_[CARD_INDEX],
                           std::regex(NON_DIGIT)))
      {
        return true;
      }
    }
  }
  else if (this->type_ == SAIL_AND_UNLOAD_SHIP)
  {
    if (this->arguments_.size() < FOUR_INPUT_ARGUMENTS)
    {
      return true;
    }
    else
    {
      int iterator = 0;
      int length_unload_order_array = 0;
      int array_check = 0;

      for (std::string argument : this->arguments_)
      {
        if (std::regex_match(argument, std::regex(NON_DIGIT)))
        {
          return true;
        }
        if (iterator == TWO_MESSAGE_ARGUMENTS)
        {
          length_unload_order_array = std::stoi(argument);
        }
        if (iterator > TWO_MESSAGE_ARGUMENTS)
        {
          array_check++;
        }
        iterator++;
      }
      if (length_unload_order_array != array_check)
      {
        return true;
      }
    }
  }
  else
  {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executeAction()
{
  if (this->type_ == GET_NEW_STONES)
  {
    return executeGetNewStones();
  }
  else if (this->type_ == PLACE_STONE_ON_SHIP)
  {
    return executePlaceStoneOnShip();
  }
  else if (this->type_ == SAIL_SHIP)
  {
    return executeSailShip();
  }
  else if (this->type_ == TAKE_MARKET_CARD)
  {
    return executeTakeMarketCard();
  }
  else if (this->type_ == PLAY_BLUE_MARKET_CARD)
  {
    return executePlayBlueCard();
  }
  else if (this->type_ == SAIL_AND_UNLOAD_SHIP)
  {
    return executeSailUnloadShip();
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executeGetNewStones()
{
  Player *player = getPlayerById(this->player_id_);
  Action *action = new Action();
  action->setActionReturnValue(GET_NEW_STONES_ERROR);
  if (player != NULL)
  {
    if (player->stones_ < THREE_PLAYER_STONES)
    {
      player->stones_ = player->stones_ + THREE_PLAYER_STONES;
      action->setActionReturnValue(GET_NEW_STONES_EXECUTED);
      return action;
    }
    else if (player->stones_ == MAX_STONES_OF_PLAYERS)
    {
      return action;
    }
    else
    {
      player->stones_ = MAX_STONES_OF_PLAYERS;
    }
    action->setActionReturnValue(GET_NEW_STONES_EXECUTED);
    return action;
  }
  else
  {
    return action;
  }
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executePlaceStoneOnShip()
{
  int ship_id = std::stoi(this->arguments_[SHIP_ID]);
  int position_on_ship = std::stoi(this->arguments_[POSITION_ON_SHIP]);

  Player *player = getPlayerById(this->player_id_);
  Ship *ship = getShipById(ship_id);
  Action *action = new Action();
  action->setActionReturnValue(PLACE_STONE_ON_SHIP_ERROR);

  if (player != NULL &&
      ship != NULL &&
      player->stones_ >= ONE_PLAYER_STONE &&
      position_on_ship <= ship->length_)
  {
    if (ship->is_sailed_ || ship->spaces_[position_on_ship] != NO_STONE_ON_PLACE)
    {
      return action;
    }
    player->stones_ = player->stones_ -
                      ONE_PLAYER_STONE;

    ship->spaces_[position_on_ship] = player->id_;

    action->setShipId(ship_id);
    action->setStonesOnShip(ship->spaces_);
    action->setActionReturnValue(PLACE_STONE_ON_SHIP_EXECUTED);
    return action;
  }
  return action;
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executeSailShip()
{
  int ship_id = std::stoi(this->arguments_[SHIP_ID]);
  int site_id = std::stoi(this->arguments_[SITE_ID]);
  Ship *ship = getShipById(ship_id);
  StorageArea *storage_area = getStorageAreaById(site_id);
  Action *action = new Action();
  action->setActionReturnValue(SAIL_SHIP_ERROR);

  if (ship != NULL && storage_area != NULL &&
      ((site_id == OBELISK) ||
       (site_id == PYRAMID) ||
       (site_id == TEMPLE) ||
       (site_id == CHAMBER) ||
       (site_id == MARKET)))
  {
    if (ship->is_sailed_)
    {
      return action;
    }
    if (!storage_area->is_free_)
    {
      return action;
    }
    int loaded_spaces = ZERO_LOADED_SPACES;
    for (int space : ship->spaces_)
    {
      if (space != NO_STONE_ON_PLACE)
      {
        loaded_spaces++;
      }
    }

    if (loaded_spaces < ship->minimun_load_)
    {
      return action;
    }
    for (int space : ship->spaces_)
    {
      if (site_id == PYRAMID)
      {
        StorageArea::pushStonesToPyramid(space);
      }
      if (site_id == TEMPLE)
      {
        StorageArea::pushStonesToTemple(space);
      }
      if (site_id == CHAMBER)
      {
        StorageArea::pushStonesToChamber(space);
      }
      if (site_id == OBELISK)
      {
        StorageArea::pushStonesToObelisk(space, storage_area->obelisk_player_and_height_);
      }
    }

    action->setActionReturnValue(SAIL_SHIP_EXECUTED);
    action->setShipId(ship_id);
    action->setSiteId(site_id);

    // set SiteIdArguments for UpdateConstructionSite (class: ServerMessage)
    switch (site_id)
    {
      case MARKET:
        action->setSiteIdArgumentsMarket(std::stoi(this->arguments_[SITE_ID]));
        break;

      case OBELISK:
        action->setSiteIdArgumentsObelisk(storage_area->obelisk_player_and_height_);
        break;

      case PYRAMID:
        action->setSiteIdArgumentsPyramid(storage_area->pyramid_player_stones_cumulated_);
        break;

      case TEMPLE:
        action->setSiteIdArgumentsTemple(storage_area->temple_player_stones_cumulated_);
        break;

      case CHAMBER:
        action->setSiteIdArgumentsChamber(storage_area->chamber_player_stones_cumulated_);
    }

    action->setStonesOnShip(ship->spaces_);
    for (auto &value : ship->spaces_)
    {
      value = NO_STONE_ON_PLACE;
    }
  }
  return action;
}

//-----------------------------------------------------------------------------
Player *PlayerMessage::getPlayerById(int id)
{
  for (Player *player : all_players_)
  {
    if (player->id_ == id)
    {
      return player;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Ship *PlayerMessage::getShipById(int id)
{
  for (Ship *ship : all_ships_)
  {
    if (ship->id_ == id)
    {
      return ship;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
StorageArea *PlayerMessage::getStorageAreaById(int id)
{
  for (StorageArea *storage_area : all_storage_areas_)
  {
    if (storage_area->id_ == id)
    {
      return storage_area;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
std::vector<std::string> PlayerMessage::getArguments()
{
  return arguments_;
}

//-----------------------------------------------------------------------------
void PlayerMessage::setAllShips(std::vector<Ship *> &all_ships)
{
  all_ships_ = all_ships;
}

//-----------------------------------------------------------------------------
PlayerMessage *PlayerMessage::processFirstPlayerMessage(net::Server::Message *all_players_connected,
                                                        std::vector<Player *> &players,
                                                        std::vector<StorageArea *> &storage_areas,
                                                        std::vector<Ship *> &ships)
{
  PlayerMessage *first_message = new PlayerMessage(players,
                                                   storage_areas,
                                                   ships);

  first_message->setPlayerMessage(all_players_connected->player_id,
                                  all_players_connected->message);

  if (first_message->messageIsMalformedInput())
  {
    delete first_message;
    for (StorageArea *storageArea : storage_areas)
    {
      delete storageArea;
    }
    for (Ship *ship : ships)
    {
      delete ship;
    }
    for (Player *player : players)
    {
      delete player;
    }
    exit(INTERNAL_ERROR);
  }
    return first_message;
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executeTakeMarketCard()
{
  Action *action = new Action();
  int index_market_card = std::stoi(this->arguments_[CARD_INDEX]);

  if (index_market_card < LOWEST_CARD_INDEX ||
      index_market_card > HIGHEST_CARD_INDEX)
  {
    action->setActionReturnValue(TAKE_MARKET_CARD_ERROR);
  }
  else
  {
    action->setSiteIdArgumentsMarket(std::stoi(this->arguments_[CARD_INDEX]));
    action->setActionReturnValue(TAKE_MARKET_CARD_EXECUTED);
  }

  return action;
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executePlayBlueCard()
{
  Action *action = new Action();
  int market_card_id = std::stoi(this->arguments_[CARD_ID]);

  if (market_card_id < BLUE_CARD_ID_8 ||
      market_card_id > BLUE_CARD_ID_11)
  {
    action->setActionReturnValue(PLAY_BLUE_CARD_ERROR);
    return action;
  }
  else if (market_card_id == BLUE_CARD_ID_9)
  {
    delete action;
    Action *get_new_stones_action = executeGetNewStones();
    get_new_stones_action->setSiteIdArgumentsMarket(market_card_id);
    get_new_stones_action->setActionReturnValue(PLAY_BLUE_CARD_EXECUTED);
    return get_new_stones_action;
  }
  else
  {
    action->setSiteIdArgumentsMarket(market_card_id);
    action->setActionReturnValue(PLAY_BLUE_CARD_EXECUTED);
    return action;
  }
}

//-----------------------------------------------------------------------------
Action *PlayerMessage::executeSailUnloadShip()
{
  Action *action = new Action();
  action->setActionReturnValue(SAIL_UNLOAD_SHIP_ERROR);

  int ship_id = std::stoi(this->arguments_[SHIP_ID]);
  int site_id = std::stoi(this->arguments_[SITE_ID]);

  Ship *ship = getShipById(ship_id);

  // basic check if ships and site_ids are valid
  if (ship != NULL &&
      ((site_id == OBELISK) ||
       (site_id == PYRAMID) ||
       (site_id == TEMPLE) ||
       (site_id == CHAMBER) ||
       (site_id == MARKET)))
  {
    if (ship->is_sailed_)
    {
      return action;
    }
  }
  else
  {
    return action;
  }

  int ship_size = ship->length_;
  std::vector<int> actual_stones_on_ship = ship->spaces_;
  // this will be the order with which the stones will be pushed to the site
  std::vector<int> new_order_spaces;

  int amount_elements_unload_order = (int) this->arguments_.size();

  // this vector will be used to update the stones later on
  std::vector<int> unload_order;

  // erase unused stones
  for (int stone_position = ship_size - ONE_PLAYER_STONE;
       stone_position >= ZERO_LOADED_SPACES;
       stone_position--)
  {
    if (actual_stones_on_ship[stone_position] == NO_STONE_ON_PLACE)
    {
      actual_stones_on_ship.erase(actual_stones_on_ship.begin() +
                                  stone_position);
    }
  }

  for (int unload_order_index = FIRST_UNLOAD_ORDER_POSITION;
       unload_order_index < amount_elements_unload_order;
       unload_order_index++)
  {
    unload_order.push_back(std::stoi(this->arguments_[unload_order_index]));
  }

  if (unload_order.size() != actual_stones_on_ship.size())
  {
    action->setActionReturnValue(SAIL_UNLOAD_SHIP_ERROR);
    return action;
  }

  new_order_spaces = actual_stones_on_ship;
  int new_order_position = 0;

  for (int stone_position : unload_order)
  {
    new_order_spaces[new_order_position] = actual_stones_on_ship[stone_position];
    new_order_position++;
  }

  // to check the validity of the input
  std::vector<int> unload_order_validity_compare = new_order_spaces;

  for (int stone_before_market_card : ship->spaces_)
  {
    for (int &stone_after_marked_card : unload_order_validity_compare)
    {
      if (stone_before_market_card == stone_after_marked_card)
      {
        stone_after_marked_card = NO_STONE_ON_PLACE;
      }
    }
  }

  // if only one stone is not on ship, send error
  for (int stone_in_new_order : unload_order_validity_compare)
  {
    if (stone_in_new_order != NO_STONE_ON_PLACE)
    {
      return action;
    }
  }


  ship->spaces_ = new_order_spaces;

  delete action;

  Action *sail_ship = executeSailShip();

  return sail_ship;
}
