#ifndef SAVELOAD_HPP
#define SAVELOAD_HPP

#include <string>
#include "Q.hpp"

using std::string;

namespace sml{

class SaveLoad
{
public :   
  static void syncQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void writeQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void readQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void initializeQ(const string& chemin, float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
};

}

#endif // SAVELOAD_HPP

