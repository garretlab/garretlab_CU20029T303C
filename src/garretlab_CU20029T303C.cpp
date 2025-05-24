#include <garretlab_CU20029T303C.h>
#include <codeconversion.h>

garretlab_CU20029T303C::garretlab_CU20029T303C(HardwareSerial *serial, int resetPin, int busyPin) {
  this->serial = serial;
  this->resetPin = resetPin;
  this->busyPin = busyPin;
}

void garretlab_CU20029T303C::begin(int baudRate) {
  this->baudRate = baudRate;
  serial->begin(baudRate);

  pinMode(resetPin, HIGH);
  pinMode(busyPin, LOW);

  hardReset();
}

void garretlab_CU20029T303C::clear() {
  serial->write((uint8_t)0x0c);
}

void garretlab_CU20029T303C::home() {
  serial->write((uint8_t)0x0d);
}

void garretlab_CU20029T303C::setCursor(int column, int row) {
  serial->write((uint8_t)0x1b);
  serial->write(column + 20 * row);
}

size_t garretlab_CU20029T303C::write(uint8_t c) {
  return serial->write(c);
}

size_t garretlab_CU20029T303C::write(uint16_t s) {
  write((uint8_t)((s & 0xff00) >> 8));
  write((uint8_t)(s & 0x00ff));
  return 2;
}
size_t garretlab_CU20029T303C::print(const char s[]) {
  uint8_t c;
  uint16_t utf16, sjis;
  size_t num = 0, ret;

  while (*s) {
    if (displayMode == kanjiMode && twoByteMode == true) {
      switch (script) {
        case japanese:
          ret = utf8ToUtf16(s, &utf16);
          if (ret == 1) {
            serial->write(*s);
          } else {
            utf16ToSjis(utf16, &sjis);
            write(sjis);
          }
          s += ret;
          num += ret;
          break;
        default:
          serial->write(*s++);
          break;
      }
    } else {
      num += serial->write(*s++);
    }
  }
  return num;
}

void garretlab_CU20029T303C::cursor(cursorShape_t type) {
  serial->write((uint8_t)0x13);
  serial->write((uint8_t)type);
}

void garretlab_CU20029T303C::noCursor() {
  serial->write((uint8_t)0x14);
}

void garretlab_CU20029T303C::blink() {
  serial->write((uint8_t)0x15);
}

void garretlab_CU20029T303C::display() {
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x28);
  serial->write((uint8_t)0x61);
  serial->write((uint8_t)0x40);
  serial->write((uint8_t)0x00);
}

void garretlab_CU20029T303C::noDisplay() {
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x28);
  serial->write((uint8_t)0x61);
  serial->write((uint8_t)0x40);
  serial->write((uint8_t)0x01);
}

void garretlab_CU20029T303C::autoscroll() {
  serial->write((uint8_t)0x12);
}

void garretlab_CU20029T303C::noAutoscroll() {
  serial->write((uint8_t)0x11);
}

void garretlab_CU20029T303C::createChar(uint8_t location, uint8_t charmap[]) {
  serial->write((uint8_t)0x1a);
  serial->write(location);
  for (int i = 0; i < 7; i++) {
    serial->write(charmap[i]);
  }
}

void garretlab_CU20029T303C::createChar15x16(uint8_t location, uint8_t charmap[]) {
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x26);
  serial->write(location);
  for (int i = 0; i < 32; i++) {
    serial->write(charmap[i]);
  }
}

void garretlab_CU20029T303C::hardReset() { 
  digitalWrite(resetPin, LOW);
  delay(2);
  digitalWrite(resetPin, HIGH);
  delay(200);
}

void garretlab_CU20029T303C::softReset() {
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x49);
}

void garretlab_CU20029T303C::setBrightness(uint8_t brightness) {
  switch (brightness) {
  case 100:
    serial->write((uint8_t)0x01);
    break;
  case 75:
    serial->write((uint8_t)0x02);
    break;
  case 50:
    serial->write((uint8_t)0x03);
    break;
  case 25:
    serial->write((uint8_t)0x04);
    break;
  default:
    break;
  }
}

void garretlab_CU20029T303C::setCompatibleMode() {
  displayMode = compatibleMode;
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x28);
  serial->write((uint8_t)0x67);
  serial->write((uint8_t)0x01);
  serial->write((uint8_t)0x00);
}

void garretlab_CU20029T303C::setKanjiMode() {
  displayMode = kanjiMode;
  serial->write((uint8_t)0x1f);
  serial->write((uint8_t)0x28);
  serial->write((uint8_t)0x67);
  serial->write((uint8_t)0x01);
  serial->write((uint8_t)0x01);
}

void garretlab_CU20029T303C::disable2ByteMode() {
  if (displayMode == kanjiMode) {
    twoByteMode = false;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x02);
    serial->write((uint8_t)0x00);
  }
}

void garretlab_CU20029T303C::enable2ByteMode() {
  if (displayMode == kanjiMode) {
    twoByteMode = true;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x02);
    serial->write((uint8_t)0x01);
  }
}

void garretlab_CU20029T303C::displayJapanese() {
  if (displayMode == kanjiMode && twoByteMode == true) {
    script = japanese;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x0f);
    serial->write((uint8_t)0x00);
  }
}

void garretlab_CU20029T303C::displayHangeul() {
  if (displayMode == kanjiMode && twoByteMode == true) {
    script = hangeul;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x0f);
    serial->write((uint8_t)0x01);
  }
}

void garretlab_CU20029T303C::displaySimplifiedChinese() {
  if (displayMode == kanjiMode && twoByteMode == true) {
    script = simplifiedChinese;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x0f);
    serial->write((uint8_t)0x02);
  }
}

void garretlab_CU20029T303C::displayTraditionalChinese() {
  if (displayMode == kanjiMode && twoByteMode == true) {
    script = traditionalChinese;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x0f);
    serial->write((uint8_t)0x03);
  }
}

void garretlab_CU20029T303C::displayAsciiThin() {
  if (displayMode == kanjiMode) {
    script = traditionalChinese;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x41);
    serial->write((uint8_t)0x00);
  }
}

void garretlab_CU20029T303C::displayAsciiThick() {
  if (displayMode == kanjiMode) {
    script = traditionalChinese;
    serial->write((uint8_t)0x1f);
    serial->write((uint8_t)0x28);
    serial->write((uint8_t)0x67);
    serial->write((uint8_t)0x41);
    serial->write((uint8_t)0x01);
  }
}