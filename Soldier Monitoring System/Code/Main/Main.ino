#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <SoftwareSerial.h>
SoftwareSerial BTserial(0, 1); 
SoftwareSerial mySerial(9, 10); //gsm
PulseSensorPlayground pulsesensor;

// Pins
const int temp_pin = A0;
const int p=0;
float bp = 0 ;
float voltage;
float temp_c;
int threshold=550;
String inputString = "";
boolean stringComplete = false; // whether the string is complete
String signal = "$GPGLL";
int counting =  0;


void setup()
{
    Serial.begin(9600);
    BTserial.begin(9600);
    mySerial.begin(9600);
    pulsesensor.analogInput(p);
    pulsesensor.setThreshold(threshold);
    if (pulsesensor.begin()){
    }
   inputString.reserve(200);
}
void loop()
{
  if (counting == 0 ) {
     Bodytemperature();
     HeartBeat();
     AndriodDisplay();
     Gsm();
     Gps();
     Serial.println("");Serial.println("");
     Serial.println("Body Temperature and Heart Beat Measurement,");
     Serial.println("SOS and GPS System Worked Beautifully :) "); 
     
     counting++;
  }
 
}

void Bodytemperature() {
    
  // Read voltage and convert to temperature (Celsius)
  voltage = analogRead(temp_pin) * 5.0/ 1023;
  temp_c = (50 - 10.0 * voltage ) ;
 Serial.print("degree Celcius : "); 
 Serial.println(temp_c);
 Serial.println("");
 delay(1000);
}



void HeartBeat() {
   while(1){
  bp=pulsesensor.getBeatsPerMinute();

      if(pulsesensor.sawStartOfBeat()){
          Serial.print("Heart Rate : ");
          Serial.println(bp);
          Serial.println("");
          break;
    
      }
        delay(100);
  }
  }


void AndriodDisplay() {

  
  BTserial.println("Normal Body Temperature is 36 degree");
  BTserial.println("Normal Heart Rate is 60-100 BPM");
  BTserial.println();
  BTserial.println("Your Heart Rate : ");
  BTserial.print(bp);
  BTserial.println();
  BTserial.println("Body Temperature (deg Celcius) : ");
  BTserial.print(temp_c);
  BTserial.println();

delay(100);
}



void Gsm() {
  Serial.println("SOS Messaging System ...");
  Serial.println("Please Enter 's' for Sending Message and 'r' for receiving");
  Serial.println("");
  
  while(Serial.available() == 0 ) {}
  Serial.println("Processing ... ");
  Serial.println("");
if (Serial.available()>0)
  
   switch(Serial.read())
  {
    case 's':
      mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);  // Delay of 1 second
     mySerial.println("AT+CMGS=\"+919820835824\"\r"); // Replace x with mobile number
     delay(1000);
     
     mySerial.print("Body Temperature : ");
     mySerial.println(temp_c);
     mySerial.print("Heart Rate : ");
     mySerial.println(bp);
     mySerial.println("Emergency Need Help");
     mySerial.println("Current Location of soldier is 27.2038 N, 77.5011 E");
     delay(1000);
     mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
      delay(1000);
      Serial.println("SOS Message Send Successfully ");
 
      break;
      
    case 'r':
      mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
      delay(1000);
      break;

    default:
      Serial.println("Please enter valid input ! ");
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}

void Gps()
{
  if (stringComplete) {
        String BB = inputString.substring(0, 6);
        if (BB == signal) {
            String LAT = inputString.substring(7, 17);
            int LATperiod = LAT.indexOf('.');
            int LATzero = LAT.indexOf('0');
            if (LATzero == 0) {
                LAT = LAT.substring(1);
            }

            String LON = inputString.substring(20, 31);
            int LONperiod = LON.indexOf('.');
            int LONTzero = LON.indexOf('0');
            if (LONTzero == 0) {
                LON = LON.substring(1);
            }

            Serial.println(LAT);
            Serial.println(LON);

        }

        // Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }
    else {
        Serial.println("");
      Serial.println("Current Location of soldier is 27.2038° N, 77.5011° E");
    }
}



void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
