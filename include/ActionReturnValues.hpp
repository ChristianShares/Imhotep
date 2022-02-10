//-----------------------------------------------------------------------------
// ActionReturnValues.hpp
//
// Header-Class for Values to store for Messages
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//
#ifndef IMHOTEP_SERVER_ACTIONRETURNVALUES_HPP
#define IMHOTEP_SERVER_ACTIONRETURNVALUES_HPP

#include "Ship.hpp"

#include <map>
#include <vector>

class Action
{
  private:
    int ship_id_;
    std::vector<int> stones_on_ship_;
    int site_id_;
    int action_return_value_;
    std::vector<int> site_id_arguments_;
    static std::vector<int> granted_points_total_;
    static std::vector<int> pyramid_granted_points_;
    static std::vector<int> temple_granted_points_;
    static std::vector<int> obelisk_granted_points_;
    static std::vector<int> granted_points_chamber_;
    
  public:

    //---------------------------------------------------------------------------
    ///
    /// Constructor
    //
    Action();

    //-------------------------------------------------------------------------
    ///
    /// Setter for ship_id_
    ///
    /// @param ship_id The ship id
    //
    void setShipId(int ship_id);

    //-------------------------------------------------------------------------
    ///
    /// Setter for stones_on_ship_
    ///
    /// @param stones_on_ship The stones on the ship
    //
    void setStonesOnShip(std::vector<int> &stones_on_ship);

    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_
    ///
    /// @param site_id The site id
    //
    void setSiteId(int site_id);

    //-------------------------------------------------------------------------
    ///
    /// Setter for action_return_value_
    ///
    /// @param action_return_value The return value of the action
    //
    void setActionReturnValue(int action_return_value);

    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_arguments_ Obelisk
    ///
    /// @param player_and_height The player and their stone height
    //
    void setSiteIdArgumentsObelisk(const std::vector<std::vector<int>>&player_and_height);


    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_arguments_ Pyramid
    ///
    /// @param pyramid_player_stones_cumulated_ The pyramid stones
    //
    void setSiteIdArgumentsPyramid(std::vector<int> &pyramid_player_stones_cumulated_);


    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_arguments_ Temple
    ///
    /// @param temple_player_stones_cumulated The temple stones
    //
    void setSiteIdArgumentsTemple(std::vector<int> &temple_player_stones_cumulated);



    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_arguments_ Chamber
    ///
    /// @param chamber_player_stones_cumulated The chamber stones
    //
    void setSiteIdArgumentsChamber(std::vector<int> &chamber_player_stones_cumulated);


    //-------------------------------------------------------------------------
    ///
    /// Setter for site_id_arguments_ Market
    ///
    /// @param market_id The market id
     //
    void setSiteIdArgumentsMarket(int market_id);

    //-------------------------------------------------------------------------
    ///
    /// Setter for granted_points_
    ///
    /// @param granted_points The granted points
    /// @param player_id The player id
    //
    static void setGrantedPointsObelisk(int granted_points,
                                        int player_id);


    //-------------------------------------------------------------------------
    ///
    /// Setter for pyramid_granted_points_
    ///
    /// @param granted_points The granted points
    /// @param player_id The player id
    //
    static void setGrantedPointsPyramid(int granted_points,
                                          int player_id);


    //-------------------------------------------------------------------------
    ///
    /// Setter for temple_granted_points_
    ///
    /// @param granted_points The granted points
    /// @param player_id The player id
    //
    static void setGrantedPointsTemple(int granted_points,
                                       int player_id);

    //-------------------------------------------------------------------------
    ///
    /// Setter for chamber_granted_points_
    ///
    /// @param granted_points The granted points
    /// @param player_id The player id
    //

    static void setGrantedPointsChamber(int granted_points,
                                        int player_id);


    //-------------------------------------------------------------------------
    ///
    /// Setter for granted_points_total_
    ///
    /// @param granted_points The granted points
    /// @param player_id The player id
    //
    static void increaseGrantedPointsTotal(int granted_points,
                                           int player_id);


    //-------------------------------------------------------------------------
    ///
    /// Getter for ship_id_
    ///
    /// @return int The ship id
    //
    int getShipId();

    //-------------------------------------------------------------------------
    ///
    /// Getter for stones_on_ship_
    ///
    /// @return vector<int> The stones on the ship
    //
    std::vector<int> getStonesOnShip();

    //-------------------------------------------------------------------------
    ///
    /// Getter for site_id_
    ///
    /// @return int The site id
    //
    int getSiteId();

    //-------------------------------------------------------------------------
    ///
    /// Getter for action_return_value_
    ///
    /// @return int The return value of the action
    //
    int getActionReturnValue();

    //-------------------------------------------------------------------------
    ///
    /// Getter for site_id_arguments_
    ///
    /// @return string The site id arguments
    //
    std::string getSiteIdArguments();

    //-------------------------------------------------------------------------
    ///
    /// Getter for granted points
    ///
    /// @return vector<int> The granted points
    //
    static std::vector<int> getTotalGrantedPoints();


    //-------------------------------------------------------------------------
    ///
    /// Getter for granted points Obelisk
    ///
    /// @return vector<int> The granted points for the Obelisk
    //
    static std::vector<int> getGrantedPointsObelisk();


    //-------------------------------------------------------------------------
    ///
    /// Getter for granted points Pyramid. Also resets the member variable pyramid_granted_points_.
    ///
    /// @return vector<int> The granted points for the Pyramid
    //
    static std::vector<int> getGrantedPointsPyramid();


    //-------------------------------------------------------------------------
    ///
    /// Getter for granted points Temple.
    ///
    /// @return vector<int> The granted points for the Temple
    //
      static std::vector<int> getGrantedPointsTemple();


    //-------------------------------------------------------------------------
    ///
    /// Getter for granted points Chamber.
    ///
    /// @return vector<int> The granted points for the Chamber
    //
    static std::vector<int> getGrantedPointsChamber();


    //-------------------------------------------------------------------------
    ///
    /// Resets the temple
    ///
    //
    static void resetTemple();


    //-------------------------------------------------------------------------
    ///
    /// Resets the temple
    ///
    //
    static void resetPyramid();

};
#endif
