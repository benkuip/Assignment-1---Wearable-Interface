// =====================================================================
// Assignment 1: Wearable Interface
// Benjamin Kuipers (3223784)
// October 12, 2026

// =====================================================================
// Please note the following code is entirely AI-generated using Claude.

#include <Adafruit_CircuitPlayground.h>

const int  TOUCH_THRESHOLD = 100;   // raise = less sensitive, lower = more sensitive
const byte CONFIRM_READS   = 3;     // consecutive touched reads required to trigger
const byte AMPLITUDE       = 20;    // 0-127; lower = less current, quieter
const unsigned int FREQ    = 880;   // Hz
const unsigned int BURST_MS = 60;

int baseline = 0;
byte hits = 0;

int readTouch() {
  long total = 0;
  for (byte i = 0; i < 5; i++) {
    total += CircuitPlayground.readCap(A4);
    delay(2);
  }
  return total / 5;
}

void beep() {
  unsigned long half = 500000UL / FREQ;
  unsigned long cycles = (unsigned long)FREQ * BURST_MS / 1000UL;

  CircuitPlayground.speaker.set(128);
  CircuitPlayground.speaker.enable(true);
  delay(5);

  for (unsigned long i = 0; i < cycles; i++) {
    CircuitPlayground.speaker.set(128 + AMPLITUDE);
    delayMicroseconds(half);
    CircuitPlayground.speaker.set(128 - AMPLITUDE);
    delayMicroseconds(half);
  }

  CircuitPlayground.speaker.set(128);
  delay(1);
  CircuitPlayground.speaker.enable(false);
}

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.speaker.enable(false);
  delay(1000);                         // set the board down, don't touch

  long total = 0;
  for (byte i = 0; i < 20; i++) {
    total += readTouch();
    delay(10);
  }
  baseline = total / 20;
}

void loop() {
  int reading = readTouch();
  int diff = reading - baseline;

  if (diff > TOUCH_THRESHOLD) {
    if (++hits >= CONFIRM_READS) {
      hits = 0;
      beep();
      delay(30);
    }
  } else {
    hits = 0;
    // Follow slow drift only while not touched
    baseline += (reading - baseline) / 16;
    delay(50);
  }
}
