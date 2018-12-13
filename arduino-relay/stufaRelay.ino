/*
Example Arduiono sketch to turn on and off the stove in
a similar way to how a relay work.

It watches logic level on pin defined as INPUT_PIN and on:
- change from HIGH to LOW: turns on the store by sending
  0x80 0x21 0x01 0xA2
- change from LOW to HIGH: turns off the stove by sending
  0x80 0x21 0x00 0xA2

It expects HIGH to be initial state.

To improve reliability of contact you can add a 10 kOhm resistor from 
INPUT_PIN to +5V (to help it more towards high state).

Copyright 2018 www.K3A.me
License MIT (you can use it freely in your derived projects)
*/

#define LED_PIN 13 // DEBUG LED (default: pin D13 for Arduino nano)
#define INPUT_PIN 2 // PIN TO WATCH (default: pin D2)

volatile byte state = HIGH;

enum RwmsType {
  RWMS_RAM = 0x00,
  RWMS_EEPROM = 0x20,
};

void debug_blink()
{ 
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(300);
  digitalWrite(LED_PIN, LOW);
  delay(300);
}

// Sends serial data using micronova's protocol and verifies response.
// Returns true on successful response.
bool sendRwms(RwmsType type, uint16_t address, byte value) {
  byte packet[4];
  packet[0] = 0x80 + type + ((address>>8)&0xff);
  packet[1] = address & 0xff;
  packet[2] = value;
  packet[3] = (packet[0]+packet[1]+packet[2])&0xff; //checksum
  Serial.write(packet, sizeof(packet));

  byte resp[2];
  if (Serial.readBytes(resp, sizeof(resp)) == sizeof(resp)) {
    return resp[0] == packet[1] && resp[1] == packet[2];
  }

  return false;
}

void turnOn() {
  debug_blink();
  sendRwms(RWMS_RAM, 0x21, 1);
  delay(10000); // block for some time
}

void turnOff() {
  debug_blink();
  sendRwms(RWMS_RAM, 0x21, 0);
  delay(10000); // block for some time
}

void setup() {
 // serup serial to 1200 baud 8N2
 Serial.begin(1200, SERIAL_8N2);

 // set INPUT_PIN as input 
 pinMode(INPUT_PIN, INPUT_PULLUP);
}

void loop() {
  // read pin state
  byte val = digitalRead(INPUT_PIN);

  // if the current pin state is different to the known one
  if (val != state) {
    state = val;

    if (val == LOW) { 
      // turn on if pin is LOW (0V)
      turnOn();
    } else { 
      // turn off if pin is HIGH (~3 - 5V)
      turnOff();
    }
  }
  
  delay(1000); // wait for 1 second before checking again
}
