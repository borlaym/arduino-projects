/*
 * Control an LCD, displaying messages
 * Light an LED when a new message arrives
 * Push button to ack new messages
 * No network connection yet
 */

#define REGISTER_SELECT 2
#define ENABLE 4
// NOTE: Read / write is wired to be always write

int data_pins[] = {5, 6, 7, 8, 9, 10, 11, 12};

void setup() {
  // put your setup code here, to run once:
  pinMode(REGISTER_SELECT, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);
  for (int i = 0; i < 8; i++) {
    pinMode(data_pins[i], OUTPUT);
    digitalWrite(data_pins[i], LOW);
  }
  Serial.begin(9600);


  // Setup
  function_set(false, false);
  
  display_on_off(true, true, true);
  entry_mode_set(true, false);
  clear_display();
  return_home();  
  write_text("profansag hosszu szoveggel");
  write_text("Victory Royale!");
}

void write_data(boolean is_character, char data, int delay_amount) {
  digitalWrite(REGISTER_SELECT, is_character ? HIGH : LOW); // LOW is for instruction, HIGH is for data
  // Set data pins to instruction / character code
  for (int i = 7; i >= 0; i--) {
    char mask = 1 << i;
    char result = data & mask;
    char digit = result >> i;
    digitalWrite(data_pins[i], digit);
  }
  // Commit on enable signal switching to LOW
  digitalWrite(ENABLE, HIGH);
  digitalWrite(ENABLE, LOW);
  if (delay_amount > 0) {
    delay(delay_amount);
  }
}

void write_instruction(char instruction) {
  write_data(false, instruction, 100);
}

void write_character(char character) {
  Serial.println(character);
  write_data(true, character, 100);
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
  write_instruction(instruction);
}

void clear_display() {
  Serial.println("Clear display");
  write_instruction(0b00000001);
}

void return_home() {
  Serial.println("Return home");
  write_instruction(0b0000010);
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
  write_instruction(instruction);
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
  write_instruction(instruction);
}

void write_text(const char *text) {
  clear_display();
  return_home();  
  entry_mode_set(true, false); // Scrolling to false
  int i = 0;
  while (*text) {
    if (++i == 16) {
      // When adding a character that would not fit on the screen
      // Set scrolling to true
      entry_mode_set(true, true);
    }
    write_character(*text);
    ++text;
  }
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
