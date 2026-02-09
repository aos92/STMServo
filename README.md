# STMServo

STMServo adalah library servo berbasis **HardwareTimer** untuk **board STM32 (Arduino Core)**.  
Library ini memungkinkan kontrol servo yang presisi tanpa mengganggu `delay()` atau task utama.

## Fitur Utama
- Menggunakan **HardwareTimer STM32**
- Kontrol servo via **derajat (0–180)** atau **mikrodetik**
- Mendukung **banyak servo** (selama timer tersedia)
- Stabil untuk aplikasi real-time & multitasking

