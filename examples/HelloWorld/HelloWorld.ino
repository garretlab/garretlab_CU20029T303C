#include <garretlab_CU20029T303C.h>

const int resetPin = 8;
const int busyPin = 9;

garretlab_CU20029T303C vfd(&Serial1, resetPin, busyPin);

void setup() {
  vfd.begin(38400);

  vfd.setKanjiMode();
  vfd.enable2ByteMode();

  vfd.print("Hello World!!");
  vfd.setCursor(0, 1);
  vfd.print("日本語表示");
}

void loop() {
  // put your main code here, to run repeatedly:
}
