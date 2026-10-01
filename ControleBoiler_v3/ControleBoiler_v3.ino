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

byte graus[8] = {
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};


// define digital pins
#define lightButton 8
#define contator 7
#define rele_boiler 12

// initializa histeresis (0.1V equivale a 2 graus Celsius)
//float hist = 0.1;
float hist = 2.0;   // or 0.1V (0.05V/C)
//float t0 = 2.56;
float t0 = 47.0;    // or 2.56V

unsigned long startTime = 0;          // the last time the input pin was toggled
unsigned long backLightTime = 10000;     // the back light ON time



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // initialize digital pins:
  pinMode(rele_boiler, OUTPUT);
  pinMode(lightButton, INPUT);
  pinMode(contator, INPUT);

  // initialize LCD
  lcd.init();

  lcd.createChar(0, graus);
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
 
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  //float temperature = 100 - voltage/0.0512;
  //float temperature = 79 - voltage/0.075;
  float temperature = 26.11 * voltage - 47.52;
  
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
  if (digitalRead(lightButton) == HIGH) {
    lcd.setBacklight(HIGH);
    startTime = millis();
  } 
  
  if (millis() - startTime > backLightTime) {
    lcd.setBacklight(LOW);
  } 
  
//lcd.setBacklight(HIGH);

  //delay(10000);
  
  //String linha1 = "Temp: " + String(100 - voltage/0.0512, 1) + " " + state;
  String linha1 = "Boiler: " + String(temperature, 0) + "/" + String(t0, 0) + " C";
  //String linha1 = "Boiler: " + String(temperature, 0) + "  " + voltage;
  String linha2 = "Aquec.: " + boiler + "    " + state;
  
  lcd.setCursor(0,0);
  lcd.print(linha1);
  lcd.setCursor(0,1);
  lcd.print(linha2);
}
