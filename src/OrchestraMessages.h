#ifndef ORCHESTRAMESSAGES_H
#define ORCHESTRAMESSAGES_H

#include <Arduino.h>


class OrchestraMessages
{
public:
  // Per default the orchestra reserves the range of values from 40 to 69
  // for custom MIDI control messages.
  const static int CTRL_MIN = 40;
  const static int CTRL_MAX = 69;
  // Default reserved values for control messages by the Orchestra base class.
  const static int CHANGE_MOVEMENT = CTRL_MIN + 0;
};


#endif //ORCHESTRAMESSAGES_H