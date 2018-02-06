/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "FastLED.h"

#define MAX_HELLIGKEIT 255
#define NUM_LEDS 10 // muss gerade sein!
#define DATA_PIN 3
#define COMM_PIN 5

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
CRGB leds[NUM_LEDS+1];
CHSV color_hsv;
uint8_t growing=0;
long start_time;
uint8_t led_count;

  
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!


  Serial.begin(9600);
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  clear_display();
  FastLED.show();
  pinMode(COMM_PIN,OUTPUT);
  digitalWrite(COMM_PIN,LOW);
  led_count=0;
}

void loop() {
  // Get the currently touched pads
  
  //cap.setThresholds( 1,  1);
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<1; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      digitalWrite(COMM_PIN,HIGH);
      growing=1;
      start_time=millis();
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
      digitalWrite(COMM_PIN,LOW);
      growing=0;
      start_time=millis();
    }

  }

  if (growing) {
    if (millis()-start_time>100) {
      if (led_count<NUM_LEDS/2) {
        led_count++;
        //Serial.print(led_count-1);
        leds[led_count-1]=CRGB::White;
        leds[NUM_LEDS-led_count]=CRGB::White;
        FastLED.show();
      }
      start_time=millis();
    }
  }
  else {
    if (millis()-start_time>100){
      if (led_count>0) {
        led_count--;    
        //Serial.print(led_count);
        leds[led_count]=CRGB::Black;
        leds[NUM_LEDS-led_count-1]=CRGB::Black;
        FastLED.show();
      }
      start_time=millis();
    }
  }
  
  
  // reset our state
  lasttouched = currtouched;

  
  // hier gibts es die Rohwerte des Sensorwertes und der Baseline aus wenn man das sehen will
  /*
  // debugging info
  for (uint8_t i=0; i<1; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
    Serial.print(" ");
    Serial.print(cap.baselineData(i)); Serial.print("\t");
    Serial.print("  ");
  }
  Serial.println();
  delay(50);
  */
  
}


void clear_display()
{
  int ct_led;

  for (int ct_x = 0; ct_x < NUM_LEDS; ct_x++)
  {
    leds[ct_x] = CRGB::Black;
  }
}
