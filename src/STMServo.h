/**
 * STMServo.h - Library servo berbasis HardwareTimer
 */

#ifndef _STM_SERVO_H_
#define _STM_SERVO_H_

#include "Arduino.h"
#include <HardwareTimer.h>

#define Servo_VERSION           2     // versi perangkat lunak library

#define MIN_PULSE_WIDTH       544     // pulsa terpendek untuk servo (uS)
#define MAX_PULSE_WIDTH      2400     // pulsa terpanjang untuk servo (uS)
#define DEFAULT_PULSE_WIDTH  1500     // pulsa default saat servo di-attach
#define REFRESH_INTERVAL    20000     // interval refresh servo (mikrodetik)

#define MAX_SERVOS  254
#define INVALID_SERVO 255

// Struktur pin servo
typedef struct {
  uint8_t nbr;            // nomor pin (0–255)
  uint8_t isActive;       // true jika channel aktif
  uint8_t prevAttached;  // true jika sebelumnya pernah di-attach
} ServoPin_t;

// Struktur data servo
typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks; // lebar pulsa
  uint32_t channel;            // channel timer
  HardwareTimer *TIM;          // pointer ke HardwareTimer
} servo_t;

// Kelas utama STMServo
class STMServo {
  public:
    STMServo();

    uint8_t attach(int pin);                   // menghubungkan servo ke pin
    uint8_t attach(int pin, int min, int max); // menghubungkan servo dengan batas min/max
    uint8_t attach();                          // attach ulang ke pin sebelumnya
    void detach();                             // melepas servo

    void write(int value);                     // menulis sudut (derajat) atau pulsa
    void writeMicroseconds(int value);         // menulis pulsa mikrodetik
    int read();                                // membaca sudut servo (0–180)
    int readMicroseconds();                    // membaca pulsa mikrodetik
    bool attached();                           // mengecek status servo

  private:
    uint8_t servoIndex;  // index servo
    int8_t min;          // nilai minimum (x4 + MIN_PULSE_WIDTH)
    int8_t max;          // nilai maksimum (x4 + MAX_PULSE_WIDTH)
};

#endif // _STM_SERVO_H_
