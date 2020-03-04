 #include <SoftwareSerial.h>

SoftwareSerial BTserial(0, 1); // RX | TX

int sensorPin = A0;

int sensorValue = 0;

void setup() {

BTserial.begin(9600); }

void loop() {

BTserial.print("1234");

BTserial.print(",");

BTserial.print("1234.0");

BTserial.print(",");

BTserial.print("1234 hPa");

BTserial.print(",");

BTserial.print("500 ml/s");

BTserial.print(",");

BTserial.print(";");

delay(20);

}
