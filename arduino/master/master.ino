// MASTER ARDUINO
//
#include <avr/sleep.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int smokeSens = 9;
int intPin = 2;

void setup() {

  Serial.begin(115200); //set baud rate to 115200 for usart
  pinMode(smokeSens, INPUT);
  pinMode(intPin, INPUT);

  Serial.println("Master");

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);

  lcd.begin(16, 2);

}

void loop() {

  digitalWrite(SS, LOW);                  //Starts communication with Slave connected to master
  SPI.transfer(0);
  digitalWrite(SS, HIGH); // disable Slave Select

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALL SAFE");
  
  gonna_sleep();
 
}

void gonna_sleep() {
  sleep_enable();
  Serial.println("Zzz");
  attachInterrupt(0, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(1000);
  sleep_cpu();

  while (!digitalRead(intPin)) {

    alert();

  }

  Serial.println("Smoke Eliminated");
  
}

void wakeUp() {

  Serial.println("SMOKE DETECTED");
  Serial.println("!ALERT!");

  sleep_disable();

  detachInterrupt(0);

}

void alert() {
  
  digitalWrite(SS, LOW);                  //Starts communication with Slave connected to master
  
  int digitalSensor = digitalRead(smokeSens);
  Serial.println(digitalSensor);

  if (digitalSensor == 1) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMOKE DETECTED");
    SPI.transfer(1);
    
  } else {
   
    SPI.transfer(0);
    
  }

  digitalWrite(SS, HIGH); // disable Slave Select
  delay(500);
  
}
