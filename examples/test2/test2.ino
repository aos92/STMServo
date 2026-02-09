/**
 * Contoh sweep servo dari 0 hingga 180 derajat
 */

#include <STMServo.h>

// Objek servo
STMServo myservo;

// Variabel posisi servo (derajat)
int pos = 0;

void setup() {
  // Hubungkan servo ke pin PB1
  myservo.attach(PB1);
}

void loop() {
  // Gerakkan servo dari 0 ke 180 derajat
  for (pos = 0; pos <= 180; pos++) {
    myservo.write(pos);   // Set sudut servo
    delay(15);            // Tunggu servo mencapai posisi
  }

  // Gerakkan servo kembali dari 180 ke 0 derajat
  for (pos = 180; pos >= 0; pos--) {
    myservo.write(pos);
    delay(15);
  }
}
