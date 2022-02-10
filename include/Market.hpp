#ifndef IMHOTEP_SERVER_MARKET_HPP
#define IMHOTEP_SERVER_MARKET_HPP

#include "GameHandler.hpp"
#include "Random.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "PlayerMessage.hpp"
#include "Server.hpp"
#include "ServerMessages.hpp"
#include "Ship.hpp"
#include "StorageArea.hpp"

class Market
{
private:
  std::vector<int> all_market_card_points_;

public:

  //---------------------------------------------------------------------------
  ///
  /// Constructor
  //
  Market();


  //---------------------------------------------------------------------------
  ///
  /// Adds the market card points
  ///
  /// @param player_id the respective player
  /// @param all_players all players
  //
  void addMarketPoints(int player_id,
                       std::vector<Player *> &all_players);


  //---------------------------------------------------------------------------
  ///
  /// Getter function
  ///
  /// @return std::vector<int> market cards points of all players
  //
  std::vector<int> getAllMarketPoints();

  //---------------------------------------------------------------------------
  ///
  /// handles the market card chisel
  ///
  /// @param server the initialized server
  /// @param board the game board with ships, players etc.
  /// @param respective_player holds the player ids for turn logic
  /// @param player_id the current player id
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  /// @param ship_was_sailed checks if ship has sailed or not
  /// @param input to wait for server input
  //
  void handleChisel(net::Server *server,
                    Board &board,
                    std::vector<int> &respective_player,
                    int &player_id,
                    int &new_total_amount,
                    int &stone_count_difference,
                    int &player_stones_at_start,
                    bool &ship_was_sailed,
                    net::Server::Message *input);


  //---------------------------------------------------------------------------
  ///
  /// handles the market card lever
  ///
  /// @param server the initialized server
  /// @param board the game board with ships, players etc.
  /// @param respective_player holds the player ids for turn logic
  /// @param player_id the current player id
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  /// @param ship_was_sailed checks if ship has sailed or not
  //
  void handleLever(net::Server *server,
                   Board &board,
                   std::vector<int> &respective_player,
                   int &player_id,
                   int &new_total_amount,
                   int &stone_count_difference,
                   int &player_stones_at_start,
                   bool &ship_was_sailed);


  //---------------------------------------------------------------------------
  ///
  /// handles the market card hammer
  ///
  /// @param server the initialized server
  /// @param action the action holding meta-data
  /// @param board the game board with ships, players etc.
  /// @param respective_player holds the player ids for turn logic
  /// @param player_id the current player id
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  /// @param ship_was_sailed checks if ship has sailed or not
  /// @param input to wait for server input
  //
  void handleHammer(net::Server *server,
                    Action *action,
                    Board &board,
                    std::vector<int> &respective_player,
                    int &player_id,
                    int &new_total_amount,
                    int &stone_count_difference,
                    int &player_stones_at_start,
                    bool &ship_was_sailed,
                    net::Server::Message *input);

  //---------------------------------------------------------------------------
  ///
  /// handles the market card sails
  ///
  /// @param server the initialized server
  /// @param board the game board with ships, players etc.
  /// @param respective_player holds the player ids for turn logic
  /// @param player_id the current player id
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  /// @param ship_was_sailed checks if ship has sailed or not
  /// @param input to wait for server input
  //
  void handleSails(net::Server *server,
                   Board &board,
                   std::vector<int> &respective_player,
                   int &player_id,
                   int &new_total_amount,
                   int &stone_count_difference,
                   int &player_stones_at_start,
                   bool &ship_was_sailed,
                   net::Server::Message *input);

};

#endif
