/**
 * Mengontrol posisi servo menggunakan potensiometer
 * Input analog sudut servo
 */

#include <STMServo.h>

// Objek servo
STMServo myservo;

// Konfigurasi pin
int potPin = A0;   // Pin analog untuk potensiometer
int val = 0;       // Variabel pembacaan ADC

void setup() {
  // Hubungkan servo ke pin PB1
  myservo.attach(PB1);
}

void loop() {
  // Baca nilai potensiometer (0–1023)
  val = analogRead(potPin);

  // Konversi ke sudut servo (0–180 derajat)
  val = map(val, 0, 1023, 0, 180);

  // Set posisi servo
  myservo.write(val);

  // Delay kecil agar servo stabil
  delay(15);
}
