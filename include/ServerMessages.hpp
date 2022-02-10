//-----------------------------------------------------------------------------
// ServerMessages.hpp
//
// Header-Class for all server messages
//
// Group: 03 (formerly 14.04)
//
// Author:  Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_SERVERMESSAGES_HPP
#define IMHOTEP_SERVER_SERVERMESSAGES_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "PlayerMessage.hpp"
#include "Server.hpp"
#include "Ship.hpp"
#include "StorageArea.hpp"

using std::string;

class ServerMessage
{
private:

public:

  //---------------------------------------------------------------------------
  ///
  /// Generates the server message StartGame
  ///
  /// @param number_of_players the player amount
  /// @param first_message first message in order to get the player names
  /// @param all_players vector with the stone count of the players

  /// @return the string used in the StartRound-server-message
  //

  static void sendStartGameMessage(net::Server *server,
                                   int &number_of_players,
                                   PlayerMessage *first_message,
                                   std::vector<Player *> &all_players);

  //---------------------------------------------------------------------------
  ///
  /// Generates the server message StartRound
  ///
  /// @param all_ships the ships used in the respective round

  /// @return the string used in the StartRound-server-message
  //
  static void sendStartRoundMessage(net::Server *server,
                                    std::vector<int> &all_market_cards,
                                    std::vector<Ship *> &all_ships);

  //---------------------------------------------------------------------------
  ///
  /// Generates the server message EndRound
  ///
  /// @param server the initialized server
  //
  static void sendEndRound(net::Server *server);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message StartTurn
  ///
  /// @param server the initialized server
  /// @param respective_player to get the player turn
  /// @param player_id the position in respective_player
  //
  static void sendStartTurnMessage(net::Server *server,
                                   std::vector<int> &respective_player,
                                   int player_id);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message EndTurn
  ///
  /// @param server the initialized server
  /// @param respective_player to get the player turn
  /// @param player_id the position in respective_player
  //
  static void sendEndTurnMessage(net::Server *server,
                                 std::vector<int> &respective_player,
                                 int player_id);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message GrantPoints
  ///
  /// @param server the initialized server
  /// @param player_id the position in granted_points
  /// @param granted_points the points of each player
  //
  static void sendGrandPointsMessage(net::Server *server,
                                     int player_id,
                                     std::vector<int> new_granted_points,
                                     std::vector<int> total_granted_points);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message GrantPoints (for the pyramid)
  ///
  /// @param server the initialized server
  //
  static void sendGrantPointsPyramid(net::Server *server);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message GrantMarketCard
  ///
  /// @param server the initialized server
  /// @param market_cards all market cards
  /// @param player_id the position in granted_points
  /// @param card_type the type of the market card
  //
  static void sendGrantMarketCard(net::Server *server,
                                  std::vector<int> market_cards,
                                  int player_id,
                                  int card_type);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message EndGame
  ///
  /// @param server the initialized server
  /// @param number_of_players the total amount of players
  //
  static void sendEndGameMessage(net::Server *server,
                                 Action *action,
                                 int number_of_players);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message RequestInput
  ///
  /// @param server the initialized server
  /// @param respective_player to get the player turn
  /// @param player_id the position in respective_player
  //
  static void sendRequestInput(net::Server *server,
                               std::vector<int> &respective_player,
                               int player_id,
                               int input_type,
                               std::vector<int> &all_market_cards);

  //---------------------------------------------------------------------------
  ///
  /// Generates the server message RequestInput (for the market)
  ///
  /// @param server the initialized server
  /// @param player_id the player requesting
  /// @param input_type the input type
  /// @param all_market_cards all market cards
  //
  static void sendRequestInputMarket(net::Server *server,
                                     int player_id,
                                     int input_type,
                                     std::vector<int> &all_market_cards);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message GetNewStones
  ///
  /// @param server the initialized server
  /// @param all_players vector containing all players
  /// @param input to get the player that has the turn
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  //
  static void sendGetNewStones(net::Server *server,
                               std::vector<Player *> all_players,
                               net::Server::Message *input,
                               int &new_total_amount,
                               int &stone_count_difference,
                               int &player_stones_at_start);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message WrongPlayer
  ///
  /// @param server the initialized server
  /// @param input to get the player that has the turn
  //
  static void sendWrongPlayer(net::Server *server, net::Server::Message *input);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message InexecutableInput
  ///
  /// @param server the initialized server
  /// @param player_id the player who made the input
  //
  static void sendInexecutableInput(net::Server *server,
                                    int &player_id);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message PlaceStoneOnShip
  ///
  /// @param server the initialized server
  /// @param all_players vector containing all players
  /// @param input to get the player that has the turn
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param new_total_amount new stone amount after stone placement
  //
  static void sendPlaceStoneOnShip(net::Server *server,
                                   std::vector<Player *> all_players,
                                   net::Server::Message *input,
                                   int &stone_count_difference,
                                   int &new_total_amount);

  //---------------------------------------------------------------------------
  ///
  /// Generates the server message UpdateShip
  ///
  /// @param server the initialized server
  /// @param action the action holding meta-data
  /// @param all_ships all ships
  //
  static void sendUpdateShip(net::Server *server,
                             Action *action,
                             std::vector<Ship *> &all_ships);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message UpdateConstructionSite
  ///
  /// @param server the initialized server
  /// @param site_id the site id
  /// @param site_id_arguments arguments as a string
  //
  static void sendUpdateConstructionSite(net::Server *server,
                                         int site_id,
                                         std::string site_id_arguments);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message UpdateMarket
  ///
  /// @param server the initialized server
  /// @param site_id the site id
  /// @param market_card_id the idea of the changed market card
  /// @param market_cards all market cards
  //
  static void sendUpdateMarket(net::Server *server,
                               int site_id,
                               int market_card_id,
                               std::vector<int> market_cards);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message SailedShip
  ///
  /// @param server the initialized server
  /// @param action the action holding meta-data
  //
  static void sendSailedShip(net::Server *server,
                             Action *action);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message PlayedMarketCard
  ///
  /// @param server the initialized server
  /// @param player_id the player who played the market cards
  /// @param market_card_id the id of the market card
  //
  static void sendPlayedMarketCard(net::Server *server,
                                   int player_id,
                                   int market_card_id);


  //---------------------------------------------------------------------------
  ///
  /// Generates the server message MalformedInput
  ///
  /// @param server the initialized server
  /// @param player_id the player who sent a malformed input
  //
  static void sendMalformedInput(net::Server *server,
                                 int player_id);
};

#endif
