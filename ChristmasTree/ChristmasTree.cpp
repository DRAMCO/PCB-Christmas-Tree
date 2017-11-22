#include "ChristmasTree.h"
#include "AnalogTouch.h"

#define offset 1

const int NR_ANODE = 3;
const int NR_CATHODE = 8;
const int anodePins[NR_ANODE] = { 3, 4, 5 };    // Arduino pins 3, 4, 5
const int cathodePinsC[4] = {4, 2, 3, 1};
const int cathodePinsD[2] = { 1, 0 };        // Arduino pins 7, 6, 1, 0
const int cathodePinsB[2] = { 3, 4 };        // Arduino pins 8, 9, 11, 12
// pins 3 4 5
const int anodeBits = B00111000;
// pins 6 7 0 1
const int cathodeBitsC = B00011110;
// pins 6 7 0 1
const int cathodeBitsD = B00000011;
// pins 8 9 11 12
const int cathodeBitsB = B00011000;

unsigned long int loopCounter = 0;
long int N,n = 0;
int M,m = 0;
unsigned long int then = 0, now = 0;

static int i = 0;
int rem = 0;

const byte NR_STATE = 11;

volatile unsigned long int lastTouch = 0;
const unsigned long int touchDelay = 500; // times 100 ms
unsigned long int lastChange;
const unsigned long int autoChangeDelay = 10000;
const unsigned long int timeout = 180000;
unsigned long int hold = 0;

int val[NR_ANODE][NR_CATHODE] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0 } };

int randoms[NR_ANODE][NR_CATHODE] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
                                      { 0, 0, 0, 0, 0, 0, 0, 0 },
                                      { 0, 0, 0, 0, 0, 0, 0, 0 } };

int randomChanged[NR_ANODE][NR_CATHODE] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
                                            { 0, 0, 0, 0, 0, 0, 0, 0 },
                                            { 0, 0, 0, 0, 0, 0, 0, 0 } };

uint8_t getTouch() {
  long unsigned int _now = millis();
  if (_now > lastTouch + touchDelay) {
    uint16_t value = analogTouchRead(A0);

    // Self calibrate - from the AnalogTouch example
    static uint16_t ref = 0xFFFF;
    if (value < (ref >> offset))
      ref = (value << offset);
    // Cool down
    else if (value >(ref >> offset))
      ref++;

    bool touched = (value - (ref >> offset)) > 40;
    if (touched) {
      if (hold) {
        if (_now > hold + 3000) {
          hold = _now;
          lastChange = _now;
          return 3;
        }
      }
      else {
        lastTouch = millis();
        lastChange = lastTouch;
        hold = lastChange;
        return 2;
      }
    }
    else {
      hold = 0;
      return 0;
    }
  }
  return 0;
}

inline void zero() {
    for (size_t j = 0; j < NR_ANODE; j++)
    {
        for (size_t k = 0; k < NR_CATHODE; k++)
        {
            val[j][k] = 0;
        }
    }
    N = 0;
}


inline int convert(unsigned int v, int r = 50) {
    n = (int)(v % (2*r)) - r;
    return abs(n);
}

inline void green() {
    if (now > then + 19) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 1; k < NR_CATHODE; k = k+2)
            {
                val[j][k] = convert(N+50, 50) - 5;
            }
        }
        then = now;
        N++;
    }
}

inline void red() {
    if (now > then + 19) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k = k+2)
            {
                val[j][k] = convert(N + 50, 50) - 5;
            }
        }
        then = now;
        N++;
    }
}

inline void orange() {
    if (now > then + 19) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                val[j][k] = convert(N + 50, 50) - 5;
            }
        }
        then = now;
        N++;
    }
}

inline void yellow() {
    if (now > then + 19) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k += 2)
            {
                val[j][k] = (convert(N + 50, 50) - 5) / 4;
            }
            for (size_t k = 1; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 50, 50) - 5;
            }
        }
        then = now;
        N++;
    }
}

inline void orangeRows() {
    if (now > then + 9) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                val[j][k] = convert(N + 110 + 13*j, 100) - 50;
            }
        }
        then = now;
        N++;
    }
}

inline void greenRows() {
    if (now > then + 9) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 1; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 100 + 13 * j, 100) - 50;
            }
        }
        then = now;
        N++;
    }
}

inline void redRows() {
    if (now > then + 9) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 100 + 13 * j, 100) - 50;
            }
        }
        then = now;
        N++;
    }
}

inline void multiRows() {
    if (now > then + 9) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 100 + 13 * j, 100) - 50;
            }
            for (size_t k = 1; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 126 + 13 * j, 100) - 50;
            }
        }
        then = now;
        N++;
    }
}

inline void randRows() {
    if (now > then + 9) {
        if (N % 200 == 0) {
            for (size_t j = 0; j < NR_ANODE; j++)
            {
                for (size_t k = 0; k < NR_CATHODE; k++)
                {
                    randoms[j][k] = random(2);
                }
            }
        }
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k += 1)
            {
                val[j][k] = (convert(N + 100 + 13 * j, 100) - 50) * randoms[j][k];
            }
        }
        then = now;
        N++;
    }
}

inline void redCols() {
    if (now > then + 9) {
        N++;
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k+=2)
            {
                val[j][k] = convert(N + 50 + 50 * (k / 2), 100) - 25;
            }
        }
        then = now;
    }
}

inline void greenCols() {
    if (now > then + 9) {
        N++;
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 1; k < NR_CATHODE; k+=2)
            {
                val[j][k] = convert(N + 50 + 50 * (k / 2), 100) - 25;
            }
        }
        then = now;
    }
}

inline void orangeCols() {
    if (now > then + 9) {
        N++;
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                val[j][k] = convert(N + 50 + 50 * (k / 2), 100) - 25;
            }
        }
        then = now;
    }
}

inline void multiCols() {
    if (now > then + 9) {
        N++;
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 50 + 50 * (k / 2), 100) - 25;
            }
            for (size_t k = 1; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 100 + 50 * (k / 2), 100) - 25;
            }
        }
        then = now;
    }
}

inline void multiMulti() {
    if (now > then + 9) {
        N++;
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 1; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 50 + 25 * (k / 2), 100) - 50;
            }
            for (size_t k = 0; k < NR_CATHODE; k += 2)
            {
                val[j][k] = convert(N + 100 + 13 * j, 100) - 50;
            }
        }
        then = now;
    }
}

inline void randFlash() {
    if (now > then + 50) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                val[j][k] = random(2) * 50;
            }
        }
        then = now;
    }
}

inline void randFade() {
    if (now > then + 9) {
        if (N % 50 == 0) {
            for (size_t j = 0; j < NR_ANODE; j++)
            {
                for (size_t k = 0; k < NR_CATHODE; k++)
                {
                    randomChanged[j][k] = (random(4) < 1);
                    if (randomChanged[j][k]) {
                        randoms[j][k] = !randoms[j][k];
                    }
                }
            }
        }
        for (size_t j = 0; j < NR_ANODE; j++) {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                if (randomChanged[j][k]) {
                    if (randoms[j][k]) {
                        val[j][k] = (N % 50) + 1;
                    }
                    else {
                        val[j][k] = 49 - (N % 50);
                    }
                }
                else {
                    val[j][k] = randoms[j][k] * 50;
                }

            }
        }
        then = now;
        N++;
    }
}

/*inline void max_power() {
    if (now > then + 10) {
        for (size_t j = 0; j < NR_ANODE; j++)
        {
            for (size_t k = 0; k < NR_CATHODE; k++)
            {
                val[j][k] = 70;
            }
        }
        then = now;
    }
}*/


inline void rest() {
    if (now > then + 29) {
        val[2][6] = convert(N+50, 50) - 25;
        N++;
        then = now;
    }
}

void updateLights(void){
  rem = loopCounter % 50;
  i = loopCounter % NR_ANODE;
  // turn all pins
  PORTD = PORTD | cathodeBitsD | anodeBits;
  PORTC = PORTC | cathodeBitsC | anodeBits;
  PORTB = PORTB | cathodeBitsB;
  // turn anode_pins[i] low - turn on P-MOSFET
  PORTD = PORTD & ~(1 << (i + 3));
  // turn cathode pins low if rem >= corresponding val
  PORTD = PORTD & ~(((rem < val[i][6]) << cathodePinsD[0]) | ((rem < val[i][7]) << cathodePinsD[1]));
  PORTB = PORTB & ~(((rem < val[i][4]) << cathodePinsB[0]) | ((rem < val[i][5]) << cathodePinsB[1]));
  PORTC = PORTC & ~(((rem < val[i][0]) << cathodePinsC[0]) | ((rem < val[i][1]) << cathodePinsC[1]) | ((rem < val[i][2]) << cathodePinsC[2]) | ((rem < val[i][3]) << cathodePinsC[3]));
  // wait a bit to see the led
  //delay(1);
  now = millis();
  loopCounter++;
}

void selectMode(mode_t mode){
  switch (mode) {
    case RED_ROWS:{
      redRows();
    } break;
    case ORANGE_ROWS:{
      orangeRows();
    } break;
    case GREEN_ROWS:{
      greenRows();
    } break;
    case MULTI_ROWS:{
      multiRows();
    } break;
    case RANDOM_ROWS:{
      randRows();
    } break;
    case GREEN_COLUMNS:{
      greenCols();
    } break;
    case ORANGE_COLUMNS:{
      orangeCols();
    } break;
    case RED_COLUMNS:{
      redCols();
    } break;
    case MULTI_COLUMNS:{
      multiCols();
    } break;
    case RANDOM_FADE:{
      randFade();
    } break;
    case SLEEP:{
      rest();
    } break;
    default:{
      randFlash();
    } break;
  }
}

ChristmasTree::ChristmasTree(){
  pinMode(13, OUTPUT);
  randomSeed(analogRead(A5));

  // set all PORTD pins to output
  DDRD = DDRD | B11111111;
  // set all PORTC pins to output
  DDRC = DDRC | B11111110;
  // PORTB 6 and 7 are crystal, set others as output
  DDRB = DDRB | B00111111;
  // set all pins to low
  PORTD = PORTD & B00000000;
  PORTC = PORTC & B00000001;
  PORTB = PORTB & B11000000;

  then = millis();
  now = millis();
}

bool ChristmasTree::WasTouched(){
  bool retval = this->touched;
  this->touched = false;
  return retval;
}

mode_t ChristmasTree::GetRandomMode(){
  return (mode_t)random(1, NR_STATES);
}

void ChristmasTree::Run(mode_t mode, unsigned long duration_ms){
  unsigned long runtime = now + duration_ms;
  unsigned long touchtime = now + 100;
  if(mode != this->prevMode){
    zero();
  }
  this->prevMode = mode;
  while(now < runtime){ // run for a specified time
    // handle touch
    if(now > touchtime){
      if(getTouch() != 0){
        digitalWrite(13, !digitalRead(13));
        this->touched = true;
        break;
      }
      touchtime = now + 100;
    }
    // set lights according to mode
    selectMode(mode);
    updateLights();
  }
}
