// SLAVE ARDUINO
#include <SPI.h>
#include <Servo.h>
#include <avr/sleep.h>

#define LED 4
#define BUZZ 3

int intPin = 2;

boolean received;
int Slavereceived;

Servo SERV;

void setup() {
  Serial.begin (115200);

  Serial.println("SLAVE");
  pinMode(MISO, OUTPUT);

  SPCR |= _BV(SPE);
  received = false;
  SPI.attachInterrupt();

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  pinMode(intPin, INPUT);

  SERV.attach(6);
}

ISR (SPI_STC_vect) {

  Slavereceived = SPDR;

  received = true;

}

void loop() {

  digitalWrite(LED, LOW);
  SERV.write(90);

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

    doWork();

  }

  Serial.println("Smoke Eliminated");

}

void wakeUp() {

  Serial.println("SMOKE DETECTED");
  Serial.println("!ALERT!");

  sleep_disable();

  detachInterrupt(0);

}

void doWork() {

  Serial.println(Slavereceived);

  if (received) {
    Serial.println(Slavereceived);
    received = false;

    if (Slavereceived == 1) {
      SERV.write(0);
      digitalWrite(LED, HIGH);
      tone(BUZZ, 1000); // Send 1KHz sound signal...
      delay(500);        // ...for 1 sec
      noTone(BUZZ);     // Stop sound...
      delay(500);        // ...for 1sec
    } else {
      digitalWrite(LED, LOW);
      SERV.write(90);
    }
  }

  delay(500);

}
