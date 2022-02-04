#define BUTTON 8
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

int step_number = 0;
int number_of_steps = 0;
boolean is_running = false;

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BUTTON) && !is_running) {
    is_running = true;
    Serial.println("PRESSED BUTTON");
  }
  if (number_of_steps == 2038) {
    is_running = false;
    number_of_steps = 0;
    Serial.println("PAUSED");
  } else if (is_running) {
    oneStep();
  }

  delay(2);
}

void oneStep() {
  switch (step_number) {
    case 0:
      digitalWrite(STEPPER_PIN_1, HIGH);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 1:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, HIGH);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 2:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, HIGH);
      digitalWrite(STEPPER_PIN_4, LOW);
      break;
    case 3:
      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, HIGH);
      break;
  }
  if (step_number == 3) {
    step_number = 0;
  } else {
    step_number += 1;
  }
  number_of_steps += 1;
}
