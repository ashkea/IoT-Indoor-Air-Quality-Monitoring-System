#include <MQ135.h>
#define PIN_MQ135 32

MQ135 mq135_sensor(PIN_MQ135);

void setup() {
   Serial.begin(115200);
}
 
void loop() {
    float rzero = mq135_sensor.getRZero();
    Serial.print("MQ135 RZero: ");
    Serial.print(rzero);
    delay(5000);
}
