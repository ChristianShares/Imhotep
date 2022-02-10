//-----------------------------------------------------------------------------
// main.cpp
//
// This file fullfills the error handling and checks for malformed and
// inexecutable input
//
// Group: 03 (formerly 14.04)
//
// Author:  Christina Schlamberger 01430700
//          Fabian Platzer 01613099
//          Christian Fusek 11827543
//-----------------------------------------------------------------------------
//

#include "../include/PlayerMessage.hpp"
#include "../include/GameHandler.hpp"
#include <regex>
#include <string>

enum InputValidity
{
  VALID_INPUT = 0,
  NO_ERROR_DURING_GAME = 0
};

enum ReturnValues
{
  ALL_WENT_WELL = 0,
  INTERNAL_ERROR = -1
};

using std::string;


//-----------------------------------------------------------------------------
///
/// This is the main function. It checks for errors and waits for input.
///
/// @param argc not used
/// @param argv not used
///
/// @return 0 if there is no error.
/// @return 1 if the number of argumets is wrong.
/// @return 2 if the arguments are wrong.
/// @return -1 if there is an internal error.
//
int main(int argc,
         const char **argv)
{
  Handler *Game = nullptr;

  int number_of_players;
  unsigned short port;

  // Command Line Input Handling
  try
  {
    int input_validity_status = Game->checkInputValidity(argc,
                                                         argv);

    if (input_validity_status == VALID_INPUT)
    {
      Game->setPortAndPlayers(argv,
                              number_of_players,
                              port);
    }
    else
    {
      delete Game;
      return input_validity_status;
    }
  }
  catch (const std::exception &)
  {
    delete Game;
    return INTERNAL_ERROR;
  }

  // execute Game and return -1 if there is a runtime error
  if (Game->executeGame(number_of_players, port) != NO_ERROR_DURING_GAME)
  {
    delete Game;
    return INTERNAL_ERROR;
  }
  delete Game;
  return ALL_WENT_WELL;
}
