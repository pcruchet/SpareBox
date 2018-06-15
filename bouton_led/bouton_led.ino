int led = 13;
int but = PC13;
volatile int bLED = LOW;

void interruptFunction() 
{
  bLED = !bLED;
}

void setup() {
   pinMode(led, OUTPUT);
   pinMode(but,INPUT_PULLDOWN);
   attachInterrupt(but, interruptFunction, FALLING); // FALLING // CHANGE //RISING
}

void loop() 
{
  digitalWrite(led,bLED);
}


