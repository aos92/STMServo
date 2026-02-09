/**
 * STMServo.cpp - Library servo berbasis HardwareTimer
 */

#include "STMServo.h"

static servo_t servos[MAX_SERVOS]; // array statis untuk menyimpan struktur servo
uint8_t ServoCount = 0;            // jumlah total servo yang terpasang
uint16_t maxValue = 65535;

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)   // nilai pulsa minimum (uS) untuk servo ini
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)   // nilai pulsa maksimum (uS) untuk servo ini

// Konstruktor STMServo()
// ----------------------
STMServo::STMServo(){
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                 // memberikan index servo untuk instance ini
    servos[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH; // nilai pulsa default
  } else {
    this->servoIndex = INVALID_SERVO;                // jumlah servo melebihi batas
  }
}

// attach()
// Menghubungkan objek servo ke pin
// --------------------------------
uint8_t STMServo::attach(int pin){
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t STMServo::attach(){
  if(servos[this->servoIndex].Pin.prevAttached) {
    return this->attach(servos[this->servoIndex].Pin.nbr, this->min, this->max);
  } else {
    return false;
  }
}

uint8_t STMServo::attach(int pin, int min, int max){
  if (this->servoIndex < MAX_SERVOS && !servos[this->servoIndex].Pin.prevAttached) {
    servos[this->servoIndex].Pin.nbr = pin;
    servos[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH;

    // Mengambil instance timer dan channel PWM dari pin
    TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pin), PinMap_PWM);
    uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin), PinMap_PWM));
    HardwareTimer *SERVO_TIMER = new HardwareTimer(Instance);

    servos[this->servoIndex].TIM = SERVO_TIMER;
    servos[this->servoIndex].channel = channel;

    // Resolusi min/max adalah 4 mikrodetik
    this->min  = (MIN_PULSE_WIDTH - min) / 4;
    this->max  = (MAX_PULSE_WIDTH - max) / 4;

    servos[this->servoIndex].TIM->setMode(
      servos[this->servoIndex].channel,
      TIMER_OUTPUT_COMPARE_PWM1,
      servos[this->servoIndex].Pin.nbr
    );

    servos[this->servoIndex].TIM->setOverflow(REFRESH_INTERVAL, MICROSEC_FORMAT);
    writeMicroseconds(DEFAULT_PULSE_WIDTH);
    servos[this->servoIndex].TIM->resume();

    servos[this->servoIndex].Pin.isActive = true;
    servos[this->servoIndex].Pin.prevAttached = true;
  } else {
    // Jika sebelumnya sudah terpasang, aktifkan kembali channel
    servos[this->servoIndex].TIM->resumeChannel(servos[this->servoIndex].channel);
    servos[this->servoIndex].Pin.isActive = true;
  }
  return this->servoIndex;
}

// detach()
// Melepaskan servo dari pin
// --------------------------
void STMServo::detach(){
  if(servos[this->servoIndex].Pin.prevAttached) {
    servos[this->servoIndex].TIM->pauseChannel(servos[this->servoIndex].channel);
    servos[this->servoIndex].Pin.isActive = false;
  }
}

// write()
// Menulis sudut servo dalam derajat
// ---------------------------------
void STMServo::write(int value){
  // Nilai di bawah MIN_PULSE_WIDTH dianggap sebagai sudut (derajat)
  if (value < MIN_PULSE_WIDTH) {
    if (value < 0) {
      value = 0;
    } else if (value > 180) {
      value = 180;
    }

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(value);
}

// writeMicroseconds()
// Menulis lebar pulsa dalam mikrodetik
// ------------------------------------
void STMServo::writeMicroseconds(int value){
  uint8_t index = this->servoIndex;

  if (index < MAX_SERVOS) {
    // Pastikan nilai pulsa berada dalam batas aman
    if (value < SERVO_MIN()) {
      value = SERVO_MIN();
    } else if (value > SERVO_MAX()) {
      value = SERVO_MAX();
    }

    servos[index].ticks = value;
    uint32_t compare_value = (float)maxValue / REFRESH_INTERVAL * value;

    servos[index].TIM->setCaptureCompare(
      servos[index].channel,
      compare_value,
      RESOLUTION_16B_COMPARE_FORMAT
    );
  }
}

// read()
// Membaca sudut servo dalam derajat
// ---------------------------------
int STMServo::read(){
  return map(readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

// readMicroseconds()
// Membaca lebar pulsa dalam mikrodetik
// ------------------------------------
int STMServo::readMicroseconds(){
  unsigned int pulsewidth;

  if (this->servoIndex != INVALID_SERVO) {
    pulsewidth = servos[this->servoIndex].ticks;
  } else {
    pulsewidth = 0;
  }

  return pulsewidth;
}

// attached()
// Mengecek apakah servo masih terhubung
// -------------------------------------
bool STMServo::attached(){
  return servos[this->servoIndex].Pin.isActive;
}
