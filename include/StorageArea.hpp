//-----------------------------------------------------------------------------
// StorageArea.hpp
//
// Header-Class for the storage area
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//
#ifndef IMHOTEP_SERVER_STORAGEAREA_HPP
#define IMHOTEP_SERVER_STORAGEAREA_HPP

#include <map>
#include "ActionReturnValues.hpp"

const int PYRAMID_STONE_AMOUNT = 14;

class StorageArea
{
  public:
    int id_;
    static std::vector<std::vector<int>> obelisk_player_and_height_;
    static bool pyramid_occupied_stones_[PYRAMID_STONE_AMOUNT];
    static std::vector<int> temple_player_stones_;

    static std::vector<int> temple_player_stones_cumulated_;
    static std::vector<int> chamber_player_stones_cumulated_;
    static std::vector<int> pyramid_player_stones_cumulated_;
    bool is_free_;

    //-------------------------------------------------------------------------
    ///
    /// Constructor
    ///
    /// @param id the storage area id
    /// @param number_players the amount of players
    //
    StorageArea(int id,
                int number_players);


    //-------------------------------------------------------------------------
    ///
    /// Method to grant the points for the obelisk
    ///
    /// @param amount_of_players The amount of players
    //
    static void grantPointsObelisk(int amount_of_players);

    //-------------------------------------------------------------------------
    ///
    /// Method to grant the points for the pyramid
    ///
    /// @param stones_on_ship the stones on the respective ship
    //
    static void grantPointsPyramid(std::vector<int>& stones_on_ship);


    //-------------------------------------------------------------------------
    ///
    /// Method to grant the points for the temple
    ///
    /// @param number_of_players the total player amount
    //
    static void grantPointsTemple(int number_of_players);


    //-------------------------------------------------------------------------
    ///
    /// Method to grant the points for the chamber
    ///
    //
    static void grantPointsChamber();


    //-------------------------------------------------------------------------
    ///
    /// Method to push the received stone to the obelisk
    ///
    /// @param space the stone
    /// @param obelisk_player_and_height the height of the obelisk for each ...
    /// ... player
    //
    static void pushStonesToObelisk(int space,
                                    std::vector<std::vector<int>> &obelisk_player_and_height);


    //-------------------------------------------------------------------------
    ///
    /// Method to push the received stone to the temple
    ///
    /// @param space the stone
    //
    static void pushStonesToTemple(int space);


    //-------------------------------------------------------------------------
    ///
    /// Method to push the received stone to the chamber
    ///
    /// @param space the stone
    //
    static void pushStonesToChamber(int space);


    //-------------------------------------------------------------------------
    ///
    /// Method to push the received stone to the pyramid
    ///
    /// @param space the stone
    //
    static void pushStonesToPyramid(int space);


    //-------------------------------------------------------------------------
    ///
    /// compares the columns
    ///
    /// @param comp_vector_1 first column
    /// @param comp_vector_2 second column
    ///
    /// @return bool To get how to sort the columns
    //
    static bool sortColumnsReversed(std::vector<int> comp_vector_1,
                                    std::vector<int> comp_vector_2);

    //-------------------------------------------------------------------------
    ///
    /// Method to get the occurence of the players
    ///
    /// @param player_and_height The players and their stone height in the storage area
    /// @param number_of_players The amount of players
    ///
    /// @return vector<vector<int> The occurrences
    //
    static std::vector<std::vector<int>> getOccurrencesCount(std::vector<std::vector<int>> player_and_height,
                                                             int number_of_players);

    //-------------------------------------------------------------------------
    ///
    /// Calculates the points
    ///
    /// @param amount_of_players The amount of players
    /// @param point_ranking The point ranking
    /// @param occurrences The Occurrences
    ///
    /// @return vector<int> The calculated points
    //
    static std::vector<int> calculatePoints(int amount_of_players,
                                            const int *point_ranking,
                                            std::vector<std::vector<int>> &occurrences);


  //-------------------------------------------------------------------------
  ///
  /// Method to generate a market card vector for the round.
  ///
  /// @param market_card_id_0 market card in vector position 0
  /// @param market_card_id_1 market card in vector position 1
  /// @param market_card_id_2 market card in vector position 2
  /// @param market_card_id_3 market card in vector position 3
  //
  static std::vector<int> generateMarketCardVector(std::vector<int> &all_market_cards,
                                                   int market_card_id_0,
                                                   int market_card_id_1,
                                                   int market_card_id_2,
                                                   int market_card_id_3);


};

#endif
