  #include <ezButton.h>
  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  
  #include "custom_chars.h"
  
  #define BUFFER_SIZE 80
  #define BUTTON_COUNT 4
  
  LiquidCrystal_I2C lcd(0x27,20,4);
  
  ezButton buttons[BUTTON_COUNT] = {
  ezButton(2),
  ezButton(3),
  ezButton(4),
  ezButton(5),
  };

void my_print(const char* buffer)
{
  int i;
  for (i = 0; i < 20; i++)
  {
    if (buffer[i] == 0) break;
    lcd.write(buffer[i]);
  }
  for (; i < 20; i++)
  {
    lcd.write(' ');
  }
}

void setup() {
  
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    buttons[i].setDebounceTime(50);
  }
  lcd.init();
  for (int i = 0; i < 6; i++)
  lcd.createChar(i, bars[i]);
  Serial.begin(9600);
  lcd.backlight();
  lcd.clear();
  lcd.print("Waiting for data");
}

void serial_get()
{
  char buffer[BUFFER_SIZE] = {0};
  char line = Serial.read();
  line = line - 48;
  if (line < 0 || line > 3) line = 0;
  Serial.readBytesUntil('\n', buffer, BUFFER_SIZE);
  String str = String(buffer);
  String substr = str.substring(0, 20);
  lcd.setCursor(0,line);
  my_print(substr.c_str());
  delay(10);
}

void loop() {
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    buttons[i].loop();
    if (buttons[i].isPressed())
    {
      Serial.write('1' + i);
    }
  }

  if (Serial.available() != 0)
  {
    serial_get();
  }
}
