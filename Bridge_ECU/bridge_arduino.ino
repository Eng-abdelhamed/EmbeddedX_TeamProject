#include <Wire.h>

#define I2C_SLAVE_ADDR 0x55   // نفس العنوان اللي STM32 بيبعت عليه

// struct الجديد - نفس اللي في STM32
typedef struct {
  uint32_t Value_adc_heigh;
  uint32_t Value_adc_Slide;
  uint32_t Value_adc_Incline;
  uint16_t height;
  uint16_t slide;
  uint16_t incline;
  uint8_t command;
} SeatStatus_t;

volatile SeatStatus_t seatData;   // نخزن فيه الداتا اللي جاية من STM
volatile bool newData = false;    // فلاغ بيقول إن في داتا جديدة

void setup() {
  Serial.begin(115200);           // UART للـ PC
  Wire.begin(I2C_SLAVE_ADDR);     // Arduino كـ I2C Slave
  Wire.onReceive(receiveEvent);   // ربط الفنكشن مع I2C

  Serial.println("Bridge Ready...");
}

void loop() {
  if (newData) {
    newData = false; // امسح الفلاغ بعد القراءة

    Serial.print("Value_adc_heigh: ");
    Serial.println(seatData.Value_adc_heigh);

    Serial.print("Value_adc_Slide: ");
    Serial.println(seatData.Value_adc_Slide);

    Serial.print("Value_adc_Incline: ");
    Serial.println(seatData.Value_adc_Incline);

    Serial.print("height: ");
    Serial.println(seatData.height);

    Serial.print("slide: ");
    Serial.println(seatData.slide);

    Serial.print("incline: ");
    Serial.println(seatData.incline);

    Serial.print("command: ");
    Serial.println(seatData.command);

    Serial.println("---------------"); // فاصل بين الرسائل
  }
}

void receiveEvent(int howMany) {
  if (howMany == sizeof(SeatStatus_t)) {
    SeatStatus_t temp;
    uint8_t *p = (uint8_t*)&temp;

    for (int i = 0; i < howMany; i++) {
      if (Wire.available()) {
        p[i] = Wire.read();
      }
    }

    seatData = temp;
    newData = true;
  } else {
    while (Wire.available()) Wire.read();
  }
}