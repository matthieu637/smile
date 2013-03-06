#ifndef SAVELOAD_HPP
#define SAVELOAD_HPP

namespace sml{

class SaveLoad
{
public :   
  static void syncQ(float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void writeQ(float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void readQ(float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
  static void initializeQ(float**** Q, int STATES_ALPHA, int STATES_DISTANCE, int ACTIONS_ACC, int ACTIONS_DIRECTION);
};

}

#endif // SAVELOAD_HPP

