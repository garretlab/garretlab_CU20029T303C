#ifndef CU20029_T303C_H
#define CU20029_T303C_H

#include <Arduino.h>

class garretlab_CU20029T303C : public Print {
public:
  using Print::print;

  enum cursorShape_t : uint8_t {
    underline = 0x16,
    block = 0x17,
    invert = 0x18,
  };

  enum displayMode_t : uint8_t {
    compatibleMode = 0x00,
    kanjiMode = 0x01,
  };

  enum script_t : uint8_t {
    japanese = 0x00,
    hangeul = 0x01,
    simplifiedChinese = 0x02,
    traditionalChinese = 0x03,
  };

  garretlab_CU20029T303C(HardwareSerial *serial, int resetPin, int busyPin);
  void begin(int baudRate);
  void clear();
  void home();
  void setCursor(int column, int row);
  size_t write(uint8_t c);
  size_t write(uint16_t s);
  size_t print(const char s[]);
  void cursor(cursorShape_t type = underline);
  void noCursor();
  void blink();
  void display();
  void noDisplay();
  void autoscroll();
  void noAutoscroll();
  void createChar(uint8_t location, uint8_t charmap[]);
  void createChar15x16(uint8_t location, uint8_t charmap[]);

  void hardReset();
  void softReset();
  void setBrightness(uint8_t brightness = 100);
  void brightness75();
  void brightness50();
  void brightness25();
  
  void setCompatibleMode();
  void setKanjiMode();
  void disable2ByteMode();
  void enable2ByteMode();
  void displayJapanese();
  void displayHangeul();
  void displaySimplifiedChinese();
  void displayTraditionalChinese();
  void displayAsciiThin();
  void displayAsciiThick();

  void thinMode();
  void thickMode();

private:
  HardwareSerial *serial;
  int resetPin;
  int busyPin;
  int baudRate;

  uint8_t displayMode = compatibleMode;
  uint8_t twoByteMode = false;
  uint8_t script = japanese;
};
#endif /* CU20029T303C_H */