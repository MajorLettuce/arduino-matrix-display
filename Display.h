#ifndef Display_h
#define Display_h

#include "Arduino.h"

class Display {
  private:
    unsigned int rowClkPin, dataPin, displayClkPin, serialClkPin, clearPin;
    unsigned int width, height;
    byte buffer[256];
    unsigned int count = 0;
  public:
    Display(unsigned int rowClkPin, unsigned int dataPin, unsigned int displayClkPin,
            unsigned int serialClkPin, unsigned int clearPin, unsigned int width, unsigned int height);
    unsigned int getSize();
    unsigned int getWidth();
    unsigned int getHeight();
    void clear();
    void display();
    void writeByte(byte data);
    void displayByte(byte data);

    void scan();
    void scanFrame();

    void setPixel(unsigned int x, unsigned int y, byte brightness);
};

#endif
