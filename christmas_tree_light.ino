#include <FastLED.h> //We are going to use the FastLED library
#define NUM_LEDS 24 //Amount of LEDs that are used on the Whadda XL kit
#define STRING1_PIN 2 //The pin on which the led string is connected

CRGBArray<NUM_LEDS> leds; //Defining the array of the LEDs;
#define POSITION_COLORS 0
#define STRING_ANIMATION 1
#define PRIDE_MODE 2
#define PATTERN1_MODE 3
#define PATTERN2_MODE 4
#define PATTERN3_MODE 5
#define STRING_PARTY 6

CRGB gBackgroundColor = CRGB::Black; // Black means turn LED off

int currentMode = POSITION_COLORS;

void btn_pressed()
{
  currentMode++;
  FastLED.clear();
}

// Main initial setup
void setup() {
  FastLED.addLeds<WS2812B, STRING1_PIN>(leds, NUM_LEDS);  //Tell the FastLED library that we are using WS2812B LEDs
  FastLED.setBrightness(150); //The max brightness value is 255
  attachInterrupt(digitalPinToInterrupt(4), btn_pressed, FALLING); // When the user button is pressed change mode
}

void position_colors() {
    for (int i = 0; i < NUM_LEDS; i++) {//We will count to 24 starting from 0, and will add 1 for every loop and check if we reached the end, 'i' will increment with 1 each time
      leds[i] = CHSV(i * 10, 255, 255);
    }
    FastLED.show();//Show the current state of the LEDs array
}

void string_animation(int speed, int fade)
{
  const byte fadeAmt = fade;
  const int cometSize = 5;
  const int deltaHue = 4;
  static byte hue = HUE_RED;
  const int lastLED = 19; 
  const int startLED = 0; 

  static int iDirection = 1;
  static int iPos = 0;
  for(int i=20;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::White;
  }
  hue += deltaHue;
  iPos += iDirection;

  if (iPos == (lastLED) || iPos == startLED)
  {
    iDirection *= -1;
  }

  for (int i = 0; i < cometSize; i++)
  {
    leds[iPos].setHue(hue);
  }

  for (int i = 0; i < 20; i++)
  {
    leds[i] = leds[i].fadeLightBy(fadeAmt);
  }
  FastLED.show();
  delay(speed);
}

void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; //gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
  FastLED.show();
}

void pattern1(uint16_t speed)
{
  static int streamer = 0;
  EVERY_N_MILLIS(speed)
  {
    switch (streamer)
    {
    case 0:
      leds[0] = CRGB::White;
      break;
    case 1:
      leds[1] = CRGB::White;
      leds[2] = CRGB::White;
      leds[3] = CRGB::White;
      break;
    case 2:
      for (int i = 4; i < 10; i++)
      {
        leds[i] = CRGB::White;
      }
      break;
    case 3:
      for (int i = 10; i < 15; i++)
      {
        leds[i] = CRGB::White;
      }
      break;
    case 4:
      for (int i = 15; i < 20; i++)
      {
        leds[i] = CRGB::White;
      }
      break;
    case 5:
      leds[20] = CRGB::White;
      break;
    case 6:
      leds[21] = CRGB::White;
      break;
    case 7:
      leds[22] = CRGB::White;
      break;
    case 8:
      leds[23] = CRGB::White;
      break;
    case 9:
      FastLED.clear();
    }
    FastLED.show();
    streamer++;
    if (streamer == 10)
    {
      streamer = 0;
    }
  }
}

void pattern2(uint16_t speed)
{
  uint8_t map[24] = {17,18,16,19,15,20,9,8,13,14,7,21,12,6,11,5,10,4,22,3,2,23,1,0};
  static uint8_t count = 0;
  static byte hue = HUE_RED;
  EVERY_N_MILLIS(speed)
  {
    leds[map[count]].setHue(hue);
    hue++;
    FastLED.show();
    count++;
    if(count == 24)
    {
      FastLED.clear();
      count = 0;
    }
  }
}

void pattern3(uint16_t speed)
{
  FastLED.clear();
  FastLED.show();
  char map[5][3] = {{5, 11, 2}, {7, 10, 1}, {9, 3, 13}, {8, 4, 12}, {6, 0, 0}};
  static int i = 0;
  for (int x = 0; x < 5; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      leds[map[x][y]] = CRGB(255, 255, 255);
      i++;
    }
    FastLED.show();
    delay(speed);
  }
  for (i = 14; i < 24; i++)
  {
    leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
  delay(300);
}

void string_party() {
    for (int i = NUM_LEDS-1; i >= 0; i--) {
      if (i < NUM_LEDS && i > NUM_LEDS-5) {
        leds[i] = CRGB::Gold; 
      }
      if (i < NUM_LEDS-4 && i > NUM_LEDS-10) {
        leds[i] = CRGB::Red;
        leds[i-5] = CRGB::Blue;
        leds[i-10] = CRGB(255, 0, 255);
        leds[i-15] = CRGB(0, 255, 0);
        delay(400);
        FastLED.show();
      }
    }
    for (int c = 0; c < NUM_LEDS -4; c++) {
      leds[c] = CRGB(255, 255, 255);
      FastLED.show();
      delay(500);
    }
}

// Main loop
void loop() { 
  switch(currentMode) {
    case POSITION_COLORS:
      position_colors();
      break;
    case STRING_ANIMATION: 
      string_animation(200, 75);
      break;
    case PRIDE_MODE:
      pride();
      break;
    case PATTERN1_MODE:
      pattern1(500);
      break;
    case PATTERN2_MODE:
      pattern2(500);
      break;
    case PATTERN3_MODE:
      pattern3(300);
      break;
    case STRING_PARTY:
      string_party();
      break;
    default:
       currentMode = POSITION_COLORS;
      break;
  }
}