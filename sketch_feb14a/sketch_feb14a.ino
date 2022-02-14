byte rowPins[] = {2,7,6,4};
byte columnPins[] = {3,1,5};

void setup() {
  // put your setup code here, to run once:
  for (int i = 1; i < 8; i++) {
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
}

int * getPressedKeys() {
  static int pressed[12];
  int i = 0;

  
  for (byte r=0; r< 4; r++) {
    pinMode(rowPins[r],INPUT_PULLUP);
  }

  for (byte c=0; c<3; c++) {
    pinMode(columnPins[c],OUTPUT);
    digitalWrite(columnPins[c], LOW);  // Begin column pulse output.
    for (byte r=0; r<4; r++) {
      pressed[i] = !digitalRead(rowPins[r]);
      i++;
    }
    // Set pin to high impedance input. Effectively ends column pulse.
    digitalWrite(columnPins[c],HIGH);
    pinMode(columnPins[c],INPUT);
  }
  return pressed;
}

char convertToChar(int active[]) {
  // First check middle col, because for some reason middle col triggers other cols as well
  if (active[4]) {
    return '2';
  }
  if (active[5]) {
    return '5';
  }
  if (active[6]) {
    return '8';
  }
  if (active[7]) {
    return '0';
  }
  
  if (active[0]) {
    return '1';
  }
  if (active[1]) {
    return '4';
  }
  if (active[2]) {
    return '7';
  }
  if (active[3]) {
    return '*';
  }
  if (active[8]) {
    return '3';
  }
  if (active[9]) {
    return '6';
  }
  if (active[10]) {
    return '9';
  }
  if (active[11]) {
    return '#';
  }
  return '_';
}

void loop() {
  int *pressed = getPressedKeys();
  Serial.println(convertToChar(pressed));
}
