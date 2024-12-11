//_____________LIBRARIES___________//
#include<LiquidCrystal.h>

//_____________VARIABLES___________//

// LCD PINS
/* Connect the LCD to pins as shown */
int RS = 7 ;
int EN = 8;
int D4 = 9;
int D5 = 10;
int D6 = 11;
int D7 = 12;

//VOLTAGE SENSOR VARIABLES
int ANALOG_IN_PIN = A0;

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;

// Float for Reference Voltage
float ref_voltage = 5.0;

// Integer for ADC value
int adc_value = 0;

//Li-ion BATTERY VARIABLES
float BatVoltage = 0.00;
float BatMaxVoltage = 4.2;
float BatChargePercentage = 0.00;

//RELAY VARIABLES
int Relay1Pin = 4;
int Relay2Pin = 5;
int Relay3Pin = 6;
//_____________OBJECTS___________//

//Creating object type - LiquidCrystal
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

//_____________FUNCTIONS___________//


void relay1(boolean state) {
  // Let's turn on the relay...
  digitalWrite(Relay1Pin, !state);
  delay(10);



}
void relay2(boolean state) {
  // Let's turn on the relay...
  digitalWrite(Relay2Pin, !state);
  delay(10);


}
void relay3(boolean state) {
  // Let's turn on the relay...
  digitalWrite(Relay3Pin, !state);
  delay(10);

}
float func_VoltageRead() {
  // Read the Analog Input

  adc_value = analogRead(ANALOG_IN_PIN);

  Serial.print("adc_value: ");
  Serial.println(adc_value);

  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 4396.0; //12-bit ADC

  // Calculate voltage at divider input
  in_voltage = (adc_voltage / (R2 / (R1 + R2))) - 0.1 ;
  if (adc_value == 0)
    in_voltage = 0;
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);


  //LCD Display
  // set-up LCD display


  // Short delay
  delay(100);

  return in_voltage;

}

void func_lcdDisplay(float batCharge, float batVoltage) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHARGE: ");
  lcd.print(batCharge);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(batVoltage);
  lcd.print("V");
  delay(300);

}
void func_BatChargeCalc() {
  BatVoltage = func_VoltageRead();
  BatChargePercentage = ((BatVoltage / BatMaxVoltage) * 100);
  if (BatChargePercentage > 100)
    BatChargePercentage = 100;

  func_lcdDisplay(BatChargePercentage, BatVoltage);

  if (BatChargePercentage > 95) {
    relay2(HIGH);

    relay1(LOW);
    relay3(LOW);
    Serial.println("BAT > 95 ");
  } else if (BatChargePercentage < 20) {
    relay2(LOW);

    relay1(HIGH);
    relay3(HIGH);
    Serial.println("BAT <20 ");
  } else if ((BatChargePercentage > 20) && (BatChargePercentage < 95)) {
    relay2(HIGH);

    relay1(LOW);
    relay3(LOW);
    Serial.println("BAT 20~95 ");
  }

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // give 9600 here and 2400  on Serial Monitor
  //Leds Pin


  pinMode(ANALOG_IN_PIN, INPUT);
  pinMode(Relay1Pin, OUTPUT);
  pinMode(Relay2Pin, OUTPUT);
  pinMode(Relay3Pin, OUTPUT);

  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:

  // func_VoltageRead();
  func_BatChargeCalc();
  // delay(200);
}
