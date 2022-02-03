// Play sounds based on a distance meter

#define SOUND 9
#define ECHO 11
#define TRIGGER 10
#define CYCLE_LENGTH 60000

void setup() {
  // put your setup code here, to run once:
  pinMode(SOUND, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  Serial.begin(9600);
}

unsigned long lastSignal = 0;
boolean isEchoStarted = false;
unsigned long echoStart = 0;
boolean isEchoOver = false;
unsigned long lastMeasuredDistance = 0;

int NOTES[] = { 261, 277, 294, 311, 330, 349, 370, 392, 415, 440 };

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
    unsigned long newMeasuredDistance = time - echoStart;
    // Init last measured distance
    if (lastMeasuredDistance == 0) {
      lastMeasuredDistance = newMeasuredDistance;
    }
    // Disable spikes and small changes
    long diff = newMeasuredDistance - lastMeasuredDistance;
    long aDiff = abs(diff);
//    boolean noBigDifference = (newMeasuredDistance < lastMeasuredDistance) || (aDiff < 500);
    boolean noSmallDifference = aDiff > 30;

    if (noSmallDifference) {
      lastMeasuredDistance = newMeasuredDistance;
//      Serial.println(lastMeasuredDistance);
    }

    isEchoOver = true;
  }

  // Only change sounds on a valid range from the detector
  int validRangeStart = 200;
  int validRangeEnd = 1000;
  int range = validRangeEnd - validRangeStart;
  if (lastMeasuredDistance  > validRangeStart && lastMeasuredDistance < validRangeEnd) {

    int sound = ((lastMeasuredDistance - validRangeStart) * 10) / range;
    if (sound < 10) {
      Serial.println(sound);
      tone(SOUND, NOTES[sound]);  
    }
  } else if (lastMeasuredDistance > 4000) {
    noTone(SOUND);
  }
}
