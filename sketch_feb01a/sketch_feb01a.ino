#define X_MOVEMENT_PIN A0
#define led1 9
#define led2 10
#define led3 11
#define led4 12



void setup() {
  // put your setup code here, to run once:
  pinMode(X_MOVEMENT_PIN, INPUT);
  Serial.begin(9600);
}

int led = 2;

void loop() {
  // put your main code here, to run repeatedly:
  
  int x_movement = analogRead(X_MOVEMENT_PIN);
  Serial.println(analogRead(x_movement));
  if (x_movement < 450) {
    if (led > 0) {
      led -= 1;
    }
  } else if (x_movement > 550) {
    if (led < 3) {
      led += 1;
    }
  }

  digitalWrite(led1, led == 0 ? HIGH : LOW);
  digitalWrite(led2, led == 1 ? HIGH : LOW);
  digitalWrite(led3, led == 2 ? HIGH : LOW);
  digitalWrite(led4, led == 3 ? HIGH : LOW);
  delay(300);
}
