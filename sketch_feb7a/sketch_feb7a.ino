int LIGHT = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LIGHT, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lightAmount = analogRead(LIGHT);
  Serial.println(lightAmount);
}
