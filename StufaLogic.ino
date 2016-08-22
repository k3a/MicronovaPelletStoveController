/* untested code */
const int LED_pin = 13;
const int IN_pin = 2;
const int OUT_pin = 3;

void in_rise()
{
  pinMode(OUT_pin, INPUT);
  
  digitalWrite(LED_pin, LOW);
}

void in_fall()
{
  pinMode(OUT_pin, OUTPUT);
  digitalWrite(OUT_pin, LOW);
  
  digitalWrite(LED_pin, HIGH);
}

void in_change()
{
  if (digitalRead(IN_pin) == HIGH)
    in_rise();
  else
    in_fall();
}

void setup() {
  pinMode(LED_pin, OUTPUT);
  
  pinMode(IN_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(IN_pin), in_change, CHANGE);
  
  pinMode(OUT_pin, INPUT);
}

void loop() {
/*  digitalWrite(LED_pin, HIGH);
  delay(2000);
  digitalWrite(LED_pin, LOW);
  delay(2000);*/
}
