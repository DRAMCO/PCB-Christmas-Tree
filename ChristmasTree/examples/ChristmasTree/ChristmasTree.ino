/*
The MIT License (MIT)

Copyright (c) 2015 Balint Balazs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <Arduino.h>
#include "ChristmasTree.h"

ChristmasTree tree = ChristmasTree();
mode_t mode;

void setup() {
  // select a random active mode
  mode = tree.GetRandomMode();
}

void loop() {
  // Run the christmas lights in the christmas tree for a duration of 10 seconds
  tree.Run(mode, 10000); // will end sooner if touch is detected

  // if touch detected: toggle between sleep mode and active modes
  // if not touched, randomly select a new active mode
  if(tree.WasTouched()){
    if(mode == SLEEP){
      mode = tree.GetRandomMode();
    }
    else{
      mode = SLEEP;
    }
  }
  else{
    mode = tree.GetRandomMode();
  }

}