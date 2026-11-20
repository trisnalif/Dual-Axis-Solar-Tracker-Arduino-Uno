#include <Servo.h>

Servo panServo, tiltServo;
const byte PIN_TL = A0, PIN_TR = A1, PIN_BL = A2, PIN_BR = A3, PIN_PV = A4; // A0 (TL), A1 (TR), A2 (BL) and A3 (BR): top / bottom and left / right as seen from the front
const byte PIN_PAN = 9, PIN_TILT = 10, LED = 13;
const int GAIN = 48;                           // ADC counts of a pair for one degree of turn: half of what one degree gives
const int MAX_STEP = 6;                        // degrees in one move
const int NIGHT = 80;                          // all four LDRs under this: it is dark
const float LOAD_OHMS = 33.0;                  // the panel's load

int pan = 90, tilt = 45;                       // pan 0 east, 90 south, 180 west; tilt 0 upright, 90 flat
bool tracking = true, onSun = false, dark = false;
unsigned int rateMs = 1000;
unsigned long nextMove = 0, nextReport = 0;
int tl, tr, bl, br;
char line[24]; byte len = 0;

void readLdrs() { tl = analogRead(PIN_TL); tr = analogRead(PIN_TR); bl = analogRead(PIN_BL); br = analogRead(PIN_BR); }

void moveOnce() {
  readLdrs();
  dark = tl < NIGHT && tr < NIGHT && bl < NIGHT && br < NIGHT;
  if (!tracking) { onSun = false; return; }
  if (dark) { pan = 0; tilt = 20; onSun = false; }            // wait for the sunrise in the east
  else {
    const int sideways = (tl + bl) - (tr + br);               // positive: the left pair is brighter - more pan
    const int upDown = (tl + tr) - (bl + br);                 // positive: the upper pair is brighter - more tilt
    const float flat = max(0.25, cos(radians(tilt)));         // 1 with the panel upright, less as it lies flat
    const int dPan = constrain(int(sideways / (GAIN * flat)), -MAX_STEP, MAX_STEP);
    const int dTilt = constrain(upDown / GAIN, -MAX_STEP, MAX_STEP);
    pan = constrain(pan + dPan, 0, 180);
    tilt = constrain(tilt + dTilt, 0, 90);
    onSun = abs(sideways) < 2 * GAIN && abs(upDown) < 2 * GAIN;   // within a few degrees
  }
  panServo.write(pan);
  tiltServo.write(tilt);
  digitalWrite(LED, onSun);
}

void report() {
  const float volts = analogRead(PIN_PV) * (5.0 / 1023.0) * 2.0;   // the divider halves the panel's voltage
  Serial.print(F("TL ")); Serial.print(tl); Serial.print(F(" TR ")); Serial.print(tr);
  Serial.print(F(" BL ")); Serial.print(bl); Serial.print(F(" BR ")); Serial.print(br);
  Serial.print(F(" | pan ")); Serial.print(pan); Serial.print(F(" tilt ")); Serial.print(tilt);
  Serial.print(F(" | ")); Serial.print(volts, 2); Serial.print(F(" V ")); Serial.print(volts * volts / LOAD_OHMS, 2); Serial.print(F(" W  "));
  Serial.println(!tracking ? F("PARKED") : dark ? F("DARK") : onSun ? F("ON THE SUN") : F("TURNING"));
}

void help() { Serial.println(F("Commands: park | track | rate 1000 | help")); }

void command(char* s) {
  while (*s == ' ') s++;
  if (!*s) return;
  if (!strcmp(s, "park")) {
    tracking = false; pan = 90; tilt = 60; panServo.write(pan); tiltServo.write(tilt); digitalWrite(LED, LOW);
    Serial.println(F("Parked: south, 60 degrees - a fixed panel"));
    return;
  }
  if (!strcmp(s, "track")) { tracking = true; Serial.println(F("Tracking the sun")); return; }
  if (!strncmp(s, "rate ", 5)) {
    rateMs = constrain(atoi(s + 5), 200, 5000);
    Serial.print(F("Reporting every ")); Serial.print(rateMs); Serial.println(F(" ms"));
    return;
  }
  if (!strcmp(s, "help")) { help(); return; }
  Serial.print(F("Unknown: ")); Serial.println(s);
  help();
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  panServo.attach(PIN_PAN);
  tiltServo.attach(PIN_TILT);
  panServo.write(pan);
  tiltServo.write(tilt);
  Serial.println(F("Sun tracker"));
  help();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') { line[len] = 0; command(line); len = 0; }
    else if (len < sizeof(line) - 1) line[len++] = c;
  }
  const unsigned long now = millis();
  if ((long)(now - nextMove) >= 0) { nextMove = now + 50; moveOnce(); }
  if ((long)(now - nextReport) >= 0) { nextReport = now + rateMs; report(); }
}
