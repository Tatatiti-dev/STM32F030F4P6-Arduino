#include <miniOled.h>
OLED oled;
void setup() {
  Serial.begin(9600);
  Serial.println("test");
  oled.init(3,2);
  oled.clearDisplay();
  for(int i = 2; i < 64; i+=3){
    oled.drawCircle(64,32,i,1);
  }
  oled.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
