/*
  Wall Ball Counter (web version)
  Arduino UNO R4 WiFi + 2x Grove ADXL345
  ==========================================

  The Arduino opens its own WiFi hotspot and serves the display page.
    WiFi name and password: see secrets.h (defaults in secrets.example.h)
    Address:                http://192.168.4.1

  Only the sensor of the active (green) zone counts:
    Upper half = Sensor O (oben, standard I2C)
    Lower half = Sensor U (unten, software I2C)

  Per impact on the active sensor:
    below noise limit        -> ignored
    noise limit .. threshold -> No Rep
    threshold and above      -> Rep
  The peak of an impact is searched within the measuring window after
  the impact starts. Noise limit, hit threshold, lockout and measuring
  window can be changed on the page via the gear icon and are saved
  permanently. The settings page also shows a frozen recording of the
  last impact (0.2 s before, up to 0.8 s after).

  The settings page is protected by a 4-digit code (start code =
  SETTINGS_PIN_DEFAULT from secrets.h, can be changed on the settings
  page). Forgot the code? Open the Serial Monitor (115200 baud), type
  PINRESET and press Enter – the code is back to the start code.

  When the goal is reached, GO! shows for 5 s, then the counter
  starts again at 0.

  Resting position: if a sensor has been completely still for 2 s but
  is no longer in its saved resting position (tilted, moved), it takes
  the new position as resting position (auto-zero). An impact must
  also RISE by at least the noise limit, so a constant offset never
  counts. The "Kalibrieren" button on the settings page re-zeroes both
  sensors by hand.

  Wiring with the Grove Base Shield V2 (switch on 5V)
    Sensor O = TOP zone    -> any I2C port
    Sensor U = BOTTOM zone -> port D6 (uses D6 + D7)
  Wiring without shield (jumper wires)
    Sensor O: yellow -> SCL, white -> SDA, red -> 5V, black -> GND
    Sensor U: yellow -> D6, white -> D7, red -> D8, black -> GND

  Keep the sensors still for ~1 s after power-on (calibration).

  Files in this sketch folder:
    wallball_web.ino   – this file (logic)
    page.h             – the web page (HTML/CSS/JS), generated from web/index.html
    secrets.example.h  – default WiFi name/password, start code, page title
    secrets.h          – optional, your own values (not in git, overrides the example)
    logo_default.h     – neutral placeholder logo
    logo.h             – optional, your own logo (not in git, overrides the placeholder)
*/

#include <ctype.h>
#include <Wire.h>
#include <WiFiS3.h>
#include <EEPROM.h>
#include "Arduino_LED_Matrix.h"
#include "page.h"

// Your own settings: copy secrets.example.h to secrets.h and edit it.
// secrets.h is not part of the repository.
#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #include "secrets.example.h"
#endif

// Your own logo: create logo.h with tools/make_logo.py (not part of the repository).
#if __has_include("logo.h")
  #include "logo.h"
#else
  #include "logo_default.h"
#endif

// ============================================================
//  Settings – change these if needed
// ============================================================

const char AP_SSID[] = WIFI_SSID;
const char AP_PASS[] = WIFI_PASS;             // at least 8 characters
const char PAGE_TITLE_TEXT[] = PAGE_TITLE;    // browser tab title

// Defaults – can be changed on the web page (gear icon), saved permanently
//   below NOISE            -> ignored (vibration, noise)
//   between NOISE and HIT  -> No Rep (too weak)
//   HIT and above          -> Rep
const float NOISE_G_DEFAULT      = 0.1;   // everything below is noise (g)
const float HIT_G_DEFAULT        = 3.0;   // impact needed for a valid rep (g)
const unsigned long LOCKOUT_DEFAULT = 1000;  // min. time between two counted impacts (ms)
const unsigned long WINDOW_DEFAULT  = 100;    // measuring window: peak search after impact start (ms)
const int DEFAULT_GOAL = 20;
const uint16_t PIN_DEFAULT = SETTINGS_PIN_DEFAULT;   // start code (also after PINRESET)
const unsigned long GO_SHOW_MS = 5000;       // GO! time, then the counter restarts at 0
const unsigned long CALM_MS = 2000;          // still this long -> auto-zero
const int   CALM_LSB = 15;                   // "still" = every axis within +-15 steps (~0.06 g)
const float ZERO_MIN_G = 0.05;               // only re-zero if the offset is at least this

// Software I2C pins for sensor 2
const int SW_SCL   = 6;
const int SW_SDA   = 7;
const int SW_POWER = 8;   // only needed for jumper wiring (with the Base Shield, port D6 has its own 5V)

// ============================================================
//  Types (defined before any function – Arduino IDE needs that)
// ============================================================

enum Zone { ZONE_UP = 0, ZONE_DOWN = 1 };
enum EventType { EV_NONE = 0, EV_HIT, EV_WEAK, EV_WRONG };

struct Sensor {
  int bus;          // 0 = hardware I2C, 1 = software I2C
  bool ok;
  float bx, by, bz; // resting position (raw units)
};

Sensor sensors[2] = {
  { 0, false, 0, 0, 0 },   // index 0 = top
  { 1, false, 0, 0, 0 }    // index 1 = bottom
};

// Auto-zero: watches whether a sensor lies completely still
struct CalmTrack {
  int16_t rx, ry, rz;          // first sample of the still period
  long sx, sy, sz;             // sums for the average
  int n;
  unsigned long startTs, lastTs;
  float recent[10];            // last 10 deflections (50 ms) for the rise check
  int recentIdx;
};
CalmTrack calm[2];

// Saved in EEPROM so settings survive a restart
// (new fields are only ever added at the end, so older saved settings still load)
struct Settings {
  uint32_t magic;
  float    hitG;
  float    noiseG;
  uint16_t lockoutMs;
  uint16_t goal;
  uint16_t windowMs;   // added in format v3
  uint16_t pin;        // added in format v4 (0..9999, shown with 4 digits)
};
const uint32_t SETTINGS_MAGIC  = 0x4C555834;   // format v4 marker (ASCII)
const uint32_t SETTINGS_MAGIC3 = 0x4C555833;   // format v3 marker (without pin)
const uint32_t SETTINGS_MAGIC2 = 0x4C555832;   // format v2 marker (without windowMs and pin)

// One entry of the "last 5 impacts" list on the settings page
struct HitRecord {
  unsigned long id;    // event number, also identifies its recording
  unsigned long t;     // millis() when the impact started
  uint8_t sensor;      // 1 or 2
  uint16_t peak;       // 1/100 g
  uint8_t type;        // EV_HIT or EV_WEAK
  uint16_t peakOff;    // ms from impact start to the peak
  uint16_t win;        // measuring window used (ms)
};

// ============================================================
//  ADXL345 registers
// ============================================================

const uint8_t ADXL_ADDR       = 0x53;
const uint8_t REG_DEVID       = 0x00;
const uint8_t REG_BW_RATE     = 0x2C;
const uint8_t REG_POWER_CTL   = 0x2D;
const uint8_t REG_DATA_FORMAT = 0x31;
const uint8_t REG_DATAX0      = 0x32;
const uint8_t REG_FIFO_CTL    = 0x38;
const uint8_t REG_FIFO_STATUS = 0x39;
const float   G_PER_LSB       = 0.0039f;

// ============================================================
//  Counter state
// ============================================================

int  reps   = 0;
int  noReps = 0;
int  goal   = DEFAULT_GOAL;
float hitG = HIT_G_DEFAULT;
float noiseG = NOISE_G_DEFAULT;
unsigned long lockoutMs = LOCKOUT_DEFAULT;
unsigned long windowMs = WINDOW_DEFAULT;
uint16_t settingsPin = PIN_DEFAULT;
int pinFails = 0;                        // wrong codes in a row
unsigned long pinBlockedUntil = 0;       // after 5 wrong codes: 30 s pause
Zone activeZone = ZONE_UP;
bool done = false;
unsigned long firstRepMs = 0;
unsigned long lastRepMs  = 0;

unsigned long eventSeq = 0;      // increases with every hit / no rep
EventType lastEvent = EV_NONE;
float lastPeakTop = 0, lastPeakBottom = 0;

// impact detection
bool inEvent = false;
unsigned long eventStartMs = 0;          // sample time of the first sample above the noise limit
unsigned long eventPeakMs = 0;           // sample time of the peak
float eventPeakPrimary = 0;
unsigned long lockoutUntil = 0;

// Recent history of both sensors: strongest value per 10 ms
const unsigned long SAMPLE_MS = 5;       // sensors measure at 200 Hz
const unsigned long BUCKET_MS = 10;
const int RING_LEN = 300;                // 300 x 10 ms = 3.0 s
uint16_t ring[2][RING_LEN];              // 1/100 g
uint32_t ringStamp[RING_LEN];            // which 10 ms bucket a slot currently holds

// Frozen recordings of the last 5 impacts (0.2 s before, 0.8 s after);
// the page shows 0.2 to 0.8 s of the part after the impact
const int SNAP_PRE  = 20;                // buckets before the impact (0.2 s)
const int SNAP_LEN  = 100;               // 1.0 s
const unsigned long SNAP_AFTER_MS = 800;
uint16_t snap[5][2][SNAP_LEN];
unsigned long snapId[5];                 // event number stored in this slot (0 = empty)
unsigned long snapStart[5];
unsigned long snapDue[5];                // when the 1.5 s after the impact are complete
bool snapReady[5];

unsigned long lastPlotRequest = 0;       // inactive sensor is only read while the settings page is open
bool wasPlotting = false;

HitRecord history[5];
int historyCount = 0;

unsigned long overflowTimes[2][8];       // when a sensor's FIFO ran full
int overflowIdx[2] = { 0, 0 };

// LED matrix
ArduinoLEDMatrix matrix;
uint8_t frame[8][12];
int  shownReps = -1;
bool matrixGo = false;
unsigned long goStartMs = 0;

WiFiServer server(80);

// ============================================================
//  Software I2C (bit-banged)
// ============================================================

// Bus speed of the software I2C. 3 µs per half step gives about 100 kHz:
// slow enough for 50 cm Grove cables, fast enough for 200 samples per second.
void swDelay()    { delayMicroseconds(3); }
void sdaLow()     { pinMode(SW_SDA, OUTPUT); digitalWrite(SW_SDA, LOW); }
void sdaRelease() { pinMode(SW_SDA, INPUT_PULLUP); }
void sclLow()     { pinMode(SW_SCL, OUTPUT); digitalWrite(SW_SCL, LOW); }
void sclRelease() {
  pinMode(SW_SCL, INPUT_PULLUP);
  unsigned long t0 = micros();
  while (digitalRead(SW_SCL) == LOW && micros() - t0 < 1000) { }
}

void swStart() { sdaRelease(); sclRelease(); swDelay(); sdaLow(); swDelay(); sclLow(); swDelay(); }
void swStop()  { sdaLow(); swDelay(); sclRelease(); swDelay(); sdaRelease(); swDelay(); }

bool swWriteByte(uint8_t b) {
  for (int i = 7; i >= 0; i--) {
    if (b & (1 << i)) sdaRelease(); else sdaLow();
    swDelay(); sclRelease(); swDelay(); sclLow();
  }
  sdaRelease(); swDelay(); sclRelease(); swDelay();
  bool ack = (digitalRead(SW_SDA) == LOW);
  sclLow(); swDelay();
  return ack;
}

uint8_t swReadByte(bool sendAck) {
  uint8_t b = 0;
  sdaRelease();
  for (int i = 0; i < 8; i++) {
    swDelay(); sclRelease(); swDelay();
    b = (b << 1) | (digitalRead(SW_SDA) == HIGH ? 1 : 0);
    sclLow();
  }
  if (sendAck) sdaLow(); else sdaRelease();
  swDelay(); sclRelease(); swDelay(); sclLow(); sdaRelease();
  return b;
}

// ============================================================
//  Register access for both buses
// ============================================================

bool writeReg(int bus, uint8_t reg, uint8_t value) {
  if (bus == 0) {
    Wire.beginTransmission(ADXL_ADDR);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
  }
  swStart();
  bool ok = swWriteByte(ADXL_ADDR << 1) && swWriteByte(reg) && swWriteByte(value);
  swStop();
  return ok;
}

unsigned long i2cErrors[2] = { 0, 0 };   // failed reads per bus (0 = sensor O, 1 = sensor U)
unsigned long slowHttpMs = 0;            // slowest web request since the last overflow note
char slowHttpPath[24] = "";
char lastHttpPath[24] = "";

bool readRegs(int bus, uint8_t reg, uint8_t *buf, int n) {
  if (bus == 0) {
    Wire.beginTransmission(ADXL_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) { i2cErrors[0]++; return false; }
    if (Wire.requestFrom(ADXL_ADDR, (size_t)n) != (size_t)n) { i2cErrors[0]++; return false; }
    for (int i = 0; i < n; i++) buf[i] = Wire.read();
    return true;
  }
  swStart();
  if (!swWriteByte(ADXL_ADDR << 1) || !swWriteByte(reg)) { swStop(); i2cErrors[1]++; return false; }
  swStart();
  if (!swWriteByte((ADXL_ADDR << 1) | 1)) { swStop(); i2cErrors[1]++; return false; }
  for (int i = 0; i < n; i++) buf[i] = swReadByte(i < n - 1);
  swStop();
  return true;
}

bool readXYZ(int bus, int16_t &x, int16_t &y, int16_t &z) {
  uint8_t b[6];
  if (!readRegs(bus, REG_DATAX0, b, 6)) return false;
  x = (int16_t)((b[1] << 8) | b[0]);
  y = (int16_t)((b[3] << 8) | b[2]);
  z = (int16_t)((b[5] << 8) | b[4]);
  return true;
}

int fifoCount(int bus) {
  uint8_t v = 0;
  if (!readRegs(bus, REG_FIFO_STATUS, &v, 1)) return 0;
  return v & 0x3F;
}

// Deflection from the resting position in g
float deflection(Sensor &s, int16_t x, int16_t y, int16_t z) {
  float dx = (x - s.bx) * G_PER_LSB;
  float dy = (y - s.by) * G_PER_LSB;
  float dz = (z - s.bz) * G_PER_LSB;
  return sqrt(dx * dx + dy * dy + dz * dz);
}

// ============================================================
//  Sensor setup
// ============================================================

bool initSensor(Sensor &s) {
  uint8_t id = 0;
  if (!readRegs(s.bus, REG_DEVID, &id, 1) || id != 0xE5) return false;
  writeReg(s.bus, REG_DATA_FORMAT, 0x0B);  // full resolution, +-16 g
  // 200 Hz: FIFO holds 160 ms, so the slow software-I2C sensor keeps up
  // even while the Arduino is busy with WiFi. Both sensors use the same
  // rate so their g values stay comparable.
  writeReg(s.bus, REG_BW_RATE,     0x0B);
  writeReg(s.bus, REG_FIFO_CTL,    0x00);  // FIFO off during calibration
  writeReg(s.bus, REG_POWER_CTL,   0x08);  // measure
  return true;
}

void calibrate() {
  long sum[2][3] = { { 0, 0, 0 }, { 0, 0, 0 } };
  int  cnt[2]    = { 0, 0 };
  unsigned long t0 = millis();
  while (millis() - t0 < 1000) {
    for (int i = 0; i < 2; i++) {
      if (!sensors[i].ok) continue;
      int16_t x, y, z;
      if (readXYZ(sensors[i].bus, x, y, z)) {
        sum[i][0] += x; sum[i][1] += y; sum[i][2] += z; cnt[i]++;
      }
    }
    delay(3);
  }
  for (int i = 0; i < 2; i++) {
    if (cnt[i] == 0) continue;
    sensors[i].bx = (float)sum[i][0] / cnt[i];
    sensors[i].by = (float)sum[i][1] / cnt[i];
    sensors[i].bz = (float)sum[i][2] / cnt[i];
    writeReg(sensors[i].bus, REG_FIFO_CTL, 0x9F);   // FIFO stream mode
  }
}

// ============================================================
//  Impact analysis
// ============================================================

// Only the sensor of the active zone counts:
//   up arrow   -> sensor 1 (index 0, hardware I2C)
//   down arrow -> sensor 2 (index 1, software I2C)
int activeSensor() { return activeZone == ZONE_UP ? 0 : 1; }

// Empty a sensor's FIFO (old samples) without analysing them
void flushFifo(int idx) {
  if (!sensors[idx].ok) return;
  int n = fifoCount(sensors[idx].bus);
  for (int i = 0; i < n; i++) {
    int16_t x, y, z;
    if (!readXYZ(sensors[idx].bus, x, y, z)) break;
  }
}

void setZone(Zone z) {
  if (z == activeZone) return;
  activeZone = z;
  inEvent = false;
  flushFifo(activeSensor());
  lockoutUntil = millis() + 200;
}

void registerEvent(EventType type, float peak) {
  unsigned long now = millis();
  lastEvent = type;
  lastPeakTop    = (activeSensor() == 0) ? peak : 0;
  lastPeakBottom = (activeSensor() == 1) ? peak : 0;
  eventSeq++;

  if (type == EV_HIT) {
    reps++;
    if (reps == 1) firstRepMs = now;
    lastRepMs = now;
    if (reps >= goal && !done) {
      done = true;
      matrixGo = true;
      goStartMs = now;
    }
  } else {
    noReps++;
  }

  // remember for the "last 5 impacts" list (newest first)
  for (int i = 4; i > 0; i--) history[i] = history[i - 1];
  history[0].id = eventSeq;
  history[0].t = eventStartMs;
  history[0].sensor = activeSensor() + 1;
  history[0].peak = (uint16_t)min(peak * 100.0f, 65000.0f);
  history[0].type = (uint8_t)type;
  history[0].peakOff = (uint16_t)(eventPeakMs - eventStartMs);
  history[0].win = (uint16_t)windowMs;
  if (historyCount < 5) historyCount++;

  // schedule the recording; it is complete 0.8 s after the impact
  // (+80 ms because sensor 2 delivers its samples in 40 ms batches)
  int slot = eventSeq % 5;
  snapId[slot] = eventSeq;
  snapStart[slot] = eventStartMs;
  snapDue[slot] = eventStartMs + SNAP_AFTER_MS + 80;
  snapReady[slot] = false;

  Serial.print(type == EV_HIT ? F("REP           ") : F("NO REP (weak) "));
  Serial.print(activeSensor() == 0 ? F("sensor O (top) = ") : F("sensor U (bottom) = "));
  Serial.print(peak, 2);
  Serial.print(F(" g   reps="));  Serial.print(reps);
  Serial.print(F(" noReps="));    Serial.println(noReps);
}

void finishEvent() {
  float peak = eventPeakPrimary;
  inEvent = false;

  if (done) return;                    // goal reached – wait for reset
  if (peak < noiseG) {                 // noise / vibration, ignore
    lockoutUntil = millis() + 100;
    return;
  }
  // Nothing else counts until lockoutMs after the START of this impact
  lockoutUntil = eventStartMs + lockoutMs;

  if (peak < hitG) registerEvent(EV_WEAK, peak);
  else             registerEvent(EV_HIT,  peak);
}

// ---------- buffer overflow tracking ----------

// gap = ms since this sensor was read last time.
// Large gap (> 160 ms): something else blocked the loop (e.g. WiFi).
// Small gap but I2C errors rising: the cable/bus is the problem.
void noteOverflow(int idx, unsigned long gap) {
  overflowTimes[idx][overflowIdx[idx] % 8] = millis();
  overflowIdx[idx]++;
  static unsigned long lastWarn[2] = { 0, 0 };
  if (millis() - lastWarn[idx] > 10000) {
    Serial.print(F("Note: sensor "));
    Serial.print(idx == 0 ? 'O' : 'U');
    Serial.print(F(" buffer overflow. Gap since last read: "));
    Serial.print(gap);
    Serial.print(F(" ms, I2C errors so far: "));
    Serial.print(i2cErrors[idx]);
    Serial.print(F(", slowest web request: "));
    Serial.print(slowHttpMs);
    Serial.print(F(" ms "));
    Serial.println(slowHttpPath);
    lastWarn[idx] = millis();
    slowHttpMs = 0;
  }
}

int overflowsLast10s(int idx) {
  int n = 0;
  unsigned long now = millis();
  for (int i = 0; i < 8; i++) {
    unsigned long t = overflowTimes[idx][i];
    if (t != 0 && now - t < 10000) n++;
  }
  return n;
}

// The graph needs both sensors – only while the settings page is open
bool plotting() { return lastPlotRequest != 0 && millis() - lastPlotRequest < 2000; }

// Store a sample in the 10 ms history
void writeRing(int idx, unsigned long ts, float m) {
  uint32_t b = ts / BUCKET_MS;
  int r = b % RING_LEN;
  if (ringStamp[r] != b) { ringStamp[r] = b; ring[0][r] = 0; ring[1][r] = 0; }
  uint16_t v = (uint16_t)min(m * 100.0f, 65000.0f);
  if (v > ring[idx][r]) ring[idx][r] = v;
}

// Restart the still-period watch with this sample
void calmRestart(CalmTrack &c, int16_t x, int16_t y, int16_t z, unsigned long ts) {
  c.rx = x; c.ry = y; c.rz = z;
  c.sx = x; c.sy = y; c.sz = z; c.n = 1;
  c.startTs = ts; c.lastTs = ts;
}

// Auto-zero: still for CALM_MS but away from the resting position -> new resting position
void trackCalm(int idx, int16_t x, int16_t y, int16_t z, unsigned long ts, bool busy) {
  CalmTrack &c = calm[idx];
  bool gap   = c.n == 0 || ts - c.lastTs > 100;     // sensor was not read for a while
  bool moved = abs(x - c.rx) > CALM_LSB || abs(y - c.ry) > CALM_LSB || abs(z - c.rz) > CALM_LSB;
  if (gap || moved || busy) { calmRestart(c, x, y, z, ts); return; }
  c.sx += x; c.sy += y; c.sz += z; c.n++;
  c.lastTs = ts;
  if (ts - c.startTs < CALM_MS) return;

  Sensor &s = sensors[idx];
  float ax = (float)c.sx / c.n, ay = (float)c.sy / c.n, az = (float)c.sz / c.n;
  float dx = (ax - s.bx) * G_PER_LSB, dy = (ay - s.by) * G_PER_LSB, dz = (az - s.bz) * G_PER_LSB;
  float off = sqrt(dx * dx + dy * dy + dz * dz);
  if (off >= ZERO_MIN_G) {
    s.bx = ax; s.by = ay; s.bz = az;
    for (int k = 0; k < 10; k++) c.recent[k] = 0;
    Serial.print(F("Auto-zero sensor ")); Serial.print(idx == 0 ? 'O' : 'U');
    Serial.print(F(": offset was ")); Serial.print(off, 2); Serial.println(F(" g"));
  }
  calmRestart(c, x, y, z, ts);
}

// Re-zero both sensors by hand ("Neu kalibrieren" on the settings page). Takes about 1 s.
void recalibrate() {
  for (int i = 0; i < 2; i++) if (sensors[i].ok) writeReg(sensors[i].bus, REG_FIFO_CTL, 0x00);
  calibrate();                              // keeps still for 1 s, switches the FIFOs back on
  flushFifo(0);
  flushFifo(1);
  for (int i = 0; i < 2; i++) { calm[i].n = 0; for (int k = 0; k < 10; k++) calm[i].recent[k] = 0; }
  inEvent = false;
  lockoutUntil = millis() + 200;
  Serial.println(F("Sensors re-zeroed."));
}

// Reads everything waiting in one sensor's FIFO.
// detect = true: this sensor decides about reps (active zone).
void readSensor(int idx, bool detect) {
  Sensor &s = sensors[idx];
  if (!s.ok) return;

  // The software-I2C sensor is read in batches every 40 ms: asking
  // "how many samples are waiting?" costs as much as reading one sample,
  // so asking constantly would waste most of the slow bus time.
  static unsigned long lastRead[2] = { 0, 0 };
  if (s.bus == 1 && millis() - lastRead[idx] < 40) return;
  unsigned long gap = millis() - lastRead[idx];
  lastRead[idx] = millis();

  int n = fifoCount(s.bus);
  if (n >= 32) noteOverflow(idx, gap);
  unsigned long now = millis();

  for (int i = 0; i < n; i++) {
    int16_t x, y, z;
    if (!readXYZ(s.bus, x, y, z)) break;
    float m = deflection(s, x, y, z);
    // The FIFO holds the samples in order, 5 ms apart; the last one is the newest.
    // Its own time stamp makes the measuring window exact, however late we read it.
    unsigned long ts = now - (unsigned long)(n - 1 - i) * SAMPLE_MS;
    writeRing(idx, ts, m);

    // lowest deflection of the last 50 ms (before this sample)
    CalmTrack &c = calm[idx];
    float recentMin = c.recent[0];
    for (int k = 1; k < 10; k++) if (c.recent[k] < recentMin) recentMin = c.recent[k];
    c.recent[c.recentIdx] = m;
    c.recentIdx = (c.recentIdx + 1) % 10;

    trackCalm(idx, x, y, z, ts, detect && inEvent);

    if (detect && inEvent && ts > eventStartMs + windowMs) finishEvent();   // window over

    if (detect && inEvent) {
      if (m > eventPeakPrimary) { eventPeakPrimary = m; eventPeakMs = ts; }
    } else if (detect && ts >= lockoutUntil && m >= noiseG && m - recentMin >= noiseG) {
      // impact starts: above the noise limit AND a real rise
      // (a sensor that is merely tilted sits constantly high and never starts one)
      inEvent = true;
      eventStartMs = ts;
      eventPeakMs = ts;
      eventPeakPrimary = m;
    } else if (m < 0.3f) {
      // follow slow drift of the resting position
      s.bx += (x - s.bx) * 0.0005f;
      s.by += (y - s.by) * 0.0005f;
      s.bz += (z - s.bz) * 0.0005f;
    }
  }
}

// Copy the 1 second around an impact out of the history once it is complete
void captureSnapshots() {
  unsigned long now = millis();
  for (int k = 0; k < 5; k++) {
    if (snapId[k] == 0 || snapReady[k] || (long)(now - snapDue[k]) < 0) continue;
    uint32_t b0 = snapStart[k] / BUCKET_MS - SNAP_PRE;
    for (int i = 0; i < SNAP_LEN; i++) {
      uint32_t b = b0 + i;
      int r = b % RING_LEN;
      bool valid = (ringStamp[r] == b);
      snap[k][0][i] = valid ? ring[0][r] : 0;
      snap[k][1][i] = valid ? ring[1][r] : 0;
    }
    snapReady[k] = true;
  }
}

// Called as often as possible – also while sending web pages
void processSensors() {
  int a = activeSensor();
  int other = 1 - a;
  bool plot = plotting();

  if (plot && !wasPlotting) flushFifo(other);   // drop stale samples
  wasPlotting = plot;

  readSensor(a, true);
  if (plot) readSensor(other, false);

  // safety net: finish an impact even if no further samples arrive
  if (inEvent && millis() - eventStartMs > windowMs + 400) finishEvent();
  captureSnapshots();
}

// ============================================================
//  LED matrix (small 3x5 digits, big GO!)
// ============================================================

const uint8_t DIGITS[10][5] = {
  { 0b111, 0b101, 0b101, 0b101, 0b111 }, { 0b010, 0b110, 0b010, 0b010, 0b111 },
  { 0b111, 0b001, 0b111, 0b100, 0b111 }, { 0b111, 0b001, 0b111, 0b001, 0b111 },
  { 0b101, 0b101, 0b111, 0b001, 0b001 }, { 0b111, 0b100, 0b111, 0b001, 0b111 },
  { 0b111, 0b100, 0b111, 0b101, 0b111 }, { 0b111, 0b001, 0b001, 0b001, 0b001 },
  { 0b111, 0b101, 0b111, 0b101, 0b111 }, { 0b111, 0b101, 0b111, 0b001, 0b111 }
};
const uint8_t BIG_G[8]    = { 0b0110, 0b1001, 0b1000, 0b1000, 0b1011, 0b1001, 0b1001, 0b0110 };
const uint8_t BIG_O[8]    = { 0b0110, 0b1001, 0b1001, 0b1001, 0b1001, 0b1001, 0b1001, 0b0110 };
const uint8_t BIG_EXCL[8] = { 1, 1, 1, 1, 1, 1, 0, 1 };

int drawGlyph(const uint8_t glyph[], int width, int height, int top, int col) {
  for (int r = 0; r < height; r++)
    for (int c = 0; c < width; c++)
      if (((glyph[r] >> (width - 1 - c)) & 1) && col + c >= 0 && col + c < 12 && top + r < 8)
        frame[top + r][col + c] = 1;
  return col + width + 1;
}

void matrixNumber(int n) {
  memset(frame, 0, sizeof(frame));
  char t[5];
  snprintf(t, sizeof(t), "%d", n);
  int len = strlen(t);
  int col = (12 - (len * 3 + len - 1)) / 2;
  for (int i = 0; i < len; i++) col = drawGlyph(DIGITS[t[i] - '0'], 3, 5, 1, col);
  matrix.renderBitmap(frame, 8, 12);
}

void matrixGoScreen() {
  memset(frame, 0, sizeof(frame));
  int col = drawGlyph(BIG_G, 4, 8, 0, 0);
  col = drawGlyph(BIG_O, 4, 8, 0, col);
  drawGlyph(BIG_EXCL, 1, 8, 0, col);
  matrix.renderBitmap(frame, 8, 12);
}

void updateMatrix() {
  if (matrixGo) {
    if (shownReps != -2) { matrixGoScreen(); shownReps = -2; }
    if (millis() - goStartMs >= 5000) matrixGo = false;
    return;
  }
  if (shownReps != reps) { matrixNumber(reps); shownReps = reps; }
}

// ============================================================
//  Web server
// ============================================================

void loadSettings() {
  Settings s;
  EEPROM.get(0, s);
  bool current = (s.magic == SETTINGS_MAGIC);
  bool older   = (s.magic == SETTINGS_MAGIC3 || s.magic == SETTINGS_MAGIC2);
  if (!current && !older) return;                      // nothing saved yet
  if (s.hitG >= 0.3f && s.hitG <= 16.0f)             hitG = s.hitG;
  if (s.noiseG >= 0.05f && s.noiseG < hitG)          noiseG = s.noiseG;
  if (s.lockoutMs >= 300 && s.lockoutMs <= 3000)     lockoutMs = s.lockoutMs;
  if (s.goal >= 1 && s.goal <= 999)                  goal = s.goal;
  // Settings from an older version: the measuring window starts at the new
  // default (100 ms) and the start code; they are saved in the new format.
  if (current && s.windowMs >= 20 && s.windowMs <= 300) windowMs = s.windowMs;
  if (current && s.pin <= 9999)                         settingsPin = s.pin;
  if (windowMs > lockoutMs - 50) windowMs = lockoutMs - 50;
  if (older) saveSettings();
}

void saveSettings() {
  Settings s = { SETTINGS_MAGIC, hitG, noiseG, (uint16_t)lockoutMs, (uint16_t)goal, (uint16_t)windowMs, settingsPin };
  EEPROM.put(0, s);
}

void resetCounter() {
  reps = 0; noReps = 0; done = false;
  firstRepMs = 0; lastRepMs = 0;
  lastEvent = EV_NONE; matrixGo = false;
  historyCount = 0;
  for (int k = 0; k < 5; k++) { snapId[k] = 0; snapReady[k] = false; }
}

void sendHeader(WiFiClient &c, const char *code, const char *type, size_t len, bool cache) {
  char h[220];
  snprintf(h, sizeof(h),
           "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %u\r\n"
           "Cache-Control: %s\r\nConnection: close\r\n\r\n",
           code, type, (unsigned)len, cache ? "max-age=86400" : "no-store");
  c.print(h);
}

// Sends larger files in pieces and keeps reading the sensors in between
void sendBody(WiFiClient &c, const uint8_t *data, size_t len) {
  size_t off = 0;
  while (off < len && c.connected()) {
    size_t chunk = min((size_t)512, len - off);
    c.write(data + off, chunk);
    off += chunk;
    processSensors();
  }
}

void sendState(WiFiClient &c) {
  unsigned long avg = (reps >= 2) ? (lastRepMs - firstRepMs) / (reps - 1) : 0;
  const char *last = lastEvent == EV_HIT ? "hit" : lastEvent == EV_WEAK ? "weak"
                   : lastEvent == EV_WRONG ? "wrong" : "none";
  char j[300];
  int n = snprintf(j, sizeof(j),
    "{\"reps\":%d,\"goal\":%d,\"noReps\":%d,\"zone\":\"%s\",\"avgMs\":%lu,"
    "\"done\":%s,\"seq\":%lu,\"last\":\"%s\",\"top\":%d,\"bottom\":%d,\"sensors\":%d,"
    "\"hit\":%d,\"noise\":%d,\"lock\":%lu,\"win\":%lu}",
    reps, goal, noReps, activeZone == ZONE_UP ? "up" : "down", avg,
    done ? "true" : "false", eventSeq, last,
    (int)(lastPeakTop * 100), (int)(lastPeakBottom * 100),   // in 1/100 g (no float printf on all boards)
    (sensors[0].ok ? 1 : 0) | (sensors[1].ok ? 2 : 0),       // bit 1 = sensor 1, bit 2 = sensor 2
    (int)(hitG * 100 + 0.5f), (int)(noiseG * 100 + 0.5f), lockoutMs, windowMs);
  sendHeader(c, "200 OK", "application/json", n, false);
  c.write((const uint8_t *)j, n);
}

bool snapIsReady(unsigned long id) {
  int k = id % 5;
  return id != 0 && snapId[k] == id && snapReady[k];
}

// Settings page: last 5 impacts, buffer status. Polling this keeps both sensors running.
// hits: [ms ago, sensor, peak 1/100 g, rep?, id, recording ready?, ms to peak, window ms]
void sendPlot(WiFiClient &c) {
  lastPlotRequest = millis();
  if (lastPlotRequest == 0) lastPlotRequest = 1;

  char j[600];
  int n = snprintf(j, sizeof(j), "{\"hits\":[");
  unsigned long now = millis();
  for (int i = 0; i < historyCount; i++) {
    HitRecord &h = history[i];
    n += snprintf(j + n, sizeof(j) - n, "%s[%lu,%u,%u,%u,%lu,%d,%u,%u]", i ? "," : "",
                  now - h.t, h.sensor, h.peak, h.type == EV_HIT ? 1 : 0,
                  h.id, snapIsReady(h.id) ? 1 : 0, h.peakOff, h.win);
  }
  n += snprintf(j + n, sizeof(j) - n, "],\"ovf\":[%d,%d],\"zone\":\"%s\",\"sensors\":%d,\"armed\":%d}",
                overflowsLast10s(0), overflowsLast10s(1), activeZone == ZONE_UP ? "up" : "down",
                (sensors[0].ok ? 1 : 0) | (sensors[1].ok ? 2 : 0),
                (!inEvent && millis() >= lockoutUntil) ? 1 : 0);
  sendHeader(c, "200 OK", "application/json", n, false);
  c.write((const uint8_t *)j, n);
}

// The recording of one impact: 100 values per sensor, 10 ms apart, impact at index 20
void sendSnap(WiFiClient &c, unsigned long id) {
  static char j[1500];
  int n;
  if (!snapIsReady(id)) {
    n = snprintf(j, sizeof(j), "{\"id\":%lu,\"ready\":0}", id);
  } else {
    int k = id % 5;
    n = snprintf(j, sizeof(j), "{\"id\":%lu,\"ready\":1,\"pre\":%d,\"step\":%lu,\"a\":[", id, SNAP_PRE * (int)BUCKET_MS, BUCKET_MS);
    for (int s = 0; s < 2; s++) {
      if (s == 1) n += snprintf(j + n, sizeof(j) - n, "],\"b\":[");
      for (int i = 0; i < SNAP_LEN && n < (int)sizeof(j) - 40; i++)
        n += snprintf(j + n, sizeof(j) - n, i ? ",%u" : "%u", snap[k][s][i]);
    }
    n += snprintf(j + n, sizeof(j) - n, "]}");
  }
  sendHeader(c, "200 OK", "application/json", n, false);
  sendBody(c, (const uint8_t *)j, n);
}

// Value of ?key=... in a path, or -1
long queryNumber(const char *path, const char *key) {
  const char *q = strstr(path, key);
  if (!q) return -1;
  return atol(q + strlen(key));
}

// 4-digit code after key=, e.g. "pin=0123" -> 123; -1 if not exactly 4 digits
long queryPin(const char *path, const char *key) {
  const char *q = strstr(path, key);
  if (!q) return -1;
  q += strlen(key);
  for (int i = 0; i < 4; i++) if (!isdigit((unsigned char)q[i])) return -1;
  if (isdigit((unsigned char)q[4])) return -1;
  return (q[0] - '0') * 1000L + (q[1] - '0') * 100L + (q[2] - '0') * 10L + (q[3] - '0');
}

// Checks a code; after 5 wrong codes in a row every check is refused for 30 s
bool pinOk(long p) {
  if ((long)(millis() - pinBlockedUntil) < 0) return false;
  if (p >= 0 && p == settingsPin) { pinFails = 0; return true; }
  if (++pinFails >= 5) { pinFails = 0; pinBlockedUntil = millis() + 30000; }
  return false;
}

void sendPinResult(WiFiClient &c, bool ok) {
  char j[40];
  long wait = (long)(pinBlockedUntil - millis());
  int n = (!ok && wait > 0) ? snprintf(j, sizeof(j), "{\"ok\":0,\"wait\":%ld}", (wait + 999) / 1000)
                            : snprintf(j, sizeof(j), "{\"ok\":%d}", ok ? 1 : 0);
  sendHeader(c, "200 OK", "application/json", n, false);
  c.write((const uint8_t *)j, n);
}

void handleClient() {
  unsigned long tStart = millis();
  handleClientInner();
  unsigned long d = millis() - tStart;
  if (d > slowHttpMs) { slowHttpMs = d; strncpy(slowHttpPath, lastHttpPath, sizeof(slowHttpPath) - 1); }
}

void handleClientInner() {
  lastHttpPath[0] = 0;
  WiFiClient client = server.available();
  if (!client) return;

  char line[160];
  int  len = 0, lineLen = 0;
  bool firstLine = true;
  unsigned long t0 = millis();

  // Read the request line, skip the rest of the headers
  while (client.connected() && millis() - t0 < 1500) {
    if (!client.available()) { processSensors(); continue; }
    char ch = client.read();
    if (ch == '\r') continue;
    if (ch == '\n') {
      if (firstLine) { firstLine = false; line[len] = 0; }
      else if (lineLen == 0) break;          // empty line = end of headers
      lineLen = 0;
      continue;
    }
    lineLen++;
    if (firstLine && len < (int)sizeof(line) - 1) line[len++] = ch;
  }
  if (firstLine) line[len] = 0;

  // "GET /path HTTP/1.1" -> "/path"
  char *path = strchr(line, ' ');
  if (!path) { client.stop(); return; }
  path++;
  char *end = strchr(path, ' ');
  if (end) *end = 0;
  strncpy(lastHttpPath, path, sizeof(lastHttpPath) - 1);
  char *qm = strchr(lastHttpPath, '?');
  if (qm) *qm = 0;                           // don't log the code

  if (strcmp(path, "/") == 0 || strncmp(path, "/index", 6) == 0) {
    // The page title comes from secrets.h: the text between <title> and </title> is replaced.
    const char *t1 = strstr(INDEX_HTML, "<title>");
    const char *t2 = strstr(INDEX_HTML, "</title>");
    if (t1 && t2 && t2 > t1) {
      t1 += 7;
      size_t headLen = t1 - INDEX_HTML, titleLen = strlen(PAGE_TITLE_TEXT), tailLen = strlen(t2);
      sendHeader(client, "200 OK", "text/html; charset=utf-8", headLen + titleLen + tailLen, false);
      sendBody(client, (const uint8_t *)INDEX_HTML, headLen);
      sendBody(client, (const uint8_t *)PAGE_TITLE_TEXT, titleLen);
      sendBody(client, (const uint8_t *)t2, tailLen);
    } else {
      size_t n = strlen(INDEX_HTML);
      sendHeader(client, "200 OK", "text/html; charset=utf-8", n, false);
      sendBody(client, (const uint8_t *)INDEX_HTML, n);
    }
  }
  else if (strcmp(path, "/logo.png") == 0) {
    sendHeader(client, "200 OK", "image/png", LOGO_PNG_LEN, true);
    sendBody(client, LOGO_PNG, LOGO_PNG_LEN);
  }
  else if (strncmp(path, "/state", 6) == 0) {
    sendState(client);
  }
  else if (strncmp(path, "/plot", 5) == 0) {
    sendPlot(client);
  }
  else if (strncmp(path, "/snap", 5) == 0) {
    long id = queryNumber(path, "id=");
    sendSnap(client, id > 0 ? (unsigned long)id : 0);
  }
  else if (strncmp(path, "/zone", 5) == 0) {
    if (strstr(path, "z=up"))   setZone(ZONE_UP);
    if (strstr(path, "z=down")) setZone(ZONE_DOWN);
    sendState(client);
  }
  else if (strncmp(path, "/goal", 5) == 0) {
    long g = queryNumber(path, "n=");
    if (g >= 1 && g <= 999) {
      goal = (int)g;
      if (reps >= goal && !done) { done = true; matrixGo = true; goStartMs = millis(); }
      saveSettings();
    }
    sendState(client);
  }
  else if (strncmp(path, "/pin", 4) == 0) {
    // /pin?check=0123            -> is the code right?
    // /pin?old=0123&new=4567     -> change the code
    long chk = queryPin(path, "check=");
    if (chk >= 0) {
      sendPinResult(client, pinOk(chk));
    } else {
      long np = queryPin(path, "new=");
      bool ok = np >= 0 && pinOk(queryPin(path, "old="));
      if (ok) { settingsPin = (uint16_t)np; saveSettings(); Serial.println(F("Settings code changed.")); }
      sendPinResult(client, ok);
    }
  }
  else if (strncmp(path, "/calib", 6) == 0) {
    bool ok = pinOk(queryPin(path, "pin="));
    if (ok) recalibrate();
    sendPinResult(client, ok);
  }
  else if (strncmp(path, "/settings", 9) == 0 && !pinOk(queryPin(path, "pin="))) {
    Serial.println(F("Settings refused: wrong code."));
    sendState(client);                        // nothing changed
  }
  else if (strncmp(path, "/settings", 9) == 0) {
    long h = queryNumber(path, "hit=");     // threshold in 1/100 g
    long z = queryNumber(path, "noise=");   // noise limit in 1/100 g
    long l = queryNumber(path, "lock=");    // lockout in ms
    long w = queryNumber(path, "win=");     // measuring window in ms
    if (h >= 30 && h <= 1600)  hitG = h / 100.0f;
    if (z >= 5 && z < h)       noiseG = z / 100.0f;
    if (noiseG >= hitG)        noiseG = hitG * 0.5f;
    if (l >= 300 && l <= 3000) lockoutMs = (unsigned long)l;
    if (w >= 20 && w <= 300)   windowMs = (unsigned long)w;
    if (windowMs > lockoutMs - 50) windowMs = lockoutMs - 50;   // window must end before the lockout
    saveSettings();
    Serial.print(F("Settings: noise=")); Serial.print(noiseG, 2);
    Serial.print(F(" g, hit="));  Serial.print(hitG, 2);
    Serial.print(F(" g, lockout="));    Serial.print(lockoutMs);
    Serial.print(F(" ms, window="));    Serial.print(windowMs);
    Serial.println(F(" ms"));
    sendState(client);
  }
  else if (strncmp(path, "/reset", 6) == 0) {
    resetCounter();
    sendState(client);
  }
  else {
    const char *msg = "Not found";
    sendHeader(client, "404 Not Found", "text/plain", strlen(msg), false);
    client.print(msg);
  }

  delay(1);
  client.stop();
}

// ============================================================
//  Setup and loop
// ============================================================

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { }

  matrix.begin();
  matrixNumber(0);

  for (int r = 0; r < RING_LEN; r++) ringStamp[r] = 0xFFFFFFFF;   // history empty

  loadSettings();
  Serial.print(F("Noise limit: "));   Serial.print(noiseG, 2);
  Serial.print(F(" g, hit threshold: ")); Serial.print(hitG, 2);
  Serial.print(F(" g, lockout: "));   Serial.print(lockoutMs);
  Serial.print(F(" ms, window: "));   Serial.print(windowMs);
  Serial.print(F(" ms, goal: "));     Serial.println(goal);

  // Sensors
  pinMode(SW_POWER, OUTPUT);
  digitalWrite(SW_POWER, HIGH);
  sdaRelease(); sclRelease();
  delay(50);
  Wire.begin();
  Wire.setClock(100000);   // 100 kHz: reliable with 50 cm Grove cables

  for (int i = 0; i < 2; i++) {
    sensors[i].ok = initSensor(sensors[i]);
    Serial.print(i == 0 ? F("Sensor O (top): ") : F("Sensor U (bottom): "));
    Serial.println(sensors[i].ok ? F("found") : F("NOT found"));
  }

  delay(20);
  Serial.println(F("Calibrating, keep still..."));
  calibrate();

  // WiFi hotspot
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("WiFi module not responding!"));
  } else {
    if (WiFi.beginAP(AP_SSID, AP_PASS) != WL_AP_LISTENING) {
      Serial.println(F("Could not start the WiFi hotspot."));
    } else {
      delay(1000);
      server.begin();
      Serial.print(F("WiFi: "));      Serial.println(AP_SSID);
      Serial.print(F("Password: "));  Serial.println(AP_PASS);
      Serial.print(F("Open: http://")); Serial.println(WiFi.localIP());
    }
  }
  // Starting WiFi takes a while – drop the samples collected meanwhile,
  // otherwise this would show up as a buffer overflow
  flushFifo(0);
  flushFifo(1);
  for (int i = 0; i < 2; i++) for (int k = 0; k < 8; k++) overflowTimes[i][k] = 0;
  Serial.println(F("Ready. (Forgot the settings code? Type PINRESET here.)"));
}

// Serial Monitor: type PINRESET + Enter to set the settings code back to the start code
void checkSerial() {
  static char buf[16];
  static int len = 0;
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\r' || ch == '\n') {
      buf[len] = 0;
      for (int i = 0; i < len; i++) buf[i] = toupper((unsigned char)buf[i]);
      if (strcmp(buf, "PINRESET") == 0) {
        settingsPin = PIN_DEFAULT; pinFails = 0; pinBlockedUntil = 0;
        saveSettings();
        Serial.println(F("Settings code reset to the start code."));
      }
      len = 0;
    } else if (len < (int)sizeof(buf) - 1) {
      buf[len++] = ch;
    }
  }
}

void loop() {
  processSensors();
  handleClient();
  // goal reached: GO! for 5 s, then the counter starts again at 0
  if (done && millis() - goStartMs >= GO_SHOW_MS) {
    resetCounter();
    Serial.println(F("GO! over – counter reset."));
  }
  updateMatrix();
  checkSerial();
}
