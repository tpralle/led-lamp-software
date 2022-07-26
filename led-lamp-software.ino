/* Copyright: 2022 Tylor Pralle
 *
 * Licence: GNU GENERAL PUBLIC LICENSE V3, 29 June 2007
 *
 * This program is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details. You should have received a copy of the
 * GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * Author: Tylor Pralle
 * Website: https://cutratedrugs.com/
 * Software Name: LED Lamp Software
 * Notes: Enjoy my positive affirmations
 */

// I am worthy of what I desire.

#include <FastLED.h>

// I can. I will. End of story.

#define LED_PIN 6          // Set the data pin for LED control.
#define KNOB_PIN A1        // Set the pin used for the potentiometer.
#define MODE_PIN 7         // Set the pin used for mode change button.
#define LED_TYPE WS2812B   // Set the type of led being used.
#define COLOR_ORDER GRB    // Set color channel order.
#define LED_COUNT 5        // Set the number of LEDs used in the lamp.
#define MAX_BRIGHTNESS 200 // Set the max led brightness, be careful of power draw. Max: 255
#define MIN_BRIGHTNESS 20  // Set the minimum led brightness. Min: 0
#define MAX_MODES 1        // Set number of modes availible.

// I am adventurous. I overcome fears by following my dreams.

CRGB leds[LED_COUNT];

// I choose to see the light that I am to this world.

uint8_t modeSwitch;
uint8_t currentMode = 1;
uint8_t hue = 0;
uint8_t outputBrightness;
bool controlBrightness = false; // Turn on external brightness control pot.

void setup()
{
  delay(3000); // We delay startup incase we screw something up, so we have time to switch to recovery.

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(MIN_BRIGHTNESS); // Initialize the LEDs to lowest brightness. FO SAFETY YO
  FastLED.clear(true);                   // Initialize all LEDs to same off state.
}

// I will not compare myself to strangers on the Internet.

void loop()
{
  modeSwitch = digitalRead(MODE_PIN);

  if (controlBrightness) // Checks to see if there is a knob installed, and adjusts brightness to current setting.
  {
    updateBrightness();
  }
  else
  {
    FastLED.setBrightness(MAX_BRIGHTNESS);
  }

  if (modeSwitch == HIGH) // Might need to add debounce. Open an issue on github if needed.
  {
    currentMode++;

    if (currentMode > MAX_MODES)
    {
      currentMode = 1;
    }
  }

  switch (currentMode)
  {
  case 1: // Rainbow Mode - Fades all LEDs through the same colors at the same time.
    modeRainbow();
    break;
  default: // If we somehow make it past our modes, default back to mode 1. Probably not needed, or can take the place of case 1 once more modes have been added, starting the swithc at case 2.
    currentMode = 1;
    break;
  }

  EVERY_N_MILLISECONDS(15)
  {
    hue++;
  }

  FastLED.setBrightness(outputBrightness);
  FastLED.show();
}

// I have the power to create change.

void updateBrightness()
{                                                                                 // function for converting sensor value from ADC to 8bit value and consraining the result.
  int mappedBrightness = map(analogRead(KNOB_PIN), 0, 1023, 0, 255);              // Converts the knob input from 10bit to 8bit so the fastled library can use it.
  outputBrightness = constrain(mappedBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS); // Gives us a variable to use that respects our min adn max brightness settings.
}

void modeRainbow()
{
  for (uint8_t x = 0; x < LED_COUNT; x++)
  {
    leds[x].setHSV(hue, 255, 255);
  }
}