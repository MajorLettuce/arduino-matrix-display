#include "Arduino.h"
#include "Display.h"

#define DISPLAY_MAX_BRIGHTNESS 10

Display::Display(unsigned int rowClkPin, unsigned int dataPin, unsigned int displayClkPin,
                 unsigned int serialClkPin, unsigned int clearPin, unsigned int width, unsigned int height) {
  // Initialize pins
  this->rowClkPin = rowClkPin;
  this->dataPin = dataPin;
  this->displayClkPin = displayClkPin;
  this->serialClkPin = serialClkPin;
  this->clearPin = clearPin;
  pinMode(rowClkPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(displayClkPin, OUTPUT);
  pinMode(serialClkPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  // Set default value for control signals
  digitalWrite(rowClkPin, LOW);
  digitalWrite(displayClkPin, LOW);
  digitalWrite(serialClkPin, LOW);
  digitalWrite(clearPin, HIGH);

  // Initialize buffer
  this->width = width;
  this->height = height;
  // Clear the buffer
  clear();
}

unsigned int Display::getSize() {
  return width * height;
}

unsigned int Display::getWidth() {
  return width;
}

unsigned int Display::getHeight() {
  return height;
}

void Display::clear() {
  //memset(buffer, 0, sizeof(buffer));
  // clear register (row content)
  writeByte(0);
  display();
  // reset row position
  digitalWrite(clearPin, LOW);
  // hardware load zero into the timer by sending clock to it
  digitalWrite(rowClkPin, HIGH);
  digitalWrite(rowClkPin, LOW);
  digitalWrite(clearPin, HIGH);
}

void Display::display() {
  digitalWrite(displayClkPin, HIGH);
  digitalWrite(displayClkPin, LOW);
}

void Display::writeByte(byte data) {
  shiftOut(dataPin, serialClkPin, MSBFIRST, data);
}

void Display::displayByte(byte data) {
  writeByte(data);
  display();
  //delay(3);
  writeByte(0);
  display();
}

void Display::scan() {
  digitalWrite(rowClkPin, HIGH);
  digitalWrite(rowClkPin, LOW);
}

void Display::scanFrame() {
  byte data;
  for (byte b = 1; b <= DISPLAY_MAX_BRIGHTNESS; b++) {
    for (int i = 0; i < height; i++) {
      data = 0;
      for (int j = 0; j < width; j++) {
        byte brightness = buffer[j + i * width];
        if (brightness == 0 || brightness < b) {
          bitClear(data, j);
        } else {
          bitSet(data, j);
        }
        //data |= (buffer[j + i * width] & 1) << j;
      }
      displayByte(data);
      scan();
    }
  }
}

void Display::setPixel(unsigned int x, unsigned int y, byte brightness) {
  if (x < width && y < height) {
    if (brightness > DISPLAY_MAX_BRIGHTNESS) {
      brightness = DISPLAY_MAX_BRIGHTNESS;
    } else if (brightness < 0) {
      brightness = 0;
    }
    buffer[x + y * width] = brightness;
  }
}


