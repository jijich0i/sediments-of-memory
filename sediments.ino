#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 112
#define DATA_PIN 3
#define SENSOR_PIN A0

CRGB leds[NUM_LEDS];

const int zoneStarts[] = {
  0, 9, 17, 22, 26,
  29, 36, 44, 51, 55,
  59, 63, 68, 74, 78,
  87, 92, 99, 103, 108
};

const int zoneEnds[] = {
  8, 16, 21, 25, 28,
  35, 43, 50, 54, 58,
  62, 67, 73, 77, 86,
  91, 98, 102, 107, 111
};

const int NUM_ZONES = 20;

uint8_t zoneHue[NUM_ZONES];
uint8_t zoneValue[NUM_ZONES];

float smoothedSensor = 0;
float smoothedZoneLevel = 0;

void fillZoneHSV(int zoneIndex, uint8_t hue, uint8_t sat, uint8_t val) {
  CRGB color = CHSV(hue, sat, val);
  for (int i = zoneStarts[zoneIndex]; i <= zoneEnds[zoneIndex]; i++) {
    leds[i] = color;
  }
}

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  Serial.begin(9600);

  randomSeed(analogRead(A1));

  // assign a random hue to each zone on startup
  for (int i = 0; i < NUM_ZONES; i++) {
    zoneHue[i] = random8();
    zoneValue[i] = 0;
  }
}

void loop() {

  int sensorValue = analogRead(SENSOR_PIN);

  // smooth sensor readings to avoid jitter
  smoothedSensor = smoothedSensor * 0.9 + sensorValue * 0.1;

  int globalBrightness = map((int)smoothedSensor, 0, 600, 20, 255);
  globalBrightness = constrain(globalBrightness, 0, 255);
  FastLED.setBrightness(globalBrightness);

  // map sensor to target zone level
  float targetZoneLevel = map((int)smoothedSensor, 0, 600, 0, NUM_ZONES * 100) / 100.0;

  // slow down zone transitions for a gradual fade effect
  smoothedZoneLevel = smoothedZoneLevel * 0.97 + targetZoneLevel * 0.03;

  int fullZones = (int)smoothedZoneLevel;
  float partial = smoothedZoneLevel - fullZones;

  for (int z = 0; z < NUM_ZONES; z++) {

    uint8_t targetVal = 0;

    if (z < fullZones) {
      targetVal = 255;
    }
    else if (z == fullZones) {
      // partially lit zone based on fractional zone level
      targetVal = (uint8_t)(partial * 255);
    }
    else {
      targetVal = 0;
    }

    // gradually transition brightness toward target
    if (zoneValue[z] < targetVal)
      zoneValue[z] = min((int)zoneValue[z] + 2, (int)targetVal);
    else if (zoneValue[z] > targetVal)
      zoneValue[z] = max((int)zoneValue[z] - 2, (int)targetVal);

    // slowly cycle hue while zone is active
    if (zoneValue[z] > 0)
      zoneHue[z] += 1;

    fillZoneHSV(z, zoneHue[z], 255, zoneValue[z]);
  }

  FastLED.show();

  delay(30);
}
