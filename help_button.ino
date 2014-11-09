#include <RBL_nRF8001.h>
#include <RBL_services.h>

#include <aci.h>
#include <aci_cmds.h>
#include <aci_evts.h>
#include <aci_protocol_defines.h>
#include <aci_queue.h>
#include <aci_setup.h>
#include <acilib.h>
#include <acilib_defs.h>
#include <acilib_if.h>
#include <acilib_types.h>
#include <ble_assert.h>
#include <boards.h>
#include <dtm.h>
#include <hal_aci_tl.h>
#include <hal_platform.h>
#include <lib_aci.h>

#include <wprogram.h>
#include <wiring_private.h>
#include <pins_arduino.h>

/*

Copyright (c) 2012-2014 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/



//"SPI.h/Nordic_nRF8001.h/RBL_nRF8001.h" are needed in every new project
#include <SPI.h>

// this constant won't change:
const int buttonPin = 2;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void setup()
{
  //  
  // For BLE Shield or Blend:
  //   Default pins set to 9 and 8 for REQN and RDYN
  //   Set your REQN and RDYN here before ble_begin() if you need
  //
  // For Blend Micro:
  //   Default pins set to 6 and 7 for REQN and RDYN
  //   So, no need to set for Blend Micro.
  //
  //ble_set_pins(3, 2);
  
  // Set your BLE advertising name here, max. length 10
  //ble_set_name("My BLE");
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // Init. and start BLE library.
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);
}

void loop()
{
  if ( ble_connected() )
  {
    buttonState = digitalRead(buttonPin);
  
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
      // if the state has changed, increment the counter
      if (buttonState == HIGH) {
        // if the current state is HIGH then the button
        // wend from off to on:
        Serial.println("on");
        Serial.print("number of button pushes:  ");
        Serial.println(buttonPushCounter);  
        ble_write('H');
        ble_write('E');
        ble_write('L');
        ble_write('P');
        ble_write('0'+buttonPushCounter);
        buttonPushCounter++;
        if(buttonPushCounter>9){
          buttonPushCounter=0;
        }
      } 
      else {
        // if the current state is LOW then the button
        // wend from on to off:
        Serial.println("off"); 
      }
    }
    // save the current state as the last state, 
    //for next time through the loop
    lastButtonState = buttonState;
  
    
    // turns on the LED every four button pushes by 
    // checking the modulo of the button push counter.
    // the modulo function gives you the remainder of 
    // the division of two numbers:
    if (buttonPushCounter % 4 == 0) {
      digitalWrite(ledPin, HIGH);
    } else {
     digitalWrite(ledPin, LOW);
    }
    
  }

  ble_do_events();
  
  if ( ble_available() )
  {
    while ( ble_available() )
    {
      Serial.write(ble_read());
    }
    
    Serial.println();
  }
  
}
