// Read info from ultrasound distance meter
// From: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf

#define ECHO 11
#define TRIGGER 10
#define CYCLE_LENGTH 1000000

void setup() {
  // put your setup code here, to run once:
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  Serial.begin(9600);
}

unsigned long lastSignal = 0;
boolean isEchoStarted = false;
unsigned long echoStart = 0;
boolean isEchoOver = false;


void loop() {
  unsigned long time = micros();
  
  // put your main code here, to run repeatedly:
  if (time - lastSignal > CYCLE_LENGTH) {
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    lastSignal = micros();
    isEchoStarted = false;
    isEchoOver = false;
    delayMicroseconds(500);
    return;
  }
  
  int read = digitalRead(ECHO);
  if (read == HIGH && !isEchoStarted) {
    echoStart = time;
    isEchoStarted = true;
  } else if (read == LOW && isEchoStarted && !isEchoOver) {
    Serial.print((time - echoStart) / 58);
    Serial.println("cm");
    isEchoOver = true;
  }
}
