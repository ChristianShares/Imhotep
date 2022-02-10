//-----------------------------------------------------------------------------
// Player.hpp
//
// Header-Class for players
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#ifndef IMHOTEP_SERVER_Player_HPP
#define IMHOTEP_SERVER_Player_HPP

#include <vector>

class Player
{
public:
    int id_;
    int stones_;
    std::vector<int> market_cards_;

    //-------------------------------------------------------------------------
    ///
    /// Constructor
    ///
    /// @param id the player id
    /// @param stones the amount of stones
    //
    Player(int id, int stones);

};

#endif
