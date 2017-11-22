#ifndef __XMASTREE_H__
#define __XMASTREE_H__

#include <Arduino.h>

#define NR_STATES 11

typedef enum modes{
  UNDEFINED,
  GREEN_ROWS,
  ORANGE_ROWS,
  RED_ROWS,
  MULTI_ROWS,
  RANDOM_ROWS,
  GREEN_COLUMNS,
  ORANGE_COLUMNS,
  RED_COLUMNS,
  MULTI_COLUMNS,
  RANDOM_FADE,
  SLEEP
} mode_t;

class ChristmasTree {
public:
  ChristmasTree();

  void Run(unsigned long duration_ms);
  bool WasTouched();
  bool IsSleeping();
  void SetMode(mode_t mode);
  void SetRandomMode();
  void WakeUp();
  void GoToSleep();
  mode_t GetRandomMode();
protected:
  mode_t prevMode = UNDEFINED;
  mode_t mode = UNDEFINED;
  bool touched = false;
};

#endif /* __XMASTREE_H__ */
