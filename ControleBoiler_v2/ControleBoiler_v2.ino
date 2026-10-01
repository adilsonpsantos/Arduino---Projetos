

/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/ReadAnalogVoltage
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Inicialize display on address 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

// initializa histeresis (0.1V equivale a 2 graus Celsius)
//float hist = 0.1;
float hist = 2.0;   // or 0.1V (0.05V/C)
//float t0 = 2.56;
float t0 = 50.0;    // or 2.56V

// define digital pins
#define light 8
#define contator 7
#define rele_boiler 12

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // initialize digital pins:
  pinMode(rele_boiler, OUTPUT);
  pinMode(light, INPUT);
  pinMode(contator, INPUT);

  // initialize LCD
  lcd.init();
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
 
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = 100 - voltage/0.0512;
  
  String state;
  String boiler;
  
  // print out the value you read:
  //Serial.println(voltage);

  // temperature monitoring
  if (temperature <= t0 - hist) {
   // turn relay ON - enable heating:
    digitalWrite(rele_boiler, HIGH);
    state = "*";
  } 
  if (temperature >= t0 + hist){
    // turn relay OFF - disable heating:
    digitalWrite(rele_boiler, LOW);
    state = " ";
  }
  
  //  check contactor status
  if (digitalRead(contator) == HIGH) {
    //  contactor ON
    boiler = "ON ";
  } else {
    //  contactor OFF
    boiler = "OFF";
  }

  //  control LCD backlight
  if (digitalRead(light) == HIGH) {
    lcd.setBacklight(HIGH);
  } else {
    lcd.setBacklight(LOW);
  }
  
  //delay(10000);
  
  //String linha1 = "Temp: " + String(100 - voltage/0.0512, 1) + " " + state;
  String linha1 = "Temp: " + String(temperature, 1) + " " + state;
  String linha2 = "Boiler: " + boiler;
  
  lcd.setCursor(0,0);
  lcd.print(linha1);
  lcd.setCursor(0,1);
  lcd.print(linha2);
}
