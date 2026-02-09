/**
 * Contoh pengujian STMServo menggunakan HardwareTimer
 * Servo digerakkan dari pulsa minimum ke maksimum lalu kembali
 */

// Library
#include <STMServo.h>

// Konfigurasi pin
uint16_t SERVO_PIN = PB1;   // Pin output servo (contoh: PB1)

// Objek servo
STMServo servo;

// Variabel lebar pulsa (mikrodetik)
int width = 0;

void setup() {
  // Menghubungkan servo ke pin
  servo.attach(SERVO_PIN);
}

void loop() {
  // Gerakkan servo dari posisi minimum ke maksimum
  for (width = 500; width < 2500; width += 10) {
    servo.writeMicroseconds(width); // Kirim pulsa ke servo
    delay(15);                      // Tunggu servo bergerak
  }

  // Gerakkan servo kembali dari maksimum ke minimum
  for (width = 2500; width > 500; width -= 10) {
    servo.writeMicroseconds(width);
    delay(15);
  }
}
