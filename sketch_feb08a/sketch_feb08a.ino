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
  function_set(false, false);
  
//  display_on_off(false, false, false);
  
  display_on_off(true, true, true);
  entry_mode_set(true, false);
    clear_display();
    return_home();  
  write_character('#');
  write_character('h');
  write_character('o');
  write_character('b');
  write_character('b');
  write_character('y');
  write_character('-');
  write_character('e');
  write_character('v');
  write_character('e');
  write_character('r');
  write_character('y');
  write_character('d');
  write_character('a');
  write_character('y');

}

void sendInstruction(char instruction) {
  digitalWrite(REGISTER_SELECT, LOW);
  digitalWrite(READ_WRITE, LOW);
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
  digitalWrite(ENABLE, LOW);
  delay(5);
  digitalWrite(ENABLE, HIGH);
  delay(5);
  digitalWrite(ENABLE, LOW);
  delay(100);
//  read_data();
}

void write_character(char character) {
  Serial.println("Sending character");
  digitalWrite(REGISTER_SELECT, HIGH);
  digitalWrite(READ_WRITE, LOW);
  Serial.println(character);
  for (int i = 7; i >= 0; i--) {
    pinMode(data_pins[i], OUTPUT);
    
  }
  for (int i = 7; i >= 0; i--) {
    char mask = 1 << i;
    char result = character & mask;
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
  digitalWrite(ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(ENABLE, HIGH);
  delayMicroseconds(1);
  digitalWrite(ENABLE, LOW);
  delay(100);
//  read_data();
  
}

void read_data() {
  digitalWrite(REGISTER_SELECT, HIGH);
  digitalWrite(READ_WRITE, HIGH);
  digitalWrite(ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(ENABLE, HIGH);
  delayMicroseconds(1);
  digitalWrite(ENABLE, LOW);
  Serial.println("READ");
  
  for (int i = 7; i >= 0; i--) {
    pinMode(data_pins[i], INPUT);
    Serial.print(digitalRead(data_pins[i]));
  }
  Serial.println("--");
  delay(50);
}

void function_set(boolean line_number, boolean font_type) {
  Serial.println("Function set");
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
  Serial.println("Clear display");
  sendInstruction(0b00000001);
}

void return_home() {
  Serial.println("Return home");
  sendInstruction(0b0000010);
}

void entry_mode_set(boolean increment, boolean shift) {
  Serial.println("Entry mode set");
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
  Serial.println("Display on/off");
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
