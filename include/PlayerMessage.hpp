//-----------------------------------------------------------------------------
// PlayerMessage.hpp
//
// Header-Class for player messages
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_PLAYERMESSAGE_HPP
#define IMHOTEP_SERVER_PLAYERMESSAGE_HPP

#include "Player.hpp"
#include "Ship.hpp"
#include "StorageArea.hpp"

#include <Server.hpp>

#include <map>
#include <string>
#include <vector>

class PlayerMessage
{
  private:
    int player_id_;
    std::string type_;
    std::vector<std::string> arguments_;
    static std::vector<Player *> all_players_;
    static std::vector<Ship *> all_ships_;
    static std::vector<StorageArea *> all_storage_areas_;

    //---------------------------------------------------------------------------
    ///
    /// Splits the message into its parts
    ///
    /// @param message the whole message object
    /// @param text the unsplitted messagetext
    ///
    //
    void splitMessage(PlayerMessage* message, std::string text);

    //---------------------------------------------------------------------------
    ///
    /// Executes the command GetNewStone
    ///
    /// @return Action the action object with all return values
    ///
    //
    Action* executeGetNewStones();

    //---------------------------------------------------------------------------
    ///
    /// Executes the command PlaceStoneOnShip
    ///
    /// @return Action the action object with all return values
    ///
    //
    Action* executePlaceStoneOnShip();

    //---------------------------------------------------------------------------
    ///
    /// Executes the command SailShip
    ///
    /// @return Action the action object with all return values
    ///
    //
    Action* executeSailShip();


    //---------------------------------------------------------------------------
    ///
    /// Executes the command TakeMarketCard
    ///
    /// @return Action the action object with all return values
    ///
    //
      Action* executeTakeMarketCard();


    //---------------------------------------------------------------------------
    ///
    /// Executes the command PlayBlueCard
    ///
    /// @return Action the action object with all return values
    ///
    //
      Action* executePlayBlueCard();


    //---------------------------------------------------------------------------
    ///
    /// Executes the command SailUnloadShip
    ///
    /// @return Action the action object with all return values
    ///
    //
      Action* executeSailUnloadShip();

    //---------------------------------------------------------------------------
    ///
    /// Function to get the ship object with a special id
    ///
    /// @param int The ship id
    /// @return Ship the ship object
    ///
    //
    Ship* getShipById(int id);

    //---------------------------------------------------------------------------
    ///
    /// Function to get the storage area object with a special id
    ///
    /// @param int The storage area id
    /// @return StorageArea the storage area object
    ///
    //
    StorageArea* getStorageAreaById(int id);

    //---------------------------------------------------------------------------
    ///
    /// Function to get the player object with a special id
    ///
    /// @param int The player id
    /// @return Player the player object
    ///
    //
    Player* getPlayerById(int id);

  public:
    //---------------------------------------------------------------------------
    ///
    /// Constructor
    //
    PlayerMessage();

    //---------------------------------------------------------------------------
    ///
    /// Constructor
    ///
    /// @param players all initialized players
    /// @param storage_areas all initalized storage areas
    /// @param ships all initalized ships
    //
    PlayerMessage(std::vector<Player *> players, std::vector<StorageArea *> storage_areas,
                  std::vector<Ship *> ships);

    //---------------------------------------------------------------------------
    ///
    /// Fills a new message object
    ///
    /// @param player_id the id of the player
    /// @param message the whole message
    //
    void setPlayerMessage(int player_id, std::string message);

    //---------------------------------------------------------------------------
    ///
    /// Checks if the message is not correct
    ///
    /// @return true if the message is not correct
    //
    bool messageIsMalformedInput();

    //---------------------------------------------------------------------------
    ///
    /// Executes an action
    ///
    /// @return Action the action object with all return values
    ///
    //
    Action* executeAction();

    //---------------------------------------------------------------------------
    ///
    /// Get the input arguments
    ///
    /// @return vector<string> the arguments
    ///
    //
    std::vector<std::string> getArguments();

    //---------------------------------------------------------------------------
    ///
    /// Set all_ships_ at the beginning of each round
    ///
    //
    static void setAllShips(std::vector<Ship *> &all_ships);

    //---------------------------------------------------------------------------
    ///
    /// @param all_players_connected to wait for server input
    /// @param players all players
    /// @param storage_areas all storage areas
    /// @param ships all ships
    ///
    /// @return PlayerMessage * the player message

    //
    static PlayerMessage *processFirstPlayerMessage(net::Server::Message *all_players_connected,
                                                    std::vector<Player *> &players,
                                                    std::vector<StorageArea *> &storage_areas,
                                                    std::vector<Ship *> &ships);

};
#endif
