//-----------------------------------------------------------------------------
// Ship.hpp
//
// Header-Class for ships
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_SHIP_HPP
#define IMHOTEP_SERVER_SHIP_HPP

#include <vector>

class Ship
{
  public:
    int id_;
    int length_;
    std::vector<int> spaces_;
    bool is_sailed_;
    int minimun_load_;

    //-------------------------------------------------------------------------
    ///
    /// Constructor
    ///
    /// @param id the ship id
    /// @param length the ship length
    //
    Ship(int id, int length);



    //-------------------------------------------------------------------------
    ///
    /// Generates a vector containing the ships of the respective round.
    ///
    /// @param all_ships the empty ship vector
    /// @param ship_length_0 length of the ship with id 0
    /// @param ship_length_1 length of the ship with id 1
    /// @param ship_length_2 length of the ship with id 2
    /// @param ship_length_3 length of the ship with id 3
    //
    static void generateAllShips(std::vector<Ship *> &all_ships,
                                 int ship_length_0,
                                 int ship_length_1,
                                 int ship_length_2,
                                 int ship_length_3);

};

#endif
