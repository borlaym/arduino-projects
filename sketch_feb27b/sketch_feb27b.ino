#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define REGISTER_SELECT RX
#define ENABLE TX
// NOTE: Read / write is wired to be always write
#define BUTTON D8

#define WIFI_SSID "Marci 2.4GHz"
#define WIFI_PASS ""

int data_pins[] = {D0, D1, D2, D3, D4, D5, D6, D7};

void setup() {
  // put your setup code here, to run once:
  pinMode(REGISTER_SELECT, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(BUTTON, INPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(data_pins[i], OUTPUT);
  }
  


  // Setup
  function_set(true, false);
  
  display_on_off(true, true, true);
  entry_mode_set(true, false);
  clear_display();
  return_home();
  write_first_line("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }
  write_second_line("Success");
  delay(1000);
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
  write_data(false, instruction, 10);
}

void write_character(char character) {
  write_data(true, character, 100);
}

void write_instant_character(char character) {
  write_data(true, character, 10);
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

/**
 * First line: 0x00 -> 0x27
 * Second line: 0x40 -> 0x67
 */
void move_cursor(char pos) {
  char instruction = 0b10000000;
  if (pos > 0b01111111) {
    Serial.println("ERROR: cursor position out of bounds");
    return;
  }
  instruction += pos;
  write_instruction(instruction);
}

void move_cursor_second_line() {
  move_cursor(0x40);
}

void write_text(const char *text) {
  while (*text) {
    write_character(*text);
    ++text;
  }
}

void write_first_line(const char *text) {
  clear_display();
  return_home();
  while (*text) {
    write_character(*text);
    ++text;
  }
}

void write_second_line(const char *text) {  
  move_cursor_second_line();
  while (*text) {
    write_character(*text);
    ++text;
  }
}

void write_scrolling_text(const char *text) {  
  char previous_line[16];
  int i = 0;
  bool finished_first_row = false;
  while (*text) {
    // Newline character
    if (*text == 10) {
      if (finished_first_row == false) {
        move_cursor_second_line();
        i = 0;
        finished_first_row = true;
      } else {
        clear_display();
        return_home();
        char *previous_pointer = previous_line;
        // Only write i characters, since only that many were saved
        int j = 0;
        while (*previous_pointer && j < i) {
          write_instant_character(*previous_pointer);
          ++previous_pointer;
          ++j;
        }
        move_cursor_second_line();
        i = 0;
      }
      delay(500);
      ++text;
      continue;
    }
    // Before we finish the first row, don't save last row
    if (finished_first_row == false) {
      write_character(*text);
      if (++i == 16) {
        move_cursor_second_line();
        i = 0;
        finished_first_row = true;
      }
    // After the first row, store characters so when a new line comes
    // we can copy the second row to the first
    } else {
      write_character(*text);
      previous_line[i] = *text; 
      if (++i == 16) {
        clear_display();
        return_home();
        char *previous_pointer = previous_line;
        while (*previous_pointer) {
          write_instant_character(*previous_pointer);
          ++previous_pointer;
        }
        move_cursor_second_line();
        i = 0;
      }
    }
    
    ++text;
  }
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client;
 HTTPClient http;
 http.begin(client, "http://www.irjafalamra.hu/api/latest");
  
 int httpCode = http.GET();
 String payload = http.getString();
 const char* payload_chars = payload.c_str();
 clear_display();
  return_home();
 write_scrolling_text(payload_chars);
 http.end();
 delay(2000);
 if (digitalRead(BUTTON)) {
    clear_display();
    return_home();
    write_first_line("Acking...");
    WiFiClient client2;
    HTTPClient http2;
    http2.begin(client2, "http://www.irjafalamra.hu/api/ack");
    int httpCode = http2.GET();
    http2.end();
    write_second_line("Done.");
    delay(1000);
  }
}
