//------------------------------------------------------------------------------
// StorageArea.cpp
//
// Class for storage areas
//
// Group: 14.04
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//------------------------------------------------------------------------------
//

#include "../include/StorageArea.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

// Points Pyramid
const int POINTS_PER_FIELD_PYRAMID[14] = {2, 1, 3, 2, 4, 3, 2, 1, 3, 2, 3, 1, 3, 4};

// Points Obelisk
const int POINTS_TWO_PLAYERS[2] = {10, 1};
const int POINTS_THREE_PLAYERS[3] = {12, 6, 1};
const int POINTS_FOUR_PLAYERS[4] = {15, 10, 5, 1};

// Stone Information
const int NO_STONE_ON_SHIP = -1;

const int TEMPLE_POINT_PER_STONE = 1;
const int TEMPLE_WIDTH_TWO_PLAYERS = 4;
const int TEMPLE_WIDTH_OVER_TWO = 5;

enum PlayerIds
{
  PLAYER_ID_0 = 0,
  PLAYER_ID_1 = 1,
  PLAYER_ID_2 = 2,
  PLAYER_ID_3 = 3,
  PLAYER_ID = 0
};

enum PlayerAmount
{
  TWO_PLAYERS = 2,
  THREE_PLAYERS = 3,
  FOUR_PLAYERS = 4
};

enum Points
{
  OBELISK_HEIGHT = 1,
  AMOUNT_OF_EQUAL_POINTS = 1,
  HALF_OF_THE_POINTS = 2,
  ONE_OCCURRENCE = 1,
  ZERO_POINTS = 0,
  ZERO_EQUAL_POINTS = 0,
  ONE_POINT = 1
};


std::vector<std::vector<int>> StorageArea::obelisk_player_and_height_;
std::vector<int> StorageArea::temple_player_stones_;
std::vector<int> StorageArea::temple_player_stones_cumulated_;
std::vector<int> StorageArea::chamber_player_stones_cumulated_;
std::vector<int> StorageArea::pyramid_player_stones_cumulated_;
bool StorageArea::pyramid_occupied_stones_[14];

//-----------------------------------------------------------------------------
StorageArea::StorageArea(int id, int number_players)
{
  id_ = id;
  is_free_ = true;

  static bool was_already_constructed = false;

  if (!was_already_constructed)
  {
    for (int player_id = PLAYER_ID_0;
         player_id < number_players;
         player_id++)
    {
      obelisk_player_and_height_.push_back({player_id, ZERO_POINTS});
    }
  }
  if (!was_already_constructed)
  {
    for (bool &block_position : pyramid_occupied_stones_)
    {
      block_position = false;
    }
  }

  was_already_constructed = true;
}

//-----------------------------------------------------------------------------
void StorageArea::grantPointsObelisk(int amount_of_players)
{
  std::vector<std::vector<int>> player_and_height_copy = obelisk_player_and_height_;

  sort(player_and_height_copy.begin(),
       player_and_height_copy.end(),
       sortColumnsReversed);

  std::vector<std::vector<int>> occurrences = getOccurrencesCount(player_and_height_copy, amount_of_players);

  if (amount_of_players == TWO_PLAYERS)
  {
    if ((occurrences[PLAYER_ID_0][AMOUNT_OF_EQUAL_POINTS] > ONE_OCCURRENCE) ||
        (occurrences[PLAYER_ID_1][AMOUNT_OF_EQUAL_POINTS] > ONE_OCCURRENCE))
    {
      Action::setGrantedPointsObelisk(floor((POINTS_TWO_PLAYERS[PLAYER_ID_0] + POINTS_TWO_PLAYERS[PLAYER_ID_1])/
                                            HALF_OF_THE_POINTS),
                                      player_and_height_copy[PLAYER_ID_0][PLAYER_ID]);

      Action::setGrantedPointsObelisk(floor((POINTS_TWO_PLAYERS[PLAYER_ID_0] + POINTS_TWO_PLAYERS[PLAYER_ID_1]) /
                                            HALF_OF_THE_POINTS),
                                      player_and_height_copy[PLAYER_ID_1][PLAYER_ID]);
    }
    else
    {
      Action::setGrantedPointsObelisk(POINTS_TWO_PLAYERS[PLAYER_ID_0],
                                      player_and_height_copy[PLAYER_ID_0][PLAYER_ID]);

      if (player_and_height_copy[PLAYER_ID_1][OBELISK_HEIGHT] != 0)
      {
        Action::setGrantedPointsObelisk(POINTS_TWO_PLAYERS[PLAYER_ID_1],
                                        player_and_height_copy[PLAYER_ID_1][PLAYER_ID]);
      }
    }
  }

  if (amount_of_players == THREE_PLAYERS)
  {
    std::vector<int> points_for_all_players = calculatePoints(amount_of_players,
                                                              POINTS_THREE_PLAYERS,
                                                              occurrences);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_0],
                                    player_and_height_copy[PLAYER_ID_0][PLAYER_ID]);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_1],
                                    player_and_height_copy[PLAYER_ID_1][PLAYER_ID]);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_2],
                                    player_and_height_copy[PLAYER_ID_2][PLAYER_ID]);
  }

  if (amount_of_players == FOUR_PLAYERS)
  {
    std::vector<int> points_for_all_players = calculatePoints(amount_of_players,
                                                              POINTS_FOUR_PLAYERS,
                                                              occurrences);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_0],
                                    player_and_height_copy[PLAYER_ID_0][PLAYER_ID]);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_1],
                                    player_and_height_copy[PLAYER_ID_1][PLAYER_ID]);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_2],
                                    player_and_height_copy[PLAYER_ID_2][PLAYER_ID]);

    Action::setGrantedPointsObelisk(points_for_all_players[PLAYER_ID_3],
                                    player_and_height_copy[PLAYER_ID_3][PLAYER_ID]);
  }
}

//-----------------------------------------------------------------------------
bool StorageArea::sortColumnsReversed(std::vector<int> comp_vector_1, std::vector<int> comp_vector_2)
{
  return comp_vector_1[OBELISK_HEIGHT] >
         comp_vector_2[OBELISK_HEIGHT];
}

//-----------------------------------------------------------------------------
std::vector<std::vector<int>> StorageArea::getOccurrencesCount(std::vector<std::vector<int>> player_and_height,
                                                               int number_of_players)
{
  std::vector<std::vector<int>> occurrences;

  int amount_of_equal_points = ZERO_EQUAL_POINTS;

  for (int player_id = PLAYER_ID_0;
       player_id < number_of_players;
       player_id++)
  {
    for (int player_id_compare = PLAYER_ID_0;
         player_id_compare < number_of_players;
         player_id_compare++)
    {
      if (player_and_height[player_id][OBELISK_HEIGHT] == player_and_height[player_id_compare][OBELISK_HEIGHT])
      {
        amount_of_equal_points++;
      }
    }
    occurrences.push_back({player_id,
                           amount_of_equal_points});

    amount_of_equal_points = ZERO_EQUAL_POINTS;
  }
  return occurrences;
}

//-----------------------------------------------------------------------------
std::vector<int> StorageArea::calculatePoints(int amount_of_players,
                                              const int *point_ranking,
                                              std::vector<std::vector<int>> &occurrences)
{
  int total_points = 0;
  int points_per_player_with_same_stone_amount;
  std::vector<int> points_for_all_players;

  for (int player_id = 0;
       player_id < amount_of_players;
       player_id++)
  {
    points_for_all_players.push_back(ZERO_POINTS);
  }

  for (int player_id = 0;
       player_id < amount_of_players;
       player_id++)
  {
    if ((occurrences[player_id][AMOUNT_OF_EQUAL_POINTS] > ONE_OCCURRENCE) &&
        (points_for_all_players[player_id] == 0))
    {
      for (int i = player_id;
           i < player_id + occurrences[player_id][AMOUNT_OF_EQUAL_POINTS];
           i++)
      {
        total_points = total_points + point_ranking[i];
      }
      points_per_player_with_same_stone_amount = floor(total_points /
                                                       occurrences[player_id][AMOUNT_OF_EQUAL_POINTS]);

      for (int i = player_id;
           i < player_id + occurrences[player_id][AMOUNT_OF_EQUAL_POINTS];
           i++)
      {
        points_for_all_players[player_id] = points_per_player_with_same_stone_amount;
      }
      total_points = ZERO_POINTS;
    }
    else
    {
      points_for_all_players[player_id] = point_ranking[player_id];
    }
  }
  return points_for_all_players;
}


//-----------------------------------------------------------------------------
void StorageArea::grantPointsPyramid(std::vector<int>& stones_on_ship)
{
  std::vector<int> stones_on_ship_reversed;

  // put stones on ship in a vector (first pushed element will be last -> reversed)
  for (int i = (int) stones_on_ship.size() - 1;
       i >= 0;
       i--)
  {
    stones_on_ship_reversed.push_back(stones_on_ship[i]);
  }

  int respective_stone = 0;

  // get position of first false value in pyramid_occupied_stones_
  for (bool & pyramid_occupied_stone : pyramid_occupied_stones_)
  {
    if (!pyramid_occupied_stone)
    {
      break;
    }
    respective_stone++;
  }


  // set pyramid points and total points
  for (int respective_player = (int) stones_on_ship.size() - 1;
       respective_player >= 0;
       respective_player--)
  {
    for (bool & pyramid_occupied_stone : pyramid_occupied_stones_)
    {
      if (!pyramid_occupied_stone &&
          (stones_on_ship_reversed[respective_player] != NO_STONE_ON_SHIP))
      {
        pyramid_occupied_stone = true;

        Action::setGrantedPointsPyramid(POINTS_PER_FIELD_PYRAMID[respective_stone],
                                        stones_on_ship_reversed[respective_player]);

        Action::increaseGrantedPointsTotal(POINTS_PER_FIELD_PYRAMID[respective_stone],
                                           stones_on_ship_reversed[respective_player]);

        stones_on_ship_reversed.erase(stones_on_ship_reversed.begin() +
                                      respective_player);
        break;
      }
    }
    respective_stone++;
  }

  for (int deleter = 0;
       deleter < (int) stones_on_ship_reversed.size();
       deleter++)
  {
    if (stones_on_ship_reversed[deleter] == NO_STONE_ON_SHIP)
    {
      stones_on_ship_reversed.erase(stones_on_ship_reversed.begin() + deleter);
    }
  }


  // if pyramid is full
  if (!stones_on_ship_reversed.empty())
  {
    for (int respective_player = (int) stones_on_ship_reversed.size() - 1;
         respective_player >= 0;
         respective_player--)
    {
      Action::setGrantedPointsPyramid(ONE_POINT,
                                      stones_on_ship_reversed[respective_player]);

      Action::increaseGrantedPointsTotal(ONE_POINT,
                                         stones_on_ship_reversed[respective_player]);

      stones_on_ship_reversed.erase(stones_on_ship_reversed.begin() +
                                    respective_player);

      if (stones_on_ship_reversed.empty())
      {
        break;
      }
    }
  }
}

//-----------------------------------------------------------------------------
std::vector<int> StorageArea::generateMarketCardVector(std::vector<int> &all_market_cards,
                                                       int market_card_id_0,
                                                       int market_card_id_1,
                                                       int market_card_id_2,
                                                       int market_card_id_3)
{
  all_market_cards.clear();

  all_market_cards.push_back(market_card_id_3);
  all_market_cards.push_back(market_card_id_2);
  all_market_cards.push_back(market_card_id_1);
  all_market_cards.push_back(market_card_id_0);

  return all_market_cards;
}


//-----------------------------------------------------------------------------
void StorageArea::grantPointsTemple(int number_of_players)
{
  int temple_width = TEMPLE_WIDTH_TWO_PLAYERS;

  if (number_of_players > TWO_PLAYERS)
  {
    temple_width = TEMPLE_WIDTH_OVER_TWO;
  }

  if (temple_player_stones_.empty())
  {
    return;
  }

  // reduce the temple to the stones that are actually visible from above
  while (temple_width < (int) temple_player_stones_.size())
  {
    temple_player_stones_.erase(temple_player_stones_.begin());
  }

  // grant points for every visible stone
  for (int player_id : temple_player_stones_)
  {
    Action::setGrantedPointsTemple(TEMPLE_POINT_PER_STONE, player_id);
    Action::increaseGrantedPointsTotal(TEMPLE_POINT_PER_STONE, player_id);
  }
}

//-----------------------------------------------------------------------------
void handleEdgeCase(std::vector<bool> &compare_chamber,
                    std::vector<bool> &compare_chamber_edge_case)
{
  for (int position_in_chamber = 0;
       position_in_chamber < (int) compare_chamber.size();
       position_in_chamber++)
  {
    if (compare_chamber_edge_case[position_in_chamber])
    {
      compare_chamber[position_in_chamber] = false;
      compare_chamber[position_in_chamber] = false;
    }
  }
}

//-----------------------------------------------------------------------------
bool isUpperRightStoneFromSamePlayer(std::vector<int> &chamber, int stone_position, int player_id)
{
  if (stone_position + 2 <= (int) chamber.size())
  {
    if (chamber[stone_position + 2] == player_id)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------
bool isNextStoneFromSamePlayer(std::vector<int> &chamber, int stone_position, int player_id)
{
  if (stone_position + 1 <= (int) chamber.size())
  {
    if (chamber[stone_position + 1] == player_id)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------
bool isStoneRightFromSamePlayer(std::vector<int> &chamber, int stone_position, int player_id)
{
  if (stone_position + 3 <= (int) chamber.size())
  {
    if (chamber[stone_position + 3] == player_id)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------

bool isLeftStoneFromSamePlayer(std::vector<int> &chamber, int stone_position, int player_id)
{
  if (stone_position - 3 >= 0)
  {
    if (chamber[stone_position - 3] == player_id)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------
bool isPreviousStoneFromSamePlayer(std::vector<int> &chamber, int stone_position, int player_id)
{
  if (stone_position - 1 >= 0)
  {
    if (chamber[stone_position - 1] == player_id)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------
bool isPreviousStoneBasicallyUnreachable(std::vector<int> &chamber, int stone_position)
{
  if (((stone_position - 1) % 3 == 2) && (stone_position - 1 >= 0))
  {
    return true;
  }
  else
  {
    return false;
  }
}


//-----------------------------------------------------------------------------
bool isNextStoneBasicallyUnreachable(std::vector<int> &chamber, int stone_position)
{
  if (((stone_position + 1) % 3 == 0))
  {
    return true;
  }
  else
  {
    return false;
  }
}


//-----------------------------------------------------------------------------
bool isActuallyNeighboring(std::vector<int> &chamber,
                           std::vector<bool> &chamber_compare,
                           int base_stone,
                           int player_id,
                           int highest_neighboring_stone)
{
  bool has_a_right_neighbor = false;

  // down under
  if (isNextStoneFromSamePlayer(chamber, base_stone, player_id) &&
      !isNextStoneBasicallyUnreachable(chamber, base_stone) &&
      chamber_compare[base_stone + 1] &&
      base_stone + 1 <= highest_neighboring_stone)
  {
    has_a_right_neighbor = true;
  }
  // up over
  if (isPreviousStoneFromSamePlayer(chamber, base_stone, player_id) &&
      !isPreviousStoneBasicallyUnreachable(chamber, base_stone) &&
      chamber_compare[base_stone - 1])
  {
    has_a_right_neighbor = true;
  }
  // right
  if (isStoneRightFromSamePlayer(chamber, base_stone, player_id) &&
      base_stone + 3 <= highest_neighboring_stone &&
      chamber_compare[base_stone + 3])
  {
    has_a_right_neighbor = true;
  }
  // left
  if (isLeftStoneFromSamePlayer(chamber, base_stone, player_id) &&
      chamber_compare[base_stone - 3])
  {
    has_a_right_neighbor = true;
  }

  return has_a_right_neighbor;
}

//-----------------------------------------------------------------------------
void addToPlayerScore(std::vector<int> &player_1_fields,
                      std::vector<int> &player_2_fields,
                      std::vector<int> &player_3_fields,
                      std::vector<int> &player_4_fields,
                      int temporary_field_size,
                      int player_id)
{
  if (player_id == 0)
  {
    player_1_fields.push_back(temporary_field_size);
  }
  if (player_id == 1)
  {
    player_2_fields.push_back(temporary_field_size);
  }
  if (player_id == 2)
  {
    player_3_fields.push_back(temporary_field_size);
  }
  if (player_id == 3)
  {
    player_4_fields.push_back(temporary_field_size);
  }
}

//-----------------------------------------------------------------------------
int getTemporaryFieldSize(std::vector<int> &chamber,
                          std::vector<bool> &chamber_compare,
                          std::vector<bool> &compare_chamber_edge_case,
                          int highest_neighboring_stone,
                          int base_stone,
                          int player_id)
{
  int field_size_so_far = 0;

  for (; base_stone + 1 <= highest_neighboring_stone; base_stone++)
  {
    if ((chamber[base_stone] == player_id) &&
        isActuallyNeighboring(chamber, chamber_compare, base_stone, player_id, highest_neighboring_stone))
    {
      field_size_so_far++;
      chamber_compare[base_stone] = true;
    }
    if ((chamber[base_stone] == player_id) &&
        !isActuallyNeighboring(chamber, chamber_compare, base_stone, player_id, highest_neighboring_stone))
    {
      compare_chamber_edge_case[base_stone] = true;
    }
  }
  return field_size_so_far;
}

//-----------------------------------------------------------------------------
void setHighestStone(int &highest_neighboring_stone,
                     int this_neighboring_stone)
{
  if (highest_neighboring_stone < this_neighboring_stone)
  {
    highest_neighboring_stone = this_neighboring_stone;
  }
}

//-----------------------------------------------------------------------------
std::vector<bool> createCompareChamber(std::vector<int> &chamber)
{
  std::vector<bool> compare_chamber;

  for ([[maybe_unused]] int i : chamber)
  {
    compare_chamber.push_back(false);
  }

  return compare_chamber;
}

//-----------------------------------------------------------------------------
void chamberModified(std::vector<int> &chamber)
{
  while ((int) chamber.size() % 3 != 0)
  {
    chamber.push_back(-1);
  }
  chamber.push_back(-1);
  chamber.push_back(-1);
  chamber.push_back(-1);
}

//-----------------------------------------------------------------------------
bool isANeighboringStone(std::vector<int> &chamber,
                         std::vector<bool> &compare_chamber,
                         int player_id,
                         int &stone_position,
                         int &highest_stone_neighbor,
                         int &field_size)
{
  bool has_a_right_neighbor = false;
  bool is_first_in_chain = false;

  if (highest_stone_neighbor == stone_position - 1)
  {
    is_first_in_chain = true;
  }

  // down under
  if (isNextStoneFromSamePlayer(chamber,
                                stone_position,
                                player_id) &&
      !isNextStoneBasicallyUnreachable(chamber,
                                       stone_position) &&
      !compare_chamber[stone_position + 1])
  {
    compare_chamber[stone_position + 1] = true;

    setHighestStone(highest_stone_neighbor,
                    stone_position + 1);

    has_a_right_neighbor = true;
  }
  // up over
  if (isPreviousStoneFromSamePlayer(chamber,
                                    stone_position,
                                    player_id) &&
      !isPreviousStoneBasicallyUnreachable(chamber,
                                           stone_position) &&
      !compare_chamber[stone_position - 1])
  {
    compare_chamber[stone_position - 1] = true;
    setHighestStone(highest_stone_neighbor,
                    stone_position - 1);
    has_a_right_neighbor = true;
  }
  // right
  if (isStoneRightFromSamePlayer(chamber,
                                 stone_position,
                                 player_id) &&
      !compare_chamber[stone_position + 3])
  {
    compare_chamber[stone_position + 3] = true;
    setHighestStone(highest_stone_neighbor,
                    stone_position + 3);
    has_a_right_neighbor = true;
  }
  // left
  if (isLeftStoneFromSamePlayer(chamber,
                                stone_position,
                                player_id) &&
      !compare_chamber[stone_position - 3])
  {
    if (is_first_in_chain)
    {
      field_size--;
    }
    else
    {
      compare_chamber[stone_position - 3] = true;
      setHighestStone(highest_stone_neighbor,
                      stone_position - 3);
      has_a_right_neighbor = true;
    }
  }
  // edge case
  if (isPreviousStoneFromSamePlayer(chamber,
                                    stone_position,
                                    player_id) &&
      (stone_position % 3 == 1) &&
      isUpperRightStoneFromSamePlayer(chamber,
                                      stone_position,
                                      player_id) &&
      !compare_chamber[stone_position + 2])
  {
    compare_chamber[stone_position + 2] = true;
    setHighestStone(highest_stone_neighbor,
                    stone_position + 2);
    has_a_right_neighbor = true;
  }

  return has_a_right_neighbor;
}

//-----------------------------------------------------------------------------
void pushFieldsToVector(std::vector<int> &chamber,
                        std::vector<bool> &compare_chamber,
                        std::vector<bool> &compare_chamber_edge_case,
                        std::vector<int> &player_1_fields,
                        std::vector<int> &player_2_fields,
                        std::vector<int> &player_3_fields,
                        std::vector<int> &player_4_fields,
                        int &temp_field_size,
                        int &player_id,
                        int &stone_position,
                        int &highest_stone_neighbor)
{
  compare_chamber[stone_position] = true;
  // field has at least 1 stone, this stone won't be recounted later
  temp_field_size = temp_field_size + 1;
  while(isANeighboringStone(chamber,
                            compare_chamber,
                            player_id,
                            stone_position,
                            highest_stone_neighbor,
                            temp_field_size))
  {
    temp_field_size = temp_field_size +
                      getTemporaryFieldSize(chamber,
                                            compare_chamber,
                                            compare_chamber_edge_case,
                                            highest_stone_neighbor,
                                            stone_position,
                                            player_id);
    stone_position = highest_stone_neighbor;
  }
  addToPlayerScore(player_1_fields,
                   player_2_fields,
                   player_3_fields,
                   player_4_fields,
                   temp_field_size,
                   player_id);
  temp_field_size = 0;
  handleEdgeCase(compare_chamber, compare_chamber_edge_case);
  if (highest_stone_neighbor - 1 > stone_position)
  {
    stone_position = highest_stone_neighbor - 1;
  }
}

//-----------------------------------------------------------------------------
void calculateActualPointsChamber(int &field_size,
                                  std::vector<int> &player_scores,
                                  int &player_id)
{
  switch (field_size)
  {
    case 1:
      player_scores[player_id] = player_scores[player_id] + 1;
      break;
    case 2:
      player_scores[player_id] = player_scores[player_id] + 3;
      break;
    case 3:
      player_scores[player_id] = player_scores[player_id] + 6;
      break;
    case 4:
      player_scores[player_id] = player_scores[player_id] + 10;
      break;
    case 5:
      player_scores[player_id] = player_scores[player_id] + 15;
      break;
  }
  if (field_size > 5)
  {
    field_size = field_size - 5;
    player_scores[player_id] = player_scores[player_id] + 15 + field_size * 2;
  }
}


//-----------------------------------------------------------------------------
void StorageArea::grantPointsChamber()
{
  // add -1 to chamber as long as chamber_size % 3 == 0
  std::vector<int> chamber = chamber_player_stones_cumulated_;
  chamberModified(chamber);
  std::vector<bool> compare_chamber = createCompareChamber(chamber);
  std::vector<bool> compare_chamber_edge_case = createCompareChamber(chamber);

  std::vector<int> player_1_fields;
  std::vector<int> player_2_fields;
  std::vector<int> player_3_fields;
  std::vector<int> player_4_fields;

  int temp_field_size = 0;

  int highest_stone_neighbor = 0;

  for (int player_id = 0; player_id < 2; player_id++)
  {
    for (int stone_position = 0; stone_position < (int) chamber.size(); stone_position++)
    {
      if ((chamber[stone_position] == player_id) && !compare_chamber[stone_position])
      {
        pushFieldsToVector(chamber,
                           compare_chamber,
                           compare_chamber_edge_case,
                           player_1_fields,
                           player_2_fields,
                           player_3_fields,
                           player_4_fields,
                           temp_field_size,
                           player_id,
                           stone_position,
                           highest_stone_neighbor);
      }
    }
    highest_stone_neighbor = 0;
  }

  // for edge case handling
  for (int player_id = 0; player_id < 4; player_id++)
  {
    for (int stone_position = 0; stone_position < (int) chamber.size(); stone_position++)
    {
      if ((chamber[stone_position] == player_id) && !compare_chamber[stone_position])
      {
        pushFieldsToVector(chamber,
                           compare_chamber,
                           compare_chamber_edge_case,
                           player_1_fields,
                           player_2_fields,
                           player_3_fields,
                           player_4_fields,
                           temp_field_size,
                           player_id,
                           stone_position,
                           highest_stone_neighbor);
      }
    }
    highest_stone_neighbor = 0;
  }

  std::vector<std::vector<int>> all_fields;
  all_fields.push_back(player_1_fields);
  all_fields.push_back(player_2_fields);
  all_fields.push_back(player_3_fields);
  all_fields.push_back(player_4_fields);

  std::vector<int> player_scores;
  for (int initializer = 0; initializer < 4; initializer++)
  {
    player_scores.push_back(0);
  }
  int player_id = 0;

  for (std::vector<int> fields_per_player : all_fields)
  {
    for (int field_size : fields_per_player)
    {
      calculateActualPointsChamber(field_size,
                                   player_scores,
                                   player_id);
    }
    player_id++;
  }

  player_id = 0;

  for (int score : player_scores)
  {
    Action::setGrantedPointsChamber(score, player_id);
    Action::increaseGrantedPointsTotal(score, player_id);
    player_id++;
  }
}

//-----------------------------------------------------------------------------
void StorageArea::pushStonesToTemple(int space)
{
  if (space != -1)
  {
    temple_player_stones_.push_back(space);
    temple_player_stones_cumulated_.push_back(space);
  }
}

//-----------------------------------------------------------------------------
void StorageArea::pushStonesToObelisk(int space, std::vector<std::vector<int>> &obelisk_player_and_height)
{
  if (space != -1)
  {
    for (auto &value : obelisk_player_and_height)
    {
      if (space == value[0])
      {
        value[1] = value[1] + 1;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void StorageArea::pushStonesToChamber(int space)
{
  if (space != -1)
  {
    chamber_player_stones_cumulated_.push_back(space);
  }

}

//-----------------------------------------------------------------------------
void StorageArea::pushStonesToPyramid(int space)
{
  if (space != -1)
  {
    pyramid_player_stones_cumulated_.push_back(space);
  }
}