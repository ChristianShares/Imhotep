//-----------------------------------------------------------------------------
// GameHandler.hpp
//
// Header-Class for the game coordination
//
// Group: 03 (formerly 14.04)
//
// Author: Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_GAMEHANDLER_HPP
#define IMHOTEP_SERVER_GAMEHANDLER_HPP

#include "Random.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "PlayerMessage.hpp"
#include "Server.hpp"
#include "ServerMessages.hpp"
#include "Ship.hpp"
#include "StorageArea.hpp"
#include "Market.hpp"

class Handler
{
private:

  //---------------------------------------------------------------------------
  ///
  /// Executes the turns of the players
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

  /// @return Action Object with Return Values.
  //
  Action *executeRoundsAndTurns(net::Server *server,
                                Board &board,
                                std::vector<int> &all_market_cards,
                                std::vector<int> &respective_player,
                                int number_of_players,
                                Action *action);


  //---------------------------------------------------------------------------
  ///
  /// Add the immediately granted points and the points, that need to be ...
  /// ... granted at the end of a game.
  ///
  /// @param the points, that where granted immediately per player
  /// @param the points, that need to be granted at the end of the game per player

  /// @return the vector containing the points at the end of the round
  //
  std::vector<int> calculateTotalPoints(std::vector<int> immediately_granted_points,
                                        std::vector<int> additional_game_end_points);


  //---------------------------------------------------------------------------
  ///
  /// Frees the memory in reference to all variables used in the class on heap.
  ///
  /// @param server the initialized server
  /// @param all_players_connected to wait for server input
  /// @param player_message to process the player message
  /// @param board the game board with ships, players etc.
  //
  static void freeMemory(net::Server *server,
                         net::Server::Message *all_players_connected,
                         PlayerMessage *player_message,
                         Board &board);


  //---------------------------------------------------------------------------
  ///
  /// Starts the server
  ///
  /// @param number_of_players the number of players
  /// @param port the port number

  /// @return net::Server * the initialized server
  //
  net::Server *startServer(int number_of_players, unsigned short port);


  //---------------------------------------------------------------------------
  ///
  /// Here the data is already processed and all objects need to be updated.
  ///
  ///
  /// @param server the initialized server
  /// @param action the action holding meta-data
  /// @param board the game board with ships, players etc.
  /// @param respective_player holds the player ids for turn logic
  /// @param player_id the current player id
  /// @param new_total_amount the total amount of the players stones after the update
  /// @param stone_count_difference the difference of the stones the player has after update
  /// @param player_stones_at_start the stones, a player has at the start of the turn
  /// @param ship_was_sailed info for the turn logic -> round must end then
  /// @param input to wait for server input
  /// @param new_message to process the player message
  //
  void makeActualMove(net::Server *server,
                      Action *action,
                      Board &board,
                      std::vector<int> &respective_player,
                      int &player_id,
                      int &new_total_amount,
                      int &stone_count_difference,
                      int &player_stones_at_start,
                      bool &ship_was_sailed,
                      net::Server::Message *input,
                      PlayerMessage &new_message);



  //---------------------------------------------------------------------------
  ///
  /// Execute Market Action
  ///
  ///
  /// @param server the initialized server
  /// @param input to wait for server input
  /// @param board the game board with ships, players etc.
  /// @param player_id_market the current player id

  //
  void handleSailedToMarket(net::Server *server,
                            net::Server::Message *input,
                            Board &board,
                            int &player_id_market);



  //---------------------------------------------------------------------------
  ///
  /// Handles blue market cards
  ///
  ///
  /// @param server the initialized server
  /// @param input to wait for server input
  /// @param board the game board with ships, players etc.
  /// @param player_id_market the current player id
  //
  void handlePlayBlueCard(net::Server *server,
                          Action *action,
                          Board &board,
                          std::vector<int> &respective_player,
                          int &player_id,
                          int &new_total_amount,
                          int &stone_count_difference,
                          int &player_stones_at_start,
                          bool &ship_was_sailed,
                          net::Server::Message *input);


public:

  //---------------------------------------------------------------------------
  ///
  /// Constructor
  //
  Handler();

  //---------------------------------------------------------------------------
  ///
  /// Checks if the input is valid
  ///
  /// @param argc the argument count
  /// @param argv the argument values

  /// @return int the validity status
  //
  int checkInputValidity(int argc,
                         const char **argv);


  //---------------------------------------------------------------------------
  ///
  /// Sets the port for the server and creates the players
  ///
  /// @param argv the argument values
  /// @param number_of_players amount of players
  /// @param port the port number
  //
  void setPortAndPlayers(const char **argv,
                         int &number_of_players,
                         unsigned short &port);


  //---------------------------------------------------------------------------
  ///
  /// This method will be called in main. It starts with the actual game ...
  /// ... once the formal checks are done.
  ///
  /// @param number_of_players amount of players
  /// @param port the port number

  /// @return int the validity status
  //
  int executeGame(int number_of_players, unsigned short port);


  //---------------------------------------------------------------------------
  ///
  /// In case of malformed input, this function will be called. It handles ...
  /// ... the malformed input.
  ///
  /// @param server the initialized server
  /// @param input to wait for server input
  /// @param new_message to process the player message
  /// @param player_id the current player id

  /// @return bool check if input is malformed or not
  //
  static bool handleMalformedInput(net::Server *server,
                                   net::Server::Message *input,
                                   PlayerMessage &new_message,
                                   int &player_id);

  //---------------------------------------------------------------------------
  ///
  /// Pauses and waits for input from server
  ///
  /// @param server the initialized server

  /// @return net::Server::Message * with id and message
  //
  net::Server::Message *waitForServerInput(net::Server *server);


  //---------------------------------------------------------------------------
  ///
  /// Executes the turns of the players
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

  /// @return Action Object with Return Values.
  //
  Action *executeTurns(net::Server *server,
                       Action *action,
                       Board &board,
                       std::vector<int> &respective_player,
                       int &player_id,
                       int &new_total_amount,
                       int &stone_count_difference,
                       int &player_stones_at_start,
                       bool &ship_was_sailed);



  //---------------------------------------------------------------------------
  ///
  /// Sail ship
  ///
  /// @param server the initialized server
  /// @param action the action holding meta-data
  /// @param input to wait for server input
  /// @param board the game board with ships, players etc.
  //
  void handleSailShipExecuted(net::Server *server,
                              Action *action,
                              net::Server::Message *input,
                              Board &board);
};

#endif
