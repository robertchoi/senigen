#include <Wire.h>
#include <MQ131.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // i2c address 0x3F

float ppm_value;
#define OFFSET_VALUE 0.5
#define relay_control 7

void setup() {
  Serial.begin(115200);

  // Init the sensor
  // - Heater control on pin 2
  // - Sensor analog read on pin A0
  // - Model LOW_CONCENTRATION
  // - Load resistance RL of 1MOhms (1000000 Ohms)
  MQ131.begin(2,A0, LOW_CONCENTRATION, 1000000);  

  Serial.println("Calibration in progress...");
  
  MQ131.calibrate();
  
  Serial.println("Calibration done!");
  Serial.print("R0 = ");
  Serial.print(MQ131.getR0());
  Serial.println(" Ohms");
  Serial.print("Time to heat = ");
  Serial.print(MQ131.getTimeToRead());
  Serial.println(" s");


 lcd.init();                      // initialize the lcd
 // Print a message to the LCD.
 lcd.backlight();
 lcd.setCursor(1,0);
 lcd.print("Sanigen");
 lcd.setCursor(2,1);
 lcd.print("             ");


 pinMode(relay_control, OUTPUT);

}

void loop() {
  
  Serial.println("Sampling...");
  MQ131.sample();
  Serial.print("Concentration O3 : ");
  ppm_value = MQ131.getO3(PPM);
  Serial.print(ppm_value);
  Serial.println(" ppm");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(PPB));
  Serial.println(" ppb");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(MG_M3));
  Serial.println(" mg/m3");
  Serial.print("Concentration O3 : ");
  Serial.print(MQ131.getO3(UG_M3));
  Serial.println(" ug/m3");


  String tempStr = String(ppm_value);
  lcd.print("O3 : " + tempStr);
  Serial.println("O3 : " + tempStr);
  if(ppm_value > OFFSET_VALUE)
  {
    digitalWrite(relay_control, HIGH);
  }
  else 
  {
    digitalWrite(relay_control, LOW);
  }

  delay(1000);
  lcd.clear();
}
