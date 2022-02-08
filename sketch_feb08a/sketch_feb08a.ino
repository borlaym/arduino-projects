#define REGISTER_SELECT 2
#define READ_WRITE 3
#define ENABLE 4

int data_pins[] = {5, 6, 7, 8, 9, 10, 11, 12};


void setup() {
  // put your setup code here, to run once:
  pinMode(REGISTER_SELECT, OUTPUT);
  pinMode(READ_WRITE, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);
  for (int i = 0; i < 8; i++) {
    pinMode(data_pins[i], OUTPUT);
    digitalWrite(data_pins[i], LOW);
  }
  Serial.begin(9600);

  // Set read-write to write always
  digitalWrite(READ_WRITE, LOW);

  // Other setup
  function_set(true, true);
  display_on_off(true, true, true);
  clear_display();
  return_home();
  entry_mode_set(true, false);
  write_character('h');
  write_character(0b01001000);
//  sendInstruction(0b00100111);
}

void sendInstruction(char instruction) {
  digitalWrite(REGISTER_SELECT, LOW);
  
  Serial.println(instruction, BIN);
  for (int i = 7; i >= 0; i--) {
    char mask = 1 << i;
    char result = instruction & mask;
    char digit = result >> i;
    digitalWrite(data_pins[i], digit);
    Serial.print("Writing to ");
    Serial.print(i);
    Serial.print(" -> ");
    if (digit) {
      Serial.println("1");
    } else {
      Serial.println("0");
    }
  }
  digitalWrite(ENABLE, HIGH);
  digitalWrite(ENABLE, LOW);
}

void write_character(char character) {
  digitalWrite(REGISTER_SELECT, HIGH);
  
  for (int i = 7; i >= 0; i--) {
    char digit = character >> i;
    digitalWrite(data_pins[i], digit);
  }
  digitalWrite(ENABLE, HIGH);
  digitalWrite(ENABLE, LOW);
  Serial.println(character);
}

void function_set(boolean line_number, boolean font_type) {
  char instruction = 0b00110000;
  if (line_number) {
    instruction += 0b00001000;
  }
  if (font_type) {
    instruction += 0b00000100;
  }
  sendInstruction(instruction);
}

void clear_display() {
  sendInstruction(0b00000001);
}

void return_home() {
  sendInstruction(0b0000010);
}

void entry_mode_set(boolean increment, boolean shift) {
  char instruction = 0b00000100;
  if (increment) {
    instruction += 0b00000010;
  }
  if (shift) {
    instruction += 0b00000001;
  }
  sendInstruction(instruction);
}

void display_on_off(boolean display, boolean cursor, boolean blink) {
  char instruction = 0b00001000;
  if (display) {
    instruction += 0b00000100;
  }
  if (cursor) {
    instruction += 0b00000010;
  }
  if (blink) {
    instruction += 0b00000001;
  }
  sendInstruction(instruction);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
