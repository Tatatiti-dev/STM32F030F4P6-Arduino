#include <miniOled.h>
OLED oled;
void setup() {
  Serial.begin(9600);
  Serial.println("test");
  oled.init(3,2);
  oled.clearDisplay();
  oled.setTextColor(WHITE,BLACK);
  oled.setTextSize(2);
  oled.println("HELLO");
  oled.setTextSize(1);
  oled.println(0xDEADBEEF,16);
  oled.println(123.56789,3);
  oled.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
