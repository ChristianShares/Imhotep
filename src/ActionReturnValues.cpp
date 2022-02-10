//------------------------------------------------------------------------------
// ActionReturnValues.cpp
//
// Class for action return values
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//------------------------------------------------------------------------------
//

#include "../include/ActionReturnValues.hpp"
#include <vector>
#include <map>

const std::string SPACE = " ";

// delete last space in argument string
const int DELETE_LAST_SPACE = 1;

enum Stones
{
  ONE_STONE = 1,
  ZERO_STONES = 0,
  NO_STONE = -1
};

enum PlayerInfo
{
  PLAYER_ID_0 = 0,
  MAX_AMOUNT_OF_PLAYERS = 4
};

enum PointCalculation
{
  ZERO_POINTS = 0,
  OBELISK_HEIGHT = 1
};

std::vector<int> Action::granted_points_total_;
std::vector<int> Action::pyramid_granted_points_;
std::vector<int> Action::temple_granted_points_;
std::vector<int> Action::obelisk_granted_points_;
std::vector<int> Action::granted_points_chamber_;

//-----------------------------------------------------------------------------
Action::Action()
{
  static bool was_already_constructed = false;

  if (!was_already_constructed)
  {
    for (int player_id = PLAYER_ID_0;
         player_id < MAX_AMOUNT_OF_PLAYERS;
         player_id++)
    {
      granted_points_total_.push_back(ZERO_POINTS);
      obelisk_granted_points_.push_back(ZERO_POINTS);
      pyramid_granted_points_.push_back(ZERO_POINTS);
      temple_granted_points_.push_back(ZERO_POINTS);
      granted_points_chamber_.push_back(ZERO_POINTS);
    }
  }
  was_already_constructed = true;
}

//-----------------------------------------------------------------------------
void Action::setShipId(int ship_id)
{
  ship_id_ = ship_id;
}

//-----------------------------------------------------------------------------
void Action::setStonesOnShip(std::vector<int> &stones_on_ship)
{
  stones_on_ship_ = stones_on_ship;
}

//-----------------------------------------------------------------------------
void Action::setSiteId(int site_id)
{
  site_id_ = site_id;
}

//-----------------------------------------------------------------------------
void Action::setActionReturnValue(int action_return_value)
{
  action_return_value_ = action_return_value;
}

//-----------------------------------------------------------------------------
void Action::setSiteIdArgumentsObelisk(const std::vector<std::vector<int>>& player_and_height)
{
  site_id_arguments_.push_back(player_and_height.size());

  for (const auto & player_id : player_and_height)
  {
    site_id_arguments_.push_back(player_id[OBELISK_HEIGHT]);
  }
}

//-----------------------------------------------------------------------------
void Action::increaseGrantedPointsTotal(int granted_points,
                                        int player_id)
{
  granted_points_total_[player_id] = granted_points_total_[player_id] +
                                     granted_points;
}


//-----------------------------------------------------------------------------
void Action::setGrantedPointsObelisk(int granted_points,
                                     int player_id)
{
  obelisk_granted_points_[player_id] = granted_points;
}

//-----------------------------------------------------------------------------
void Action::setGrantedPointsPyramid(int granted_points,
                                     int player_id)
{
  pyramid_granted_points_[player_id] = pyramid_granted_points_[player_id] +
                                       granted_points;
}

//-----------------------------------------------------------------------------
void Action::setGrantedPointsTemple(int granted_points,
                                    int player_id)
{
  temple_granted_points_[player_id] = temple_granted_points_[player_id] +
                                      granted_points;
}

//-----------------------------------------------------------------------------
void Action::setGrantedPointsChamber(int granted_points,
                                     int player_id)
{
  granted_points_chamber_[player_id] = granted_points_chamber_[player_id] +
                                       granted_points;
}

//-----------------------------------------------------------------------------
int Action::getShipId()
{
  return ship_id_;
}

//-----------------------------------------------------------------------------
std::vector<int> Action::getStonesOnShip()
{
  // remove empty places
  for (int stone_position = (int) stones_on_ship_.size() -
                            ONE_STONE;
       stone_position >= ZERO_STONES;
       stone_position--)
  {
    if (stones_on_ship_[stone_position] == NO_STONE)
    {
      stones_on_ship_.erase(stones_on_ship_.begin() +
                            stone_position);
    }
  }

  return stones_on_ship_;
}

//-----------------------------------------------------------------------------
int Action::getSiteId()
{
  return site_id_;
}

//-----------------------------------------------------------------------------
int Action::getActionReturnValue()
{
  return action_return_value_;
}

//-----------------------------------------------------------------------------
std::string Action::getSiteIdArguments()
{
  std::string args_as_string;

  for (int arg : site_id_arguments_)
  {
    args_as_string += std::to_string(arg) +
                      SPACE;
  }

  args_as_string.erase(args_as_string.size() -
                       DELETE_LAST_SPACE);

  return args_as_string;
}

//-----------------------------------------------------------------------------
std::vector<int> Action::getTotalGrantedPoints()
{
  return granted_points_total_;
}


//-----------------------------------------------------------------------------
std::vector<int> Action::getGrantedPointsObelisk()
{
  return obelisk_granted_points_;
}

//-----------------------------------------------------------------------------
void Action::resetTemple()
{
  for (int &score_for_every_player : temple_granted_points_)
  {
    score_for_every_player = ZERO_POINTS;
  }
}

//-----------------------------------------------------------------------------
void Action::resetPyramid()
{
  for (int &score_for_every_player : pyramid_granted_points_)
  {
    score_for_every_player = ZERO_POINTS;
  }
}

//-----------------------------------------------------------------------------
std::vector<int> Action::getGrantedPointsPyramid()
{
  return pyramid_granted_points_;
}

//-----------------------------------------------------------------------------
std::vector<int> Action::getGrantedPointsTemple()
{
  return temple_granted_points_;
}

//-----------------------------------------------------------------------------
std::vector<int> Action::getGrantedPointsChamber()
{
  return granted_points_chamber_;
}

//-----------------------------------------------------------------------------
void Action::setSiteIdArgumentsPyramid(std::vector<int> &pyramid_player_stones_cumulated_)
{
  site_id_arguments_.push_back(pyramid_player_stones_cumulated_.size());

  for (const auto & player_id : pyramid_player_stones_cumulated_)
  {
    site_id_arguments_.push_back(player_id);
  }
}

//-----------------------------------------------------------------------------
void Action::setSiteIdArgumentsTemple(std::vector<int> &temple_player_stones_cumulated)
{
  site_id_arguments_.push_back(temple_player_stones_cumulated.size());

  for (const auto & player_id : temple_player_stones_cumulated)
  {
    site_id_arguments_.push_back(player_id);
  }
}

//-----------------------------------------------------------------------------
void Action::setSiteIdArgumentsChamber(std::vector<int> &chamber_player_stones_cumulated)
{
  site_id_arguments_.push_back(chamber_player_stones_cumulated.size());

  for (const auto & player_id : chamber_player_stones_cumulated)
  {
    site_id_arguments_.push_back(player_id);
  }
}

//-----------------------------------------------------------------------------
void Action::setSiteIdArgumentsMarket(int market_id)
{
  site_id_arguments_.push_back(market_id);
}
