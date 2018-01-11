#include "Display.h"

Display display = Display(7, 6, 5, 4, 3, 5, 6);

enum Demo {
  None,
  Waves,
  Stars
};
Demo currentDemo = None;
int position[5];
bool started[5] = { false, false, false, false, false };

struct Star {
  unsigned int x = 0, y = 0;
  byte brightness = 0;
} stars[10];

int wait = 0;

void setup() {
  pinMode(13, INPUT_PULLUP);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  //while (!Serial.available());
  //Serial.println(display.getSize());
  //display.writeByte(0b1);
  //display.display();
  //display.setPixel(0, 0, 7);
  //display.setPixel(1, 0, 1);

  memset(position, 0, sizeof(position));
  //memset(stars, 0, sizeof(stars));
  /*
    for(int i = 0; i < sizeof(stars); i++) {
      stars[i] = new Star(0, 0);
    }
  */
  //display.clear();

  //started[2] = true;
  currentDemo = random(1, 3);
}

void loop() {
  display.scanFrame();

  switch (currentDemo) {
    case Waves: {
        if (wait > 5) {

          for (int i = 0; i < sizeof(started); i++) {
            if (started[i] == false) {
              started[i] = true;
              break;
            }
          }

          for (int i = 0; i < sizeof(started); i++) {
            if (started[i] == false) {
              continue;
            }
            display.setPixel(i, position[i], 5);
            display.setPixel(i, position[i] + 1, 0);
            display.setPixel(i, position[i] + 2, 0);
            if (position[i] > 0) {
              display.setPixel(i, position[i] - 1, 10);
              display.setPixel(i, position[i] - 2, 5);
              display.setPixel(i, position[i] - 3, 0);
              display.setPixel(i, position[i] - 4, 0);
            }
            position[i] = ++position[i] % 6;
          }
          wait = 0;
        }
        wait++;
        break;
      }
    case Stars: {
        if (wait > 1) {
          unsigned int count = sizeof(stars) / sizeof(Star);
          for (int i = 0; i < count; i++) {
            bool overlap = false;
            // Star can be refreshed
            if (stars[i].brightness == 0) {
              stars[i].x = random(0, display.getWidth());
              stars[i].y = random(0, display.getHeight());
              for (int j = 0; j < count; j++) {
                if (stars[i].x == stars[j].x && stars[i].y == stars[j].y && stars[j].brightness > 0) {
                  overlap = true;
                  break;
                }
              }
              if (!overlap) {
                //display.setPixel(stars[i].x, stars[i].y, 9);
                if (random(0, 10) == 0) {
                  stars[i].brightness = 9;
                }
              }
            } else {
              if (random(0, 3) == 0) {
                stars[i].brightness--;
              }
            }
            if (!overlap) {
              display.setPixel(stars[i].x, stars[i].y, stars[i].brightness);
            }
          }
          wait = 0;
        }
        wait++;
        break;
        /*
          //display.setPixel(1, 1, 9);
          //if (random(0, 10) == 0) {
          unsigned int star = random(0, sizeof(stars));
          if (stars[star].brightness == 0) {
            stars[star].brightness = 9;
            stars[star].x = random(0, display.getWidth());
            stars[star].y = random(0, display.getHeight());
          }
          //}
          for (int i = 0; i < sizeof(stars); i++) {
            if (stars[i].brightness > 0 && random(0, 3) == 0) {
              //display.setPixel(stars[i].x, stars[i].y, stars[i].brightness--);
            } else {
              //display.setPixel(stars[i].x, stars[i].y, 0);
            }
          }
          wait = 0;
          }
          wait++;
        */
        break;
      }
  }
}
